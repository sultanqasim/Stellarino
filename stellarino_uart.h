/*  stellarino_uart.h
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

#ifndef STELLARINO_UART_H
#define STELLARINO_UART_H

#include "stellarino.h"

// Non-standard function for positive integer powers
unsigned long power(unsigned long base, int exp);

void puts(const char * str);	// Will not append newline automatically
char * gets(char * str, int num);	// Reads until newline or num chars

void putc(char c);
void putln(void);

char getc(void);
char peek(void);
char peekBlocking(void);

void puti(long i);
long geti(void);

void putu(unsigned long u, unsigned char digits);
unsigned long getu(unsigned char digits);

void puth(unsigned long h, unsigned char digits);
unsigned long geth(unsigned char digits);

/* Coming soon
void putf(float f, unsigned char digits);
float getf(unsigned char digits);
*/

#endif
