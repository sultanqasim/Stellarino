/*  LED Fade Demo Program
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
// UART Test code
int main() {
	init();
	while(1) {
		puts("Enter numbers:\n");
		unsigned long a = geth(10), b = geth(10);
		putu(a, 10);
		puts(" + ");
		putu(b, 10);
		puts(" = ");
		putu(a + b, 10);
		putln();
	}
}
*/
