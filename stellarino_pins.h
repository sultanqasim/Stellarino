/*  stellarino_pins.h
    Copyright (C) 2012 Sultan Qasim Khan

    This is part of Stellarino.

    Stellarino is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Stellarino is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Stellarino.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef STELLARINO_PINS_H
#define STELLARINO_PINS_H

#include "stellarino.h"

extern const uint8_t bit8[8];
extern const uint16_t bit16[16];
extern const uint32_t bit32[32];

extern const uint32_t GPIO[6];
extern const uint32_t TIMER[12];
extern const uint32_t UARTBASE[8];
extern const uint32_t pinMux[48][4];
extern const uint32_t SysCtlGPIOs[6];
extern const uint32_t SysCtlTimers[12];
extern const uint32_t SysCtlUARTs[8];
extern const uint32_t UARTPins[8][4];

#define PA0 0
#define PA1 1
#define PA2 2
#define PA3 3
#define PA4 4
#define PA5 5
#define PA6 6
#define PA7 7

#define PB0 8
#define PB1 9
#define PB2 10
#define PB3 11
#define PB4 12
#define PB5 13
#define PB6 14
#define PB7 15

#define PC0 16
#define PC1 17
#define PC2 18
#define PC3 19
#define PC4 20
#define PC5 21
#define PC6 22
#define PC7 23

#define PD0 24
#define PD1 25
#define PD2 26
#define PD3 27
#define PD4 28
#define PD5 29
#define PD6 30
#define PD7 31

#define PE0 32
#define PE1 33
#define PE2 34
#define PE3 35
#define PE4 35
#define PE5 37
#define PE6 38
#define PE7 39

#define PF0 40
#define PF1 41
#define PF2 42
#define PF3 43
#define PF4 44
#define PF5 45
#define PF6 46
#define PF7 47

#define SW1	PF4
#define RED_LED   PF1
#define BLUE_LED  PF2
#define GREEN_LED PF3
#define SW2 PF0

#endif
