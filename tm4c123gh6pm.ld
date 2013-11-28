/*  TM4C.ld
    Portions Copyright (C) 2013 Sultan Qasim Khan

    Based off eehusky's Stellaris GCC template found at
    https://github.com/eehusky/Stellaris-GCC

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

_stack_size = 2K;

MEMORY
{
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 256K
    SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 32K
}

SECTIONS
{
    .text :
    {
        _text = .;
        KEEP(*(.isr_vector))
        *(.text*)
        *(.rodata*)
        _etext = .;
    } > FLASH

    .data : 
    {
        _data = .;
        *(vtable)
        *(.data*)
        _edata = .;
    } > SRAM AT>FLASH

    .bss :
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
    } > SRAM
    
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > SRAM

    _heap_bottom = .;
    _heap_top = ORIGIN(SRAM) + LENGTH(SRAM) - _stack_size;

    _stack_bottom = _heap_top;
    _stack_top = ORIGIN(SRAM) + LENGTH(SRAM);
}
