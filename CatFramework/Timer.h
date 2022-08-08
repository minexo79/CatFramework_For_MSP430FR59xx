#ifndef _TIMER_H_
#define _TIMER_H_

#include "../CatFramework.h"

// Timer Unit ===========================================

/*
 * Register Map
 *
 * timerControlReg          => TxxCTL
 * timerCapComReg           => TxxCCR0
 * timerCapComControlReg    => TxxCCTL0
 */
extern volatile unsigned *  timerControlReg         [];
extern volatile unsigned *  timerCapComReg          [];
extern volatile unsigned *  timerCapComControlReg   [];

// Enum
enum timerReg
    {TA0, TA1, TA2, TA3} timerReg;
enum timerControl
    {Stop, Up, Continuous} timerControl;
enum timerDivider
    {Div1, Div2, Div4, Div8} timerDivider;
enum timerInterrupt
    {NOIE, IE} timerInterrupt;

// Function
extern void Initialize_Timer        (char, char, char, unsigned, char);
extern void Stop_Timer              (char);

#endif
