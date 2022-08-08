#ifndef _SYSTEMCLOCK_H_
#define _SYSTEMCLOCK_H_

#include "../CatFramework.h"

// Enum
extern enum dcoClock {NON, C1MHZ, C8MHZ, C16MHZ} dcoClock;

// Function
extern void SystemClock_Init (enum dcoClock);

#endif
