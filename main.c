/*  Stellarino Demo Programs
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

#include "stellarino.h"

void setup();
void loop();

int main(void) {
	init();
	setup();
	while(1) loop();
}

// LED Blink Demo
void setup() {
	pinMode(PF2, OUTPUT);
}

void loop() {
	digitalWrite(PF2, HIGH);
	delay(500);
	digitalWrite(PF2, LOW);
	delay(500);
}

/* 2-Speed LED Fade Example
#include "stellarino.h"

int main(void) {
	init();
	pinMode(SW1, INPUT_PULLUP);
	pinMode(GREEN_LED, OUTPUT_PWM);
	int a = 0, dir = 1;
	while(1) {
		if (a == 255) dir = -1;
		else if (a == 0) dir = 1;
		analogWrite(GREEN_LED, a);
		a += dir;
		if (digitalRead(SW1)) delay(2);
		else delayMicroseconds(500);
	}
}
*/

/*
// UART Adder Demo Code
#include "stellarino.h"

int main(void) {
	init();
	while(1) {
		puts("Enter numbers:\n");
		long a = geti(), b = geti();
		puti(a);
		puts(" + ");
		puti(b);
		puts(" = ");
		puti(a + b);
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
