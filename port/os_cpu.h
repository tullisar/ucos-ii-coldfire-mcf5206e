/*
*********************************************************************************************************
*                                               uC/OS-II
*                                        The Real-Time Kernel
*
*                         (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                          All Rights Reserved
*
*                                     Coldfire MCF5206e Specific Code
* 
* file:    os_cpu.h
* author:  Robert Bartlett-Schneider
*          robert@savetherobots.org
*          http://www.savetherobots.org
* purpose: contains defines and macros specific to the MCF5206e port of uC/OS-II
*********************************************************************************************************
*/

#ifdef  OS_CPU_GLOBALS
#define OS_CPU_EXT
#else
#define OS_CPU_EXT  extern
#endif


// #include "mcf5206e.h" // Only include for ROM stationary

/*
*********************************************************************************************************
*                                              DATA TYPES
*                                         (Compiler Specific)
*********************************************************************************************************
*/

typedef unsigned char       BOOLEAN;
typedef unsigned char       INT8U;                    /* Unsigned  8 bit quantity                      */
typedef signed   char       INT8S;                    /* Signed    8 bit quantity                      */
typedef unsigned int        INT16U;                   /* Unsigned 16 bit quantity                      */
typedef signed   int        INT16S;                   /* Signed   16 bit quantity                      */
typedef unsigned long       INT32U;                   /* Unsigned 32 bit quantity                      */
typedef signed   long       INT32S;                   /* Signed   32 bit quantity                      */
typedef unsigned long long  INT64U;                   /* Unsigned 64 bit quantity                      */
typedef signed long long    INT64S;                   /* Signed 64 bit quantity                        */
typedef float               FP32;                     /* Single precision floating point               */
typedef double              FP64;                     /* Double precision floating point               */

typedef unsigned long       OS_STK;                   /* Each stack entry is 32-bit wide               */
typedef unsigned int        OS_CPU_SR;                /* The CPU Status Word is 16-bit wide            */

/*********************************************************************************************************
*                            MCF5206e Specific Defines & Macros
*********************************************************************************************************/

#define  STK_ALIGN(src,off) (OS_STK *)((long)(src) + (off)) /* Used for re-aligning the stack, credit   */
                                                            /* to Ross Berteig in his MCF5307 ucos port */

#define  OS_MASK_INTERRUPT    0x2700
#define  OS_SR_DEFAULT        0x2000

#define  OS_STK_GROWTH        1                   /* Stack grows from HIGH to LOW memory on MCF5206e  */

inline asm void OS_TASK_SW()                      /* Function is inline, as CodeWarrior only allows   */
{                                                 /* inline assembly at the function level. This way  */
    TRAP #14                                      /* when OS_TASK_SW() is called, there is no JSR and */
}                                                 /* it is treated like a #define macro               */

inline asm void CPUA_HALT()                       /* Inlines the HALT instruction for a function */
{
    HALT
}

inline asm void CPUA_NOP()                        /* Inlines the NOP instruction for a function */
{
    NOP
}

inline asm void CPU_SPFIX()
{
    SUBQ.L  #4,SP
}




/**********************************************************************************************************
*                              OS_CRITICAL_METHOD
*
* Method #4:  Disable/Enable interrupts by preserving the state of interrupts. Stores the status register
*             in a global variable, as the assembler (inline or otherwise) cannot reference local variables
*             of other functions that are declared in another file (even when declared here as extern).
*             This is a limitation of the CodeWarrior assembler.
*********************************************************************************************************/
#define  OS_CRITICAL_METHOD    4


// extern OS_CPU_SR cpu_sr;
/* OLD METHOD
inline asm void OS_ENTER_CRITICAL() 
{
    MOVE.W SR,D0
    MOVE.W D0,cpu_sr
    MOVE.W #OS_MASK_INTERRUPT,SR
}

inline asm void OS_EXIT_CRITICAL() 
{
    MOVE.W  cpu_sr,D0
    MOVE.W  D0,SR
}
*/

#ifndef OS_CPU_CRITICAL
#define OS_CPU_CRITICAL

/* This method didn't work, stack issues.
inline asm OS_CPU_SR CPUSetSR(OS_CPU_SR cpu_sr)
{
    fralloc+
    MOVE.W SR,D0
    MOVE.W cpu_sr,D1
    MOVE.W D1,SR
    frfree
}
*/

// Working Method
OS_CPU_EXT OS_CPU_SR CPURecentSR;

inline asm OS_CPU_SR INT_MASK()
{
    MOVE.W  SR,D0
    MOVE.W  #OS_MASK_INTERRUPT,SR
}

inline asm void INT_RESTORE(OS_CPU_SR cpu_sr)
{
    MOVE.L  (SP)+,D0
    MOVE.W  D0,D1
    MOVE.W  D1,SR
}
    
#define OS_ENTER_CRITICAL() do { CPURecentSR = INT_MASK(); } while(0)
#define OS_EXIT_CRITICAL() do { INT_RESTORE(CPURecentSR); } while(0)
#endif

/*********************************************************************************************************
*                            MCF5206e Internal Memory Map
*********************************************************************************************************/


/*********************************************************************************************************
*                                            GLOBAL VARIABLES
*********************************************************************************************************/


/*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************/
