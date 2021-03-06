/*  stellarino_spi.h
    Copyright (C) 2012-2014 Sultan Qasim Khan

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

void enableSPI(uint16_t SPINum, uint16_t wordLength, uint32_t dataRate);
void SPIWrite(uint16_t SPINum, uint32_t data);
uint32_t SPIRead(uint16_t SPINum);

#endif
