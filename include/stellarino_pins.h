/*  stellarino_pins.h
    Copyright (C) 2012-2014 Sultan Qasim Khan

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

#ifndef STELLARINO_PINS_H
#define STELLARINO_PINS_H

#include <stellarino.h>

static const uint8_t bit8[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

static const uint16_t bit16[] =
{
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800,
    0x1000, 0x2000, 0x4000, 0x8000
};

static const uint32_t bit32[] =
{
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800,
    0x1000, 0x2000, 0x4000, 0x8000, 0x10000, 0x20000, 0x40000, 0x80000,
    0x100000, 0x200000, 0x400000, 0x800000, 0x1000000, 0x2000000, 0x4000000,
    0x8000000
};

// The new TM4C129x microcontrollers can have a lot of pins...
typedef enum PinNumber
{
    PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7,
    PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7,
    PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7,
    PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7,
    PE0, PE1, PE2, PE3, PE4, PE5, PE6, PE7,
    PF0, PF1, PF2, PF3, PF4, PF5, PF6, PF7,
    PG0, PG1, PG2, PG3, PG4, PG5, PG6, PG7,
    PH0, PH1, PH2, PH3, PH4, PH5, PH6, PH7,
    PJ0, PJ1, PJ2, PJ3, PJ4, PJ5, PJ6, PJ7,
    PK0, PK1, PK2, PK3, PK4, PK5, PK6, PK7,
    PL0, PL1, PL2, PL3, PL4, PL5, PL6, PL7,
    PM0, PM1, PM2, PM3, PM4, PM5, PM6, PM7,
    PN0, PN1, PN2, PN3, PN4, PN5, PN6, PN7,
    PP0, PP1, PP2, PP3, PP4, PP5, PP6, PP7,
    PQ0, PQ1, PQ2, PQ3, PQ4, PQ5, PQ6, PQ7
} PinNumber;

#if defined(TARGET_TM4C123) || defined(TARGET_LM4F120)
#include <stellarino_tm4c123.h>
#elif defined(TARGET_TM4C129)
#include <stellarino_tm4c129.h>
#else
#error "Unknown target"
#endif

#endif
