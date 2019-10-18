#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>

#include "efm32gg.h"

// Include generated header file
#include "../generator/sounds.h"

// Curent playing sound
Sound* currentSound = NULL;

void playSound();

/*
 * The period between sound samples, in clock cycles
 */

// Timer clock divided by sample count
// 14000000/5000 => cca 2800
#define SAMPLE_PERIOD 2800

/*
 * Declaration of peripheral setup functions 
 */
void enableTimer(uint16_t period);
void enableDAC();
void setupGPIO();


/*
 * Your code will start executing here
 */
int main(void)
{
	/*
	 * Call the peripheral setup functions 
	 */
	setupGPIO();
	enableDAC();
	enableTimer(SAMPLE_PERIOD);

	uint64_t counter = 0;
	while (true)
	{
		// Check which sound to play
		playSound();

		// Read interrupt register (no interrupt generation)
		if(*TIMER1_IF > 0)
		{
			// Clear the IF register
			*TIMER1_IFC = *TIMER1_IF;
			
			counter++;
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
	}

	return 0;
}

// Picks one sound based on which button is pressed
void playSound()
{
	// Sanity test - Light up corresponding LEDs
	*GPIO_PA_DOUT = (*GPIO_PC_DIN << 8);

	// Pressed buttons check
	uint32_t button = ~(*GPIO_PC_DIN);
	if(BUTTON_PRESSED(button, 0))
	{
		currentSound = &maria;
	}
	else if(BUTTON_PRESSED(button, 1))
	{
		currentSound = &smoke;
	}
	else if(BUTTON_PRESSED(button, 2))
	{
		currentSound = &laugh;
	}
	else if(BUTTON_PRESSED(button, 3))
	{
		currentSound = &scream;
	}
	else if(BUTTON_PRESSED(button, 4))
	{
		currentSound = &explosion;
	}
	else
	{
		currentSound = NULL;
	}
}
