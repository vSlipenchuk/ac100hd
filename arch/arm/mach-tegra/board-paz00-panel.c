/*
 * arch/arm/mach-tegra/board-paz00-panel.c
 *
 * Copyright (c) 2010, NVIDIA Corporation.
 *               2010, Marc Dietrich <marvin24@gmx.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <asm/mach-types.h>

#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/regulator/consumer.h>
#include <linux/resource.h>
#include <linux/platform_device.h>
#include <linux/pwm_backlight.h>
#include <linux/err.h>

#include <mach/nvhost.h>
#include <mach/nvmap.h>
#include <mach/irqs.h>
#include <mach/iomap.h>
#include <mach/dc.h>
#include <mach/fb.h>

#include "devices.h"
#include "gpio-names.h"
#include "board-paz00.h"

static int paz00_backlight_init(struct device *dev)
{
	static struct regulator *reg = NULL;
	int ret;

	pr_warning(">>> backlight_enable\n");

	reg = regulator_get(NULL, "vddio_lcd");
	if (IS_ERR(reg)) {
		pr_warning("Couldn't get regulator vddio_lcd\n");
		return -1;
	}

	if (regulator_is_enabled(reg)) {
		pr_warning("regulator avdd_lvds already enabled\n");
	} else {
		ret = regulator_set_voltage(reg, 1800000, 1800000);
		if (ret) {
			pr_warning("Couldn't set regulator voltage vddio_lcd\n");
			return ret;
		}

		ret = regulator_enable(reg);
		if (ret)
			pr_warning("Couldn't enable regualtor vddio_lcd\n");
	}

	ret = gpio_request(TEGRA_BACKLIGHT, "blacklight_enable");
	if (ret) {
		pr_warning("could not request TEGRA_BACKLIGHT gpio\n");
		return ret;
	}

	ret = gpio_direction_output(TEGRA_BACKLIGHT, 1);
	if (ret) {
		pr_warning("could not set output direction of lvds_shutdown\n");
		return ret;
	}

	gpio_set_value(TEGRA_BACKLIGHT, 1);

	return 0;

};

static void paz00_backlight_exit(struct device *dev)
{
	gpio_set_value(TEGRA_BACKLIGHT, 0);
	gpio_free(TEGRA_BACKLIGHT);
}

static int paz00_backlight_notify(struct device *unused, int brightness)
{
	gpio_set_value(TEGRA_EN_VDD_PNL, !!brightness);
	gpio_set_value(TEGRA_LVDS_SHUTDOWN, !!brightness);
	gpio_set_value(TEGRA_BACKLIGHT, !!brightness);
	return brightness;
}

static int paz00_disp1_check_fb(struct device *dev, struct fb_info *info);

static struct platform_pwm_backlight_data paz00_backlight_data = {
	.pwm_id		= 0,
	.max_brightness	= 255,
	.dft_brightness	= 224,
	.pwm_period_ns	= 5000000,
	.init		= paz00_backlight_init,
	.exit		= paz00_backlight_exit,
	.notify		= paz00_backlight_notify,
	.check_fb	= paz00_disp1_check_fb,
};

static struct platform_device paz00_backlight_device = {
	.name	= "pwm-backlight",
	.id	= -1,
	.dev	= {
		.platform_data = &paz00_backlight_data,
	},
};

static int paz00_panel_enable(void)
{
	gpio_set_value(TEGRA_LVDS_SHUTDOWN, 1);
	return 0;
}

static int paz00_panel_disable(void)
{
	gpio_set_value(TEGRA_LVDS_SHUTDOWN, 0);
	return 0;
}

static struct regulator *paz00_hdmi_reg;
static struct regulator *paz00_hdmi_pll;

static int paz00_hdmi_enable(void)
{
	pr_warning(">>> hdmi enable\n");
	if (WARN_ON(!paz00_hdmi_reg || !paz00_hdmi_pll))
		return -ENODEV;

	regulator_enable(paz00_hdmi_reg);
	regulator_enable(paz00_hdmi_pll);
	return 0;
}

static int paz00_hdmi_disable(void)
{
	pr_warning(">>> hdmi disable\n");
	if (WARN_ON(!paz00_hdmi_reg || !paz00_hdmi_pll))
		return -ENODEV;

	regulator_disable(paz00_hdmi_reg);
	regulator_disable(paz00_hdmi_pll);
	return 0;
}

static struct resource paz00_disp1_resources[] = {
	{
		.name	= "irq",
		.start	= INT_DISPLAY_GENERAL,
		.end	= INT_DISPLAY_GENERAL,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.name	= "regs",
		.start	= TEGRA_DISPLAY_BASE,
		.end	= TEGRA_DISPLAY_BASE + TEGRA_DISPLAY_SIZE-1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name	= "fbmem",
		.start	= 0x1fd95000,
		.end	= 0x1fd95000 + 0x384000 - 1,	/* HD fixed 2.4 MB @ 509 MB */
		.flags	= IORESOURCE_MEM,
	},
};

static struct resource paz00_disp2_resources[] = {
	{
		.name	= "irq",
		.start	= INT_DISPLAY_B_GENERAL,
		.end	= INT_DISPLAY_B_GENERAL,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.name	= "regs",
		.start	= TEGRA_DISPLAY2_BASE,
		.end	= TEGRA_DISPLAY2_BASE + TEGRA_DISPLAY2_SIZE-1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name	= "hdmi_regs",
		.start	= TEGRA_HDMI_BASE,
		.end	= TEGRA_HDMI_BASE + TEGRA_HDMI_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	}
};

static struct tegra_dc_mode paz00_panel_modes[] = {
	{
		.pclk = 71735520 , // HD Fixed from 42430000,
		.h_ref_to_sync = 4,
		.v_ref_to_sync = 2,
		.h_sync_width = 136,
		.v_sync_width = 4,
		.h_back_porch = 138,
		.v_back_porch = 21,
		.h_active = 1280, //1024,
		.v_active = 720, // 600,
		.h_front_porch = 34,
		.v_front_porch = 4,
	},
};

static struct tegra_fb_data paz00_fb_data = {
	.win		= 0,
	.xres		= 1280, //1024,
	.yres		= 720, // 600,
	.bits_per_pixel	= 16,
	.flags		= TEGRA_FB_FLIP_ON_PROBE,
};

static struct tegra_fb_data paz00_hdmi_fb_data = {
	.win		= 0,
	.xres		= 1280,
	.yres		= 720,
	.bits_per_pixel	= 16,
};

static struct tegra_dc_out paz00_disp1_out = {
	.type		= TEGRA_DC_OUT_RGB,

	.align		= TEGRA_DC_ALIGN_MSB,
	.order		= TEGRA_DC_ORDER_RED_BLUE,
	.depth		= 18,
	.dither		= TEGRA_DC_ORDERED_DITHER,

	.modes		= paz00_panel_modes,
	.n_modes	= ARRAY_SIZE(paz00_panel_modes),

	.enable		= paz00_panel_enable,
	.disable	= paz00_panel_disable,
};

static struct tegra_dc_out paz00_disp2_out = {
	.type		= TEGRA_DC_OUT_HDMI,
	.flags		= TEGRA_DC_OUT_HOTPLUG_HIGH,

	.dcc_bus	= 1,
	.hotplug_gpio	= TEGRA_HDMI_HPD,

	.align		= TEGRA_DC_ALIGN_MSB,
	.order		= TEGRA_DC_ORDER_RED_BLUE,

	.enable		= paz00_hdmi_enable,
	.disable	= paz00_hdmi_disable,

	/* DVFS tables only updated up to 148.5MHz for HDMI currently */
	.max_pclk_khz   = 148500,
};

static struct tegra_dc_platform_data paz00_disp1_pdata = {
	.flags		= TEGRA_DC_FLAG_ENABLED,
	.default_out	= &paz00_disp1_out,
	.fb		= &paz00_fb_data,
	.emc_clk_rate	= 300000000,
};

static struct tegra_dc_platform_data paz00_disp2_pdata = {
	.flags		= 0,
	.default_out	= &paz00_disp2_out,
	.fb		= &paz00_hdmi_fb_data,
};

static struct nvhost_device paz00_disp1_device = {
	.name		= "tegradc",
	.id		= 0,
	.resource	= paz00_disp1_resources,
	.num_resources	= ARRAY_SIZE(paz00_disp1_resources),
	.dev = {
		.platform_data = &paz00_disp1_pdata,
	},
};

static int paz00_disp1_check_fb(struct device *dev, struct fb_info *info)
{
	return info->device == &paz00_disp1_device.dev;
}

static struct nvhost_device paz00_disp2_device = {
	.name		= "tegradc",
	.id		= 1,
	.resource	= paz00_disp2_resources,
	.num_resources	= ARRAY_SIZE(paz00_disp2_resources),
	.dev = {
		.platform_data = &paz00_disp2_pdata,
	},
};

static struct nvmap_platform_carveout paz00_carveouts[] = {
	[0] = {
		.name		= "iram",
		.usage_mask	= NVMAP_HEAP_CARVEOUT_IRAM,
		.base		= TEGRA_IRAM_BASE,
		.size		= TEGRA_IRAM_SIZE,
		.buddy_size	= 0, /* no buddy allocation for IRAM */
	},
	[1] = {
		.name		= "generic-0",
		.usage_mask	= NVMAP_HEAP_CARVEOUT_GENERIC,
		.base		= 0x1C000000,   /* carveout starts at 448 MB */
		.size		= SZ_64M - 0xC00000,
		.buddy_size	= SZ_32K,
	},
};

static struct nvmap_platform_data paz00_nvmap_data = {
	.carveouts	= paz00_carveouts,
	.nr_carveouts	= ARRAY_SIZE(paz00_carveouts),
};

static struct platform_device paz00_nvmap_device = {
	.name	= "tegra-nvmap",
	.id	= -1,
	.dev	= {
		.platform_data = &paz00_nvmap_data,
	},
};

static struct platform_device *paz00_gfx_devices[] __initdata = {
	&paz00_nvmap_device,
	&tegra_grhost_device,
	&tegra_pwfm0_device,
	&paz00_backlight_device,
};

int __init paz00_panel_init(size_t fb_addr)
{
	int err;

	gpio_request_one(TEGRA_EN_VDD_PNL, GPIOF_OUT_INIT_HIGH, "en_vdd_pnl");
	gpio_request_one(TEGRA_BACKLIGHT_VDD, GPIOF_OUT_INIT_HIGH, "bl_vdd");
	gpio_request_one(TEGRA_LVDS_SHUTDOWN, GPIOF_OUT_INIT_HIGH, "lvds_shdn");
	gpio_request_one(TEGRA_HDMI_HPD, GPIOF_IN, "hdmi_hpd");
	gpio_export(TEGRA_LVDS_SHUTDOWN, 0);

	if (fb_addr) {
		paz00_disp1_resources[2].start = fb_addr;
		/* 2.4 MB framebuffer should be enough */
		paz00_disp1_resources[2].end =   fb_addr + 0x26B000 - 1;
	}

	err = platform_add_devices(paz00_gfx_devices,
				   ARRAY_SIZE(paz00_gfx_devices));

	if (!err)
		err = nvhost_device_register(&paz00_disp1_device);
	else
		pr_warning("registered disp1 device\n");

	if (!err)
		err = nvhost_device_register(&paz00_disp2_device);
	else
		pr_warning("registered disp2 device\n");

	return err;
}

static int __init paz00_hdmi_late_init(void)
{
	int ret;

	paz00_hdmi_reg = regulator_get(NULL, "avdd_hdmi");
	if (IS_ERR_OR_NULL(paz00_hdmi_reg)) {
		ret = PTR_ERR(paz00_hdmi_reg);
		goto fail1;
	}

	paz00_hdmi_pll = regulator_get(NULL, "avdd_hdmi_pll");
	if (IS_ERR_OR_NULL(paz00_hdmi_pll)) {
		ret = PTR_ERR(paz00_hdmi_pll);
		goto fail2;
	}

	return 0;

fail2:
	if (paz00_hdmi_pll) {
		regulator_disable(paz00_hdmi_pll);
		paz00_hdmi_pll = NULL;
	}

fail1:
	return ret;
}

late_initcall(paz00_hdmi_late_init);
