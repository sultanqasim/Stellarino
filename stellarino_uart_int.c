/*  stellarino_uart_int.c
    Copyright (C) 2013-2015 Sultan Qasim Khan

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
static volatile int32_t rxBufferHead[8] = {0};  // Where to write
static volatile int32_t rxBufferTail[8] = {0};  // Where to read

// Flags to indicate rx buffer overflow (one bit per UART)
static volatile uint8_t rxOverflow = 0;

static inline int32_t rxQSize(uint8_t UART)
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
    // This is not reentrant, so disable its interrupt while running
    ROM_UARTIntDisable(UARTBASE[UART], UART_INT_RX);

    // Clear the interrupt flag
    ROM_UARTIntClear(UARTBASE[UART], UART_INT_RX);

    // Read from the FIFO and place in the circular buffer in memory
    while (ROM_UARTCharsAvail(UARTBASE[UART]))
    {
        if (rxQSize(UART) < UART_BUFFSIZE - 1)
        {
            rxEnqueue(UART, ROM_UARTCharGet(UARTBASE[UART]));
        }
        else
        {
            // Any new data will be discarded till room is available in queue
            rxOverflow |= bit8[UART];
            break;
        }
    }

    // Re-enable the interrupt
    ROM_UARTIntEnable(UARTBASE[UART], UART_INT_RX);
}

void rxInt0(void) {flushReadFIFO(0);}
void rxInt1(void) {flushReadFIFO(1);}
void rxInt2(void) {flushReadFIFO(2);}
void rxInt3(void) {flushReadFIFO(3);}
void rxInt4(void) {flushReadFIFO(4);}
void rxInt5(void) {flushReadFIFO(5);}
void rxInt6(void) {flushReadFIFO(6);}
void rxInt7(void) {flushReadFIFO(7);}

static void (*rxInterrupts[8])(void) =
{
    rxInt0, rxInt1, rxInt2, rxInt3, rxInt4, rxInt5, rxInt6, rxInt7
};

static uint32_t UARTInts[8] =
{
    INT_UART0, INT_UART1, INT_UART2, INT_UART3,
    INT_UART4, INT_UART5, INT_UART6, INT_UART7
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
    ROM_SysCtlPeripheralEnable(SysCtlGPIOs[UARTPins[UART][0] >> 3]);
    ROM_SysCtlPeripheralSleepEnable(SysCtlGPIOs[UARTPins[UART][0] >> 3]);
    ROM_SysCtlPeripheralEnable(SysCtlUARTs[UART]);
    ROM_SysCtlPeripheralSleepEnable(SysCtlUARTs[UART]);

    // Configure the associated GPIO pins for UART
    ROM_GPIOPinConfigure(UARTPins[UART][2]);
    ROM_GPIOPinConfigure(UARTPins[UART][3]);
    ROM_GPIOPinTypeUART(GPIO[UARTPins[UART][0] >> 3],
            bit8[UARTPins[UART][0] & 0x7] | bit8[UARTPins[UART][1] & 0x7]);

    // Configure the UART
    ROM_UARTConfigSetExpClk(UARTBASE[UART], ROM_SysCtlClockGet(), baudRate,
            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    ROM_UARTFIFOEnable(UARTBASE[UART]);

    // Configure the UART receive (rx) interrupt
    ROM_IntEnable(UARTInts[UART]);
    ROM_UARTIntEnable(UARTBASE[UART], UART_INT_RX);
    UARTFIFOLevelSet(UARTBASE[UART], UART_FIFO_TX2_8, UART_FIFO_RX6_8);
    UARTIntRegister(UARTBASE[UART], rxInterrupts[UART]);

    // Enable the UART
    ROM_UARTEnable(UARTBASE[UART]);
}

int32_t UARTgetBufferLevel(uint8_t UART)
{
    flushReadFIFO(UART);
    return rxQSize(UART);
}

bool UARToverflow(uint8_t UART)
{
    if (rxOverflow & bit8[UART]) return true;
    return false;
}

// This throws away all data in the buffer
void UARTflushReceiveBuffer(uint8_t UART)
{
    rxBufferHead[UART] = rxBufferTail[UART] = 0;
    flushReadFIFO(UART);
    rxBufferHead[UART] = rxBufferTail[UART] = 0;
    rxOverflow &= ~(bit8[UART]);
}

void UARTputc(uint8_t UART, char c)
{
    ROM_UARTCharPut(UARTBASE[UART], c);
}

char UARTgetc(uint8_t UART)
{
    // Wait for data if the buffer is empty
    while (!rxQSize(UART)) flushReadFIFO(UART);

    return (char)rxDequeue(UART);
}

int UARTpeek(uint8_t UART)
{
    // If there is data in the buffer return that
    if (rxQSize(UART)) return rxBuff[UART][rxBufferTail[UART]];

    // Try flushing the hardware FIFO and see if we can get data from there
    flushReadFIFO(UART);
    if (rxQSize(UART)) return rxBuff[UART][rxBufferTail[UART]];

    // No data was found
    return -255;
}

char UARTpeekBlocking(uint8_t UART)
{
    // If there is data in the buffer return that
    if (rxQSize(UART)) return rxBuff[UART][rxBufferTail[UART]];

    // Wait for data if the buffer is empty
    while (!rxQSize(UART)) flushReadFIFO(UART);

    // Return what was read without popping it from the queue
    return rxBuff[UART][rxBufferTail[UART]];
}
