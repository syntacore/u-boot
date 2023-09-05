// SPDX-License-Identifier: GPL-2.0+

/*
 *
 * Copyright (C) 2020, Syntacore Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 * Also add information on how to contact you by electronic and paper mail.
 */

#include <command.h>
#include <vsprintf.h>
#include <asm/global_data.h>
#include <cpu_func.h>
#include <dm.h>
#include <asm/asm.h>
#include <asm/sections.h>
#include <stdlib.h>
#include <fdt_support.h>
#include <env.h>
#include <miiphy.h>

phys_addr_t prior_stage_fdt_address __section(".data");

void reset_phy(void)
{
#if defined(XILINX_ETH_NAME)
	struct mii_dev *bus;
	int i;

	if (miiphy_set_current_dev(XILINX_ETH_NAME))
		return;

	bus = mdio_get_current_dev();
	if (!bus) {
		printf("failed to get miiphy device\n");
		return;
	}

	for (i = 0; i < PHY_MAX_ADDR; i++) {
		struct phy_device *phydev = bus->phymap[i];

		if (phydev) {
			if (!phy_reset(phydev))
				printf("%s: reset %d ok\n", __func__, phydev->addr);
		}
	}
#endif
}

void *board_fdt_blob_setup(int *err)
{
	*err = 0;
	/* SCBL copies DTB from OCRAM to DRAM. Then DTB address is passed
	 * to the next boot stage software (Linux or U-Boot) via s1 register.
	 * U-Boot after that saves this address via SREG s1, GD_FIRMWARE_FDT_ADDR(gp).
	 * Since it is defined as DEFINE(GD_FIRMWARE_FDT_ADDR, offsetof(gd_t, arch.firmware_fdt_addr)),
	 * our DTB address is stored in gd->arch.firmware_fdt_addr.
	 */
	return (ulong *)(uintptr_t)gd->arch.firmware_fdt_addr;
}

int board_init(void)
{
	return 0;
}
