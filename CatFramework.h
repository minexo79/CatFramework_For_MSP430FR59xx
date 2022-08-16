/*
 *  CatFramework
 *
 *     Version: 1.0 (Dev)
 *  Created on: 2022.08.08
 *      Author: Tsai XO
 */

#ifndef _CATFRAMEWORK_H_
#define _CATFRAMEWORK_H_

// SFR Data Type
#define _Reg8           volatile unsigned char
#define _Reg16          volatile unsigned

// Bit Operation
#define AssignBit(x, y)    x = y
#define SetBit(x, y)       x |= y
#define ResetBit(x, y)     x &= ~y
#define ToggleBit(x, y)    x ^= y

// Standard Define
#include <stdio.h>
#include <stdlib.h>

// MSP430 Define
#include <msp430.h>

// GPIO
#include "CatFramework/GPIO.h"

// System Clock
#include "CatFramework/SystemClock.h"

// UART
#include "CatFramework/UART.h"

// Timer
#include "CatFramework/Timer.h"

// WatchDog
#include "CatFramework/WatchDog.h"

// Useful Function
#include "CatFramework/Useful.h"

#endif /* CATFRAMEWORK_H_ */
