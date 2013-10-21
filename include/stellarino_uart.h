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
#include <stdbool.h>

void enableUART(uint8_t UART, unsigned long baudRate);

int16_t UARTgetBufferLevel(uint8_t UART);
bool UARToverflow(uint8_t UART);
void UARTflushReceiveBuffer(uint8_t UART);

// Will not append newline automatically
#define puts(s) UARTputs(0, (s))
void UARTputs(uint8_t UART, const char * str);

// Reads until newline or num chars
#define gets(s, n) UARTgets(0, (s), (n))
char *UARTgets(uint8_t UART, char * str, int num);

#define putc(c) UARTputc(0, (c))
void UARTputc(uint8_t UART, char c);

#define putln() UARTputln(0)
void UARTputln(uint8_t UART);

#define getc() UARTgetc(0)
char UARTgetc(uint8_t UART);

// These return -255 if buffers are empty
#define peek() UARTpeek(0)
int UARTpeek(uint8_t UART);

#define peekBlocking() UARTpeekBlocking(0)
char UARTpeekBlocking(uint8_t UART);

#define puti(i) UARTputi(0, (i))
void UARTputi(uint8_t UART, long i);

#define geti() UARTgeti(0)
long UARTgeti(uint8_t UART);

#define putu(u, d) UARTputu(0, (u), (d))
void UARTputu(uint8_t UART, unsigned long u, uint8_t digits);

#define getu(d) UARTgetu(0, (d))
unsigned long UARTgetu(uint8_t UART, uint8_t digits);

#define puth(h, d) UARTputh(0, (h), (d))
void UARTputh(uint8_t UART, unsigned long h, uint8_t digits);

#define geth(d) UARTgeth(0, (d))
unsigned long UARTgeth(uint8_t UART, uint8_t digits);

#define putf(f, d) UARTputf(0, (f), (d))
void UARTputf(uint8_t UART, float f, uint8_t decimal);

#define getf() UARTgetf(0)
float UARTgetf(uint8_t UART);

#endif
