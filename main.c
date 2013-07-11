/*  Stellarino Demo Programs
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

#include "stellarino.h"

short r = 0, g = 85, b = 171, dr = 1, dg = 1, db = 1;

void setup();
void loop();

int main(void) {
    init();
    setup();
    while(1) loop();
}

// 2-Speed LED Colour Fade Demo
void setup() {
    pinMode(RED_LED, OUTPUT_PWM);
    pinMode(GREEN_LED, OUTPUT_PWM);
    pinMode(BLUE_LED, OUTPUT_PWM);
    pinMode(SW1, INPUT_PULLUP);
}

void loop() {
    analogWrite(RED_LED, r);
    analogWrite(GREEN_LED, g);
    analogWrite(BLUE_LED, b);

    if (r >= 255) dr = -1;
    else if (r <= 0) dr = 1;
    r += dr;

    if (g >= 255) dg = -1;
    else if (g <= 0) dg = 1;
    g += dg;

    if (b >= 255) db = -1;
    else if (b <= 0) db = 1;
    b += db;

    if (digitalRead(SW1)) delay(5);
    else delayMicroseconds(1000);
}

/*
// UART Adder Demo Code
#include "stellarino.h"

int main(void) {
    float a, b;

    init();
    while(1) {
        puts("Enter numbers:\n");
        a = getf();
        b = getf();
        putf(a, 2);
        puts(" + ");;
        putf(b, 2);
        puts(" = ");
        putf(a + b, 2);
        putln();
    }
}
*/

/*
// MCP4921 DAC Demo using SPI
// Generates a sawtooth wave, with the DAC connected to SSI0 and the
// LDAC pin on PD0. SPI communication at 16 MHz.
#include "stellarino.h"

void DACWrite(short val);

int main(void) {
    init();
    pinMode(PD0, OUTPUT);
    enableSPI(0, 16, 16000000);
    short a = 0;
    while (1) {
        if (a > 4095) a = 0;
        DACWrite(a);
        a += 5;
        delayMicroseconds(5);
    }
}

void DACWrite(short val) {
    if (val < 0) val = 0;
    else if (val > 4095) val = 4095;
    SPIWrite(0, val + bit16[12] + bit16[13]);

    // Trigger output on DAC
    digitalWrite(PD0, LOW);
    digitalWrite(PD0, HIGH);
}
*/
