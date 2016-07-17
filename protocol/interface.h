#ifndef _PROTOCOL_INTERFACE_H
#define _PROTOCOL_INTERFACE_H

/*!
 * JTAGICE mkII Protocol library interface.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (see COPYING); if not, write to the Free
 * Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */

#include <stdint.h>
#include "util/fifo.h"

/*! External pointer to host UART */
extern struct fifo_t proto_host_uart;

/*! External pointer to target UART */
extern struct fifo_t proto_target_uart;

/*! Handle the internal tick counter */
void proto_tick();

/*!
 * Set the target baud rate: this needs to be implemented by the
 * application.
 *
 * @param[in]	rate	Baud rate in bps
 * @retval	0	Success
 * @retval	<0	Error setting baud rate.
 */
extern void proto_target_baud(uint32_t rate);

#endif
