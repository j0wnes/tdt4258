#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/*
 * function to setup the timer 
 */
void enableTimer(uint16_t period)
{
	// Enable clock to timer
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;
	
	// Write the period to register
	*TIMER1_TOP = period;

	// Enable timer interrupt generation but no execution
	*TIMER1_IEN = 1;

	// Start the timer
	*TIMER1_CMD = 1;
}

void disableTimer()
{
	// Disable clock to timer
	*CMU_HFPERCLKEN0 &= ~CMU2_HFPERCLKEN0_TIMER1;

	// Stop the timer
	*TIMER1_CMD = 0;
}
