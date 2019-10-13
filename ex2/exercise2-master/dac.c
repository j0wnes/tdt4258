#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

void setupDAC()
{
	/*
	 * TODO enable and set up the Digital-Analog Converter
	 * 
	 * 1. Enable the DAC clock by setting bit 17 in CMU_HFPERCLKEN0 2.
	 * Prescale DAC clock by writing 0x50010 to DAC0_CTRL 3. Enable left
	 * and right audio channels by writing 1 to DAC0_CH0CTRL and
	 * DAC0_CH1CTRL 4. Write a continuous stream of samples to the DAC
	 * data registers, DAC0_CH0DATA and DAC0_CH1DATA, for example from a
	 * timer interrupt
	 */

	// 1
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_DAC0;
	// 2
	*DAC0_CTRL = 0x50010;
	// 3
	*DAC0_CH0CTRL = 1;
	*DAC0_CH1CTRL = 1;
	
	
	// Hacky square wave test
	// DAC0_CH0DATA goes from 0 to 4095 (12 bits)
	
	/*
	int counter = 0;
	bool flip = true;
	while (true)
	{
		counter++;
		
		if (counter % 1000 == 0)
		{
			flip = !flip;
		}

		if (flip)
		{
			*DAC0_CH0DATA = 10;
			*DAC0_CH1DATA = 10;
		}
		else
		{
			*DAC0_CH0DATA = 0;
			*DAC0_CH1DATA = 0;
		}
	}
	*/
}
