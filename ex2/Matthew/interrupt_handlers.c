#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "generator/sounds.h"

/*
 * TIMER1 interrupt handler 
 */

uint64_t counter = 0;
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
	if(counter > chord.sampleCount-1)
	{
		counter = 0;
	}

	*DAC0_CH0DATA = chord.samples[counter];
	*DAC0_CH1DATA = chord.samples[counter];
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

	// Read the IRQ source
	uint32_t source = *GPIO_IF;

	// Quick test = Lighting up corresponding LED
	*GPIO_PA_DOUT = ~(source << 8);

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

	// Read the IRQ source
	uint32_t source = *GPIO_IF;

	// Quick test = Lighting up corresponding LED
	*GPIO_PA_DOUT = ~(source << 8);

	// Clear the interrupt
	*GPIO_IFC = *GPIO_IF;
}