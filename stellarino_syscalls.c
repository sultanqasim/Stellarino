/*  stellarino_syscalls.c
    Copyright (C) 2013 Sultan Qasim Khan

    This module defines system functions necessary to use functions in
    stdio.h, stdlib.h, and many other parts of the C standard library.
    Inspired by implementations at http://sourceware.org/newlib/libc.html

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

#include <errno.h>
#include <stellarino.h>

static char *heap_end = 0;
extern unsigned long _heap_bottom;
extern unsigned long _heap_top;

typedef char * caddr_t;

// This function is necessary to implement malloc
caddr_t _sbrk(int incr)
{
    char *prev_heap_end;
    
    // Executed the first time through to set the static heap_end pointer to the
    // bottom of the head
    if (heap_end == 0) heap_end = (caddr_t)&_heap_bottom;
    
    // We need to keep track of the current heap end because we'll return it to
    // the caller
    prev_heap_end = heap_end;
    
    // Don't allow allocating more memory than we have room for in our heap
    if (heap_end + incr > (caddr_t)&_heap_top) return (caddr_t)0;
    
    // Shift up the heap end so that it won't get reallocated
    heap_end += incr;
    
    // Return the allocated memory on the heap
    return (caddr_t) prev_heap_end;
}

int _close(int file)
{
    // Do nothing for now
    return -1;
}

int _fstat(int file)
{
    return -1;
}

int _isatty(int file)
{
    return -1;
}

int _lseek(int file, int ptr, int dir)
{
    return -1;
}

int _open(const char *name, int flags, int mode)
{
    return -1;
}

int _read(int file, char *ptr, int len)
{
    int i;
    
    // For now just read from UART0
    for (i = 0; i < len; i++) ptr[i] = UARTgetc(0);
    
    return i;
}

int _write(int file, char *ptr, unsigned int len)
{
    int i;
    
    // For now just write to UART0
    for (i = 0; i < len; i++) UARTputc(0, ptr[i]);
    
    return i;
}
