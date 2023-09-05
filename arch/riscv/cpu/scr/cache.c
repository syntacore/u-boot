// SPDX-License-Identifier: GPL-2.0+

/*
 *
 * Copyright (C) 2017, Syntacore Ltd.
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

#include <config.h>
#include <asm/cache.h>
#include <asm/csr.h>
#include <asm/io.h>
#include <asm/barrier.h>
#include <linux/types.h>
#include <cpu_func.h>

#define ENC_CLINV(regn) (0x10800073 | (((regn) & 0x1f) << 15))
#define ENC_CLFLUSH(regn) (0x10900073 | (((regn) & 0x1f) << 15))

void flush_dcache_range(unsigned long start, unsigned long end)
{
	register unsigned long a0 asm("a0") = (unsigned long)start;
	a0 &= -CONFIG_SCR_CMO_BLOCK_SIZE;

	/* Flush the cache for the requested range */
	for (; a0 <  end; a0 += CONFIG_SCR_CMO_BLOCK_SIZE)
		__asm__ __volatile__ (".word %0" :: "i"(ENC_CLFLUSH(10)), "r"(a0) : "memory");

	mb();
}

void invalidate_dcache_range(unsigned long start, unsigned long end)
{
	mb();

	register unsigned long a0 asm("a0") = (unsigned long)start;
	a0 &= -CONFIG_SCR_CMO_BLOCK_SIZE;

	for (; a0 <  end; a0 += CONFIG_SCR_CMO_BLOCK_SIZE)
		__asm__ __volatile__ (".word %0" :: "i"(ENC_CLINV(10)), "r"(a0) : "memory");
}
