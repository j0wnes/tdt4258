#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>

#include "efm32gg.h"

#define CONST_PI 3.14159265358979323846

/*
 * TIMER1 interrupt handler 
 */

int FREQUENCY = 440; // Standard A tone
int AMPLITUDE = 50; // Reasonable volume

uint64_t counter = 0;
bool flip = true;

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

	/*
	// Quick test of the timer = plays square wave
	if (counter % 100 == 0)
	{
		flip = !flip;
	}

	if(flip)
	{
		//*DAC0_CH0DATA = 10;
		//*DAC0_CH1DATA = 10;
	}
	else
	{
		//*DAC0_CH0DATA = 0;
		//*DAC0_CH1DATA = 0;
	}
	*/

	// This function is invoked 48000 times a second
	// The range for potential counter is 0-48000
	// Sine wave has range/period 0-2PI
	// Therefore 2PI / 48000 gives us 1hz tone
	// Therefore FREQUENCY * (2PI / 48000) will give us the final frequency

	double sampleValue = sin(((FREQUENCY * 2.0 * CONST_PI) / 48000) * counter); // Actual sample value
	uint16_t dacValue = (uint16_t)(((sampleValue + 1.0) / 2.0 ) * AMPLITUDE); // Scale it to the register range

	// Finally write to the register
	*DAC0_CH0DATA = dacValue;
	*DAC0_CH1DATA = dacValue;
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

	// Quick test = change frequency
	FREQUENCY += 10;

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

	// Quick test = change frequency
	FREQUENCY -= 10;

	// Clear the interrupt
	*GPIO_IFC = *GPIO_IF;
}