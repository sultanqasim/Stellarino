/*  stellarino_uart.h
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

#ifndef STELLARINO_UART_H
#define STELLARINO_UART_H

#include "stellarino.h"

// Non-standard function for positive integer powers
unsigned long power(unsigned long base, int exp);

void puts(const char * str);	// Will not append newline automatically
void UARTputs(unsigned char UART, const char * str);

char * gets(char * str, int num);	// Reads until newline or num chars
char * UARTgets(unsigned char UART, char * str, int num);

void putc(char c);
void UARTputc(unsigned char UART, char c);

void putln(void);
void UARTputln(unsigned char UART);

char getc(void);
char UARTgetc(unsigned char UART);

char peek(void);
char UARTpeek(unsigned char UART);

char peekBlocking(void);
char UARTpeekBlocking(unsigned char UART);

void puti(long i);
void UARTputi(unsigned char UART, long i);

long geti(void);
long UARTgeti(unsigned char UART);

void putu(unsigned long u, unsigned char digits);
void UARTputu(unsigned char UART, unsigned long u, unsigned char digits);

unsigned long getu(unsigned char digits);
unsigned long UARTgetu(unsigned char UART, unsigned char digits);

void puth(unsigned long h, unsigned char digits);
void UARTputh(unsigned char UART, unsigned long h, unsigned char digits);

unsigned long geth(unsigned char digits);
unsigned long UARTgeth(unsigned char UART, unsigned char digits);

/* Coming soon
void putf(float f, unsigned char digits);
float getf(unsigned char digits);
*/

#endif
