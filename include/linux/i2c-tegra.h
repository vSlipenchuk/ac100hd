/*
 * drivers/i2c/busses/i2c-tegra.c
 *
 * Copyright (C) 2010 Google, Inc.
 * Author: Colin Cross <ccross@android.com>
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

#ifndef _LINUX_I2C_TEGRA_H
#define _LINUX_I2C_TEGRA_H

#include <linux/i2c.h>
#include <mach/pinmux.h>

#define TEGRA_I2C_MAX_BUS 3

struct tegra_i2c_platform_data {
	int adapter_nr;
	int bus_count;
	const struct tegra_pingroup_config *bus_mux[TEGRA_I2C_MAX_BUS];
	int bus_mux_len[TEGRA_I2C_MAX_BUS];
	unsigned long bus_clk_rate[TEGRA_I2C_MAX_BUS];
	bool is_dvc;
	bool is_slave;
	u16 slave_addr;
};

struct tegra_i2c_bus {
	struct tegra_i2c_dev *dev;
	const struct tegra_pingroup_config *mux;
	int mux_len;
	unsigned long bus_clk_rate;
	struct i2c_adapter adapter;
};

/*
 * struct tegra_i2c_dev - per device i2c context
 * @dev: device reference for power management
 * @adapter: core i2c layer adapter information
 * @clk: clock reference for i2c controller
 * @i2c_clk: clock reference for i2c bus
 * @iomem: memory resource for registers
 * @base: ioremapped registers cookie
 * @cont_id: i2c controller id, used for for packet header
 * @irq: irq number of transfer complete interrupt
 * @is_dvc: identifies the DVC i2c controller, has a different register layout
 * @msg_complete: transfer completion notifier
 * @msg_err: error code for completed message
 * @msg_buf: pointer to current message data
 * @msg_buf_remaining: size of unsent data in the message buffer
 * @msg_read: identifies read transfers
 * @bus_clk_rate: current i2c bus clock rate
 * @is_suspended: prevents i2c controller accesses after suspend is called
 */
struct tegra_i2c_dev {
	struct device *dev;
	struct clk *clk;
	struct clk *i2c_clk;
	struct resource *iomem;
	struct rt_mutex dev_lock;
	void __iomem *base;
	int cont_id;
	int irq;
	bool irq_disabled;
	int is_dvc;
	int is_slave;
	struct completion msg_complete;
	int msg_err;
	u8 *msg_buf;
	size_t msg_buf_remaining;
	int msg_read;
	struct i2c_msg *msg;
	int msg_transfer_complete;
	bool is_suspended;
	int bus_count;
	const struct tegra_pingroup_config *last_mux;
	int last_mux_len;
	unsigned long last_bus_clk;
	u16 slave_addr;
	unsigned long last_bus_clk_rate;
	struct tegra_i2c_bus busses[1];
};

#endif /* _LINUX_I2C_TEGRA_H */
