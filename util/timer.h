#ifndef _UTIL_TIMER_H
#define _UTIL_TIMER_H

/*!
 * Simple interrupt-driven polling timer.  The timer is started by
 * presetting the counter to the number of ticks and setting the ACTIVE
 * flag.
 *
 * The timer_tick function is then called from a timer interrupt, and
 * decrements the timer by one each tick.  When it reaches zero, the
 * EXPIRED flag is set and the ACTIVE flag is cleared.
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

#define TIMER_FLAG_ACTIVE	(1 << 0)	/*!< Timer is active */
#define TIMER_FLAG_EXPIRED	(1 << 1)	/*!< Timer has expired */

/*!
 * Timer state machine.
 */
struct timer_t {
	uint8_t counter;	/*!< Tick counter */
	uint8_t flags;		/*!< Timer flags */
};

/*!
 * Start the timer ticking.  If counter is non-zero, the counter value
 * is reset to the given value.
 */
static void timer_start(struct timer_t* const timer, uint8_t counter) {
	if (counter)
		timer->counter = counter;

	if (timer->counter) {
		timer->flags &= ~TIMER_FLAG_EXPIRED;
		timer->flags |= TIMER_FLAG_ACTIVE;
	} else {
		timer->flags |= TIMER_FLAG_EXPIRED;
		timer->flags &= ~TIMER_FLAG_ACTIVE;
	}
}

/*!
 * Stop/pause the timer.  Counter is left unchanged.
 */
static void timer_stop(struct timer_t* const timer) {
	timer->flags &= ~TIMER_FLAG_ACTIVE;
}

/*! Tick down the timer */
static void timer_tick(struct timer_t* const timer) {
	if ((timer->flags & TIMER_FLAG_ACTIVE) && timer->counter) {
		timer->counter--;
		if (!timer->counter) {
			timer->flags |= TIMER_FLAG_EXPIRED;
			timer->flags &= ~TIMER_FLAG_ACTIVE;
		}
	}
}

#endif
