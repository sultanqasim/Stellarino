/*  stellarino.c
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

#include <stellarino.h>

static const long pwmPeriod = 80000000 / PWMFREQ;

void init(void)
{
    // Set system clock to 80 MHz
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    // Enable FPU and allow floating point operations in interrupts
    ROM_FPUEnable();
    ROM_FPULazyStackingEnable();

    // Enable the UART connected to the ICDI on the Launchpad
    // UART will run at 115200 baud, 8 data bits, 1 stop bit
    enableUART(0, 115200);

    // Configure WTIMER4 for Timer functions
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER4);
    ROM_TimerConfigure(WTIMER4_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_ONE_SHOT | TIMER_CFG_B_ONE_SHOT));
    ROM_TimerPrescaleSet(WTIMER4_BASE, TIMER_A, 39999);         // 0.5 ms per cycle
    ROM_TimerPrescaleSet(WTIMER4_BASE, TIMER_B, 79);            // 1 us per cycle
    ROM_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_WTIMER4);
    ROM_TimerEnable(WTIMER4_BASE, TIMER_A);
    ROM_TimerEnable(WTIMER4_BASE, TIMER_B);

    // Configure WTIMER5 for delay() function
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER5);
    ROM_TimerConfigure(WTIMER5_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_ONE_SHOT | TIMER_CFG_B_ONE_SHOT));
    ROM_TimerPrescaleSet(WTIMER5_BASE, TIMER_A, 7999);          // 0.1 ms per cycle
    ROM_TimerPrescaleSet(WTIMER5_BASE, TIMER_B, 7);             // 0.1 us per cycle
    TimerIntRegister(WTIMER5_BASE, TIMER_A, delayInterrupt);    // Attach interrupt to function
    ROM_IntMasterEnable();	// Enable interrupts
    ROM_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_WTIMER5);
}

void pinMode(unsigned char pin, unsigned char mode)
{
    // Anti-brick JTAG Protection
    if (pin >= PC0 && pin <= PC3) return;

    ROM_SysCtlPeripheralEnable(SysCtlGPIOs[pin/8]);
    ROM_SysCtlPeripheralSleepEnable(SysCtlGPIOs[pin/8]);

    switch (mode)
    {
    case INPUT:
        ROM_GPIOPinTypeGPIOInput(GPIO[pin/8], bit8[pin%8]);
        ROM_GPIOPadConfigSet(GPIO[pin/8], bit8[pin%8], GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
        break;

    case OUTPUT:
        ROM_GPIOPinTypeGPIOOutput(GPIO[pin/8], bit8[pin%8]);
        ROM_GPIOPadConfigSet(GPIO[pin/8], bit8[pin%8], GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
        break;
        
    case OUTPUT_OD:
        ROM_GPIOPinTypeGPIOOutputOD(GPIO[pin/8], bit8[pin%8]);
        ROM_GPIOPadConfigSet(GPIO[pin/8], bit8[pin%8], GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_OD);
        break;

    case INPUT_PULLUP:
        ROM_GPIOPinTypeGPIOInput(GPIO[pin/8], bit8[pin%8]);
        ROM_GPIOPadConfigSet(GPIO[pin/8], bit8[pin%8], GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
        break;

    case INPUT_PULLDOWN:
        ROM_GPIOPinTypeGPIOInput(GPIO[pin/8], bit8[pin%8]);
        ROM_GPIOPadConfigSet(GPIO[pin/8], bit8[pin%8], GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPD);
        break;

    case INPUT_ANALOG:
        ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
        ROM_GPIOPinTypeADC(GPIO[pin/8], bit8[pin%8]);
        break;

    case OUTPUT_PWM:
    case OUTPUT_PWM_OD:
        if (pinMux[pin][0] == 12) break;
        ROM_SysCtlPeripheralEnable(SysCtlTimers[pinMux[pin][0]]);
        ROM_SysCtlPeripheralSleepEnable(SysCtlTimers[pinMux[pin][0]]);

        ROM_GPIOPinTypeTimer(GPIO[pin/8], bit8[pin%8]);
        ROM_GPIOPinConfigure(pinMux[pin][2]);

        if (mode == OUTPUT_PWM_OD)
            ROM_GPIOPadConfigSet(GPIO[pin/8], bit8[pin%8], GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_OD);
        else
            ROM_GPIOPadConfigSet(GPIO[pin/8], bit8[pin%8], GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);

        ROM_TimerConfigure(TIMER[pinMux[pin][0]], (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM | TIMER_CFG_B_PWM));

        // The narrow timers may need to use a prescaler to get the desired PWM period
        if (pinMux[pin][0] < 6)
            ROM_TimerPrescaleSet(TIMER[pinMux[pin][0]], pinMux[pin][1], (pwmPeriod & 0xFFFF0000) >> 16);
        else ROM_TimerPrescaleSet(TIMER[pinMux[pin][0]], pinMux[pin][1], 0);

        // Timer will load this value on timeout
        ROM_TimerLoadSet(TIMER[pinMux[pin][0]], pinMux[pin][1], pwmPeriod);

        // Initial duty cycle of 0
        ROM_TimerMatchSet(TIMER[pinMux[pin][0]], pinMux[pin][1], pwmPeriod - 1);
        if (pinMux[pin][0] < 6)
            ROM_TimerPrescaleMatchSet(TIMER[pinMux[pin][0]], pinMux[pin][1],
                    (pwmPeriod & 0xFFFF0000) >> 16);
        else ROM_TimerPrescaleMatchSet(TIMER[pinMux[pin][0]], pinMux[pin][1], 0);

        ROM_TimerControlLevel(TIMER[pinMux[pin][0]], pinMux[pin][1], 0);
        ROM_TimerEnable(TIMER[pinMux[pin][0]], TIMER_BOTH);
        break;

    case OUTPUT_SERVO:
        if (pinMux[pin][0] == 12) break;
        ROM_SysCtlPeripheralEnable(SysCtlTimers[pinMux[pin][0]]);
        ROM_SysCtlPeripheralSleepEnable(SysCtlTimers[pinMux[pin][0]]);
        ROM_GPIOPadConfigSet(GPIO[pin/8], bit8[pin%8], GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
        ROM_GPIOPinTypeTimer(GPIO[pin/8], bit8[pin%8]);
        ROM_GPIOPinConfigure(pinMux[pin][2]);
        ROM_TimerConfigure(TIMER[pinMux[pin][0]], (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM | TIMER_CFG_B_PWM));

        // The narrow timers may need to use a prescaler to get the desired PWM period
        if (pinMux[pin][0] < 6)
            ROM_TimerPrescaleSet(TIMER[pinMux[pin][0]], pinMux[pin][1], (1600000 & 0xFFFF0000) >> 16);
        else ROM_TimerPrescaleSet(TIMER[pinMux[pin][0]], pinMux[pin][1], 0);

        // Timer will load this value on timeout
        ROM_TimerLoadSet(TIMER[pinMux[pin][0]], pinMux[pin][1], 1600000);

        // Initial duty cycle of 1500 us
        ROM_TimerMatchSet(TIMER[pinMux[pin][0]], pinMux[pin][1], 1480000);
        if (pinMux[pin][0] < 6)
            ROM_TimerPrescaleMatchSet(TIMER[pinMux[pin][0]], pinMux[pin][1],
                    (1480000 & 0xFFFF0000) >> 16);
        else ROM_TimerPrescaleMatchSet(TIMER[pinMux[pin][0]], pinMux[pin][1], 0);

        ROM_TimerControlLevel(TIMER[pinMux[pin][0]], pinMux[pin][1], 0);
        ROM_TimerEnable(TIMER[pinMux[pin][0]], TIMER_BOTH);
        break;
    }
}

int digitalRead(unsigned char pin)
{
    if (ROM_GPIOPinRead(GPIO[pin/8], bit8[pin%8])) return 1;
    else return 0;
}

int analogRead(unsigned char pin)
{
    if (pinMux[pin][3] == 12) return 0;	// Non-ADC pin

    ROM_ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    ROM_ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_IE | ADC_CTL_END | pinMux[pin][3]);
    ROM_ADCSequenceEnable(ADC0_BASE, 0);

    ROM_ADCProcessorTrigger(ADC0_BASE, 0);
    while(!ROM_ADCIntStatus(ADC0_BASE, 0, false));
    uint32_t value;
    ROM_ADCSequenceDataGet(ADC0_BASE, 0, &value);

    return value;	// From 0 to 4095
}

void digitalWrite(unsigned char pin, short val)
{
    if (val) ROM_GPIOPinWrite(GPIO[pin/8], bit8[pin%8], bit8[pin%8]);
    else ROM_GPIOPinWrite(GPIO[pin/8], bit8[pin%8], 0);
}

void analogWrite(unsigned char pin, short val)
{
    long period;	// Period the output is low

    if (val > 255) period = 0;
    else if (val <= 0) period = pwmPeriod - 1;
    else period = (pwmPeriod * (255-val)) >> 8;

    if (pinMux[pin][0] < 6)	// Narrow timers may need a prescaler
        ROM_TimerPrescaleMatchSet(TIMER[pinMux[pin][0]], pinMux[pin][1], (period & 0xFFFF0000) >> 16);
    ROM_TimerMatchSet(TIMER[pinMux[pin][0]], pinMux[pin][1], period);
}

void servoWrite(unsigned char pin, short val)
{
    long period;

    if (val < 600) val = 600;
    else if (val > 2400) val = 2400;
    period = 1600000 - (80 * val);

    if (pinMux[pin][0] < 6)	// Narrow timers need a prescaler
        ROM_TimerPrescaleMatchSet(TIMER[pinMux[pin][0]], pinMux[pin][1], (period & 0xFFFF0000) >> 16);
    ROM_TimerMatchSet(TIMER[pinMux[pin][0]], pinMux[pin][1], period);
}

void pwmWrite(unsigned char pin, float frequency, float duty)
{
    // First set up the pin for the given frequency
    int customPwmPeriod = 80000000 / frequency;

    // The narrow timers may need to use a prescaler to get the desired PWM period
    if (pinMux[pin][0] < 6)
        ROM_TimerPrescaleSet(TIMER[pinMux[pin][0]], pinMux[pin][1], (customPwmPeriod & 0xFFFF0000) >> 16);
    else ROM_TimerPrescaleSet(TIMER[pinMux[pin][0]], pinMux[pin][1], 0);

    // Timer will load this value on timeout
    ROM_TimerLoadSet(TIMER[pinMux[pin][0]], pinMux[pin][1], customPwmPeriod);

    // Initial duty cycle of 0
    ROM_TimerMatchSet(TIMER[pinMux[pin][0]], pinMux[pin][1], customPwmPeriod - 1);
    if (pinMux[pin][0] < 6)
        ROM_TimerPrescaleMatchSet(TIMER[pinMux[pin][0]], pinMux[pin][1],
                (customPwmPeriod & 0xFFFF0000) >> 16);
    else ROM_TimerPrescaleMatchSet(TIMER[pinMux[pin][0]], pinMux[pin][1], 0);

    // Now set the period the output is low, based on the duty cycle
    long period;
    if (duty >= 1) period = 0;
    else if (duty <= 0) period = customPwmPeriod - 1;
    else period = customPwmPeriod * (1.0f - duty);

    if (pinMux[pin][0] < 6)	// Narrow timers may need a prescaler
        ROM_TimerPrescaleMatchSet(TIMER[pinMux[pin][0]], pinMux[pin][1], (period & 0xFFFF0000) >> 16);
    ROM_TimerMatchSet(TIMER[pinMux[pin][0]], pinMux[pin][1], period);
}

unsigned long pulseIn(unsigned char pin, short val, unsigned long timeout)
{
    // Max supported pulse length is 7 minutes
    ROM_TimerEnable(WTIMER5_BASE, TIMER_B);

    ROM_TimerLoadSet(WTIMER5_BASE, TIMER_B, timeout * 10);
    ROM_SysCtlDelay(5);	// Give time for timer to reload and count down once
    while (digitalRead(pin) != val)
    {
        // Timeout if reload has occurred
        if (ROM_TimerValueGet(WTIMER5_BASE, TIMER_B) == timeout * 10) return 0;
    }

    ROM_TimerLoadSet(WTIMER5_BASE, TIMER_B, 4294967295);
    ROM_SysCtlDelay(5);
    while (digitalRead(pin) == val);
    unsigned long pulseLength = (4294967295 - ROM_TimerValueGet(WTIMER5_BASE, TIMER_B)) / 10;

    ROM_TimerDisable(WTIMER5_BASE, TIMER_B);
    return pulseLength;
}
