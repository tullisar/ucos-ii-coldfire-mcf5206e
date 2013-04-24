/*******************************************************************************
 * file:   vectors.h
 * author: Robert Bartlett-Schneider 
 *         robert@savetherobots.org
 *         http://www.savetherobots.org
 *  
 * Contains information on the VBR and the exception vector table for the
 * MCF5206e evaluation board used for this port.
 *******************************************************************************/

#ifndef _VECTORS_
#define _VECTORS_

#define _VBR                0x20000000      // Vector Base Address Register

#define     _INITSP         _VBR+0x00       // Initial Stack Pointer
#define     _INITPC         _VBR+0x04       // Initial Program Counter
#define     ERR_ACCESS      _VBR+0x08       // Access Error
#define     ERR_ADDRESS     _VBR+0x0C       // Address Error
#define     ERR_ILLEGAL     _VBR+0x10       // Illegal Instruction
#define     RESERVED_01     _VBR+0x14       // Reserved
#define     RESERVED_02     _VBR+0x18       // Reserved
#define     RESERVED_03     _VBR+0x1C       // Reserved
#define     ERR_PRIVELAGE   _VBR+0x20       // Privelage Violation
#define     INT_TRACE       _VBR+0x24       // Trace
#define     UNIMPLEMENTED_A _VBR+0x28       // Unimplemented line-a opcode
#define     UNIMPLEMENTED_F _VBR+0x2C       // Unimplemented line-f opcode
#define     INT_DEBUG       _VBR+0x30       // Debug Interrupt
#define     RESERVED_04     _VBR+0x34       // Reserved
#define     ERR_FORMAT      _VBR+0x38       // Format Error
#define     ERR_INT_UNINIT  _VBR+0x3C       // Uninitialized Interrupt
#define     RESERVED_05     _VBR+0x40       // Reserved
#define     RESERVED_06     _VBR+0x44       // Reserved
#define     RESERVED_07     _VBR+0x48       // Reserved
#define     RESERVED_08     _VBR+0x4C       // Reserved
#define     RESERVED_09     _VBR+0x50       // Reserved
#define     RESERVED_10     _VBR+0x54       // Reserved
#define     RESERVED_11     _VBR+0x58       // Reserved
#define     RESERVED_12     _VBR+0x5C       // Reserved
#define     INT_SPURIOUS    _VBR+0x60       // Spurious Interrupt
#define     INT_AVEC1       _VBR+0x64       // Autovector Level 1
#define     INT_AVEC2       _VBR+0x68       // Autovector Level 2
#define     INT_AVEC3       _VBR+0x6C       // Autovector Level 3
#define     INT_AVEC4       _VBR+0x70       // Autovector Level 4
#define     INT_AVEC5       _VBR+0x74       // Autovector Level 5
#define     INT_AVEC6       _VBR+0x78       // Autovector Level 6
#define     INT_AVEC7       _VBR+0x7C       // Autovector Level 7
#define     INT_TRAP_0      _VBR+0x80       // TRAP #0
#define     INT_TRAP_1      _VBR+0x84       // TRAP #1
#define     INT_TRAP_2      _VBR+0x88       // TRAP #2
#define     INT_TRAP_3      _VBR+0x8C       // TRAP #3
#define     INT_TRAP_4      _VBR+0x90       // TRAP #4
#define     INT_TRAP_5      _VBR+0x94       // TRAP #5
#define     INT_TRAP_6      _VBR+0x98       // TRAP #6
#define     INT_TRAP_7      _VBR+0x9C       // TRAP #7
#define     INT_TRAP_8      _VBR+0xA0       // TRAP #8
#define     INT_TRAP_9      _VBR+0xA4       // TRAP #9
#define     INT_TRAP_10     _VBR+0xA8       // TRAP #10
#define     INT_TRAP_11     _VBR+0xAC       // TRAP #11
#define     INT_TRAP_12     _VBR+0xB0       // TRAP #12
#define     INT_TRAP_13     _VBR+0xB4       // TRAP #13
#define     INT_TRAP_14     _VBR+0xB8       // TRAP #14
#define     INT_TRAP_15     _VBR+0xBC       // TRAP #15

/*
#define     RESERVED_13     _VBR+0xC0       // Reserved
.....
#define     RESERVED_28     _VBR+0xFC       // Reserved
*/

/*
#define     INT_USER_0      _VBR+0x100      // User-defined Interrupt 0
.....
#define     INT_USER_191    _VBR+0x3FC      // User-defined Interrupt 191
*/

inline asm void initialize_vbr()
{
    LEA     _VBR,A0
    MOVEC.L A0,VBR
    
}

#endif