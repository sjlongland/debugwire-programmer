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

/*! Handle the internal tick counter */
void proto_tick();

/*! Process incoming data from the USB host */
void proto_host_rx(const uint8_t* buffer, uint8_t sz);

/*!
 * Write data to the USB host: this needs to be implemented by the
 * application.
 * @param[in]	buffer	Pointer to buffer to be sent to the host.
 * @param[in]	sz	Number of bytes to send to the host.
 * @retval	>0	Number of bytes sent to the host.
 * @retval	<=0	Error state.
 */
int16_t proto_host_tx(const uint8_t* buffer, uint8_t sz);

/*!
 * Set the target baud rate: this needs to be implemented by the
 * application.
 *
 * @param[in]	rate	Baud rate in bps
 * @retval	0	Success
 * @retval	<0	Error setting baud rate.
 */
void proto_target_baud(uint32_t rate);

/*! Process incoming data from the target */
void proto_target_rx(const uint8_t* buffer, uint8_t sz);

/*!
 * Write data to the target: this needs to be implemented by the
 * application.
 * @param[in]	buffer	Pointer to buffer to be sent to the host.
 * @param[in]	sz	Number of bytes to send to the host.
 * @retval	>0	Number of bytes sent to the host.
 * @retval	<=0	Error state.
 */
int16_t proto_target_tx(const uint8_t* buffer, uint8_t sz);

#endif
