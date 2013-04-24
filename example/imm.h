/*******************************************************************************
 * file:   imm.h
 * author: Robert Bartlett-Schneider 
 *         robert@savetherobots.org
 *         http://www.savetherobots.org
 *  
 * Contains information on the MCF5206e internal memory map.
 *******************************************************************************/

#ifndef _IMM_
#define _IMM_

// Internal Memory Map
#define MBAR      0x10000000

#define ICR9      MBAR+0x01C    // Interrupt Control Register 9
#define IMR       MBAR+0x036    // Interrupt Mask Register
#define TMR1      MBAR+0x100    // Timer 1 Module Register
#define TRR1      MBAR+0x104    // Timer 1 Reference Register
#define TCR1      MBAR+0x108    // Timer 1 Capture Register
#define TCN1      MBAR+0x10C    // Timer 1 Counter
#define TER1      MBAR+0x111    // Timer 1 Event Register

#endif