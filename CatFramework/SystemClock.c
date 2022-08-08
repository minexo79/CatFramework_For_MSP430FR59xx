#include "SystemClock.h"

// System Clock =======================================
enum dcoClock dcoClock = NON;

/*
 * Initialize And Configure System Clock
 */
void SystemClock_Init (enum dcoClock clock)
{
    // System Clock and XT1 Setup
    CSCTL0_H = CSKEY >> 8;                            // Unlock CS registers
    switch(clock) {                                   // Change DCO Clock
        case C1MHZ:
            dcoClock = C1MHZ;
            CSCTL1 = DCOFSEL_0;                       // Set DCO to 1MHz
            break;
        case C8MHZ:
            dcoClock = C8MHZ;
            CSCTL1 = DCOFSEL_3 | DCORSEL;             // Set DCO to 8MHz
            break;
        case C16MHZ:
            dcoClock = C16MHZ;
            CSCTL1 = DCOFSEL_4 | DCORSEL;             // Set DCO to 16MHz
            break;
    }

    CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;             // Set all dividers to 1
    CSCTL4 &= ~LFXTOFF;                               // Enable LFXT1

    do {
        CSCTL5 &= ~LFXTOFFG;                          // Clear XT1 fault flag
        SFRIFG1 &= ~OFIFG;
    }while (SFRIFG1&OFIFG);                           // Test oscillator fault flag

    CSCTL0_H = 0;                                     // Lock CS registers
}
