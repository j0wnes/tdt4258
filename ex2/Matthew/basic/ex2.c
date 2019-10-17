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
 * TODO calculate the appropriate sample period for the sound wave(s) you 
 * want to generate. The core clock (which the timer clock is derived
 * from) runs at 14 MHz by default. Also remember that the timer counter
 * registers are 16 bits.
 */
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


	/*
	 * TODO for higher energy efficiency, sleep while waiting for
	 * interrupts instead of infinite loop for busy-waiting 
	 */

	/*
	// Timer interrupts
	*TIMER1_IEN = 1;
	*ISER0 |= (1 << 12);

	// GPIO interrupts
	*GPIO_EXTIPSELL = 0x22222222;
	*GPIO_EXTIFALL = 0xff;
	*GPIO_EXTIRISE = 0xff;
	*GPIO_IEN = 0xff;
	*ISER0 |= 0x802;
	*/

	currentSound = &maria;

	uint64_t counter = 0;
	while (true)
	{
		// Check which sound to play
		//playSound();
		
		//if()
		//{
			//counter++;
		//}
		//counter = *TIMER1_CNT;
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

void playSound()
{
	// Sanity test - Light up corresponding LEDs
	*GPIO_PA_DOUT = (*GPIO_PC_DIN << 8);

	if(BUTTON_PRESSED(*GPIO_PC_DIN, 0))
	{
		currentSound = &maria;
	}
	else if(BUTTON_PRESSED(*GPIO_PC_DIN, 1))
	{
		currentSound = &smoke;
	}
	else if(BUTTON_PRESSED(*GPIO_PC_DIN, 2))
	{
		currentSound = &laugh;
	}
	else if(BUTTON_PRESSED(*GPIO_PC_DIN, 3))
	{
		currentSound = &scream;
	}
	else if(BUTTON_PRESSED(*GPIO_PC_DIN, 4))
	{
		currentSound = &explosion;
	}
	else
	{
		currentSound = NULL;
	}
}
