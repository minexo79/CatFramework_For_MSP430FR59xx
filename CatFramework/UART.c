#include "UART.h"

/*
 * Initialize And Configure UART Unit
 * =================================
 * FR5969: UCA0 & UCA1
 * FR5994: UCA0 & UCA1 & UCA2 & UCA3
 */

// UART ===============================================
_Reg8 * uartSelPort0 [] =
{
    &P2SEL0,           // UART0
    &P2SEL0,           // UART1
#ifdef __MSP430FR5994__
    &P5SEL0,           // UART2
    &P6SEL0            // UART3
#endif
};

_Reg8 * uartSelPort1 [] =
{
    &P2SEL1,           // UART0
    &P2SEL1,           // UART1
#ifdef __MSP430FR5994__
    &P5SEL1,           // UART2
    &P6SEL1            // UART3
#endif
};

_Reg16 * uartControlReg [] =
{
    &UCA0CTLW0,        // UART0
    &UCA1CTLW0,        // UART1
#ifdef __MSP430FR5994__
    &UCA2CTLW0,        // UART2
    &UCA3CTLW0,        // UART3
#endif
};

_Reg16 * uartBaudrateReg [] =
{
    &UCA0BRW,          // UART0
    &UCA1BRW,          // UART1
#ifdef __MSP430FR5994__
    &UCA2BRW,          // UART2
    &UCA3BRW           // UART3
#endif
};

_Reg16 * uartInterruptReg [] =
{
    &UCA0IE,           // UART0
    &UCA1IE,           // UART1
#ifdef __MSP430FR5994__
    &UCA2IE,           // UART2
    &UCA3IE            // UART3
#endif
};

_Reg16 * uartInterFlagReg [] =
{
    &UCA0IFG,          // UART0
    &UCA1IFG,          // UART1
#ifdef __MSP430FR5994__
    &UCA2IFG,          // UART2
    &UCA3IFG           // UART3
#endif
};

_Reg16 * uartWriteReg [] =
{
    &UCA0TXBUF,        // UART0
    &UCA1TXBUF,        // UART1
#ifdef __MSP430FR5994__
    &UCA2TXBUF,        // UART2
    &UCA3TXBUF,        // UART3
#endif
};

const _Reg8 uartSelPin [] =
{
    (BIT0 | BIT1),      // UART0
    (BIT5 | BIT6),      // UART1
#ifdef __MSP430FR5994__
    (BIT4 | BIT5),      // UART2
    (BIT0 | BIT1)       // UART3
#endif
};

void Uart_ParamInit(uartParam * ptr)
{
    ptr->unit = UART0;
    ptr->baud = 9600;
    ptr->interruptMode = NONIE;
}

void Uart_Init (uartParam * ptr)
{
    // Pin Config
    // UCA0 / UCA1: PxSEL0 = 0, PxSEL1 = 1;
    if (ptr->unit == UART0 || ptr->unit == UART1)
    {
        *(uartSelPort0[ptr->unit]) &=~  uartSelPin[ptr->unit];
        *(uartSelPort1[ptr->unit]) |=   uartSelPin[ptr->unit];
    }
    // UCA2 / UCA3: PxSEL0 = 1, PxSEL1 = 0;
    else
    {
        *(uartSelPort0[ptr->unit]) |=   uartSelPin[ptr->unit];
        *(uartSelPort1[ptr->unit]) &=~  uartSelPin[ptr->unit];
    }

    // Switch To Factory Mode
    *(uartControlReg[ptr->unit]) =  (UCSWRST | UCSSEL__SMCLK);

    // Config Baudrate
    if (dcoClock == C16MHZ)         // 16MHz
        *(uartBaudrateReg[ptr->unit]) = 16000000 / ptr->baud;
    else if (dcoClock == C8MHZ)     // 8MHz
        *(uartBaudrateReg[ptr->unit]) = 8000000 / ptr->baud;
    else                            // 1MHz : Default
        *(uartBaudrateReg[ptr->unit]) = 1000000 / ptr->baud;

    // Reset To Running Mode
    *(uartControlReg[ptr->unit]) &=~ (UCSWRST);

    // Enable Interrupt Or Not?
    if(ptr->interruptMode == TXIE)
        *(uartInterruptReg[ptr->unit]) |=   (UCTXIE);
    else if (ptr->interruptMode == RXIE)
        *(uartInterruptReg[ptr->unit]) |=   (UCRXIE);
    else if (ptr->interruptMode == TRXIE)
        *(uartInterruptReg[ptr->unit]) |=   (UCTXIE | UCRXIE);
    else
        *(uartInterruptReg[ptr->unit]) &=~  (UCTXIE | UCRXIE);
}

void Uart_Write (uartParam * ptr, char word)
{
    // wait previous buffer was sent.
    while((*uartInterFlagReg[ptr->unit] & UCTXIFG) != UCTXIFG);
    // send buffer
    *uartWriteReg[ptr->unit] = word;
}

void Uart_WriteStr (uartParam * ptr, char * str, char length)
{
    if(ptr->interruptMode == NONIE)
    {
        char _len = 0;
        // String was sent?
        while(_len < length)
        {
            // wait previous buffer was sent.
            while((*uartInterFlagReg[ptr->unit] & UCTXIFG) != UCTXIFG);
            // send buffer and go to next word.
            *uartWriteReg[ptr->unit] = str[_len++];
        }
    }
}

void Uart_WriteStrln (uartParam * ptr, char * str, char length)
{
    if(ptr->interruptMode == NONIE)
    {
        char _len = 0;
        // String was sent?
        while(_len < length)
        {
            // wait previous buffer was sent.
            while((*uartInterFlagReg[ptr->unit] & UCTXIFG) != UCTXIFG);
            // send buffer and go to next word.
            *uartWriteReg[ptr->unit] = str[_len++];
        }

        // CR
        while((*uartInterFlagReg[ptr->unit] & UCTXIFG) != UCTXIFG);
        *uartWriteReg[ptr->unit] = 0x0D;

        // LF
        while((*uartInterFlagReg[ptr->unit] & UCTXIFG) != UCTXIFG);
        *uartWriteReg[ptr->unit] = 0x0A;
    }
}