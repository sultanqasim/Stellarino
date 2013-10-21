/*  stellarino_spi.h
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

#ifndef STELLARINO_SPI_H
#define STELLARINO_SPI_H

#include <stellarino.h>

// Format: {SCK, SS, MISO, MOSI, SCK Config, SS Config, MISO Config, MOSI Config}
static const unsigned long SPIPins[4][8] =
{
    {PA2, PA3, PA4, PA5, GPIO_PA2_SSI0CLK, GPIO_PA3_SSI0FSS, GPIO_PA4_SSI0RX, GPIO_PA5_SSI0TX},
    {PF2, PF3, PF0, PF1, GPIO_PF2_SSI1CLK, GPIO_PF3_SSI1FSS, GPIO_PF0_SSI1RX, GPIO_PF1_SSI1TX},
    {PB4, PB5, PB6, PB7, GPIO_PB4_SSI2CLK, GPIO_PB5_SSI2FSS, GPIO_PB6_SSI2RX, GPIO_PB7_SSI2TX},
    {PD0, PD1, PD2, PD3, GPIO_PD0_SSI3CLK, GPIO_PD1_SSI3FSS, GPIO_PD2_SSI3RX, GPIO_PD3_SSI3TX}
};

static const unsigned long SysCtlSSI[4] =
{
    SYSCTL_PERIPH_SSI0, SYSCTL_PERIPH_SSI1, SYSCTL_PERIPH_SSI2, SYSCTL_PERIPH_SSI3
};

static const unsigned long SSIBase[4] =
{
    SSI0_BASE, SSI1_BASE, SSI2_BASE, SSI3_BASE
};

void enableSPI(unsigned short SPINum, unsigned short wordLength, unsigned long dataRate);
void SPIWrite(unsigned short SPINum, unsigned long data);
unsigned long SPIRead(unsigned short SPINum);

#endif
