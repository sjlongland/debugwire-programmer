#ifndef _HARDWARE_USART_H
#define _HARDWARE_USART_H

/*!
 * Low-level USART interface.
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

#define USART_MODE_ASYNC	(0 << 14) /*!< USART async mode */
#define USART_MODE_SYNCS	(1 << 14) /*!< USART sync slave mode */
#define USART_MODE_SYNCM	(3 << 14) /*!< USART sync master mode */
#define USART_MODE_RXEN		(1 << 13) /*!< USART Rx enabled */
#define USART_MODE_TXEN		(1 << 12) /*!< USART Tx enabled */
#define USART_MODE_5DBIT	(0 <<  9) /*!< 5 data bit mode */
#define USART_MODE_6DBIT	(1 <<  9) /*!< 6 data bit mode */
#define USART_MODE_7DBIT	(2 <<  9) /*!< 7 data bit mode */
#define USART_MODE_8DBIT	(3 <<  9) /*!< 8 data bit mode */
#define USART_MODE_2SBIT	(1 <<  8) /*!< 2 stop bit mode */
#define USART_MODE_CPOLF	(1 <<  7) /*!< Clock out on Falling Edge */
#define USART_MODE_NPAR		(0 <<  5) /*!< No parity */
#define USART_MODE_OPAR		(2 <<  5) /*!< Odd parity */
#define USART_MODE_EPAR		(3 <<  5) /*!< Even parity */

/*!
 * Initialise USART
 */
int8_t usart_init(uint32_t baud, uint16_t mode);

/*! FIFO buffer for USART receive data */
extern struct fifo_t usart_fifo_rx;

/*! FIFO buffer for USART transmit data */
extern struct fifo_t usart_fifo_tx;

#endif
