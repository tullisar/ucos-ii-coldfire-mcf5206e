/*
*********************************************************************************************************
*                                               uC/OS-II
*                                         The Real-Time Kernel
*
*                        (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                          All Rights Reserved
*
*                                       Motorola Coldfire MCF5206e
*                                         Metrowerks CodeWarrior
*
* File         : os_cpu_c.c
* RTOS Author  : Jean J. Labrosse
* Port Author  : Robert Bartlett-Schneider
*********************************************************************************************************
*/

#define  OS_CPU_GLOBALS
#include "includes.h"

#ifdef OS_CPU_GLOBALS
#define OS_CPU_EXT
#else
#define OS_CPU_EXT extern
#endif

/*  TaskReturnDelete()
 *
 *	Provide a simulated call site for every task function.
 *
 *  This allows a task function which accidentally returns to
 *  delete itself.
 *
 */
void TaskReturnDelete()
{
    CPUA_NOP();
    //printf("!!!!! Task at prio %d function returned.\n", OSPrioCur);
    OSTaskDel(OS_PRIO_SELF);
    CPUA_HALT();
}




/*
*********************************************************************************************************
*                                        INITIALIZE A TASK'S STACK
*
* Description: This function is called by either OSTaskCreate() or OSTaskCreateExt() to initialize the
*              stack frame of the task being created.  This function is highly processor specific.
*
* Arguments  : task          is a pointer to the task code
*
*              pdata         is a pointer to a user supplied data area that will be passed to the task
*                            when the task first executes.
*
*              ptos          is a pointer to the top of stack.  It is assumed that 'ptos' points to
*                            a 'free' entry on the task stack.  If OS_STK_GROWTH is set to 1 then 
*                            'ptos' will contain the HIGHEST valid address of the stack.  Similarly, if
*                            OS_STK_GROWTH is set to 0, the 'ptos' will contains the LOWEST valid address
*                            of the stack.
*
*              opt           specifies options that can be used to alter the behavior of OSTaskStkInit().
*                            (see uCOS_II.H for OS_TASK_OPT_???).
*
* Returns    : Always returns the location of the new top-of-stack' once the processor registers have
*              been placed on the stack in the proper order.
*  Note that the ColdFire stack *must* be 4 byte aligned, and
*  that the exception flag word at the top contains an alignment
*  code describing the initial misalignment at the time of
*  the exception.
*
*  This code will adjust the passed top of stack for the
*  required alignment *before* building the stack frame
*  and thus use the constant value 4 for the format field
*  in the exception frame.
*
*  The stack will contain an exception taken after the call of
*  the task function and before the execution of any of the
*  task function prolog.  A dummy return address will be pushed
*  pointing to a call site which will fall into a call to
*  OSDeleteTask() to delete any task whos function returns.
*
*  (high address)
*                +------+------+------+------+
*  TOP_OF_STACK: |           pdata           |
*                +------+------+------+------+
*                |   dummy return address    |
*                +------+------+------+------+
*                | format, vector, fault, SR |
*                +------+------+------+------+
*                |PC of exception (task func)|
*                +------+------+------+------+
*                |             A6            |
*                +------+------+------+------+
*                |                           |
*                            . . .
*                |                           |
*                +------+------+------+------+
*                |        A1 (pdata)         |
*                +------+------+------+------+
*                |                           |
*                            . . .
*                |                           |
*                +------+------+------+------+
*   SP --->      |             D0            |
*                +------+------+------+------+
*   (low address)
*********************************************************************************************************
*/
OS_STK  *OSTaskStkInit (void (*task)(void *pd), void *pdata, OS_STK *ptos, INT16U opt)
{
    OS_STK *stk;
    OS_STK stkframe = 0x40000000;
    opt    = opt;                           /* 'opt' is not used, prevent warning                      */
    stk    = (OS_STK *)ptos;                /* Load stack pointer                                      */
    
    /* The stack needs to be 4 byte aligned, thus we have to check that first */
    /* I don't entirely understand how the stack can get mis-aligned, but this is how */
    /* it was handled on another ColdFire processor */
    switch ( (long) ptos & (long) 0x00000003 )
    {
        case 0:
            stk = STK_ALIGN(stk, -0);
            break;
        case 1:
            stk = STK_ALIGN(stk, -1);
            break;
        case 2:
            stk = STK_ALIGN(stk, -2);
            break;
        case 3:
            stk = STK_ALIGN(stk, -3);
            break;
    }
    
    stkframe = stkframe | (OS_STK)OS_SR_DEFAULT;
    
    *stk   = 0;                         /* TODO: WHAT IS THIS? */

    *--stk = (OS_STK)pdata;             /* Parameters */
    *--stk = (OS_STK)TaskReturnDelete;  /* See above, address of TaskReturnDelete */
    
    *--stk = (OS_STK)task;              /* Exception frame setup per MCF5206e */
    *--stk = (OS_STK)stkframe;          /* user manual. See section 3.4 */
    
    /*
    Imitate the execution of
    MOVEM.L D0-D7/A0-A6,-(SP)
    in order to save all registers to the stack. For debugging purposes
    registers are filled with meaningful values. Normally they should
    all be set to 0
    */
    
    *--stk = (OS_STK)ptos;      /* a6 */
    *--stk = 0xA5UL;            /* a5 */
    *--stk = 0xA4UL;            /* a4 */
    *--stk = 0xA3UL;            /* a3 */
    *--stk = 0xA2UL;            /* a2 */
    *--stk = (OS_STK)pdata;     /* a1 */
    *--stk = 0xA0UL;            /* a0 */
    *--stk = 0xD7UL;            /* d7 */
    *--stk = 0xD6UL;            /* d6 */
    *--stk = 0xD5UL;            /* d5 */
    *--stk = 0xD4UL;            /* d4 */
    *--stk = 0xD3UL;            /* d3 */
    *--stk = 0xD2UL;            /* d2 */
    *--stk = 0xD1UL;            /* d1 */
    *--stk = 0xD0UL;            /* d0 */

    return ((OS_STK *)stk);
}

/***** NON IMPLEMENTED FUNCTIONS *********/
/*
*********************************************************************************************************
*                                           TASK SWITCH HOOK
*
* Description: This function is called when a task switch is performed.  This allows you to perform other
*              operations during a context switch.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are disabled during this call.
*              2) It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                 will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the 
*                 task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0
void OSTaskSwHook (void)
{
}
#endif

/*
*********************************************************************************************************
*                                           OSTCBInit() HOOK
*
* Description: This function is called by OS_TCBInit() after setting up most of the TCB.
*
* Arguments  : ptcb    is a pointer to the TCB of the task being created.
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void  OSTCBInitHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                                           /* Prevent Compiler warning                 */
}
#endif

/*
*********************************************************************************************************
*                                               TICK HOOK
*
* Description: This function is called every tick.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0
void OSTimeTickHook (void) { }
#endif

/*
 *********************************************************************************************************
 *                                       OS INITIALIZATION HOOK
 *                                            (BEGINNING)
 *
 * Description: This function is called by OSInit() at the beginning of OSInit().
 *
 * Arguments  : none
 *
 * Note(s)    : 1) Interrupts should be disabled during this call.
 *********************************************************************************************************
 */
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void  OSInitHookBegin (void)
{
}
#endif

/*
 *********************************************************************************************************
 *                                       OS INITIALIZATION HOOK
 *                                               (END)
 *
 * Description: This function is called by OSInit() at the end of OSInit().
 *
 * Arguments  : none
 *
 * Note(s)    : 1) Interrupts should be disabled during this call.
 *********************************************************************************************************
 */
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void  OSInitHookEnd (void)
{
}
#endif

/*
 *********************************************************************************************************
 *                                          TASK CREATION HOOK
 *
 * Description: This function is called when a task is created.
 *
 * Arguments  : ptcb   is a pointer to the task control block of the task being created.
 *
 * Note(s)    : 1) Interrupts are disabled during this call.
 *********************************************************************************************************
 */
#if OS_CPU_HOOKS_EN > 0 
void  OSTaskCreateHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
}
#endif

/*
 *********************************************************************************************************
 *                                           TASK DELETION HOOK
 *
 * Description: This function is called when a task is deleted.
 *
 * Arguments  : ptcb   is a pointer to the task control block of the task being deleted.
 *
 * Note(s)    : 1) Interrupts are disabled during this call.
 *********************************************************************************************************
 */
#if OS_CPU_HOOKS_EN > 0 
void  OSTaskDelHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
}
#endif

/*
 *********************************************************************************************************
 *                                             IDLE TASK HOOK
 *
 * Description: This function is called by the idle task.  This hook has been added to allow you to do  
 *              such things as STOP the CPU to conserve power.
 *
 * Arguments  : none
 *
 * Note(s)    : 1) Interrupts are enabled during this call.
 *********************************************************************************************************
 */
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION >= 251
void  OSTaskIdleHook (void)
{
    
}
#endif

/*
 *********************************************************************************************************
 *                                           STATISTIC TASK HOOK
 *
 * Description: This function is called every second by uC/OS-II's statistics task.  This allows your 
 *              application to add functionality to the statistics task.
 *
 * Arguments  : none
 *********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 
void  OSTaskStatHook (void)
{
}
#endif
/***** END NON IMPLEMENTED FUNCTIONS *******/