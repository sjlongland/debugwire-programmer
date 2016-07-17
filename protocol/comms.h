#ifndef _PROTOCOL_COMMS_H
#define _PROTOCOL_COMMS_H

/*!
 * JTAGICE mkII Protocol states.
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

/*! Process incoming character */
void proto_rx(uint8_t byte);

/*!
 * Write data to the host: this needs to be implemented by the
 * application.
 * @param[in]	buffer	Pointer to buffer to be sent to the host.
 * @param[in]	sz	Number of bytes to send to the host.
 * @retval	>0	Number of bytes sent to the host.
 * @retval	<=0	Error state.
 */
int16_t proto_tx(const uint8_t* buffer, uint8_t sz);

#endif
