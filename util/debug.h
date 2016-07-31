#ifndef _UTIL_DEBUG_H
#define _UTIL_DEBUG_H

/*!
 * Debugging aids.
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

#ifdef DEBUG_CONSOLE
#include <stdio.h>
/*!
 * Flag: Indicates whether the debug console is ready to receive data.
 */
const extern uint8_t debug_console_ready;

/*!
 * Stream for debugging data.
 */
extern FILE debug_stream;
#endif

#endif
