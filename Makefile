#   Makefile
#   Copyright (C) 2013-2014 Sultan Qasim Khan
#
#   Inspired by Mauro Scomparin's Stellaris GCC template at
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

#==============================================================================
#                         Build tools
#==============================================================================

# Prefix for the GNU ARM tools
# Use this toolchain: https://launchpad.net/gcc-arm-embedded
PREFIX_ARM = arm-none-eabi

# TivaWare path
TIVAWARE_PATH = ~/tiva_tools/tivaware/

# GNU ARM tools
CC = ${PREFIX_ARM}-gcc
LD = ${PREFIX_ARM}-ld
CP = ${PREFIX_ARM}-objcopy
OD = ${PREFIX_ARM}-objdump

# Flashing tool (lm4flash from lm4tools)
FLASHER = lm4flash
FLASHER_FLAGS =

#==============================================================================
#                         Target requirements
#==============================================================================

ALLOWEDTARGETS = LM4F120 TM4C123 TM4C129

ifeq ($(TARGET),)
    TARGET = TM4C123
else
    ifneq ($(TARGET), $(findstring $(TARGET), $(ALLOWEDTARGETS)))
        $(error 'TARGET $(TARGET) not in ALLOWEDTARGETS $(ALLOWEDTARGETS)')
    endif
endif

ifeq ($(TARGET),LM4F120)
    LINKER_FILE = tm4c123gh6pm.ld
    CFLAGS = -D TARGET_LM4F120
endif

ifeq ($(TARGET),TM4C123)
    LINKER_FILE = tm4c123gh6pm.ld
    CFLAGS = -D TARGET_TM4C123
endif

ifeq ($(TARGET),TM4C129)
    LINKER_FILE = tm4c1294ncpdt.ld
    CFLAGS = -D TARGET_TM4C129
endif

#==============================================================================
#                         Build arguments/flags
#==============================================================================

# Option arguments for C compiler.
MCU_FLAGS = -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
SECTION_FLAGS = -ffunction-sections -fdata-sections
CFLAGS += ${MCU_FLAGS} ${SECTION_FLAGS} -O2 -MD -std=c99 -Wall -pedantic
CFLAGS += -I ${TIVAWARE_PATH} -I ./include

ifeq ($(DEBUG),1)
    CFLAGS += -g
endif

# Flags for LD
LFLAGS = --gc-sections -T $(LINKER_FILE)

# Flags for objcopy
CPFLAGS = -Obinary

# flags for objectdump
ODFLAGS = -S

# Paths to important static libraries that we must link with
LIB_GCC_PATH = ${shell ${CC} ${CFLAGS} -print-libgcc-file-name}
LIBC_PATH = ${shell ${CC} ${CFLAGS} -print-file-name=libc.a}
LIBM_PATH = ${shell ${CC} ${CFLAGS} -print-file-name=libm.a}
PDL_PATH = ${TIVAWARE_PATH}driverlib/gcc/libdriver.a

LIB_PATHS = ${LIB_GCC_PATH} ${LIBC_PATH} ${LIBM_PATH} ${PDL_PATH}

#==============================================================================
#                         Project properties
#==============================================================================

# Name for the output binary
PROJECT_NAME = main

SRC = $(wildcard *.c)
OBJS = $(patsubst %.c,build/%.o,$(SRC))

#==============================================================================
#                      Rules to make the target
#==============================================================================

all: $(OBJS) ${PROJECT_NAME}.axf ${PROJECT_NAME}

dirs:
	mkdir -p build

build/%.o: %.c dirs
	@echo
	@echo Compiling $<...
	$(CC) -c $(CFLAGS) $< -o $@

${PROJECT_NAME}.axf: $(OBJS)
	@echo
	@echo Making driverlib
	$(MAKE) -C ${TIVAWARE_PATH}driverlib/
	@echo
	@echo Linking...
	$(LD) $(LFLAGS) -o build/$@ $(OBJS) ${LIB_PATHS}

${PROJECT_NAME}: ${PROJECT_NAME}.axf
	@echo
	@echo Copying...
	$(CP) $(CPFLAGS) build/$< build/$@.bin
	@echo
	@echo Creating list file...
	$(OD) $(ODFLAGS) build/$< > build/$@.lst

clean:
	rm -rf build

# Rule to load the project to the board
# You can add sudo to the line to flash without a udev rule
load: ${PROJECT_NAME}
	${FLASHER} build/${PROJECT_NAME}.bin ${FLASHER_FLAGS}
