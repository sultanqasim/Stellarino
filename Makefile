#   Makefile
#   Portions Copyright (C) 2013 Sultan Qasim Khan
#
#   Based off Mauro Scomparin's Stellaris GCC template at
#   https://github.com/scompo/stellaris-launchpad-template-gcc
#
#   This is part of Stellarino.
#
#   Stellarino is free software; you can redistribute it and/or modify it
#   under the terms of the GNU Lesser General Public License as published by
#   the Free Software Foundation; either version 3 of the License, or
#   (at your option) any later version.
#
#   Stellarino is distributed in the hope that it will be useful, but WITHOUT
#   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#   FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
#   License for more details.
#
#   You should have received a copy of the GNU Lesser General Public License
#   along with Stellarino. If not, see <http://www.gnu.org/licenses/>.

# Copyright (c) 2012, Mauro Scomparin
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of Mauro Scomparin nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY Mauro Scomparin ``AS IS'' AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL Mauro Scomparin BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# File:			Makefile.
# Author:		Mauro Scomparin <http://scompoprojects.worpress.com>.
# Version:		1.0.0.
# Description:	Sample makefile.

#==============================================================================
#           Cross compiling toolchain / tools specifications
#==============================================================================

# Prefix for the arm-eabi-none toolchain.
# I recommend the gcc-arm-embedded toolchain available here:
# https://launchpad.net/gcc-arm-embedded
PREFIX_ARM = arm-none-eabi

# Microcontroller properties.
PART=TM4C123GH6PM
CPU=-mcpu=cortex-m4
FPU=-mfpu=fpv4-sp-d16 -mfloat-abi=softfp

# TivaWare path
TIVAWARE_PATH=~/tiva_tools/tivaware/

# Program name definition for ARM GNU C compiler.
CC = ${PREFIX_ARM}-gcc
# Program name definition for ARM GNU Linker.
LD = ${PREFIX_ARM}-ld
# Program name definition for ARM GNU Object copy.
CP = ${PREFIX_ARM}-objcopy
# Program name definition for ARM GNU Object dump.
OD = ${PREFIX_ARM}-objdump

# Option arguments for C compiler.
CFLAGS=-mthumb ${CPU} ${FPU} -O0 -ffunction-sections -fdata-sections -MD -std=c99 -Wall -pedantic -c -g
# Library stuff passed as flags!
CFLAGS+= -I ${TIVAWARE_PATH} -I ./include -DPART_$(PART) -c -DTARGET_IS_BLIZZARD_RA1

# Flags for LD
LFLAGS  = --gc-sections

# Flags for objcopy
CPFLAGS = -Obinary

# flags for objectdump
ODFLAGS = -S

# I want to save the path to libgcc, libc.a and libm.a for linking.
# I can get them from the gcc frontend, using some options.
# See gcc documentation
LIB_GCC_PATH=${shell ${CC} ${CFLAGS} -print-libgcc-file-name}
LIBC_PATH=${shell ${CC} ${CFLAGS} -print-file-name=libc.a}
LIBM_PATH=${shell ${CC} ${CFLAGS} -print-file-name=libm.a}

# Uploader tool path.
# Set a relative or absolute path to the upload tool program.
# I used this project: https://github.com/utzig/lm4tools
FLASHER=lm4flash
# Flags for the uploader program.
FLASHER_FLAGS=

#==============================================================================
#                         Project properties
#==============================================================================

# Project name (W/O .c extension eg. "main")
PROJECT_NAME = main
# Startup file name (W/O .c extension eg. "LM4F_startup")
STARTUP_FILE = stellarino_startup_gcc
# Linker file name
LINKER_FILE = tm4c123gh6pm.ld

SRC = $(wildcard *.c)
OBJS = $(patsubst %.c,build/%.o,$(SRC))

#==============================================================================
#                      Rules to make the target
#==============================================================================

#make all rule
all: $(OBJS) ${PROJECT_NAME}.axf ${PROJECT_NAME}

dirs:
	mkdir -p build

build/%.o: %.c dirs
	@echo
	@echo Compiling $<...
	$(CC) -c $(CFLAGS) ${<} -o ${@}

${PROJECT_NAME}.axf: $(OBJS)
	@echo
	@echo Making driverlib
	$(MAKE) -C ${TIVAWARE_PATH}driverlib/
	@echo
	@echo Linking...
	$(LD) -T $(LINKER_FILE) $(LFLAGS) -o build/${PROJECT_NAME}.axf $(OBJS) ${TIVAWARE_PATH}driverlib/gcc/libdriver.a $(LIBM_PATH) $(LIBC_PATH) $(LIB_GCC_PATH)

${PROJECT_NAME}: ${PROJECT_NAME}.axf
	@echo
	@echo Copying...
	$(CP) $(CPFLAGS) build/${PROJECT_NAME}.axf build/${PROJECT_NAME}.bin
	@echo
	@echo Creating list file...
	$(OD) $(ODFLAGS) build/${PROJECT_NAME}.axf > build/${PROJECT_NAME}.lst

# make clean rule
clean:
	rm -rf build

# Rule to load the project to the board
# You can add sudo to the line to flash without a udev rule
load: ${PROJECT_NAME}
	${FLASHER} build/${PROJECT_NAME}.bin ${FLASHER_FLAGS}

