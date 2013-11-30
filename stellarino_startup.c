/*  stellarino_startup.c
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

#ifndef __GNUC__

void ResetISR(void);
static void GenericISR(void);

// Linker variable for top of stack
extern unsigned long __STACK_TOP;

// Initial vector table
#pragma DATA_SECTION(g_pfnVectors, ".intvecs")
void (* const g_pfnVectors[])(void) =
{
    (void (*)(void))((unsigned long)&__STACK_TOP),  // Initial stack pointer
    ResetISR,                                       // Reset handler
    GenericISR                                      // Everything else handler
};

// Initializes C and starts program
void ResetISR(void)
{
    __asm("    .global _c_int00\n"
          "    b.w     _c_int00");
}

// This interrupt handler does nothing
static void GenericISR(void)
{
    while(1);
}

#endif
