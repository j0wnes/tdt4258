#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"


/*
 * function to set up GPIO mode and interrupts
 */
void setupGPIO()
{
	/*
	* TODO set input and output pins for the joystick 
	*/

	// GPIO clock
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO;

	// LEDs
	*GPIO_PA_CTRL = 2; // Set high drive strength
	*GPIO_PA_MODEH = 0x55555555; // Set pins A8-15 as output
	*GPIO_PA_DOUT = 0xff00; // Turn off all the LEDs (LEDs are active low)
	
	// Quick test = Turn on LEDs D4-D8
	//*GPIO_PA_DOUT = 0x0700;


	// Buttons
	*GPIO_PC_MODEL = 0x33333333; // Set pins A0-7 as input
	*GPIO_PC_DOUT = 0xff; // Enable internal pull-ups

	// Quick test = Pooling of buttons and LEDs
	/*
	while (true)
	{
		*GPIO_PA_DOUT = (*GPIO_PC_DIN << 8);
	}
	*/

	/*
	 * Example of HW access from C code: turn on joystick LEDs D4-D8 check 
	 * efm32gg.h for other useful register definitions 
	 */
}
