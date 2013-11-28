/*  stellarino_uart.h
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

#ifndef STELLARINO_UART_H
#define STELLARINO_UART_H

#include <stellarino.h>

void enableUART(uint8_t UART, unsigned long baudRate);

int16_t UARTgetBufferLevel(uint8_t UART);
bool UARToverflow(uint8_t UART);
void UARTflushReceiveBuffer(uint8_t UART);

void UARTputc(uint8_t UART, char c);
char UARTgetc(uint8_t UART);
int UARTpeek(uint8_t UART);
char UARTpeekBlocking(uint8_t UART);

void UARTputln(uint8_t UART);

// Will not append newline automatically
void UARTputs(uint8_t UART, const char * str);

// Reads until newline or num chars
char *UARTgets(uint8_t UART, char * str, int num);

void UARTputi(uint8_t UART, long i);
long UARTgeti(uint8_t UART);

void UARTputu(uint8_t UART, unsigned long u, uint8_t digits);
unsigned long UARTgetu(uint8_t UART, uint8_t digits);

void UARTputh(uint8_t UART, unsigned long h, uint8_t digits);
unsigned long UARTgeth(uint8_t UART, uint8_t digits);

void UARTputf(uint8_t UART, float f, uint8_t decimal);
float UARTgetf(uint8_t UART);

// Some of these macros have names that conflict with stdio.h
// You can #define PERMIT_STDIO to avoid the conflict
#ifndef PERMIT_STDIO

#define puts(s) UARTputs(0, (s))
#define gets(s, n) UARTgets(0, (s), (n))
#define putc(c) UARTputc(0, (c))
#define putln() UARTputln(0)
#define getc() UARTgetc(0)
#define peek() UARTpeek(0)
#define peekBlocking() UARTpeekBlocking(0)
#define puti(i) UARTputi(0, (i))
#define geti() UARTgeti(0)
#define putu(u, d) UARTputu(0, (u), (d))
#define getu(d) UARTgetu(0, (d))
#define puth(h, d) UARTputh(0, (h), (d))
#define geth(d) UARTgeth(0, (d))
#define putf(f, d) UARTputf(0, (f), (d))
#define getf() UARTgetf(0)

#endif

#endif
