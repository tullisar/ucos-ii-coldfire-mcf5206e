/********************************************************************************************************
*                                               uC/OS-II
*                                         The Real-Time Kernel
*
*                          (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                          All Rights Reserved
*
*
*                                       Motorola Coldfire MCF5206e
*                                         Metrowerks CodeWarrior
*
* File         : os_cpu_a.s
* RTOS Author  : Jean J. Labrosse
* Port Author  : Robert Bartlett-Schneider
********************************************************************************************************/

        .public _OSStartHighRdy
        .public _OSCtxSw
        .public _OSIntCtxSw
        .public _OSTickISR
        .public _CPUFormatError
        .public _CPUSaveSR
        .public _CPUDisableInterrupts
        .public _CPURestoreSR
        
        
        .extern _OSIntEnter
        .extern _OSIntExit
        .extern _OSTimeTick
        .extern _OSTaskSwHook
        .extern _OSIntNesting
        .extern _OSTickDOSCtr
        .extern _OSPrioHighRdy
        .extern _OSPrioCur
        .extern _OSRunning
        .extern _OSTCBCur
        .extern _OSTCBHighRdy

/********************************************************************************************************
Some basic defines needed for the tick ISR, since the ColdFire assembler can't import equates,
C #defines, or local labels from another file 
********************************************************************************************************/
    
__MBAR  .equ $10000000
__TMR1  .equ __MBAR+$100
__TRR1  .equ __MBAR+$104
__TCR1  .equ __MBAR+$108
__TCN1  .equ __MBAR+$10C
__TER1  .equ __MBAR+$111

        .text
    
/********************************************************************************************************
*                                          START MULTITASKING
*                                       void OSStartHighRdy(void)
********************************************************************************************************/
_OSStartHighRdy:

    JSR     _OSTaskSwHook       // Call the switch hook function
    MOVEQ   #1,D0
    MOVE.B  D0,(_OSRunning)     // Tell the kernel it is running
    MOVE.L  (_OSTCBHighRdy),A1  // Get the highest priority task
    MOVE.L  (A1),A7             // Get pointer to top of task's stack
    MOVEM.L (A7),D0-D7/A0-A6    // Load all the registers
    LEA     60(A7),A7           // Advance the stack pointer
    
    RTE                         // Return to task

/********************************************************************************************************
*                                PERFORM A CONTEXT SWITCH (From an ISR)
*                                        void OSIntCtxSw(void)
*
********************************************************************************************************/
_OSIntCtxSw:
    ADDA.L #12,A7               // Compensate stack pointer
    BRA    _OSCtxSwMain         // Handle the context switch
    
/********************************************************************************************************
*                                PERFORM A CONTEXT SWITCH (From task level)
*                                           void OSCtxSw(void)
*
* Note(s): 1) Upon entry,
*             OSTCBCur     points to the OS_TCB of the task to suspend
*             OSTCBHighRdy points to the OS_TCB of the task to resume
********************************************************************************************************/
_OSCtxSw:
    LEA     -60(A7),A7          // Offset the stack pointer
    MOVEM.L D0-D7/A0-A6,(A7)    // Save the current task's stack
    
_OSCtxSwMain:
    MOVE.L  (_OSTCBCur),A1      // Save stack pointer in TCB
    MOVE.L  A7,(A1)             
    
    JSR _OSTaskSwHook           // Call the hook proc
    
    MOVE.L  (_OSTCBHighRdy),A1  // Point to highest priority task ready
    MOVE.L  A1,(_OSTCBCur)      // Current TCB becomes current
    MOVE.B  (_OSPrioHighRdy),D0 // Track the highest priority
    MOVE.B  D0,(_OSPrioCur)
    
    MOVE.L  (A1),A7             // Get new task stack pointer
    MOVEM.L (A7),D0-D7/A0-A6    // Restore other registers
    LEA     60(A7),A7           // Update stack pointer
    
    RTE
    
            
/*********************************************************************************************************
*                                            HANDLE TICK ISR
*
* Description: Uses TIMER1 on the MCF5206e as the timer for a consistent interrupt source.
*
* Arguments  : none
*
* Returns    : none
*********************************************************************************************************/    
_OSTickISR:
    LEA     -60(A7),A7          // Save registers to task stack
    MOVEM.L D0-D7/A0-A6,(A7)
    
    LEA     __TER1,A0           // Clear Timer Event Register 1 by writing 
    MOVE.B  #0x03,(A0)          // ones to bits 0:1.
    
    JSR     _OSIntEnter         // OSIntNesting++
    JSR     _OSTimeTick         // OSTimeTick() notifies uC/OS-II a tick has occured
    JSR     _OSIntExit          // OSIntExit() end of ISR
    
    MOVEM.L (A7),D0-D7/A0-A6    // Restore registers that were saved onto the stack
    LEA     60(A7),A7          
    
    RTE                         // Return from interrupt
    

/* FORMAT ERROR */
_CPUFormatError:
    NOP
    NOP
    NOP
    NOP
    HALT
    
