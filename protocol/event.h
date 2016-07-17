#ifndef _PROTOCOL_EVENT_H
#define _PROTOCOL_EVENT_H

/*!
 * JTAGICE mkII Protocol events.
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

/* AVR067 Section 6.2 */

#define PROTO_EVT_BREAK					(0xe0)
#define PROTO_EVT_TARGET_POWER_ON			(0xe4)
#define PROTO_EVT_TARGET_POWER_OFF			(0xe5)
#define PROTO_EVT_EXT_RESET				(0xe7)
#define PROTO_EVT_TARGET_SLEEP				(0xe8)
#define PROTO_EVT_TARGET_WAKEUP				(0xe9)
#define PROTO_EVT_POWER_ERROR_STATE			(0xea)
#define PROTO_EVT_POWER_OK				(0xeb)
#define PROTO_EVT_IDR_DIRTY				(0xec)
#define PROTO_EVT_ERROR_PHY_FORCE_BREAK_TIMEOUT 	(0xe2)
#define PROTO_EVT_ERROR_PHY_RELEASE_BREAK_TIMEOUT 	(0xe3)
#define PROTO_EVT_ERROR_PHY_MAX_BIT_LENGTH_DIFF 	(0xed)
#define PROTO_EVT_ERROR_PHY_SYNC_TIMEOUT 		(0xf0)
#define PROTO_EVT_ERROR_PHY_SYNC_TIMEOUT_BAUD 		(0xf4)
#define PROTO_EVT_ERROR_PHY_SYNC_OUT_OF_RANGE 		(0xf5)
#define PROTO_EVT_ERROR_PHY_SYNC_WAIT_TIMEOUT 		(0xf6)
#define PROTO_EVT_ERROR_PHY_RECEIVE_TIMEOUT 		(0xf7)
#define PROTO_EVT_ERROR_PHY_RECEIVED_BREAK 		(0xf8)
#define PROTO_EVT_ERROR_PHY_OPT_RECEIVE_TIMEOUT 	(0xf9)
#define PROTO_EVT_ERROR_PHY_OPT_RECEIVED_BREAK 		(0xfa)
#define PROTO_EVT_RESULT_PHY_NO_ACTIVITY 		(0xfb)

#endif
