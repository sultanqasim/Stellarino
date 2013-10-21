/*  stellarino_timer.h
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

#ifndef STELLARINO_TIMER_H
#define STELLARINO_TIMER_H

#include <stellarino.h>

// Use WTIMER5 TIMER_A
void delay(unsigned long nTime);
void delayInterrupt(void);

void delayMicroseconds(unsigned long nTime);

void resetMillis(void);
unsigned long millis(void);

void resetMicros(void);
unsigned long micros(void);

#endif
