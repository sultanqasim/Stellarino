/*  stellarino_spi.h
    Copyright (C) 2012 Sultan Qasim Khan

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

#ifndef STELLARINO_SPI_H
#define STELLARINO_SPI_H

#include "stellarino.h"

void enableSPI(unsigned short SPINum, unsigned short wordLength, unsigned long dataRate);
void SPIWrite(unsigned short SPINum, unsigned long data);
unsigned long SPIRead(unsigned short SPINum);

#endif
