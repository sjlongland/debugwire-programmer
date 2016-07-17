#ifndef _PROTOCOL_RESPONSE_H
#define _PROTOCOL_RESPONSE_H

/*!
 * JTAGICE mkII Protocol responses.
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

/* AVR067 Section 5.2 */

#define PROTO_RSP_OK			(0x80)
#define PROTO_RSP_FAILED		(0xa0)
#define PROTO_RSP_ILLEGAL_PARAMETER	(0xa1)
#define PROTO_RSP_PARAMETER		(0x81)
#define PROTO_RSP_ILLEGAL_MEMORY_TYPE	(0xa2)
#define PROTO_RSP_ILLEGAL_MEMORY_RANGE	(0xa3)
#define PROTO_RSP_MEMORY		(0x82)
#define PROTO_RSP_GET_BREAK		(0x83)
#define PROTO_RSP_ILLEGAL_EMULATOR_MODE	(0xa4)
#define PROTO_RSP_ILLEGAL_MCU_STATE	(0xa5)
#define PROTO_RSP_PC			(0x84)
#define PROTO_RSP_SELFTEST		(0x85)
#define PROTO_RSP_SPI_DATA		(0x88)
#define PROTO_RSP_ILLEGAL_COMMAND	(0xaa)
#define PROTO_RSP_ILLEGAL_VALUE		(0xa6)
#define PROTO_RSP_SIGN_ON		(0x86)
#define PROTO_RSP_ILLEGAL_BREAKPOINT	(0xa8)
#define PROTO_RSP_ILLEGAL_JTAG_ID	(0xa9)
#define PROTO_RSP_NO_TARGET_POWER	(0xab)
#define PROTO_RSP_DEBUGWIRE_SYNC_FAILED	(0xac)
#define PROTO_RSP_ILLEGAL_POWER_STATE	(0xad)
#define PROTO_RSP_SCAN_CHAIN_READ	(0x87)

#endif
