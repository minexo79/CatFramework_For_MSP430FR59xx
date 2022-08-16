#include "SystemClock.h"

// System Clock =======================================
enum dcoClock dcoClock = NON;

/*
 * Initialize And Configure System Clock
 */
void SystemClock_Init (enum dcoClock clock)
{
    // System Clock and XT1 Setup
    AssignBit(CSCTL0_H, CSKEY >> 8);                  // Unlock CS registers
    switch(clock)                                     // Change DCO Clock
    {
        case C1MHZ:
            dcoClock = C1MHZ;
            AssignBit(CSCTL1, DCOFSEL_0);             // Set DCO to 1MHz
            break;
        case C8MHZ:
            dcoClock = C8MHZ;
            AssignBit(CSCTL1, (DCOFSEL_3 | DCORSEL)); // Set DCO to 8MHz
            break;
        case C16MHZ:
            dcoClock = C16MHZ;
            AssignBit(CSCTL1, (DCOFSEL_4 | DCORSEL)); // Set DCO to 16MHz
            break;
    }

    // Set Clock Source
    AssignBit(CSCTL2, (SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK));
    // Set all dividers to 1
    AssignBit(CSCTL3, (DIVA__1 | DIVS__1 | DIVM__1));
    // Enable LFXT1
    ResetBit(CSCTL4, LFXTOFF);

    do
    {
        ResetBit(CSCTL5, LFXTOFFG);                   // Clear XT1 fault flag
        ResetBit(SFRIFG1, OFIFG);
    }
    while (SFRIFG1&OFIFG);                            // Test oscillator fault flag

    AssignBit(CSCTL0_H , 0);                          // Lock CS registers
}
