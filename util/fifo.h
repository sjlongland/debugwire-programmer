#ifndef _UTIL_FIFO_H
#define _UTIL_FIFO_H

/*!
 * Simple ring FIFO buffer.
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

/*!
 * FIFO Buffer interface.
 */
struct fifo_t {
	uint8_t* const buffer;	/*!< Buffer storage location */
	uint8_t	total_sz;	/*!< Buffer total size */
	uint8_t stored_sz;	/*!< Buffer usage size */
	uint8_t read_ptr;	/*!< Read pointer location */
	uint8_t write_ptr;	/*!< Write pointer location */
};

/*!
 * Empty the buffer.
 */
static void fifo_empty(struct fifo_t* const fifo) {
	fifo->stored_sz = 0;
	fifo->read_ptr = 0;
	fifo->write_ptr = 0;
}

/*!
 * Initialise the buffer
 */
static void fifo_init(struct fifo_t* const fifo,
		uint8_t* buffer, uint8_t sz) {
	fifo_empty(fifo);
	fifo->buffer = buffer;
	fifo->total_sz = sz;
}

/*!
 * Read a byte from the buffer.  Returns the byte read, or -1 if no
 * data is available.
 */
static int16_t fifo_read_one(struct fifo_t* const fifo) {
	if (!fifo->stored_sz)
		return -1;

	uint8_t byte = fifo->buffer[fifo->read_ptr];
	fifo->stored_sz--;
	fifo->read_ptr = (fifo->read_ptr + 1) % fifo->total_sz;
	return byte;
}

/*!
 * Write a byte to the buffer.  Returns 1 on success,
 * 0 if no space available.
 */
static uint8_t fifo_write_one(struct fifo_t* const fifo, uint8_t byte) {
	if (fifo->stored_sz >= fifo->total_sz)
		return 0;

	fifo->buffer[fifo->write_ptr] = byte;
	fifo->stored_sz++;
	fifo->write_ptr = (fifo->write_ptr + 1) % fifo->total_sz;
	return 1;
}

/*!
 * Read bytes from the buffer
 */
static uint8_t fifo_read(struct fifo_t* const fifo,
		uint8_t* buffer, uint8_t sz) {
	uint8_t count = 0;
	while(sz && (byte >= 0)) {
		*buffer = byte;
		sz--;
		buffer++;
		count++;
		byte = fifo_read_one(fifo);
	}
	return count;
}

/*!
 * Write bytes to the buffer
 */
static uint8_t fifo_write(struct fifo_t* const fifo,
		const uint8_t* buffer, uint8_t sz) {
	uint8_t count = 0;
	while(sz && fifo_write_one(fifo, *buffer)) {
		buffer++;
		sz--;
		count++;
	}
	return count;
}

#endif
