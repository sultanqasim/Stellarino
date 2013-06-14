/*  stellarino_timer.c
    Copyright (C) 2012-2013 Sultan Qasim Khan

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

#include "stellarino_timer.h"

void delay(unsigned long nTime) {
    ROM_TimerLoadSet(WTIMER5_BASE, TIMER_A, nTime * 10);

    ROM_TimerIntEnable(WTIMER5_BASE, TIMER_TIMA_TIMEOUT);
    ROM_TimerEnable(WTIMER5_BASE, TIMER_A);
    ROM_SysCtlSleep();

    ROM_TimerIntDisable(WTIMER5_BASE, TIMER_TIMA_TIMEOUT);
    ROM_TimerDisable(WTIMER5_BASE, TIMER_A);
}

void delayInterrupt(void) {
    ROM_TimerIntClear(WTIMER5_BASE, TIMER_TIMA_TIMEOUT);
}

void delayMicroseconds(unsigned long nTime) {
    if (!nTime) return;
    else if (nTime < 3) SysCtlDelay(12 * nTime);	// empirical
    else {
        // Compensates for latency
        unsigned long endtime = micros() + nTime - 1;
        while(micros() < endtime);
    }
}

void resetMillis(void) {
    ROM_TimerLoadSet(WTIMER4_BASE, TIMER_A, 4294967295);
}

unsigned long millis(void) {
    return (4294967295 - ROM_TimerValueGet(WTIMER4_BASE, TIMER_A)) / 2;
}

void resetMicros(void) {
    ROM_TimerLoadSet(WTIMER4_BASE, TIMER_B, 4294967295);
}

unsigned long micros(void) {
    return 4294967295 - ROM_TimerValueGet(WTIMER4_BASE, TIMER_B);
}
