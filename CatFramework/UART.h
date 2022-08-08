#ifndef _UART_H_
#define _UART_H_

#include "../CatFramework.h"

// UART Unit ============================================

/*
 * Register Map
 *
 * uartSelPort0         => PxSEL0
 * uartSelPort1         => PxSEL1
 * uartControlReg       => UCAxCTLW0
 * uartBaudrateReg      => UCAxBRW
 * uartInterruptReg     => UCAxIE
 * uartInterFlagReg     => UCAxIFG
 * uartWriteReg         => UCAxTXBUF
 * uartSelPin           => (BITx | BITx)
 */

extern _Reg8 *  uartSelPort0        [];
extern _Reg8 *  uartSelPort1        [];
extern _Reg16 * uartControlReg      [];
extern _Reg16 * uartBaudrateReg     [];
extern _Reg16 * uartInterruptReg    [];
extern _Reg16 * uartInterFlagReg    [];
extern _Reg16 * uartWriteReg        [];
extern const _Reg8 uartSelPin       [];

// Enum
enum uartUnit       {UART0, UART1, UART2, UART3}    uartUnit;
enum uartInterrupt  {NONIE, TXIE, RXIE, TRXIE}      uartInterrupt;

typedef struct uartParam
{
    enum uartUnit unit;
    unsigned baud;
    enum uartInterrupt interruptMode;
} uartParam;

// Function
extern void Uart_ParamInit  (uartParam *);
extern void Uart_Init       (uartParam *);

extern void Uart_Write      (uartParam *, char);
extern void Uart_WriteStr  (uartParam *, char *, char);

#endif
