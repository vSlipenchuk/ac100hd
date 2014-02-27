/*
 * arch/arm/mach-tegra/board-paz00-memory.c
 *
 * Copyright (c) 2010-2011, Marc Dietrich <marvin24@gmx.de>
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

#include <linux/init.h>
#include <linux/io.h>
#include <linux/kernel.h>

#include <asm/mach-types.h>

#include <mach/iomap.h>

#include "board-paz00.h"
#include "fuse.h"
#include "tegra2_emc.h"

struct tegra_board_emc_table {
	int				id; /* Boot strap ID */
	const struct tegra_emc_table	*table;
	const int			table_size;
	const char			*name;
};

/* HY5PS1G831CLFP-Y5 */
static const struct tegra_emc_table paz00_emc_tables_Hynix_333Mhz[] = {
	{
//		.rate = 166500,   /* SDRAM frequency */
//		.regs = {
//			0x0000000a,   /* RC */
//			0x00000016,   /* RFC */
//			0x00000008,   /* RAS */
//			0x00000003,   /* RP */
//			0x00000004,   /* R2W */
//			0x00000004,   /* W2R */
//			0x00000002,   /* R2P */
//			0x0000000c,   /* W2P */
//			0x00000003,   /* RD_RCD */
//			0x00000003,   /* WR_RCD */
//			0x00000002,   /* RRD */
//			0x00000001,   /* REXT */
//			0x00000004,   /* WDV */
//			0x00000005,   /* QUSE */
//			0x00000004,   /* QRST */
//			0x00000009,   /* QSAFE */
//			0x0000000d,   /* RDV */
//			0x000004df,   /* REFRESH */
//			0x00000000,   /* BURST_REFRESH_NUM */
//			0x00000003,   /* PDEX2WR */
//			0x00000003,   /* PDEX2RD */
//			0x00000003,   /* PCHG2PDEN */
//			0x00000003,   /* ACT2PDEN */
//			0x00000001,   /* AR2PDEN */
//			0x0000000a,   /* RW2PDEN */
//			0x000000c8,   /* TXSR */
//			0x00000003,   /* TCKE */
//			0x00000006,   /* TFAW */
//			0x00000004,   /* TRPAB */
//			0x00000008,   /* TCLKSTABLE */
//			0x00000002,   /* TCLKSTOP */
//			0x00000000,   /* TREFBW */
//			0x00000000,   /* QUSE_EXTRA */
//			0x00000002,   /* FBIO_CFG6 */
//			0x00000000,   /* ODT_WRITE */
//			0x00000000,   /* ODT_READ */
//			0x00000083,   /* FBIO_CFG5 */
//			0xe03b0323,   /* CFG_DIG_DLL */
//			0x007fe010,   /* DLL_XFORM_DQS */
//			0x00001414,   /* DLL_XFORM_QUSE */
//			0x00000000,   /* ZCAL_REF_CNT */
//			0x00000000,   /* ZCAL_WAIT_CNT */
//			0x00000000,   /* AUTO_CAL_INTERVAL */
//			0x00000000,   /* CFG_CLKTRIM_0 */
//			0x00000000,   /* CFG_CLKTRIM_1 */
//			0x00000000,   /* CFG_CLKTRIM_2 */
//		}
//	}, {
		.rate = 333000,   /* SDRAM frequency */
		.regs = {
			0x00000018,   /* RC */
			0x00000033,   /* RFC */
			0x00000012,   /* RAS */
			0x00000004,   /* RP */
			0x00000004,   /* R2W */
			0x00000005,   /* W2R */
			0x00000003,   /* R2P */
			0x0000000c,   /* W2P */
			0x00000006,   /* RD_RCD */
			0x00000006,   /* WR_RCD */
			0x00000003,   /* RRD */
			0x00000001,   /* REXT */
			0x00000004,   /* WDV */
			0x00000005,   /* QUSE */
			0x00000004,   /* QRST */
			0x00000009,   /* QSAFE */
			0x0000000d,   /* RDV */
			0x00000bff,   /* REFRESH */
			0x00000000,   /* BURST_REFRESH_NUM */
			0x00000003,   /* PDEX2WR */
			0x00000003,   /* PDEX2RD */
			0x00000006,   /* PCHG2PDEN */
			0x00000006,   /* ACT2PDEN */
			0x00000001,   /* AR2PDEN */
			0x00000011,   /* RW2PDEN */
			0x000000c8,   /* TXSR */
			0x00000003,   /* TCKE */
			0x0000000e,   /* TFAW */
			0x00000007,   /* TRPAB */
			0x00000008,   /* TCLKSTABLE */
			0x00000002,   /* TCLKSTOP */
			0x00000000,   /* TREFBW */
			0x00000000,   /* QUSE_EXTRA */
			0x00000002,   /* FBIO_CFG6 */
			0x00000000,   /* ODT_WRITE */
			0x00000000,   /* ODT_READ */
			0x00000083,   /* FBIO_CFG5 */
			0xf0440303,   /* CFG_DIG_DLL */
			0x007fe010,   /* DLL_XFORM_DQS */
			0x00001414,   /* DLL_XFORM_QUSE */
			0x00000000,   /* ZCAL_REF_CNT */
			0x00000000,   /* ZCAL_WAIT_CNT */
			0x00000000,   /* AUTO_CAL_INTERVAL */
			0x00000000,   /* CFG_CLKTRIM_0 */
			0x00000000,   /* CFG_CLKTRIM_1 */
			0x00000000,   /* CFG_CLKTRIM_2 */
		}
	}
};

/* MT47H128M8CF-25:H */
static const struct tegra_emc_table paz00_emc_tables_Micron_333Mhz[] = {
	{
//		.rate = 166500,   /* SDRAM frequency */
//		.regs = {
//			0x0000000b,   /* RC */
//			0x00000026,   /* RFC */
//			0x00000008,   /* RAS */
//			0x00000003,   /* RP */
//			0x00000004,   /* R2W */
//			0x00000004,   /* W2R */
//			0x00000002,   /* R2P */
//			0x0000000b,   /* W2P */
//			0x00000003,   /* RD_RCD */
//			0x00000003,   /* WR_RCD */
//			0x00000002,   /* RRD */
//			0x00000001,   /* REXT */
//			0x00000003,   /* WDV */
//			0x00000004,   /* QUSE */
//			0x00000003,   /* QRST */
//			0x00000009,   /* QSAFE */
//			0x0000000c,   /* RDV */
//			0x0000059f,   /* REFRESH */
//			0x00000000,   /* BURST_REFRESH_NUM */
//			0x00000003,   /* PDEX2WR */
//			0x00000003,   /* PDEX2RD */
//			0x00000003,   /* PCHG2PDEN */
//			0x00000003,   /* ACT2PDEN */
//			0x00000001,   /* AR2PDEN */
//			0x0000000a,   /* RW2PDEN */
//			0x000000c8,   /* TXSR */
//			0x00000003,   /* TCKE */
//			0x00000007,   /* TFAW */
//			0x00000004,   /* TRPAB */
//			0x0000000f,   /* TCLKSTABLE */
//			0x00000002,   /* TCLKSTOP */
//			0x00000000,   /* TREFBW */
//			0x00000000,   /* QUSE_EXTRA */
//			0x00000003,   /* FBIO_CFG6 */
//			0x00000000,   /* ODT_WRITE */
//			0x00000000,   /* ODT_READ */
//			0x00000083,   /* FBIO_CFG5 */
//			0xa06204ae,   /* CFG_DIG_DLL */
//			0x007e4010,   /* DLL_XFORM_DQS */
//			0x00000000,   /* DLL_XFORM_QUSE */
//			0x00000000,   /* ZCAL_REF_CNT */
//			0x00000000,   /* ZCAL_WAIT_CNT */
//			0x00000000,   /* AUTO_CAL_INTERVAL */
//			0x00000000,   /* CFG_CLKTRIM_0 */
//			0x00000000,   /* CFG_CLKTRIM_1 */
//			0x00000000,   /* CFG_CLKTRIM_2 */
//		}
//	}, // {
		.rate = 333000,   /* SDRAM frequency */
		.regs = {
			0x00000014,   /* RC */
			0x0000002b,   /* RFC */
			0x0000000f,   /* RAS */
			0x00000005,   /* RP */
			0x00000004,   /* R2W */
			0x00000005,   /* W2R */
			0x00000004,   /* R2P */
			0x0000000c,   /* W2P */
			0x00000005,   /* RD_RCD */
			0x00000005,   /* WR_RCD */
			0x00000003,   /* RRD */
			0x00000001,   /* REXT */
			0x00000004,   /* WDV */
			0x00000005,   /* QUSE */
			0x00000004,   /* QRST */
			0x00000009,   /* QSAFE */
			0x0000000d,   /* RDV */
			0x000009ff,   /* REFRESH */
			0x00000000,   /* BURST_REFRESH_NUM */
			0x00000003,   /* PDEX2WR */
			0x00000003,   /* PDEX2RD */
			0x00000005,   /* PCHG2PDEN */
			0x00000005,   /* ACT2PDEN */
			0x00000001,   /* AR2PDEN */
			0x0000000f,   /* RW2PDEN */
			0x000000c8,   /* TXSR */
			0x00000003,   /* TCKE */
			0x0000000c,   /* TFAW */
			0x00000006,   /* TRPAB */
			0x00000008,   /* TCLKSTABLE */
			0x00000002,   /* TCLKSTOP */
			0x00000000,   /* TREFBW */
			0x00000000,   /* QUSE_EXTRA */
			0x00000002,   /* FBIO_CFG6 */
			0x00000000,   /* ODT_WRITE */
			0x00000000,   /* ODT_READ */
			0x00000083,   /* FBIO_CFG5 */
			0x00000016,   /* CFG_DIG_DLL */
			0x00000010,   /* DLL_XFORM_DQS */
			0x00000008,   /* DLL_XFORM_QUSE */
			0x00000000,   /* ZCAL_REF_CNT */
			0x00000000,   /* ZCAL_WAIT_CNT */
			0x00000000,   /* AUTO_CAL_INTERVAL */
			0x00000000,   /* CFG_CLKTRIM_0 */
			0x00000000,   /* CFG_CLKTRIM_1 */
			0x00000000,   /* CFG_CLKTRIM_2 */
		},
	},
};

struct tegra_board_emc_table paz00_emc[] = {
	{
		.table 		= paz00_emc_tables_Hynix_333Mhz,
		.table_size	= ARRAY_SIZE(paz00_emc_tables_Hynix_333Mhz),
		.name		= "Hynix 333MHz",
	},
	{
		.table 		= paz00_emc_tables_Micron_333Mhz,
		.table_size	= ARRAY_SIZE(paz00_emc_tables_Micron_333Mhz),
		.name		= "Micron 333MHz",
	},
};

#define STRAP_OPT 0x008
#define GMI_AD0 (1 << 4)
#define GMI_AD1 (1 << 5)
#define RAM_ID_MASK (GMI_AD0 | GMI_AD1)
#define RAM_CODE_SHIFT 4

void __init paz00_emc_init(void)
{
	u32 reg;
	int ram_id;
	void __iomem *apb_misc = IO_ADDRESS(TEGRA_APB_MISC_BASE);

	/* read out ram strap configuration */
	/* ram_id: 0->Hynix, 1->Micron */
	reg = readl(apb_misc + STRAP_OPT);
	ram_id = (reg & RAM_ID_MASK) >> RAM_CODE_SHIFT;

	pr_warning("EMC table: ramd_id: %d, tegra_sku_id %d\n", ram_id, tegra_sku_id);

	if(ram_id <= ARRAY_SIZE(paz00_emc))
	{
		pr_warning("EMC table: using %s\n", paz00_emc[ram_id].name);
#if 0
		tegra_init_emc(paz00_emc[ram_id].table, paz00_emc[ram_id].table_size);
#endif
	} else {
		pr_warning("EMC table: unknown RAM ID - Please report !!!\n");
		tegra_init_emc(NULL, 0);
	}
}
