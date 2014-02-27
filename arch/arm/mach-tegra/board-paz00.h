/*
 * arch/arm/mach-tegra/board-paz00.h
 *
 * Copyright (C) 2010 Marc Dietrich <marvin24@gmx.de>
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

#ifndef _MACH_TEGRA_BOARD_PAZ00_H
#define _MACH_TEGRA_BOARD_PAZ00_H

/* SDCARD */
#define TEGRA_GPIO_SD1_CD	TEGRA_GPIO_PV5
#define TEGRA_GPIO_SD1_WP	TEGRA_GPIO_PH1
#define TEGRA_GPIO_SD1_POWER	TEGRA_GPIO_PV1

/* ULPI */
#define TEGRA_ULPI_RST		TEGRA_GPIO_PV0

/* WIFI */
#define TEGRA_WIFI_PWRN		TEGRA_GPIO_PK5
#define TEGRA_WIFI_RST		TEGRA_GPIO_PD1
#define TEGRA_WIFI_LED		TEGRA_GPIO_PD0

/* Panel */
#define TEGRA_BACKLIGHT		TEGRA_GPIO_PU4
#define TEGRA_BACKLIGHT_VDD	TEGRA_GPIO_PW0
#define TEGRA_BACKLIGHT_PWM	TEGRA_GPIO_PU3
#define TEGRA_LVDS_SHUTDOWN	TEGRA_GPIO_PM6
#define TEGRA_EN_VDD_PNL	TEGRA_GPIO_PA4
#define TEGRA_HDMI_HPD		TEGRA_GPIO_PN7

/* WakePad */
#define TEGRA_GPIO_POWERKEY	TEGRA_GPIO_PJ7

/* EC */
#define TEGRA_NVEC_REQ		TEGRA_GPIO_PV2

/* Audio */
#define TEGRA_HP_DET		TEGRA_GPIO_PW2

void paz00_pinmux_init(void);
int  paz00_panel_init(size_t fb_addr);
int  paz00_regulator_init(void);
void paz00_emc_init(void);

#endif
