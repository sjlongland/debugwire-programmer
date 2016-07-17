#ifndef _PROTOCOL_STATE_H
#define _PROTOCOL_STATE_H

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
#include "util/timer.h"

/* AVR067 Table 4-1 */
#define PROTO_STATE_START	(0)	/*!< Initial state */
#define PROTO_STATE_SEQ_NO	(1)	/*!< Get Sequence Number */
#define PROTO_STATE_MSG_SZ	(2)	/*!< Get Message Size */
#define PROTO_STATE_TOKEN	(3)	/*!< Get Message Token */
#define PROTO_STATE_DATA	(4)	/*!< Get data */
#define PROTO_STATE_CRC		(5)	/*!< Get CRC */

#define PROTO_TIMEOUT_TICKS	(10)	/*!< Number of ticks
					  before time-out (a guess) */

/*!
 * Protocol state machine variables
 */
struct proto_state_t {
	const char* msg;	/*!< Message buffer */
	uint16_t msg_sz;	/*!< Message size */
	uint16_t seq;		/*!< Sequence number */
	uint8_t state;		/*!< State number */
	struct timer_t timer;	/*!< Time-out timer */
};

#endif
