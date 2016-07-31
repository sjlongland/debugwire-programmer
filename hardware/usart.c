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

#include <avr/interrupt.h>
#include <avr/io.h>
#include "hardware/usart.h"
#include "util/debug.h"

#define F_CPU_DIV_8	(F_CPU/8)
#define F_CPU_DIV_16	(F_CPU/16)

/*! Compute approximate ratio between f_cpu and baud */
#define UBRR_VAL(f_cpu, baud)	\
	(((f_cpu) + ((baud)/2))/(baud))

/*! Compute effective baud rate */
#define UBRR_ERR(f_cpu, div, baud)	\
	(((f_cpu) * (div)) - (baud))

/*! Duplex control state register */
static uint8_t usart_duplex = 0;
#define DUPLEX_RX_EN		(1 << 0)	/*!< Receiver enabled */
#define DUPLEX_TX_EN		(1 << 1)	/*!< Transmitter enabled */
#define DUPLEX_STATE_OFF	(0 << 2)	/*!< Transceiver offline */
#define DUPLEX_STATE_RX		(1 << 2)	/*!< Receive mode */
#define DUPLEX_STATE_TX		(2 << 2)	/*!< Transmit mode */
#define DUPLEX_STATE_FULL	(3 << 2)	/*!< Full duplex */

/*! Duplex direction enable bit mask */
#define DUPLEX_EN_MASK		(DUPLEX_RX_EN|DUPLEX_TX_EN)
/*! Duplex direction state bit mask */
#define DUPLEX_STATE_MASK	(3 << 2)

/*! Handler for transmit data */
static void usart_txfifo_evth(struct fifo_t* const fifo, uint8_t events);

#define USART1B_RX		((1 << RXCIE1) | (1 << RXEN1))
#define USART1B_TX		((1 << TXCIE1) | (1 << TXEN1) \
					| (1 << UDRIE1))

/*! Update USART direction settings according to usart_duplex */
static void usart_update_dir() {
#ifdef DEBUG_USART
	if (debug_console_ready)
		fprintf(&debug_stream,
				"%s: state=%02x en=%02x\r\n",
				__func__,
				usart_duplex & DUPLEX_STATE_MASK,
				usart_duplex & DUPLEX_EN_MASK);
#endif
	switch(usart_duplex & DUPLEX_STATE_MASK) {
		case DUPLEX_STATE_FULL:
			UCSR1B |= USART1B_RX | USART1B_TX;
			break;
		case DUPLEX_STATE_TX:
			UCSR1B |= USART1B_TX;
			UCSR1B &= ~USART1B_RX;
			break;
		case DUPLEX_STATE_RX:
			UCSR1B |= USART1B_RX;
			UCSR1B &= ~USART1B_TX;
			break;
		default:
			UCSR1B &= ~(USART1B_RX | USART1B_TX);
	}
}
/*! Set the new USART direction */
static void usart_set_dir(uint8_t dir) {
	usart_duplex	= (usart_duplex & ~DUPLEX_STATE_MASK)
			| (dir & DUPLEX_STATE_MASK);
	usart_update_dir();
}

/*!
 * Initialise USART
 */
int8_t usart_init(uint32_t baud, uint16_t mode) {
	/* Compute baud rate prescaler, try both /8 and /16 modes */
	uint32_t ubrr_div16 = UBRR_VAL(F_CPU_DIV_16, baud);
	uint32_t ubrr_div8 = UBRR_VAL(F_CPU_DIV_8, baud);

	/* If we have neither option, fail now */
	if (!ubrr_div16 && !ubrr_div8)
		return -1;

	/* If we have both options, pick the closest */
	if (ubrr_div16 && ubrr_div8) {
		int32_t err_div16 = UBRR_ERR(F_CPU_DIV_16,
				ubrr_div16, baud);
		int32_t err_div8 = UBRR_ERR(F_CPU_DIV_8,
				ubrr_div8, baud);
		if (err_div16 < 0)
			err_div16 = -err_div16;
		if (err_div8 < 0)
			err_div8 = -err_div8;
		if (err_div16 > err_div8)
			ubrr_div16 = 0;	/* Ignore /16 */
	}

	/* Shut everything down first */
	UCSR1B = 0;

	/* Figure out the duplex settings */
	usart_duplex 	= ((mode & USART_MODE_RXEN) ? DUPLEX_RX_EN : 0)
			| ((mode & USART_MODE_TXEN) ? DUPLEX_TX_EN : 0);
	/* Are we in half-duplex mode? */
	if (mode & USART_MODE_HDUPLEX) {
		/*
		 * We are.  If the receiver is enabled, go to receive
		 * mode, otherwise, turn everything off.
		 */
		usart_duplex |= ((usart_duplex & DUPLEX_RX_EN)
				? DUPLEX_STATE_RX
				: DUPLEX_STATE_OFF);
	} else {
		/* Set the bits according to user selection */
		usart_duplex |= (usart_duplex & DUPLEX_EN_MASK) << 2;
	}

	/* Set up IO pins */
	DDRD &= ~(1 << 2);	/* PD2 == RX; input */
	DDRD |= (1 << 3);	/* PD3 == TX; output */

	UCSR1C	= (((mode >> 14) & 0x03) << UMSEL10)	/* USART mode */
		| (((mode >> 5) & 0x03) << UPM10)	/* Parity mode */
		| (((mode >> 8) & 0x01) << USBS1)	/* Stop bits */
		| (((mode >> 9) & 0x03) << UCSZ10)	/* Frame size */
		| (((mode >> 7) & 0x01) << UCPOL1);	/* SCK polarity */

	UBRR1 = (ubrr_div16) ? ubrr_div16 : ubrr_div8;
	UCSR1A = (ubrr_div16) ? 0 : U2X1;
	UCSR1B |= ((mode >> 11) & 0x01) << UCSZ12;
	usart_update_dir();

	/* Set up FIFO handler */
	usart_fifo_tx.consumer_evth = usart_txfifo_evth;
	usart_fifo_tx.consumer_evtm = FIFO_EVT_NEW;
	return 0;
}

static void usart_send_next() {
	/* Ready to send next byte */
	int16_t byte = fifo_read_one(&usart_fifo_tx);
	if (byte >= 0) {
		UDR1 = byte;
		if (&usart_led_tx)
			led_pulse(&usart_led_tx, LED_ACT_ON,
					usart_led_delay, LED_ACT_OFF, 0);
	}
}

static void usart_txfifo_evth(struct fifo_t* const fifo, uint8_t events) {
	if (events & FIFO_EVT_NEW) {
		if (!(usart_duplex & DUPLEX_TX_EN)) {
			/* Not enabled for transmit, silently discard! */
			fifo_read_one(&usart_fifo_tx);
			return;
		} else if ((usart_duplex & DUPLEX_STATE_MASK)
					== DUPLEX_STATE_RX) {
			/* We are presently in receive mode. */
			usart_set_dir(DUPLEX_STATE_TX);
		}
		/* Kick the transmit done interrupt */
		UCSR1A |= (1 << TXC1);
	}
}

ISR(USART1_RX_vect) {
	fifo_write_one(&usart_fifo_rx, UDR1);
	if (&usart_led_rx)
		led_pulse(&usart_led_rx, LED_ACT_ON,
				usart_led_delay, LED_ACT_OFF, 0);
}
ISR(USART1_TX_vect) {
	usart_send_next();
}
ISR(USART1_UDRE_vect) {
	if (fifo_peek_one(&usart_fifo_tx)) {
		/* We've got more */
		usart_send_next();
	} else {
		/* No more to send. */
		switch (usart_duplex & DUPLEX_STATE_MASK) {
			case DUPLEX_STATE_FULL:
				/* We're in full-duplex mode, do nothing */
				return;
			case DUPLEX_STATE_TX:
				/* We're in half-duplex transmit */
				if (usart_duplex & DUPLEX_RX_EN) {
					/* Go to receive mode */
					usart_set_dir(DUPLEX_STATE_RX);
				} else {
					/* Turn off transmitter */
					usart_set_dir(DUPLEX_STATE_OFF);
				}
			default:
				/* Unknown state, turn off */
				usart_set_dir(DUPLEX_STATE_OFF);
		}
	}
}
