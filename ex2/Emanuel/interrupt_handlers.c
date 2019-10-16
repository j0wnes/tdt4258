#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

void pushSample();
void handleButtonPress();

int i = 0;

/*
 * TIMER1 interrupt handler 
 */
void __attribute__ ((interrupt)) TIMER1_IRQHandler() {
    /*
     * TODO feed new samples to the DAC remember to clear the pending
     * interrupt by writing 1 to TIMER1_IFC
     */

    *TIMER1_IFC = 1;
    pushSample();

    // interrupts execution sanity check
    // if (i == 100) {
    //     *GPIO_PA_DOUTTGL = 0xffff;
    //     i = 0;
    // } else {
    //     i++;
    // }

}

// buffer used up interrupt
void __attribute__ ((interrupt)) LESENSE_IRQHandler() {
    // fillBuffer();
}

/*
 * GPIO even pin interrupt handler 
 */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler() {
    /*
     * TODO handle button pressed event, remember to clear pending
     * interrupt
     */
    handleButtonPress();
    *GPIO_IFC = 0xffff;
}

/*
 * GPIO odd pin interrupt handler
 */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler() {
    /*
     * TODO handle button pressed event, remember to clear pending
     * interrupt
     */
    handleButtonPress();
    *GPIO_IFC = 0xffff;
}
