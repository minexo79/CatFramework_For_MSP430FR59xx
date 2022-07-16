/*
 *  Node.c
 *
 *  Created on: 2021.09.17
 *      Author: Xin You Tsai
 */

#include <Node.h>

// System Clock =======================================
enum dcoClock dcoClock = NON;

// UART ===============================================
volatile unsigned char * uartSelPort0 [] =
{
    &P2SEL0,            // UART0
    &P2SEL0,            // UART1
#ifdef __MSP430FR5994__
    &P5SEL0,            // UART2
    &P6SEL0             // UART3
#endif
};

volatile unsigned char * uartSelPort1 [] =
{
    &P2SEL1,            // UART0
    &P2SEL1,            // UART1
#ifdef __MSP430FR5994__
    &P5SEL1,            // UART2
    &P6SEL1             // UART3
#endif
};

volatile unsigned int * uartControlReg [] =
{
    &UCA0CTLW0,         // UART0
    &UCA0CTLW1,         // UART1
#ifdef __MSP430FR5994__
    &UCA0CTLW2,         // UART2
    &UCA0CTLW3,         // UART3
#endif
};

volatile unsigned * uartBaudrateReg [] =
{
     &UCA0BRW,          // UART0
     &UCA1BRW,          // UART1
#ifdef __MSP430FR5994__
     &UCA2BRW,          // UART2
     &UCA3BRW           // UART3
#endif
};

volatile unsigned * uartInterruptReg [] =
{
     &UCA0IE,           // UART0
     &UCA1IE,           // UART1
#ifdef __MSP430FR5994__
     &UCA2IE,           // UART2
     &UCA3IE            // UART3
#endif
};

const volatile unsigned char uartSelPin [] =
{
    (BIT0 | BIT1),      // UART0
    (BIT5 | BIT6),      // UART1
#ifdef __MSP430FR5994__
    (BIT4 | BIT5),      // UART2
    (BIT0 | BIT1)       // UART3
#endif
};

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


int stoi(char * src)
{
    char    index = 0;
    int     res = 0;

    for(;src[index] > 0;index++)
        res = (res * 10) + (src[index] - '0');

    return res;
}

void ledBlink (volatile unsigned char * port,
               volatile unsigned char pin,
               unsigned char step)
{
    while(step--)
    {
        *(port) |= (pin);
        DELAY_US(100000);
        *(port) &= ~(pin);
        DELAY_US(100000);
    }
}


void Initialize_GPIO(void)
{
    // Reset GPIO Level and Status
    P1OUT = 0x00;
    P1DIR = 0xFF;

    P2OUT = 0x00;
    P2DIR = 0xFF;

    P3OUT = 0x00;
    P3DIR = 0xFF;

    P4OUT = 0x00;
    P4DIR = 0xFF;

#ifdef __MSP430FR5994__
    P5OUT = 0x00;
    P5DIR = 0xFF;

    P6OUT = 0x00;
    P6DIR = 0xFF;

    P7OUT = 0x00;
    P7DIR = 0xFF;
#endif

    PJOUT = 0x00;
    PJSEL0 = BIT4 | BIT5;                       // For XT1
    PJDIR = 0xFFFF;
}

void sort(unsigned *arr, unsigned size)
{
    char i, j;
    unsigned temp;
    for(i=1;i<size;i++)
    {
        temp = arr[i];                          // current value
        for(j=i;j>0;j--)
        {
            if(temp < arr[j-1])                 // less than prevoius, right shift value
                arr[j] = arr[j-1];
            else
                break;
        }
        arr[j] = temp;                          // put in last address
    }
}

/*
 * Initialize And Configure System Clock
 */
void Initialize_SystemClock(char clock)
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

/*
 * Initialize And Configure UART Unit
 * =================================
 * FR5969: UCA0 & UCA1
 * FR5994: UCA0 & UCA1 & UCA2 & UCA3
 */
void Initialize_UART (char unit, unsigned baud, char interruptSel)
{
    // Pin Config
    *(uartSelPort0[unit]) |=    uartSelPin[unit];
    *(uartSelPort1[unit]) &=~   uartSelPin[unit];

    // Switch To Factory Mode
    *(uartControlReg[unit]) |= (UCSWRST | UCSSEL__SMCLK);

    // Config Baudrate
    // TODO: ToMap
    if (dcoClock == C1MHZ)          // 1MHz
        *(uartBaudrateReg[unit]) = 1000000 / baud;
    else if (dcoClock == C8MHZ)     // 8MHz
        *(uartBaudrateReg[unit]) = 8000000 / baud;
    else if (dcoClock == C16MHZ)    // 16MHz
        *(uartBaudrateReg[unit]) = 16000000 / baud;
    else                            // Default
        *(uartBaudrateReg[unit]) = 1000000 / baud;

    // Reset To Running Mode
    *(uartControlReg[unit]) &=~ (UCSWRST);

    // Enable Interrupt Or Not?
    // TODO: ToMap
    if(interruptSel == TXIE)
        *(uartInterruptReg[unit]) |=    (UCTXIE);
    else if (interruptSel == RXIE)
        *(uartInterruptReg[unit]) |=    (UCRXIE);
    else if (interruptSel == TRXIE)
        *(uartInterruptReg[unit]) |=    (UCTXIE | UCRXIE);
    else
        *(uartInterruptReg[unit]) &=~   (UCTXIE | UCRXIE);
}

/*
 * Initialize And Configure Timer Unit
 * =================================
 * FR59xx: TA0, TA1, TA2, TA3
 */

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

//void Initialize_Timer_A0(unsigned period)
//{
//    TA0CTL |= (TASSEL__ACLK | ID_3 | MC_1 | TACLR);     // TA0_DIV /8, Up Mode
//    TA0CCR0 = period;
//    TA0CCTL0 |= CCIE;                                   // Enable TimerA Interrupt
//}
//
//void Initialize_Timer_A1(unsigned period)
//{
//    TA1CTL |= (TASSEL__ACLK | ID_3 | MC_1 | TACLR);     // TA1 DIV /8, Up Mode
//    TA1CCR0 = period;
//    TA1CCTL0 |= CCIE;                                   // Enable TimerA Interrupt
//}
//
//void Initialize_Timer_A2(unsigned period)
//{
//    TA2CTL |= (TASSEL__ACLK | ID_3 | MC_1 | TACLR);     // TA2 DIV /8, Up Mode
//    TA2CCR0 = period;
//    TA2CCTL0 |= CCIE;                                   // Enable TimerA2 Interrupt
//}
