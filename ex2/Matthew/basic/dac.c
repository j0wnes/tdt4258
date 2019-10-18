#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

void enableDAC()
{
	// DAC0_CH0DATA goes from 0 to 4095 (12 bits)

	// Enable the DAC clock
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_DAC0;

	// Prescale DAC clock
	*DAC0_CTRL = 0x50010;
	
	// Enable left and right audio channels
	*DAC0_CH0CTRL = 1;
	*DAC0_CH1CTRL = 1;
}

void disableDAC()
{
	// Disable the DAC clock
	*CMU_HFPERCLKEN0 &= ~CMU2_HFPERCLKEN0_DAC0;

	// Disable left and right audio channels
	*DAC0_CH0CTRL = 0;
	*DAC0_CH1CTRL = 0;
}
