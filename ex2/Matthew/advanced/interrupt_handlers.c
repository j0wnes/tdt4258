#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "efm32gg.h"

// Include generated header file
#include "../generator/sounds.h"

// Curent playing sound
Sound* currentSound = NULL;

// Counter for moving through array of samples
uint64_t counter = 0;

void playSound();

/*
 * TIMER1 interrupt handler 
 */
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	/*
	* TODO feed new samples to the DAC remember to clear the pending
	* interrupt by writing 1 to TIMER1_IFC
	*/

	// Clear the interrupt
	*TIMER1_IFC = 1;

	// Increment counter
	counter++;

	// Play a song
	if (currentSound != NULL)
	{
		if(counter > (*currentSound).sampleCount-1)
		{
			counter = 0;
		}

		*DAC0_CH0DATA = (*currentSound).samples[counter];
		*DAC0_CH1DATA = (*currentSound).samples[counter];
	}
}

/*
 * GPIO even pin interrupt handler 
 */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	/*
	* TODO handle button pressed event, remember to clear pending
	* interrupt
	*/

	// Sanity test = Lighting up corresponding LED
	*GPIO_PA_DOUT = ~(*GPIO_IF << 8);

	// Play sounds
	playSound();

	// Clear the interrupt
	*GPIO_IFC = *GPIO_IF;
}

/*
 * GPIO odd pin interrupt handler 
 */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	/*
	* TODO handle button pressed event, remember to clear pending
	* interrupt 
	*/

	// Sanity test = Lighting up corresponding LED
	*GPIO_PA_DOUT = ~(*GPIO_IF << 8);

	// Play sounds
	playSound();

	// Clear the interrupt
	*GPIO_IFC = *GPIO_IF;
}

// Play some generated sounds
void playSound()
{
	if(BUTTON_PRESSED(*GPIO_IF, 0))
	{
		currentSound = &maria;
	}
	else if(BUTTON_PRESSED(*GPIO_IF, 1))
	{
		currentSound = &smoke;
	}
	else if(BUTTON_PRESSED(*GPIO_IF, 2))
	{
		currentSound = &laugh;
	}
	else if(BUTTON_PRESSED(*GPIO_IF, 3))
	{
		currentSound = &scream;
	}
	else if(BUTTON_PRESSED(*GPIO_IF, 4))
	{
		currentSound = &explosion;
	}
	else
	{
		currentSound = NULL;
	}

	// Reset counter
	counter = 0;
}
