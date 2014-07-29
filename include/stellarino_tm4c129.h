/*  stellarino_tm4c129.h
    Copyright (C) 2014 Sultan Qasim Khan

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

#ifndef STELLARINO_TM4C129_H
#define STELLARINO_TM4C129_H

/*****************************************************************************
 THIS IS NOT READY FOR USE. IT IS CURRENTLY JUST A COPY OF THE EQUIVALENT
 FILE FOR THE TM4C123x LINE. IN OTHER WORDS, THIS IS A PLACEHOLDER.
 *****************************************************************************/

#include <stellarino.h>

#define SW1         PF4
#define RED_LED     PF1
#define BLUE_LED    PF2
#define GREEN_LED   PF3
#define SW2         PF0

static const uint32_t GPIO[] =
{
    GPIO_PORTA_BASE, GPIO_PORTB_BASE, GPIO_PORTC_BASE, GPIO_PORTD_BASE,
    GPIO_PORTE_BASE, GPIO_PORTF_BASE
};

static const uint32_t TIMER[] =
{
    TIMER0_BASE, TIMER1_BASE, TIMER2_BASE, TIMER3_BASE, TIMER4_BASE,
    TIMER5_BASE, WTIMER0_BASE, WTIMER1_BASE, WTIMER2_BASE, WTIMER3_BASE,
    WTIMER4_BASE, WTIMER5_BASE
};

static const uint32_t UARTBASE[] =
{
    UART0_BASE, UART1_BASE, UART2_BASE, UART3_BASE, UART4_BASE, UART5_BASE,
    UART6_BASE, UART7_BASE
};

static const uint32_t SysCtlGPIOs[] =
{
    SYSCTL_PERIPH_GPIOA, SYSCTL_PERIPH_GPIOB, SYSCTL_PERIPH_GPIOC,
    SYSCTL_PERIPH_GPIOD, SYSCTL_PERIPH_GPIOE, SYSCTL_PERIPH_GPIOF
};

static const uint32_t SysCtlTimers[] =
{
    SYSCTL_PERIPH_TIMER0, SYSCTL_PERIPH_TIMER1, SYSCTL_PERIPH_TIMER2,
    SYSCTL_PERIPH_TIMER3, SYSCTL_PERIPH_TIMER4, SYSCTL_PERIPH_TIMER5,
    SYSCTL_PERIPH_WTIMER0, SYSCTL_PERIPH_WTIMER1, SYSCTL_PERIPH_WTIMER2,
    SYSCTL_PERIPH_WTIMER3, SYSCTL_PERIPH_WTIMER4, SYSCTL_PERIPH_WTIMER5
};

static const uint32_t SysCtlUARTs[] =
{
        SYSCTL_PERIPH_UART0, SYSCTL_PERIPH_UART1, SYSCTL_PERIPH_UART2,
        SYSCTL_PERIPH_UART3, SYSCTL_PERIPH_UART4, SYSCTL_PERIPH_UART5,
        SYSCTL_PERIPH_UART6, SYSCTL_PERIPH_UART7
};

// Pins associated w/ WTIMER5 will not be used for PWM
// Timers 0-5 are TIMER0-TIMER5 and 6-11 are WTIMER0-WTIMER5
// Timer 12 means it is not a timer pin, ADC Channel 12 means not an ADC pin
// Format: {Timer #, half-timer, Timer GPIO pin-mux, ADC Channel}
static const uint32_t pinMux[][4] =
{
    {12, 0, 0, 12},     // PA0
    {12, 0, 0, 12},     // PA1
    {12, 0, 0, 12},     // PA2
    {12, 0, 0, 12},     // PA3
    {12, 0, 0, 12},     // PA4
    {12, 0, 0, 12},     // PA5
    {12, 0, 0, 12},     // PA6
    {12, 0, 0, 12},     // PA7
    {2, TIMER_A, GPIO_PB0_T2CCP0, 12},  // PB0
    {2, TIMER_B, GPIO_PB1_T2CCP1, 12},  // PB1
    {3, TIMER_A, GPIO_PB2_T3CCP0, 12},  // PB2
    {3, TIMER_B, GPIO_PB3_T3CCP1, 12},  // PB3
    {1, TIMER_A, GPIO_PB4_T1CCP0, ADC_CTL_CH10},    // PB4
    {1, TIMER_B, GPIO_PB5_T1CCP1, ADC_CTL_CH11},    // PB5
    {0, TIMER_A, GPIO_PB6_T0CCP0, 12},  // PB6
    {0, TIMER_B, GPIO_PB7_T0CCP1, 12},  // PB7
    {4, TIMER_A, GPIO_PC0_T4CCP0, 12},  // PC0
    {4, TIMER_B, GPIO_PC1_T4CCP1, 12},  // PC1
    {5, TIMER_A, GPIO_PC2_T5CCP0, 12},  // PC2
    {5, TIMER_B, GPIO_PC3_T5CCP1, 12},  // PC3
    {6, TIMER_A, GPIO_PC4_WT0CCP0, 12}, // PC4
    {6, TIMER_B, GPIO_PC5_WT0CCP1, 12}, // PC5
    {7, TIMER_A, GPIO_PC6_WT1CCP0, 12}, // PC6
    {7, TIMER_B, GPIO_PC7_WT1CCP1, 12}, // PC7
    {8, TIMER_A, GPIO_PD0_WT2CCP0, ADC_CTL_CH7},    // PD0
    {8, TIMER_B, GPIO_PD1_WT2CCP1, ADC_CTL_CH6},    // PD1
    {9, TIMER_A, GPIO_PD2_WT3CCP0, ADC_CTL_CH5},    // PD2
    {9, TIMER_B, GPIO_PD3_WT3CCP1, ADC_CTL_CH4},    // PD3
    {12, 0, 0, 12},     // PD4 (WTIMER4 is needed for elapsed time functions)
    {12, 0, 0, 12},     // PD5 (WTIMER4 is needed for elapsed time functions)
    {12, 0, 0, 12},     // PD6 (WTIMER5 A is needed for delay)
    {12, 0, 0, 12},     // PD7 (WTIMER5 B is needed for pulseIn)
    {12, 0, 0, ADC_CTL_CH3},    // PE0
    {12, 0, 0, ADC_CTL_CH2},    // PE1
    {12, 0, 0, ADC_CTL_CH1},    // PE2
    {12, 0, 0, ADC_CTL_CH0},    // PE3
    {12, 0, 0, ADC_CTL_CH9},    // PE4
    {12, 0, 0, ADC_CTL_CH8},    // PE5
    {12, 0, 0, 12},     // PE6
    {12, 0, 0, 12},     // PE7
    {0, TIMER_A, GPIO_PF0_T0CCP0, 12},  // PF0 (shared w/ PB6)
    {0, TIMER_B, GPIO_PF1_T0CCP1, 12},  // PF1 (shared w/ PB7)
    {1, TIMER_A, GPIO_PF2_T1CCP0, 12},  // PF2 (shared w/ PB4)
    {1, TIMER_B, GPIO_PF3_T1CCP1, 12},  // PF3 (shared w/ PB5)
    {2, TIMER_A, GPIO_PF4_T2CCP0, 12},  // PF4 (shared w/ PB0)
    {12, 0, 0, 12},     // PF5
    {12, 0, 0, 12},     // PF6
    {12, 0, 0, 12}      // PF7
};

// RX Pin, TX Pin, RX Pin Config, TX Pin Config
static const uint32_t UARTPins[][4] =
{
    {PA0, PA1, GPIO_PA0_U0RX, GPIO_PA1_U0TX},
    {PB0, PB1, GPIO_PB0_U1RX, GPIO_PB1_U1TX},
    {PD6, PD7, GPIO_PD6_U2RX, GPIO_PD7_U2TX},
    {PC6, PC7, GPIO_PC6_U3RX, GPIO_PC7_U3TX},
    {PC4, PC5, GPIO_PC4_U4RX, GPIO_PC5_U4TX},
    {PE4, PE5, GPIO_PE4_U5RX, GPIO_PE5_U5TX},
    {PD4, PD5, GPIO_PD4_U6RX, GPIO_PD5_U6TX},
    {PE0, PE1, GPIO_PE0_U7RX, GPIO_PE1_U7TX}
};

// Format: {SCK, SS, MISO, MOSI, SCK Config, SS Config, MISO Config, MOSI Config}
static const uint32_t SPIPins[4][8] =
{
    {PA2, PA3, PA4, PA5, GPIO_PA2_SSI0CLK, GPIO_PA3_SSI0FSS, GPIO_PA4_SSI0RX, GPIO_PA5_SSI0TX},
    {PF2, PF3, PF0, PF1, GPIO_PF2_SSI1CLK, GPIO_PF3_SSI1FSS, GPIO_PF0_SSI1RX, GPIO_PF1_SSI1TX},
    {PB4, PB5, PB6, PB7, GPIO_PB4_SSI2CLK, GPIO_PB5_SSI2FSS, GPIO_PB6_SSI2RX, GPIO_PB7_SSI2TX},
    {PD0, PD1, PD2, PD3, GPIO_PD0_SSI3CLK, GPIO_PD1_SSI3FSS, GPIO_PD2_SSI3RX, GPIO_PD3_SSI3TX}
};

static const uint32_t SysCtlSSI[4] =
{
    SYSCTL_PERIPH_SSI0, SYSCTL_PERIPH_SSI1, SYSCTL_PERIPH_SSI2, SYSCTL_PERIPH_SSI3
};

static const uint32_t SSIBase[4] =
{
    SSI0_BASE, SSI1_BASE, SSI2_BASE, SSI3_BASE
};

// TODO: Add PWM unit support
// TODO: Add EEPROM support

#endif
