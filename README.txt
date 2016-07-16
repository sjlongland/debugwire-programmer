JTAGICE mkII compatible DebugWire debugger
==========================================

This project is an attempt to produce a DebugWire-compatible debugger for use
with `avarice` using the JTAGICE mkII protocol.

At the moment, this is merely a proof of concept.

Hardware
--------

The hardware for now is being prototyped on a Freetronics LeoStick, with the
Arduino firmware erased.  The LeoStick is basically a thumbdrive-sized
development board for the Atmel ATMega32U4 and incorporates a few on-board
peripherals, namely a pushbutton, two RGB LEDs, and a piezo buzzer.

The plan is to make use of the ATMega32U4's UART (pins 1 & 2) via a
non-inverting open-collector buffer to drive the nRESET pin of the target
device.  SPI to the target will be bit-banged (the ATMega32U4 SPI pins are
wired to the ISP port of the LeoStick itself).

Software
--------

We will use the LUFA stack to provide the comms to the host.  Aim is to be
compatible with `avarice` for use with `avr-gdb`.

Protocol
--------

AVR067 (http://www.atmel.com/images/doc2587.pdf) documents the JTAGICE
protocol.

License
-------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program (see COPYING); if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
