#include "GPIO.h"

// Reset GPIO Level and Status
void GPIO_Init (void)
{
    AssignBit(P1OUT, 0x00);    AssignBit(P1DIR, 0xFF);
    AssignBit(P2OUT, 0x00);    AssignBit(P2DIR, 0xFF);
    AssignBit(P3OUT, 0x00);    AssignBit(P3DIR, 0xFF);
    AssignBit(P4OUT, 0x00);    AssignBit(P4DIR, 0xFF);

#ifdef __MSP430FR5994__
    AssignBit(P5OUT, 0x00);    AssignBit(P5DIR, 0xFF);
    AssignBit(P6OUT, 0x00);    AssignBit(P6DIR, 0xFF);
    AssignBit(P7OUT, 0x00);    AssignBit(P7DIR, 0xFF);
#endif

    AssignBit(PJOUT,   0x00);
    AssignBit(PJSEL0, (BIT4 | BIT5));      // For XT1
    AssignBit(PJDIR,   0xFFFF);

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    ResetBit(PM5CTL0, LOCKLPM5);
}
