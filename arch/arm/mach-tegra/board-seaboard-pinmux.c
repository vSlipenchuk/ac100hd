/*
 * Copyright (C) 2010 NVIDIA Corporation
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/gpio.h>

#include <mach/iomap.h>
#include <mach/pinmux.h>
#include <mach/pinmux-t2.h>

#include "gpio-names.h"
#include "board-seaboard.h"

#define DEFAULT_DRIVE(_name)					\
	{							\
		.pingroup = TEGRA_DRIVE_PINGROUP_##_name,	\
		.hsm = TEGRA_HSM_DISABLE,			\
		.schmitt = TEGRA_SCHMITT_ENABLE,		\
		.drive = TEGRA_DRIVE_DIV_1,			\
		.pull_down = TEGRA_PULL_31,			\
		.pull_up = TEGRA_PULL_31,			\
		.slew_rising = TEGRA_SLEW_SLOWEST,		\
		.slew_falling = TEGRA_SLEW_SLOWEST,		\
	}

#define VI_CSI_DRIVE(_name)					\
	{							\
		.pingroup = TEGRA_DRIVE_PINGROUP_##_name,	\
		.hsm = TEGRA_HSM_ENABLE,			\
		.schmitt = TEGRA_SCHMITT_ENABLE,		\
		.drive = TEGRA_DRIVE_DIV_1,			\
		.pull_down = TEGRA_PULL_31,			\
		.pull_up = TEGRA_PULL_31,			\
		.slew_rising = TEGRA_SLEW_SLOWEST,		\
		.slew_falling = TEGRA_SLEW_SLOWEST,		\
	}

#define I2C_DRIVE(_name)					\
	{							\
		.pingroup = TEGRA_DRIVE_PINGROUP_##_name,	\
		.hsm = TEGRA_HSM_DISABLE,			\
		.schmitt = TEGRA_SCHMITT_ENABLE,		\
		.drive = TEGRA_DRIVE_DIV_1,			\
		.pull_down = TEGRA_PULL_31,			\
		.pull_up = TEGRA_PULL_31,			\
		.slew_rising = TEGRA_SLEW_FASTEST,		\
		.slew_falling = TEGRA_SLEW_FASTEST,		\
	}


static __initdata struct tegra_drive_pingroup_config seaboard_drive_pinmux[] = {
	I2C_DRIVE(DBG),
	I2C_DRIVE(AO1),
	I2C_DRIVE(AT1),
	I2C_DRIVE(VI2),

	DEFAULT_DRIVE(SDIO1),
	VI_CSI_DRIVE(VI1),
};

static __initdata struct tegra_pingroup_config seaboard_pinmux[] = {
	{TEGRA_PINGROUP_ATA,   TEGRA_MUX_IDE,           TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_ATB,   TEGRA_MUX_SDIO4,         TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_ATC,   TEGRA_MUX_NAND,          TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_ATD,   TEGRA_MUX_GMI,           TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_ATE,   TEGRA_MUX_GMI,           TEGRA_PUPD_NORMAL,    TEGRA_TRI_TRISTATE},
	{TEGRA_PINGROUP_CDEV1, TEGRA_MUX_PLLA_OUT,      TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_CDEV2, TEGRA_MUX_PLLP_OUT4,     TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_CRTP,  TEGRA_MUX_CRT,           TEGRA_PUPD_PULL_UP,   TEGRA_TRI_TRISTATE},
	{TEGRA_PINGROUP_CSUS,  TEGRA_MUX_VI_SENSOR_CLK, TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_DAP1,  TEGRA_MUX_DAP1,          TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_DAP2,  TEGRA_MUX_DAP2,          TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_DAP3,  TEGRA_MUX_DAP3,          TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_DAP4,  TEGRA_MUX_DAP4,          TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_DDC,   TEGRA_MUX_RSVD4,         TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_DTA,   TEGRA_MUX_VI,            TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_DTB,   TEGRA_MUX_VI,            TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_DTC,   TEGRA_MUX_VI,            TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_DTD,   TEGRA_MUX_VI,            TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_DTE,   TEGRA_MUX_VI,            TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_DTF,   TEGRA_MUX_I2C3,          TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_GMA,   TEGRA_MUX_SDIO4,         TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_GMB,   TEGRA_MUX_GMI,           TEGRA_PUPD_PULL_UP,   TEGRA_TRI_TRISTATE},
	{TEGRA_PINGROUP_GMC,   TEGRA_MUX_UARTD,         TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_GMD,   TEGRA_MUX_SFLASH,        TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_GME,   TEGRA_MUX_SDIO4,         TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_GPU,   TEGRA_MUX_PWM,           TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_GPU7,  TEGRA_MUX_RTCK,          TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_GPV,   TEGRA_MUX_PCIE,          TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_HDINT, TEGRA_MUX_HDMI,          TEGRA_PUPD_NORMAL,    TEGRA_TRI_TRISTATE},
	{TEGRA_PINGROUP_I2CP,  TEGRA_MUX_I2C,           TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_IRRX,  TEGRA_MUX_UARTB,         TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_IRTX,  TEGRA_MUX_UARTB,         TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_KBCA,  TEGRA_MUX_KBC,           TEGRA_PUPD_PULL_UP,   TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_KBCB,  TEGRA_MUX_KBC,           TEGRA_PUPD_PULL_UP,   TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_KBCC,  TEGRA_MUX_KBC,           TEGRA_PUPD_PULL_UP,   TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_KBCD,  TEGRA_MUX_KBC,           TEGRA_PUPD_PULL_UP,   TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_KBCE,  TEGRA_MUX_KBC,           TEGRA_PUPD_PULL_UP,   TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_KBCF,  TEGRA_MUX_KBC,           TEGRA_PUPD_PULL_UP,   TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LCSN,  TEGRA_MUX_RSVD4,         TEGRA_PUPD_NORMAL,    TEGRA_TRI_TRISTATE},
	{TEGRA_PINGROUP_LD0,   TEGRA_MUX_DISPLAYA,      TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LD1,   TEGRA_MUX_DISPLAYA,      TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LD10,  TEGRA_MUX_DISPLAYA,      TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LD11,  TEGRA_MUX_DISPLAYA,      TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LD12,  TEGRA_MUX_DISPLAYA,      TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LD13,  TEGRA_MUX_DISPLAYA,      TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LD14,  TEGRA_MUX_DISPLAYA,      TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LD15,  TEGRA_MUX_DISPLAYA,      TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LD16,  TEGRA_MUX_DISPLAYA,      TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LD17,  TEGRA_MUX_DISPLAYA,      TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LD2,   TEGRA_MUX_DISPLAYA,      TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LD3,   TEGRA_MUX_DISPLAYA,      TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LD4,   TEGRA_MUX_DISPLAYA,      TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LD5,   TEGRA_MUX_DISPLAYA,      TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LD6,   TEGRA_MUX_DISPLAYA,      TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LD7,   TEGRA_MUX_DISPLAYA,      TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LD8,   TEGRA_MUX_DISPLAYA,      TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LD9,   TEGRA_MUX_DISPLAYA,      TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LDC,   TEGRA_MUX_RSVD4,         TEGRA_PUPD_NORMAL,    TEGRA_TRI_TRISTATE},
	{TEGRA_PINGROUP_LDI,   TEGRA_MUX_DISPLAYA,      TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LHP0,  TEGRA_MUX_DISPLAYA,      TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LHP1,  TEGRA_MUX_DISPLAYA,      TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LHP2,  TEGRA_MUX_DISPLAYA,      TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LHS,   TEGRA_MUX_DISPLAYA,      TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LM0,   TEGRA_MUX_RSVD4,         TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LM1,   TEGRA_MUX_CRT,           TEGRA_PUPD_NORMAL,    TEGRA_TRI_TRISTATE},
	{TEGRA_PINGROUP_LPP,   TEGRA_MUX_DISPLAYA,      TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LPW0,  TEGRA_MUX_HDMI,          TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LPW1,  TEGRA_MUX_RSVD4,         TEGRA_PUPD_NORMAL,    TEGRA_TRI_TRISTATE},
	{TEGRA_PINGROUP_LPW2,  TEGRA_MUX_HDMI,          TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LSC0,  TEGRA_MUX_DISPLAYA,      TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LSC1,  TEGRA_MUX_HDMI,          TEGRA_PUPD_NORMAL,    TEGRA_TRI_TRISTATE},
	{TEGRA_PINGROUP_LSCK,  TEGRA_MUX_HDMI,          TEGRA_PUPD_NORMAL,    TEGRA_TRI_TRISTATE},
	{TEGRA_PINGROUP_LSDA,  TEGRA_MUX_HDMI,          TEGRA_PUPD_NORMAL,    TEGRA_TRI_TRISTATE},
	{TEGRA_PINGROUP_LSDI,  TEGRA_MUX_RSVD4,         TEGRA_PUPD_NORMAL,    TEGRA_TRI_TRISTATE},
	{TEGRA_PINGROUP_LSPI,  TEGRA_MUX_DISPLAYA,      TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LVP0,  TEGRA_MUX_RSVD4,         TEGRA_PUPD_NORMAL,    TEGRA_TRI_TRISTATE},
	{TEGRA_PINGROUP_LVP1,  TEGRA_MUX_DISPLAYA,      TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LVS,   TEGRA_MUX_DISPLAYA,      TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_OWC,   TEGRA_MUX_RSVD2,         TEGRA_PUPD_NORMAL,    TEGRA_TRI_TRISTATE},
	{TEGRA_PINGROUP_PMC,   TEGRA_MUX_PWR_ON,        TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_PTA,   TEGRA_MUX_HDMI,          TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_RM,    TEGRA_MUX_I2C,           TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_SDB,   TEGRA_MUX_SDIO3,         TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_SDC,   TEGRA_MUX_SDIO3,         TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_SDD,   TEGRA_MUX_SDIO3,         TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_SDIO1, TEGRA_MUX_SDIO1,         TEGRA_PUPD_PULL_UP,   TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_SLXA,  TEGRA_MUX_PCIE,          TEGRA_PUPD_PULL_UP,   TEGRA_TRI_TRISTATE},
	{TEGRA_PINGROUP_SLXC,  TEGRA_MUX_SPDIF,         TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_SLXD,  TEGRA_MUX_SPDIF,         TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_SLXK,  TEGRA_MUX_PCIE,          TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_SPDI,  TEGRA_MUX_RSVD2,         TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_SPDO,  TEGRA_MUX_RSVD2,         TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_SPIA,  TEGRA_MUX_GMI,           TEGRA_PUPD_PULL_UP,   TEGRA_TRI_TRISTATE},
	{TEGRA_PINGROUP_SPIB,  TEGRA_MUX_GMI,           TEGRA_PUPD_NORMAL,    TEGRA_TRI_TRISTATE},
	{TEGRA_PINGROUP_SPIC,  TEGRA_MUX_GMI,           TEGRA_PUPD_PULL_UP,   TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_SPID,  TEGRA_MUX_SPI1,          TEGRA_PUPD_NORMAL,    TEGRA_TRI_TRISTATE},
	{TEGRA_PINGROUP_SPIE,  TEGRA_MUX_SPI1,          TEGRA_PUPD_NORMAL,    TEGRA_TRI_TRISTATE},
	{TEGRA_PINGROUP_SPIF,  TEGRA_MUX_SPI1,          TEGRA_PUPD_PULL_DOWN, TEGRA_TRI_TRISTATE},
	{TEGRA_PINGROUP_SPIG,  TEGRA_MUX_SPI2_ALT,      TEGRA_PUPD_PULL_UP,   TEGRA_TRI_TRISTATE},
	{TEGRA_PINGROUP_SPIH,  TEGRA_MUX_SPI2_ALT,      TEGRA_PUPD_PULL_UP,   TEGRA_TRI_TRISTATE},
	{TEGRA_PINGROUP_UAA,   TEGRA_MUX_ULPI,          TEGRA_PUPD_PULL_UP,   TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_UAB,   TEGRA_MUX_ULPI,          TEGRA_PUPD_PULL_UP,   TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_UAC,   TEGRA_MUX_RSVD2,         TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_UAD,   TEGRA_MUX_IRDA,          TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_UCA,   TEGRA_MUX_UARTC,         TEGRA_PUPD_PULL_UP,   TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_UCB,   TEGRA_MUX_UARTC,         TEGRA_PUPD_PULL_UP,   TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_UDA,   TEGRA_MUX_ULPI,          TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_CK32,  TEGRA_MUX_NONE,          TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_DDRC,  TEGRA_MUX_NONE,          TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_PMCA,  TEGRA_MUX_NONE,          TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_PMCB,  TEGRA_MUX_NONE,          TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_PMCC,  TEGRA_MUX_NONE,          TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_PMCD,  TEGRA_MUX_NONE,          TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_PMCE,  TEGRA_MUX_NONE,          TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_XM2C,  TEGRA_MUX_NONE,          TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_XM2D,  TEGRA_MUX_NONE,          TEGRA_PUPD_NORMAL,    TEGRA_TRI_NORMAL},
};




static struct tegra_gpio_table common_gpio_table[] = {
	{ .gpio = TEGRA_GPIO_SD2_CD,		.enable = true },
	{ .gpio = TEGRA_GPIO_SD2_WP,		.enable = true },
	{ .gpio = TEGRA_GPIO_SD2_POWER,		.enable = true },
	{ .gpio = TEGRA_GPIO_LIDSWITCH,		.enable = true },
	{ .gpio = TEGRA_GPIO_POWERKEY,		.enable = true },
	{ .gpio = TEGRA_GPIO_ISL29018_IRQ,	.enable = true },
	{ .gpio = TEGRA_GPIO_USB1,		.enable = true },
	{ .gpio = TEGRA_GPIO_USB3,		.enable = true },
	{ .gpio = TEGRA_GPIO_NCT1008_THERM2_IRQ,.enable = true },
	{ .gpio = TEGRA_GPIO_WLAN_POWER,	.enable = true },
	{ .gpio = TEGRA_GPIO_AC_ONLINE,		.enable = true },
	{ .gpio = TEGRA_GPIO_HP_DET,		.enable = true },
	{ .gpio = TEGRA_GPIO_MXT_IRQ,		.enable = true },
	{ .gpio = TEGRA_GPIO_HDMI_ENB,		.enable = true },
	{ .gpio = TEGRA_GPIO_MPU3050_IRQ,	.enable = true },
	{ .gpio = TEGRA_GPIO_DISABLE_CHARGER,	.enable = true },
	{ .gpio = TEGRA_GPIO_RECOVERY_SWITCH,	.enable = true },
	{ .gpio = TEGRA_GPIO_DEV_SWITCH,	.enable = true },
	{ .gpio = TEGRA_GPIO_WP_STATUS,		.enable = true },
	{ .gpio = TEGRA_GPIO_BACKLIGHT,		.enable = true },
	{ .gpio = TEGRA_GPIO_LVDS_SHUTDOWN,	.enable = true },
	{ .gpio = TEGRA_GPIO_BACKLIGHT_VDD,	.enable = true },
	{ .gpio = TEGRA_GPIO_EN_VDD_PNL,	.enable = true },
	{ .gpio = TEGRA_GPIO_BT_RESET,		.enable = true },
	{ .gpio = TEGRA_GPIO_BT_SHUTDOWN,	.enable = true },
	{ .gpio = TEGRA_GPIO_HDMI_HPD,		.enable = true },
	{ .gpio = TEGRA_GPIO_CYTP_INT,		.enable = true },
};

static struct tegra_gpio_table seaboard_gpio_table[] = {
	{ .gpio = SEABOARD_GPIO_MXT_RST,	.enable = true },
};

static struct tegra_gpio_table asymptote_gpio_table[] = {
	{ .gpio = ASYMPTOTE_GPIO_MXT_RST,	.enable = true },
	{ .gpio = ASYMPTOTE_GPIO_TSL2563_IRQ,	.enable = true },
};

void __init seaboard_common_pinmux_init(void)
{
	/*
	 * PINGROUP_SPIC contains two pins:
	 * + PX2, DISABLE_CHRGR (output)
	 * + PX3, WM8903 codec IRQ (input)
	 *
	 * The pinmux module can only configure TRISTATE vs. NORMAL on a
	 * per-group rather than per-pin basis. The group must be NORMAL
	 * since at least one pin is an output. However, we must ensure that
	 * the WM8903 IRQ is never driven, since the WM8903 itself is driving
	 * it, and we don't want multiple drivers. To ensure this, configure
	 * PX3 as a GPIO here, and set is as an input, before the pinmux table
	 * is written, which is when the pins will be un-tristated.
	 */
	tegra_gpio_enable(TEGRA_GPIO_CDC_IRQ);
	gpio_request(TEGRA_GPIO_CDC_IRQ, "wm8903");
	gpio_direction_input(TEGRA_GPIO_CDC_IRQ);

	/* Ensure the reset line stays high. */
	gpio_request(TEGRA_GPIO_RESET, "reset");
	gpio_direction_output(TEGRA_GPIO_RESET, 1);
	tegra_gpio_enable(TEGRA_GPIO_RESET);

	tegra_pinmux_config_table(seaboard_pinmux, ARRAY_SIZE(seaboard_pinmux));

	tegra_drive_pinmux_config_table(seaboard_drive_pinmux,
					ARRAY_SIZE(seaboard_drive_pinmux));

	tegra_gpio_config(common_gpio_table, ARRAY_SIZE(common_gpio_table));
}

void __init seaboard_pinmux_init(void)
{
	seaboard_common_pinmux_init();
	tegra_gpio_config(seaboard_gpio_table, ARRAY_SIZE(seaboard_gpio_table));
}

static void __init update_pinmux(struct tegra_pingroup_config *newtbl, int size)
{
	int i, j;
	struct tegra_pingroup_config *new_pingroup, *base_pingroup;

	/* Update base seaboard pinmux table with secondary board
	 * specific pinmux table table.
	 */
	for (i = 0; i < size; i++) {
		new_pingroup = &newtbl[i];
		for (j = 0; j < ARRAY_SIZE(seaboard_pinmux); j++) {
			base_pingroup = &seaboard_pinmux[j];
			if (new_pingroup->pingroup == base_pingroup->pingroup) {
				*base_pingroup = *new_pingroup;
				break;
			}
		}
	}
}

#define STRAP_OPT 0x008
#define GMI_AD0 (1 << 4)
#define GMI_AD1 (1 << 5)
#define RAM_ID_MASK (GMI_AD0 | GMI_AD1)
#define RAM_CODE_SHIFT 4

void __init fixup_pinmux_for_26Mhz(void)
{
	const struct tegra_pingroup_config pingroup_CDEV2_MUX_OSC[] = {
		{TEGRA_PINGROUP_CDEV2, TEGRA_MUX_OSC,
		 TEGRA_PUPD_NORMAL, TEGRA_TRI_NORMAL}
	};
	tegra_pinmux_config_table(pingroup_CDEV2_MUX_OSC,
				  ARRAY_SIZE(pingroup_CDEV2_MUX_OSC));
}

void __init kaen_pinmux_init(void)
{
	/* TODO: Refactor this entire board revision specialization when
	 * Flattened Device Tree and apbio interfaces are ready.
	 */

	void __iomem *apb_misc = IO_ADDRESS(TEGRA_APB_MISC_BASE);
	int ram_id;
	u32 reg;

	seaboard_pinmux_init();

	/*
	 * For Kaen DVT2 (and not DVT1) there is a single pinmux register
	 * change to be made which accomodates a 26Mhz reference oscillator.
	 * If DVT2, then overwrite the value that was just written in the call
	 * to seaboard_pinmux_init().
	 *
	 * Detect that 26Mhz oscillator is present for Kaen based on ram_id.
	 *   ram_id = {0, 1} for DVT2, 26Mhz oscillator
	 *   ram_id = {2, 3} for DVT1, 12Mhz oscillator (same as seaboard)
	 */
	reg = readl(apb_misc + STRAP_OPT);
	ram_id = (reg & RAM_ID_MASK) >> RAM_CODE_SHIFT;

	if ((ram_id == 0) || (ram_id == 1))
		fixup_pinmux_for_26Mhz();
}

void __init aebl_pinmux_init(void)
{
	seaboard_pinmux_init();
	fixup_pinmux_for_26Mhz();
}

static __initdata struct tegra_pingroup_config asymptote_pinmux[] = {
	{TEGRA_PINGROUP_UCB,  TEGRA_MUX_NONE,      TEGRA_PUPD_NORMAL, TEGRA_TRI_NORMAL},
};

void __init asymptote_pinmux_init(void)
{
	update_pinmux(asymptote_pinmux, ARRAY_SIZE(asymptote_pinmux));
	seaboard_common_pinmux_init();
	tegra_gpio_config(asymptote_gpio_table,
			  ARRAY_SIZE(asymptote_gpio_table));
}

static __initdata struct tegra_pingroup_config ventana_pinmux[] = {
	{TEGRA_PINGROUP_DAP3, TEGRA_MUX_DAP3,     TEGRA_PUPD_NORMAL,	TEGRA_TRI_TRISTATE},
	{TEGRA_PINGROUP_DDC,  TEGRA_MUX_RSVD2,    TEGRA_PUPD_NORMAL,	TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_DTA,  TEGRA_MUX_VI,       TEGRA_PUPD_PULL_DOWN, TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_DTB,  TEGRA_MUX_VI,       TEGRA_PUPD_PULL_DOWN, TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_DTC,  TEGRA_MUX_VI,       TEGRA_PUPD_PULL_DOWN, TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_DTD,  TEGRA_MUX_VI,       TEGRA_PUPD_PULL_DOWN, TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_GMD,  TEGRA_MUX_SFLASH,   TEGRA_PUPD_NORMAL,	TEGRA_TRI_TRISTATE},
	{TEGRA_PINGROUP_LPW0, TEGRA_MUX_RSVD4,    TEGRA_PUPD_NORMAL,	TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LPW2, TEGRA_MUX_RSVD4,    TEGRA_PUPD_NORMAL,	TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LSC1, TEGRA_MUX_RSVD4,    TEGRA_PUPD_NORMAL,	TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_LSCK, TEGRA_MUX_RSVD4,    TEGRA_PUPD_NORMAL,	TEGRA_TRI_TRISTATE},
	{TEGRA_PINGROUP_LSDA, TEGRA_MUX_RSVD4,    TEGRA_PUPD_NORMAL,	TEGRA_TRI_TRISTATE},
	{TEGRA_PINGROUP_PTA,  TEGRA_MUX_RSVD2,    TEGRA_PUPD_NORMAL,	TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_SLXC, TEGRA_MUX_SDIO3,    TEGRA_PUPD_NORMAL,	TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_SLXK, TEGRA_MUX_SDIO3,    TEGRA_PUPD_NORMAL,	TEGRA_TRI_NORMAL},
	{TEGRA_PINGROUP_SPIA, TEGRA_MUX_GMI,      TEGRA_PUPD_NORMAL,	TEGRA_TRI_TRISTATE},
	{TEGRA_PINGROUP_SPIC, TEGRA_MUX_GMI,      TEGRA_PUPD_NORMAL,	TEGRA_TRI_TRISTATE},
	{TEGRA_PINGROUP_SPIG, TEGRA_MUX_SPI2_ALT, TEGRA_PUPD_NORMAL,	TEGRA_TRI_TRISTATE},
};

void __init ventana_pinmux_init(void)
{
	update_pinmux(ventana_pinmux, ARRAY_SIZE(ventana_pinmux));
	seaboard_pinmux_init();
}
