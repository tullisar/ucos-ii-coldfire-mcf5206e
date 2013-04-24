/*******************************************************************************
* file:   main.c
* author: Robert Bartlett-Schneider 
*         robert@savetherobots.org
*         http://www.savetherobots.org
*  
* Contains initialization code for uC/OS-II and the MCF5206e board
*******************************************************************************/

#include "includes.h"

OS_STK    StartupTaskStk[100];
INT8U     led_state;

// Set up Timer 1
// PS: 54, CE Disable, ORI enable
// FRR, Stopped, Enabled
#define TMR1_SET   0x3600 |\
                   0x0010 |\
                   0x0008 |\
                   0x0001

// Timer 1 start
// CLK: Master System Clock
#define TMR1_GO    0x00000002

// Timer 1 reset & enable
#define TMR1_RESET 0x0000
#define TMR1_ENBLE 0x0001

inline asm void setup_timer() 
{
    // Clear D2
    CLR.L   D2
    
    // Temporarily disable interrupt
    LEA     IMR,A4          
    MOVE.W  (A4),D2         
    BSET    #9,D2            // mask
    MOVE.W  D2,(A4)
    
    // Reset the timer
    LEA     TMR1,A1
    MOVE.W  #TMR1_ENBLE,(A1) // enable
    MOVE.W  #TMR1_RESET,(A1) // reset
    
    // Set up the timer
    MOVE.W  #TMR1_ENBLE,(A1) // enable
    MOVE.W  #TMR1_SET,(A1)   // set up
    
    LEA     TCN1,A2          // timer count 0
    MOVE.W  #0x0000,(A2)
    
    LEA     TRR1,A3          // reference 10000
    MOVE.W  #0x2710,(A3)
    
    MOVE.W  (A1),D2          // start the timer
    ORI.L   #TMR1_GO,D2
    MOVE.W  D2,(A1)
    
    LEA     ICR9,A5          // AVEC level 5, priority 3
    MOVE.B  #0x97,(A5)
    
    MOVE.W  (A4),D2          // unmask
    BCLR    #9,D2
    MOVE.W  D2,(A4)
}

inline asm void light_on() 
{
    LEA     LED_COL,A0
    LEA     LED_ROW,A1
    MOVE.W  #0x5500,(A1)
    MOVE.W  #0x0000,(A0)
}

inline asm void light_off() 
{
    LEA     LED_COL,A0
    LEA     LED_ROW,A1
    MOVE.W  #0x0000,(A1)
    MOVE.W  #0x0700,(A0)
}

void registerInterrupt(unsigned long function, unsigned long *vect)
{
    unsigned long *vector;
    vector = vect;
    *vector = function;             
}


void registerAllInterrupts()
{
    // TODO: Figure out why the preprocessor is calculating values incorrectly
    // leading to _VBR+0x38 being equal to 0x200000E0 instead of 0x20000038
    // For now, using hard coded values.
    extern void CPUFormatError(void);
    registerInterrupt((unsigned long)&CPUFormatError, (unsigned long *)0x20000038);  
    registerInterrupt((unsigned long)&OSTickISR, (unsigned long *)0x20000074);   
    registerInterrupt((unsigned long)&OSCtxSw,(unsigned long *)0x200000B8);
}

void StartupTask (void *pdata) {

    #if OS_CRITICAL_METHOD == 3     // allocate space for status register
    OS_CPU_SR  cpu_sr;              // not needed with current port
    #endif
    
    pdata = pdata;                  // prevent compiler warning
    OS_ENTER_CRITICAL();
    setup_timer();
    OS_EXIT_CRITICAL();
 
    led_state = 1;
    light_on();
    
    while (1) {
        OSTimeDly(10);              // delay for 10 ticks
        //OSTimeDlyHMSM(0,0,1,0);   // delay for 1 second
        if (led_state == 0) {
            led_state = 1;
            light_on();
        } else {
            led_state = 0;
            light_off();
        }
    }
}

void main()
{
    initialize_vbr();
    OSInit();
    light_off();
    registerAllInterrupts();
    OSTaskCreate(StartupTask, (void *)0, &StartupTaskStk[99], 5);
    OSStart();
}