#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/*
 * function to set up GPIO mode and interrupts
 */
void setupGPIO() {
    /*
     * TODO set input and output pins for the joystick
     */

    /*
     * Example of HW access from C code: turn on joystick LEDs D4-D8 check
     * efm32gg.h for other useful register definitions
     */

    *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO;    /* enable GPIO clock */
    // enable LEDS
    *GPIO_PA_CTRL = 1;    /* set high drive strength */
    *GPIO_PA_MODEH = 0x55555555;    /* set pins A8-15 as output */
    *GPIO_PA_DOUT = 0xffff;    /* turn on LEDs D4-D8 (LEDs are active low) */
    // enable buttons
    *GPIO_PC_MODEL = 0x33333333;
    *GPIO_PC_DOUT = 0xff;

    *GPIO_EXTIPSELL = 0x22222222;
    *GPIO_EXTIRISE = 0xff;
    *GPIO_IEN = 0xff;

    // clear possible interrupts
    *GPIO_IFC = 0xffff;
}
