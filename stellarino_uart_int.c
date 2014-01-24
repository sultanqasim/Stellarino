/*  stellarino_uart_int.c
    Copyright (C) 2013 Sultan Qasim Khan

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

#include <stellarino_uart.h>

#define UART_BUFFSIZE 256

// Circular buffers
static uint8_t rxBuff[8][UART_BUFFSIZE];

// Current positions in the buffers
static int16_t rxBufferHead[8] = {0};  // Where to write
static int16_t rxBufferTail[8] = {0};  // Where to read

// Flags to indicate rx buffer overflow (one bit per UART)
static uint8_t overflow = 0;

static inline int16_t rxQSize(uint8_t UART)
{
    return (rxBufferHead[UART] - rxBufferTail[UART]) % UART_BUFFSIZE;
}

// Assumes buffer has room
static inline void rxEnqueue(uint8_t UART, uint8_t c)
{
    rxBuff[UART][rxBufferHead[UART]] = c;
    rxBufferHead[UART] = (rxBufferHead[UART] + 1) % UART_BUFFSIZE;
}

// Assumes buffer is not empty
static inline uint8_t rxDequeue(uint8_t UART)
{
    uint8_t c = rxBuff[UART][rxBufferTail[UART]];
    rxBufferTail[UART] = (rxBufferTail[UART] + 1) % UART_BUFFSIZE;
    return c;
}

// Called when UART RX FIFO has data in it
static void flushReadFIFO(uint8_t UART)
{
    // Read from the FIFO and place in the circular buffer in memory
    while (ROM_UARTCharsAvail(UARTBASE[UART]))
    {
        if (rxQSize(UART) < UART_BUFFSIZE - 1)
            rxEnqueue(UART, ROM_UARTCharGet(UARTBASE[UART]));
        else
            ROM_UARTCharGet(UARTBASE[UART]);
    }

    // If the circular queue is full, set the overflow flag and return
    if (rxQSize(UART) == UART_BUFFSIZE - 1) overflow |= bit8[UART];

    // Clear the appropriate interrupt flag
    ROM_UARTIntClear(UARTBASE[UART], UART_INT_RX);
}

void rxInt0(void) {flushReadFIFO(0);}
void rxInt1(void) {flushReadFIFO(1);}
void rxInt2(void) {flushReadFIFO(2);}
void rxInt3(void) {flushReadFIFO(3);}
void rxInt4(void) {flushReadFIFO(4);}
void rxInt5(void) {flushReadFIFO(5);}
void rxInt6(void) {flushReadFIFO(6);}
void rxInt7(void) {flushReadFIFO(7);}

void (*rxInterrupts[8])(void) =
{
    rxInt0, rxInt1, rxInt2, rxInt3, rxInt4, rxInt5, rxInt6, rxInt7
};

void enableUART(uint8_t UART, unsigned long baudRate)
{
    // We must unlock PD7 to use UART2
    if (UART == 2)
    {
        // GPIO Port D Lock Register is at 0x40007520
        HWREG(0x40007520) = GPIO_LOCK_KEY;
        // GPIO Port D Control Register is at 0x40007524
        HWREG(0x40007524) = 0x80;
    }

    // Enable the UART peripheral in SysCtl
    ROM_SysCtlPeripheralEnable(SysCtlGPIOs[UARTPins[UART][0] / 8]);
    ROM_SysCtlPeripheralSleepEnable(SysCtlGPIOs[UARTPins[UART][0] / 8]);
    ROM_SysCtlPeripheralEnable(SysCtlUARTs[UART]);
    ROM_SysCtlPeripheralSleepEnable(SysCtlUARTs[UART]);

    // Configure the associated GPIO pins for UART
    ROM_GPIOPinConfigure(UARTPins[UART][2]);
    ROM_GPIOPinConfigure(UARTPins[UART][3]);
    ROM_GPIOPinTypeUART(GPIO[UARTPins[UART][0] / 8],
            bit8[UARTPins[UART][0] % 8] | bit8[UARTPins[UART][1] % 8]);

    // Configure the UART
    ROM_UARTConfigSetExpClk(UARTBASE[UART], ROM_SysCtlClockGet(), baudRate,
            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    ROM_UARTFIFOEnable(UARTBASE[UART]);

    // Configure the UART receive (rx) interrupt
    ROM_UARTIntEnable(UARTBASE[UART], UART_INT_RX);
    UARTFIFOLevelSet(UARTBASE[UART], UART_FIFO_TX4_8, UART_FIFO_RX4_8);
    UARTIntRegister(UARTBASE[UART], rxInterrupts[UART]);

    // Enable the UART
    ROM_UARTEnable(UARTBASE[UART]);
}

int16_t UARTgetBufferLevel(uint8_t UART)
{
    flushReadFIFO(UART);
    return rxQSize(UART);
}

bool UARToverflow(uint8_t UART)
{
    if (overflow & bit8[UART]) return true;
    return false;
}

// This throws away all data in the buffer
void UARTflushReceiveBuffer(uint8_t UART)
{
    rxBufferHead[UART] = rxBufferTail[UART] = 0;
    flushReadFIFO(UART);
    rxBufferHead[UART] = rxBufferTail[UART] = 0;
    overflow &= ~(bit8[UART]);
}

void UARTputc(uint8_t UART, char c)
{
    ROM_UARTCharPut(UARTBASE[UART], c);
}

char UARTgetc(uint8_t UART)
{
    if (!rxQSize(UART))
    {
        // Disable interrupt to avoid possible race condition
        ROM_UARTIntDisable(UARTBASE[UART], UART_INT_RX);

        // Wait for data if the buffer is empty
        while (!rxQSize(UART)) flushReadFIFO(UART);

        // Return to the previous state
        ROM_UARTIntEnable(UARTBASE[UART], UART_INT_RX);
    }

    return (char)rxDequeue(UART);
}

int UARTpeek(uint8_t UART)
{
    // If there is data in the buffer return that
    if (rxQSize(UART)) return rxBuff[UART][rxBufferTail[UART]];

    // Disable interrupt to avoid possible race condition
    ROM_UARTIntDisable(UARTBASE[UART], UART_INT_RX);

    // Try flushing the hardware FIFO and see if we can get data from there
    flushReadFIFO(UART);
    if (rxQSize(UART)) return rxBuff[UART][rxBufferTail[UART]];

    // Return to the previous state
    ROM_UARTIntEnable(UARTBASE[UART], UART_INT_RX);

    // No data was found
    return -255;
}

char UARTpeekBlocking(uint8_t UART)
{
    // If there is data in the buffer return that
    if (rxQSize(UART)) return rxBuff[UART][rxBufferTail[UART]];

    // Disable interrupt to avoid possible race condition
    ROM_UARTIntDisable(UARTBASE[UART], UART_INT_RX);

    // Wait for data if the buffer is empty
    while (!rxQSize(UART)) flushReadFIFO(UART);

    // Return to the previous state
    ROM_UARTIntEnable(UARTBASE[UART], UART_INT_RX);

    // Return what was read without popping it from the queue
    return rxBuff[UART][rxBufferTail[UART]];
}
