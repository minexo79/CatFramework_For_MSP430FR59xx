#include "WatchDog.h"

// Stop Watchdog Timer
void WatchDog_Shutdown (void)
{
    AssignBit(WDTCTL, (WDTPW | WDTHOLD));
}
