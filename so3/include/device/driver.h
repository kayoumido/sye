/*
 * Copyright (C) 2014-2017 Daniel Rossier <daniel.rossier@heig-vd.ch>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef DRIVER_H
#define DRIVER_H

#include <linker.h>

#include <device/fdt/fdt.h>
#include <device/device.h>

/* driver registering */

struct driver_entry {
	char	*compatible; /* compatible Name */

	int (*init)(dev_t *dev);
};
typedef struct driver_entry	driver_entry_t;

#define REGISTER_DRIVER(_name, _compatible, _init) ll_entry_declare(struct driver_entry, _name, driver) = { \
		.compatible = _compatible,	\
		.init = _init, \
}

void init_driver_from_dtb(void);

#endif /* DRIVER_H */
