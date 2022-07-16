/*
 *  Node.c
 *
 *  Created on: 2021.09.17
 *      Author: Xin You Tsai
 */

#ifndef NODE_H_
#define NODE_H_

#include <stdio.h>
#include <string.h>
#include <msp430.h>

#define CYCLES_PER_US   8                           // depends on the CPU speed
#define CYCLES_PER_MS   (CYCLES_PER_US * 1000L)
#define CYCLES_PER_S    (CYCLES_PER_MS * 1000L)

#define DELAY_US(x)     __delay_cycles((x * CYCLES_PER_US))
#define DELAY_MS(x)     __delay_cycles((x * CYCLES_PER_MS))
#define DELAY_S(x)      __delay_cycles((x * CYCLES_PER_S))

/*
 * System Clock Params
 */
extern enum dcoClock
    {NON, C1MHZ, C8MHZ, C16MHZ} dcoClock;

extern void Initialize_SystemClock  (char);

// UART Unit ============================================

/*
 * Register Map
 *
 * uartSelPort0         => PxSEL0
 * uartSelPort1         => PxSEL1
 * uartControlReg       => UCAxCTLW0
 * uartBaudrateReg      => UCAxBRW
 * uartInterruptReg     => UCAxIE
 * uartSelPin           => (BITx | BITx)
 */

extern volatile unsigned char * uartSelPort0        [];
extern volatile unsigned char * uartSelPort1        [];
extern volatile unsigned *      uartControlReg      [];
extern volatile unsigned *      uartBaudrateReg     [];
extern volatile unsigned *      uartInterruptReg    [];
extern const volatile unsigned char uartSelPin      [];

// Enum
enum uartUnit
    {UART0, UART1, UART2, UART3} uartUnit;
enum uartInterrupt
    {NONIE, TXIE, RXIE, TRXIE} uartInterrupt;

// Function
extern void Initialize_UART         (char, unsigned, char);

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
    {NONIE, IE} timerInterrupt;

// Function
extern void Initialize_Timer        (char, char, char, unsgined, char);
extern void Stop_Timer              (char);

/** Useful Function **/
extern int  stoi                    (char *);                           // String to int
extern void sort                    (unsigned *, unsigned);             // sort
extern void ledBlink                (volatile unsigned char *,
                                     volatile unsigned char,
                                     unsigned char);                    // blink led

/** MSP430 Config Function **/
extern void Initialize_GPIO         (void);
extern void Initialize_Timer_A0     (unsigned);
extern void Initialize_Timer_A1     (unsigned);
extern void Initialize_Timer_A2     (unsigned);

#endif /* NODE_H_ */
