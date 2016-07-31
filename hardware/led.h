#ifndef _UTIL_LED_H
#define _UTIL_LED_H

/*!
 * Simple LED state handler.
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

#include <string.h>
#include <stdint.h>
#include "util/timer.h"

#define LED_ACT_NONE	(0 << 0)	/*!< LED unchanged */
#define LED_ACT_TOGGLE	(1 << 0)	/*!< LED toggled */
#define LED_ACT_OFF	(2 << 0)	/*!< LED turned off */
#define LED_ACT_ON	(3 << 0)	/*!< LED turned on */

#define LED_CFG_INVERT	(1 << 7)	/*!< LED is active low */
#define LED_CONFIG_BITS	(0xf0)		/*!< Configuration bits */
#define LED_CONFIG_ACT	(0x0f)		/*!< Action bits */

/*!
 * LED state structure.
 */
struct led_t {
	/*! LED timer */
	struct timer_t		timer;
	/*! LED I/O port */
	volatile uint8_t*	port;
	/*! LED I/O bit */
	uint8_t			bit;
	/*! LED timer period */
	uint8_t			period;
	/*! LED configuration */
	uint8_t			config;
};

/*!
 * Return the immediate state of the LED.
 */
static uint8_t led_get_state(const struct led_t* const led) {
	return (*(led->port) & (led->bit));
}

/*!
 * Set the immediate state of the LED.
 */
static void led_set_state(const struct led_t* const led,
		uint8_t state) {
	if (state == LED_ACT_TOGGLE) {
		*(led->port) ^= (led->bit);
		return;
	}

	if (state == LED_ACT_ON) {
		if (led->config & LED_CFG_INVERT)
			*(led->port) &= ~(led->bit);
		else
			*(led->port) |= (led->bit);
	} else if (state == LED_ACT_OFF) {
		if (led->config & LED_CFG_INVERT)
			*(led->port) |= (led->bit);
		else
			*(led->port) &= ~(led->bit);
	}
}

/*!
 * Cancel any timers on the LED
 */
static void led_stop(struct led_t* const led) {
	led->period = 0;
	timer_stop(&(led->timer));
}

/*!
 * Initialise a LED
 */
static void led_init(struct led_t* const led, volatile uint8_t* port,
		volatile uint8_t* ddr, uint8_t bit, uint8_t config) {
	memset(led, 0, sizeof(struct led_t));
	led->port = port;
	led->bit = bit;
	*ddr |= bit;
	led_set_state(led, LED_ACT_OFF);
}

/*!
 * Set up the LED to pulse for a given period.
 */
static void led_pulse(struct led_t* const led,
		uint8_t now, uint8_t delay, uint8_t after,
		uint8_t repeat_delay) {
	/* Set flash period, set the timeout action */
	led->period = repeat_delay;
	led->config = (led->config & LED_CONFIG_BITS)
		    | (after & LED_CONFIG_ACT);

	/* Start the timer */
	timer_start(&(led->timer), delay);

	/* Set the action asked for now. */
	led_set_state(led, now);
}

/*!
 * Handle LED timer events.
 */
static void led_tick(struct led_t* const led) {
	timer_tick(&(led->timer));
	if (led->timer.flags & TIMER_FLAG_EXPIRED) {
		/* Is there a period set? */
		if (led->period)
			/* Re-start the timer */
			timer_start(&(led->timer), led->period);
		else
			/* ACK expired flag */
			timer_ack(&(led->timer));
		/* Perform action for timeout */
		led_set_state(led, led->config & LED_CONFIG_ACT);
	}
}

#endif
