/*  stellarino_uart.c
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

#include "stellarino_uart.h"

char peekedChar[8], peeked[8] = {0};

static unsigned long power(unsigned long base, int exp) {
    int res = 1;
    int i;
    for (i = 0; i < exp; i++) res *= base;
    return res;
}

void enableUART(unsigned char UART, unsigned long baudRate) {
    ROM_SysCtlPeripheralEnable(SysCtlGPIOs[UARTPins[UART][0]/8]);
    ROM_SysCtlPeripheralSleepEnable(SysCtlGPIOs[UARTPins[UART][0]/8]);
    ROM_SysCtlPeripheralEnable(SysCtlUARTs[UART]);
    ROM_GPIOPinConfigure(UARTPins[UART][2]);
    ROM_GPIOPinConfigure(UARTPins[UART][3]);
    ROM_GPIOPinTypeUART(GPIO[UARTPins[UART][0]/8],
            bit8[UARTPins[UART][0] % 8] | bit8[UARTPins[UART][1] % 8]);
    ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), baudRate, (UART_CONFIG_WLEN_8 |
            UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    ROM_UARTEnable(UARTBASE[UART]);
}

void puts(const char * str) {
    UARTputs(0, str);
}

void UARTputs(unsigned char UART, const char * str) {
    int a = 0;
    while (str[a] != '\0') {
        if (str[a] == '\n') {
            UARTputc(UART, '\r');
            UARTputc(UART, '\n');
        }
        else UARTputc(UART, str[a]);
        a++;
    }
}

char * gets(char * str, int num) {
    return UARTgets(0, str, num);
}

char * UARTgets(unsigned char UART, char * str, int num) {
    int a = 0;
    while (a < num - 1) {
        str[a] = UARTpeekBlocking(UART);	// Waits for a character
        if (str[a] == '\n' || str[a] == '\r') break;
        peeked[UART] = 0;	// Permits peek() to move on to the next char
        a++;
    }

    // Clear the trailing newline char(s)
    while (UARTpeek(UART) == '\n' || UARTpeek(UART) == '\r') peeked[UART] = 0;

    str[a] = '\0';
    return str;
}

void putc(char c) {
    ROM_UARTCharPut(UART0_BASE, c);
}

void UARTputc(unsigned char UART, char c) {
    ROM_UARTCharPut(UARTBASE[UART], c);
}

void putln(void) {
    ROM_UARTCharPut(UART0_BASE, '\r');
    ROM_UARTCharPut(UART0_BASE, '\n');
}

void UARTputln(unsigned char UART) {
    ROM_UARTCharPut(UARTBASE[UART], '\r');
    ROM_UARTCharPut(UARTBASE[UART], '\n');
}

char getc(void) {
    if (peeked[0]) {
        peeked[0] = 0;
        return peekedChar[0];
    }
    while (!ROM_UARTCharsAvail(UART0_BASE));	// Wait for a char if buffer empty
    return ROM_UARTCharGet(UART0_BASE);
}

char UARTgetc(unsigned char UART) {
    if (peeked[UART]) {
        peeked[UART] = 0;
        return peekedChar[UART];
    }
    while (!ROM_UARTCharsAvail(UARTBASE[UART]));	// Wait for a char if buffer empty
    return ROM_UARTCharGet(UARTBASE[UART]);
}

char peek(void) {
    if (peeked[0]) return peekedChar[0];	// Already peeked a char

    if (!ROM_UARTCharsAvail(UART0_BASE)) return (char)-1;

    peeked[0] = 1;
    return peekedChar[0] = ROM_UARTCharGet(UART0_BASE);
}

char UARTpeek(unsigned char UART) {
    if (peeked[UART]) return peekedChar[UART];	// Already peeked a char

    if (!ROM_UARTCharsAvail(UARTBASE[UART])) return (char)-1;

    peeked[UART] = 1;
    return peekedChar[UART] = ROM_UARTCharGet(UARTBASE[UART]);
}

char peekBlocking(void) {
    if (peeked[0]) return peekedChar[0];	// Already peeked a char

    while (!ROM_UARTCharsAvail(UART0_BASE));	// Wait for a char
    peeked[0] = 1;
    return peekedChar[0] = ROM_UARTCharGet(UART0_BASE);
}

char UARTpeekBlocking(unsigned char UART) {
    if (peeked[UART]) return peekedChar[UART];	// Already peeked a char

    while (!ROM_UARTCharsAvail(UARTBASE[UART]));	// Wait for a char
    peeked[UART] = 1;
    return peekedChar[UART] = ROM_UARTCharGet(UARTBASE[UART]);
}

void puti(long i) {
    UARTputi(0, i);
}

void UARTputi(unsigned char UART, long i) {
    unsigned char digs[12], reversed[12], a = 0, b, neg = 0;

    if (i < 0) {
        neg = 1;
        i = -i;
    }

    do {
        b = i % 10;
        digs[a] = b + 48;	// Convert to digit ASCII
        i /= 10;
        a++;
    } while (i);

    // Reverse the digits into most significant to least significant
    if (neg) {
        reversed[0] = '-';
        for (b = 1; b < a + neg; b++) reversed[b] = digs[a-b];
        a += 1;	// Extend the length of the string by 1 due to - sign
    }
    else for (b = 0; b < a; b++) reversed[b] = digs[a-b-1];

    reversed[a] = '\0';
    UARTputs(UART, (char *)reversed);
}

long geti() {
    return UARTgeti(0);
}

long UARTgeti(unsigned char UART) {
    unsigned char digs[10], a, b, neg = 0;

    // Clear the leading non-number characters
    UARTpeekBlocking(UART);	// Peeked char is now in peekedChar
    while ((peekedChar[UART] < 48 || peekedChar[UART] > 57) && peekedChar[UART] != '-') {
        peeked[UART] = 0;
        UARTpeekBlocking(UART);
    }

    // Read in digits
    for (a = 0; a < 10; a++) {
        digs[a] = getc();
        if (digs[a] == '-') {
            neg ^= 1;
            a--;	// No digit was read
            continue;
        }
        else if (digs[a] < 48 || digs[a] > 57) break;
    }

    // Convert to integer
    long i = 0;
    for (b = 0; b < a; b++) i += (digs[b] - 48) * power(10, a-b-1);
    if (neg) i = -i;

    return i;
}

void putu(unsigned long u, unsigned char digits) {
    UARTputu(0, u, digits);
}

void UARTputu(unsigned char UART, unsigned long u, unsigned char digits) {
    if (digits > 10) digits = 10;

    unsigned char digs[11], reversed[11], a = 0, b, c;

    do {
        b = u % 10;
        digs[a] = b + 48;	// Convert to digit ASCII
        u /= 10;
        a++;
    } while (u);

    // Add padding zeroes if necessary
    b = 0;
    if(a < digits) {
        for(; b < digits - a; b++) reversed[b] = '0';
    }

    // Reverse the digits into most significant to least significant
    for (c = 0; b < digits; b++, c++) reversed[b] = digs[a-c-1];

    reversed[digits] = '\0';
    UARTputs(UART, (char *)reversed);
}

unsigned long getu(unsigned char digits) {
    return UARTgetu(0, digits);
}

unsigned long UARTgetu(unsigned char UART, unsigned char digits) {
    if (digits > 10) digits = 10;

    unsigned char digs[10], a, b;

    // Clear the leading non-number characters
    UARTpeekBlocking(UART);	// Peeked char is now in peekedChar
    while (peekedChar[UART] < 48 || peekedChar[UART] > 57) {
        peeked[UART] = 0;
        UARTpeekBlocking(UART);
    }

    // Read in digits
    for (a = 0; a < digits; a++) {
        digs[a] = getc();
        if (digs[a] < 48 || digs[a] > 57) break;
    }

    // Convert to integer
    unsigned long u = 0;
    for (b = 0; b < a; b++) u += (digs[b] - 48) * power(10, a-b-1);

    return u;
}

void puth(unsigned long h, unsigned char digits) {
    UARTputh(0, h, digits);
}

void UARTputh(unsigned char UART, unsigned long h, unsigned char digits) {
    if (digits > 8) digits = 8;

    unsigned char digs[9], reversed[9], a = 0, b, c;

    do {
        b = h % 16;
        // Convert to digit ASCII
        if (b < 10) digs[a] = b + 48;
        else digs[a] = b + 55;
        h /= 16;
        a++;
    } while (h);

    // Add padding zeroes if necessary
    b = 0;
    if(a < digits) {
        for(; b < digits - a; b++) reversed[b] = '0';
    }

    // Reverse the digits into most significant to least significant
    for (c = 0; b < digits; b++, c++) reversed[b] = digs[a-c-1];

    reversed[digits] = '\0';
    UARTputs(UART, (char *)reversed);
}

unsigned long geth(unsigned char digits) {
    return UARTgeth(0, digits);
}

unsigned long UARTgeth(unsigned char UART, unsigned char digits) {
    if (digits > 8) digits = 8;

    unsigned char digs[8], a, b;

    // Clear the leading non-number characters
    UARTpeekBlocking(UART);	// Peeked char is now in peekedChar
    while (! ((peekedChar[UART] > 47 && peekedChar[UART] < 58) ||
            (peekedChar[UART] > 64 && peekedChar[UART] < 71)) ) {
        peeked[UART] = 0;
        UARTpeekBlocking(UART);
    }

    // Read in digits
    for (a = 0; a < digits; a++) {
        digs[a] = getc();
        if (! ((digs[a] > 47 && digs[a] < 58) ||
                (digs[a] > 64 && digs[a] < 71)) ) break;
    }

    // Convert to integer
    unsigned long h = 0;
    for (b = 0; b < a; b++) {
        if (digs[b] < 58) h += (digs[b] - 48) * power(16, a-b-1);
        else h += (digs[b] - 55) * power(16, a-b-1);
    }

    return h;
}

/* Coming soon
void putf(float f, unsigned char digits);
float getf(unsigned char digits);
*/
