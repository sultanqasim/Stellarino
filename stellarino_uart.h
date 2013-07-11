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

void enableUART(uint8_t UART, unsigned long baudRate);

void puts(const char * str);	// Will not append newline automatically
void UARTputs(uint8_t UART, const char * str);

char * gets(char * str, int num);	// Reads until newline or num chars
char * UARTgets(uint8_t UART, char * str, int num);

void putc(char c);
void UARTputc(uint8_t UART, char c);

void putln(void);
void UARTputln(uint8_t UART);

char getc(void);
char UARTgetc(uint8_t UART);

char peek(void);
char UARTpeek(uint8_t UART);

char peekBlocking(void);
char UARTpeekBlocking(uint8_t UART);

void puti(long i);
void UARTputi(uint8_t UART, long i);

long geti(void);
long UARTgeti(uint8_t UART);

void putu(unsigned long u, uint8_t digits);
void UARTputu(uint8_t UART, unsigned long u, uint8_t digits);

unsigned long getu(uint8_t digits);
unsigned long UARTgetu(uint8_t UART, uint8_t digits);

void puth(unsigned long h, uint8_t digits);
void UARTputh(uint8_t UART, unsigned long h, uint8_t digits);

unsigned long geth(uint8_t digits);
unsigned long UARTgeth(uint8_t UART, uint8_t digits);

void putf(float f, uint8_t decimal);
void UARTputf(uint8_t UART, float f, uint8_t decimal);

float getf();
float UARTgetf(uint8_t UART);

#endif
