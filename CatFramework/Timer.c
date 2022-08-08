#include "Timer.h"

/*
 * Initialize And Configure Timer Unit
 * =================================
 * FR59xx: TA0, TA1, TA2, TA3
 */

// Timer =============================================
volatile unsigned * timerControlReg [] =
{
    &TA0CTL,            // TA0
    &TA1CTL,            // TA1
    &TA2CTL,            // TA2
    &TA3CTL,            // TA3
};
volatile unsigned * timerCapComReg [] =
{
    &TA0CCR0,           // TA0
    &TA1CCR0,           // TA1
    &TA2CCR0,           // TA2
    &TA3CCR0,           // TA3
};
volatile unsigned * timerCapComControlReg [] =
{
    &TA0CCTL0,          // TA0
    &TA1CCTL0,          // TA1
    &TA2CCTL0,          // TA2
    &TA3CCTL0,          // TA3
};

void Initialize_Timer (char unit, char divider, char mode, unsigned period, char enableInt)
{
    // Configure Clock Source & Reset Timer Value
    *(timerControlReg[unit]) |= (TASSEL__ACLK | TACLR);   // Default: 32.768Hz OSC

    // TODO: Configure Divider

    // TODO: Configure period

    // TODO: Enable Interrupt Or Not?

    // Configure Mode
    // TODO: ToMap
    if (mode == Up)
        *(timerControlReg[unit]) |= (MC__UP);
    else if (mode == Continuous)
        *(timerControlReg[unit]) |= (MC__CONTINUOUS);
    else
        *(timerControlReg[unit]) |= (MC__STOP);
}

void Stop_Timer (char unit)
{

}
