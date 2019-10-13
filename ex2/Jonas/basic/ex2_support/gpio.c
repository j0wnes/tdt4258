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

	/*
	 * Example of HW access from C code: turn on joystick LEDs D4-D8 check 
	 * efm32gg.h for other useful register definitions 
	 */
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO;	/* enable GPIO clock */
	*GPIO_PA_CTRL = 2;	/* set high drive strength */
	*GPIO_PA_MODEH = 0x55555555;	/* set pins A8-15 as output */
	*GPIO_PA_DOUT = 0x0700;	/* turn on LEDs D4-D8 (LEDs are active
				 * low) */

	// configure Buttons (compare with ex1)
	*GPIO_PC_MODEL = 0x33333333;    // set pins 0-7 as input
	*GPIO_PC_DOUT = 0xff;           // enable internal pull-up

}

int buttonReadout()
{
    uint32_t b_temp = *GPIO_PC_DIN & 0xff;
    if(b_temp)
    {
        for(int i=0;i<8;i++)
        {
            if(b_temp & (1 << i))
            {
                return i + 1;       // no switch 0, only 1-8
            }
        }
    }
    return 0;                       // no button pressed
}

void updateLED()
{
    *GPIO_PA_DOUT = (*GPIO_PC_DIN << 8);
}
