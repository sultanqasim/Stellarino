/*  stellarino_spi.c
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

#include <stellarino_spi.h>

void enableSPI(unsigned short SPINum, unsigned short wordLength,
        unsigned long dataRate)
{
    ROM_SysCtlPeripheralEnable(SysCtlSSI[SPINum]);
    ROM_SysCtlPeripheralSleepEnable(SysCtlSSI[SPINum]);
    ROM_SysCtlPeripheralEnable(GPIO[SPIPins[SPINum][0] / 8]);

    int i;
    for (i = 0; i < 4; i++)
    {
        ROM_GPIOPinTypeSSI(GPIO[SPIPins[SPINum][i] / 8],
                bit8[SPIPins[SPINum][i] % 8]);
        ROM_GPIOPinConfigure(SPIPins[SPINum][i + 4]);
    }

    // Polarity 0, Phase 0
    ROM_SSIConfigSetExpClk(SSIBase[SPINum], SysCtlClockGet(), 0,
            SSI_MODE_MASTER, dataRate, wordLength);

    SSIEnable(SSIBase[SPINum]);
}

void SPIWrite(unsigned short SPINum, unsigned long data)
{
    ROM_SSIDataPut(SSIBase[SPINum], data);
}

unsigned long SPIRead(unsigned short SPINum)
{
    uint32_t data;
    ROM_SSIDataGet(SSIBase[SPINum], &data);
    return data;
}
