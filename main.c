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
#include "hardware/usart.h"

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

/*!
 * Main program entry point. This routine contains the overall
 * program flow, including initial setup of all components and the
 * main program loop.
 */
int main(void)
{
	fifo_init(&target_fifo_rx,
		target_fifo_rx_buffer, sizeof(target_fifo_rx_buffer));
	fifo_init(&target_fifo_tx,
		target_fifo_tx_buffer, sizeof(target_fifo_tx_buffer));
	fifo_init(&host_fifo_rx,
		host_fifo_rx_buffer, sizeof(host_fifo_rx_buffer));
	fifo_init(&host_fifo_tx,
		host_fifo_tx_buffer, sizeof(host_fifo_tx_buffer));

	SetupHardware();
	GlobalInterruptEnable();

	for (;;)
	{
		int16_t in = CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);
		if (in >= 0)
			fifo_write_one(&host_fifo_rx, in);

		in = fifo_read_one(&host_fifo_rx);
		if (in >= 0)
			fifo_write_one(&target_fifo_tx, in);

		in = fifo_read_one(&target_fifo_rx);
		if (in >= 0)
			CDC_Device_SendByte(&VirtualSerial_CDC_Interface, in);
		CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
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
	usart_init(9600,
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
	bool ConfigSuccess = true;

	ConfigSuccess &= CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface);

	//LEDs_SetAllLEDs(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	CDC_Device_ProcessControlRequest(&VirtualSerial_CDC_Interface);
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
}
