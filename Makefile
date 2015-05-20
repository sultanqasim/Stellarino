# Makefile for Stellarino
# Copyright (C) 2015 Sultan Qasim Khan

# Follow gcc_instructions.txt to set up your toolchain
# This makefile assumes your tools to be in ~/tiva_tools

# BUILD TOOL PATHS
TOOL_BASE = ~/tiva_tools
TIVAWARE_PATH = ${TOOL_BASE}/tivaware

# BUILD TOOLS
CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
OC = arm-none-eabi-objcopy
OD = arm-none-eabi-objdump

# FLASHER (from lm4tools)
FLASHER = lm4flash
FLASHER_FLAGS =

# BUILD FLAGS AND CONFIGURATION
BUILD_FLAGS = -std=c99 -Wall -Wextra -pedantic
ARM_FLAGS = -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
SECTION_FLAGS = -ffunction-sections -fdata-sections
INCLUDE_FLAGS = -I ${TIVAWARE_PATH} -I include
LFLAGS = --gc-sections -T tm4c123gh6pm.ld
OCFLAGS = -Obinary
ODFLAGS = -S
BUILD_DIR = build

ifeq ($(DEBUG),1)
    BUILD_FLAGS += -g -O0
else
    BUILD_FLAGS += -O3
endif

CFLAGS = ${BUILD_FLAGS} ${ARM_FLAGS} ${SECTION_FLAGS} ${INCLUDE_FLAGS}

# STATIC LIBRARY PATHS
LIB_GCC_PATH = ${shell ${CC} ${CFLAGS} -print-libgcc-file-name}
LIBC_PATH = ${shell ${CC} ${CFLAGS} -print-file-name=libc.a}
LIBM_PATH = ${shell ${CC} ${CFLAGS} -print-file-name=libm.a}
PDL_PATH = ${TIVAWARE_PATH}/driverlib/gcc/libdriver.a

LIB_PATHS = ${LIB_GCC_PATH} ${LIBC_PATH} ${LIBM_PATH} ${PDL_PATH}

# PROJECT SETTINGS
PRODUCT_NAME = main
SRC = $(wildcard *.c)
OBJS = $(patsubst %.c,build/%.o,$(SRC))

# MAKE RULES
all: $(OBJS) ${PRODUCT_NAME}

.PHONY: load clean dirs ${PRODUCT_NAME}

dirs: ${BUILD_DIR}

${BUILD_DIR}:
	mkdir -p $@

${BUILD_DIR}/%.o: %.c | dirs
	$(CC) -c $(CFLAGS) $< -o $@

${BUILD_DIR}/${PRODUCT_NAME}.axf: $(OBJS)
	$(MAKE) -C ${TIVAWARE_PATH}/driverlib/
	$(LD) $(LFLAGS) -o $@ $(OBJS) ${LIB_PATHS}

${BUILD_DIR}/${PRODUCT_NAME}: ${BUILD_DIR}/${PRODUCT_NAME}.axf
	$(OC) $(OCFLAGS) $< $@
	$(OD) $(ODFLAGS) $< > $@.lst

${PRODUCT_NAME}: build/${PRODUCT_NAME}

clean:
	rm -rf ${BUILD_DIR}

load: ${PRODUCT_NAME}
	${FLASHER} ${BUILD_DIR}/${PRODUCT_NAME} ${FLASHER_FLAGS}
