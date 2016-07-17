#ifndef _PROTOCOL_COMMAND_H
#define _PROTOCOL_COMMAND_H

/*!
 * JTAGICE mkII Protocol commands.
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

/* AVR067 Section 5.1 */

#define PROTO_CMND_SIGN_OFF			(0x00)
#define PROTO_CMND_GET_SIGN_ON			(0x01)
#define PROTO_CMND_SET_PARAMETER		(0x02)
#define PROTO_CMND_GET_PARAMETER		(0x03)
#define PROTO_CMND_WRITE_MEMORY			(0x04)
#define PROTO_CMND_READ_MEMORY			(0x05)
#define PROTO_CMND_WRITE_PC			(0x06)
#define PROTO_CMND_READ_PC			(0x07)
#define PROTO_CMND_GO				(0x08)
#define PROTO_CMND_SINGLE_STEP			(0x09)
#define PROTO_CMND_FORCED_STOP			(0X0a)
#define PROTO_CMND_RESET			(0X0b)
#define PROTO_CMND_SET_DEVICE_DESCRIPTOR	(0x0c)
#define PROTO_CMND_ERASEPAGE_SPM		(0x0d)
#define PROTO_CMND_GET_SYNC			(0x0f)
#define PROTO_CMND_SELFTEST			(0x10)
#define PROTO_CMND_SET_BREAK			(0x11)
#define PROTO_CMND_GET_BREAK			(0x12)
#define PROTO_CMND_CHIP_ERASE			(0x13)
#define PROTO_CMND_ENTER_PROGMODE		(0x14)
#define PROTO_CMND_LEAVE_PROGMODE		(0x15)
#define PROTO_CMND_CLR_BREAK			(0x1a)
#define PROTO_CMND_RUN_TO_ADDR			(0x1c)
#define PROTO_CMND_SPI_CMD			(0x1d)
#define PROTO_CMND_CLEAR_EVENTS			(0x22)
#define PROTO_CMND_RESTORE_TARGET		(0x23)
#define PROTO_CMND_ISP_PACKET			(0x2f)
#define PROTO_CMND_JTAG_INSTR			(0x24)
#define PROTO_CMND_JTAG_DATA			(0x25)
#define PROTO_CMND_JTAG_SAB_WRITE		(0x28)
#define PROTO_CMND_JTAG_SAB_READ		(0x29)
#define PROTO_CMND_JTAG_BLOCK_READ		(0x2c)
#define PROTO_CMND_JTAG_BLOCK_WRITE		(0x2d)
#define PROTO_CMND_XMEGA_ERASE			(0x34)

#endif
