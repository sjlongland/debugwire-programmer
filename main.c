/*!
 * DebugWire programmer firmware main entrypoint.
 *
 * (C) Stuart Longland, 2016.
 * Based on VirtualSerial demo, (C) Dean Camera, 2016.
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

#include "main.h"
#include "util/fifo.h"
#include "hardware/led.h"
#include "hardware/usart.h"

#ifndef DEBUG_CONSOLE
/*!
 * LUFA CDC Class driver interface configuration and state information. This structure is
 * passed to all CDC Class driver functions, so that multiple instances of the same class
 * within a device can be differentiated from one another.
 */
USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface =
	{
		.Config =
			{
				.ControlInterfaceNumber   = INTERFACE_ID_CDC_CCI,
				.DataINEndpoint           =
					{
						.Address          = CDC_TX_EPADDR,
						.Size             = CDC_TXRX_EPSIZE,
						.Banks            = 1,
					},
				.DataOUTEndpoint =
					{
						.Address          = CDC_RX_EPADDR,
						.Size             = CDC_TXRX_EPSIZE,
						.Banks            = 1,
					},
				.NotificationEndpoint =
					{
						.Address          = CDC_NOTIFICATION_EPADDR,
						.Size             = CDC_NOTIFICATION_EPSIZE,
						.Banks            = 1,
					},
			},
	};
#else
USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface =
	{
		.Config =
			{
				.ControlInterfaceNumber   = INTERFACE_ID_CDC1_CCI,
				.DataINEndpoint           =
					{
						.Address          = CDC1_TX_EPADDR,
						.Size             = CDC_TXRX_EPSIZE,
						.Banks            = 1,
					},
				.DataOUTEndpoint =
					{
						.Address          = CDC1_RX_EPADDR,
						.Size             = CDC_TXRX_EPSIZE,
						.Banks            = 1,
					},
				.NotificationEndpoint =
					{
						.Address          = CDC1_NOTIFICATION_EPADDR,
						.Size             = CDC_NOTIFICATION_EPSIZE,
						.Banks            = 1,
					},
			},
	};

USB_ClassInfo_CDC_Device_t debug_console_cdc =
	{
		.Config =
			{
				.ControlInterfaceNumber   = INTERFACE_ID_CDC2_CCI,
				.DataINEndpoint           =
					{
						.Address          = CDC2_TX_EPADDR,
						.Size             = CDC_TXRX_EPSIZE,
						.Banks            = 1,
					},
				.DataOUTEndpoint =
					{
						.Address          = CDC2_RX_EPADDR,
						.Size             = CDC_TXRX_EPSIZE,
						.Banks            = 1,
					},
				.NotificationEndpoint =
					{
						.Address          = CDC2_NOTIFICATION_EPADDR,
						.Size             = CDC_NOTIFICATION_EPSIZE,
						.Banks            = 1,
					},

			},
	};
uint8_t debug_console_ready = 0;
#endif

/* LEDs */
static struct led_t led1_r __attribute__((nocommon));
static struct led_t led1_g __attribute__((nocommon));
static struct led_t led2_r __attribute__((nocommon));
extern struct led_t usart_led_tx __attribute__((alias ("led2_r")));
static struct led_t led2_g __attribute__((nocommon));
extern struct led_t usart_led_rx __attribute__((alias ("led2_g")));
static struct led_t led2_b __attribute__((nocommon));
const uint8_t usart_led_delay = 50;

/*
 * FIFO buffers for target communications.
 */
static struct fifo_t target_fifo_rx __attribute__((nocommon));
static uint8_t target_fifo_rx_buffer[128];
extern struct fifo_t usart_fifo_rx __attribute__((alias ("target_fifo_rx")));
extern struct fifo_t proto_target_uart_rx __attribute__((alias ("target_fifo_rx")));

static struct fifo_t target_fifo_tx __attribute__((nocommon));
static uint8_t target_fifo_tx_buffer[128];
extern struct fifo_t usart_fifo_tx __attribute__((alias ("target_fifo_tx")));
extern struct fifo_t proto_target_uart_tx __attribute__((alias ("target_fifo_tx")));

/*
 * FIFO buffers for host communications.
 */
static struct fifo_t host_fifo_rx __attribute__((nocommon));
static uint8_t host_fifo_rx_buffer[128];
extern struct fifo_t proto_host_uart_rx __attribute__((alias ("host_fifo_rx")));
static struct fifo_t host_fifo_tx __attribute__((nocommon));
static uint8_t host_fifo_tx_buffer[128];
extern struct fifo_t proto_host_uart_tx __attribute__((alias ("host_fifo_tx")));

/* Timer demo */
struct timer_t demo_timer;

/*!
 * Main program entry point. This routine contains the overall
 * program flow, including initial setup of all components and the
 * main program loop.
 */
int main(void)
{
	led_init(&led1_r, &PORTD, &DDRD, 1 << 5, LED_CFG_INVERT);
	led_init(&led1_g, &PORTB, &DDRB, 1 << 0, LED_CFG_INVERT);
	led_init(&led2_r, &PORTC, &DDRC, 1 << 7, 0);
	led_init(&led2_g, &PORTB, &DDRB, 1 << 5, 0);
	led_init(&led2_b, &PORTB, &DDRB, 1 << 6, 0);

	led_set_state(&led2_b, LED_ACT_ON);

	fifo_init(&target_fifo_rx,
		target_fifo_rx_buffer, sizeof(target_fifo_rx_buffer));
	fifo_init(&target_fifo_tx,
		target_fifo_tx_buffer, sizeof(target_fifo_tx_buffer));
	fifo_init(&host_fifo_rx,
		host_fifo_rx_buffer, sizeof(host_fifo_rx_buffer));
	fifo_init(&host_fifo_tx,
		host_fifo_tx_buffer, sizeof(host_fifo_tx_buffer));

	timer_start(&demo_timer, 100);
	usart_init(9600, USART_MODE_ASYNC | USART_MODE_RXEN
			| USART_MODE_TXEN | USART_MODE_8DBIT
			| USART_MODE_NPAR);

	SetupHardware();
	proto_init();

	GlobalInterruptEnable();

	for (;;)
	{
		int16_t in = CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);
		if (in >= 0) {
			/* Indicate received data from host, push to FIFO */
			led_pulse(&led1_g, LED_ACT_OFF, 50, LED_ACT_ON, 0);
			fifo_write_one(&host_fifo_rx, in);
		}
#ifdef DEBUG_CONSOLE
		/* Read and echo back */
		in = CDC_Device_ReceiveByte(&debug_console_cdc);
		if ((in >= 0) && debug_console_ready)
			CDC_Device_SendByte(&debug_console_cdc, in);
#endif

		in = fifo_read_one(&host_fifo_rx);
		if (in >= 0) {
			fifo_write_one(&target_fifo_tx, in);
		}

		in = fifo_read_one(&target_fifo_rx);
		if (in >= 0) {
			fifo_write_one(&host_fifo_tx, in);
		}

		in = fifo_read_one(&host_fifo_tx);
		if (in >= 0) {
			/* Indicate sent data to host, push to host */
			led_pulse(&led1_r, LED_ACT_OFF, 50, LED_ACT_ON, 0);
			CDC_Device_SendByte(&VirtualSerial_CDC_Interface, in);
		}
		CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
#ifdef DEBUG_CONSOLE
		CDC_Device_USBTask(&debug_console_cdc);
#endif
		USB_USBTask();
	}
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
#if (ARCH == ARCH_AVR8)
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);
#elif (ARCH == ARCH_XMEGA)
	/* Start the PLL to multiply the 2MHz RC oscillator to 32MHz and switch the CPU core to run from it */
	XMEGACLK_StartPLL(CLOCK_SRC_INT_RC2MHZ, 2000000, F_CPU);
	XMEGACLK_SetCPUClockSource(CLOCK_SRC_PLL);

	/* Start the 32MHz internal RC oscillator and start the DFLL to increase it to 48MHz using the USB SOF as a reference */
	XMEGACLK_StartInternalOscillator(CLOCK_SRC_INT_RC32MHZ);
	XMEGACLK_StartDFLL(CLOCK_SRC_INT_RC32MHZ, DFLL_REF_INT_USBSOF, F_USB);

	PMIC.CTRL = PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;
#endif

	/* Hardware Initialization */
	USB_Init();

	/* Timer1 initialisation */
	OCR1A =		/* Output Compare Register A = Interrupt frequency */
			F_CPU / 64 / 100;	/* 100 Hz */
	TCCR1A =	/* Control Register A */
			/* OC0A disconnected */
			(0 << COM1A0)
			/* OC0B disconnected */
		|	(0 << COM1B0)
			/* WGM[3:0] = CTC (0x03); set WGM[1:0] here */
		|	(2 << WGM00);
	TCCR1B =	/* Control Register B */
			/* Disable input noise capture */
			(0 << ICNC1)
			/* Input Capture Edge: don't care */
		|	(0 << ICES1)
			/* Clear WGM[3:2] */
		|	(0 << WGM12)
			/* Clock source: clk_io/64 = 0x03 */
		|	(3 << CS00);
	TIMSK1	=	/* Interrupt mask; enable overflow interrupt */
			(1 << TOIE1);
}

void proto_target_baud(uint32_t rate) {
	usart_init(rate,
		USART_MODE_ASYNC | USART_MODE_RXEN | USART_MODE_TXEN
		| USART_MODE_8DBIT | USART_MODE_NPAR);
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
	fifo_empty(&target_fifo_tx);
	fifo_empty(&target_fifo_rx);
	fifo_empty(&host_fifo_tx);
	fifo_empty(&host_fifo_rx);
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
	fifo_empty(&target_fifo_tx);
	fifo_empty(&target_fifo_rx);
	fifo_empty(&host_fifo_tx);
	fifo_empty(&host_fifo_rx);
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface);
#ifdef DEBUG_CONSOLE
	CDC_Device_ConfigureEndpoints(&debug_console_cdc);
#endif
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	CDC_Device_ProcessControlRequest(&VirtualSerial_CDC_Interface);
#ifdef DEBUG_CONSOLE
	CDC_Device_ProcessControlRequest(&debug_console_cdc);
#endif
}

/** CDC class driver callback function the processing of changes to the virtual
 *  control lines sent from the host..
 *
 *  \param[in] CDCInterfaceInfo  Pointer to the CDC class interface configuration structure being referenced
 */
void EVENT_CDC_Device_ControLineStateChanged(USB_ClassInfo_CDC_Device_t *const CDCInterfaceInfo)
{
	/* You can get changes to the virtual CDC lines in this callback; a common
	   use-case is to use the Data Terminal Ready (DTR) flag to enable and
	   disable CDC communications in your application when set to avoid the
	   application blocking while waiting for a host to become ready and read
	   in the pending data from the USB endpoints.
	*/
	bool HostReady = (CDCInterfaceInfo->State.ControlLineStates.HostToDevice & CDC_CONTROL_LINE_OUT_DTR) != 0;
#ifdef DEBUG_CONSOLE
	if (CDCInterfaceInfo == &debug_console_cdc) {
		debug_console_ready = HostReady ? 1 : 0;
	}
#endif
}

ISR(TIMER1_OVF_vect) {
	/* Tick the LEDs: TODO: put in timer interrupt */
	led_tick(&led1_r);
	led_tick(&led1_g);
	led_tick(&led2_r);
	led_tick(&led2_g);
	led_tick(&led2_b);

	timer_tick(&demo_timer);
}
