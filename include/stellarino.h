/*  stellarino.h
    Copyright (C) 2012-2013 Sultan Qasim Khan

    This is part of Stellarino.

    Stellarino is free software; you can redistribute it and/or modify it
    under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    Stellarino is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
    License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Stellarino. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef STELLARINO_H
#define STELLARINO_H

#include <stdint.h>
#include <stdbool.h>

#ifndef PART_TM4C123GH6PM
#define PART_TM4C123GH6PM
#define TARGET_IS_BLIZZARD_RA1
#endif

#include "inc/tm4c123gh6pm.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/rom.h"
#include "driverlib/interrupt.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/adc.h"
#include "driverlib/uart.h"
#include "driverlib/ssi.h"

#include <stellarino_pins.h>
#include <stellarino_timer.h>
#include <stellarino_uart.h>
#include <stellarino_spi.h>

#define INPUT 0
#define OUTPUT 1
#define OUTPUT_OD 2
#define INPUT_PULLUP 3
#define INPUT_PULLDOWN 4
#define INPUT_ANALOG 5
#define OUTPUT_PWM 6
#define OUTPUT_PWM_OD 7
#define OUTPUT_SERVO 8

#define HIGH 1
#define LOW 0

#define PWMFREQ 500	// PWM Frequency in Hz, when using analogWrite

// Linearly maps i from range (imin, imax) to range (omin, omax)
#define map(i, imin, imax, omin, omax) ( (((i)-(imin))/((imax)-(imin)))*((omax)-(omin)) + (omin) )

void init(void);	// Must be called in main(), enables everything

void pinMode(unsigned char pin, unsigned char mode);
int digitalRead(unsigned char pin);
int analogRead(unsigned char pin);
void digitalWrite(unsigned char pin, short val);
void analogWrite(unsigned char pin, short val);
void servoWrite(unsigned char pin, short val);
void pwmWrite(unsigned char pin, float frequency, float duty);
unsigned long pulseIn(unsigned char pin, short val, unsigned long timeout);

#endif
