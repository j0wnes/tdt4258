#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/*
 * TODO calculate the appropriate sample period for the sound wave(s) you 
 * want to generate. The core clock (which the timer clock is derived
 * from) runs at 14 MHz by default. Also remember that the timer counter
 * registers are 16 bits. 
 */

#define CLOCK_RATE 14000000
#define BUFFER_SIZE 100

uint16_t sampling_rate = 20000;
// The period between sound samples, in clock cycles
uint16_t sample_period;
uint16_t granularity = 1000;
uint16_t frequency = 440;
uint16_t amplitude = 100;

/*
 * Declaration of peripheral setup functions 
 */
void setupGPIO();
void setupDAC();
void setupTimer(uint32_t period);
void setupNVIC();
void pushSample();
void fillBuffer(bool buffer);
uint16_t pulseWave(uint16_t offset, uint16_t amplitude, uint16_t dutycycle);
void handleButtonPress();


// global variables
uint16_t buffer0[BUFFER_SIZE];
uint16_t buffer1[BUFFER_SIZE];

bool currentBuffer = false;
uint16_t currentBufferPos = 0;
uint16_t globalOffset = 0;

/*
 * Your code will start executing here 
 */
int main(void) {
    /*
     * Call the peripheral setup functions
     */
    sample_period = CLOCK_RATE / sampling_rate;

    setupGPIO();
    setupDAC();
    setupTimer(sample_period);

    fillBuffer(false);
    fillBuffer(true);

    /*
     * Enable interrupt handling
     */
    setupNVIC();

    /*
     * TODO for higher energy efficiency, sleep while waiting for
     * interrupts instead of infinite loop for busy-waiting
     */
    while (true);

    return 0;
}

void setupNVIC() {
    /*
     * TODO use the NVIC ISERx registers to enable handling of
     * interrupt(s) remember two things are necessary for interrupt
     * handling: - the peripheral must generate an interrupt signal - the
     * NVIC must be configured to make the CPU handle the signal You will
     * need TIMER1, GPIO odd and GPIO even interrupt handling for this
     * assignment.
     */

    // *ISER0 |= I_LESENSE_BIT;
    *ISER0 |= I_TIMER1_BIT|I_GPIO_EVEN_BIT|I_GPIO_ODD_BIT;
}

void pushSample() {
    uint16_t *bufferArray = currentBuffer ? buffer1 : buffer0;
    uint16_t sample = bufferArray[currentBufferPos];
    *DAC0_CH0DATA = sample;
    *DAC0_CH1DATA = sample;

    currentBufferPos++;
    if (currentBufferPos >= BUFFER_SIZE) {
        currentBuffer = !currentBuffer;
        currentBufferPos = 0;
        // TODO: trigger software interrupt
        fillBuffer(!currentBuffer);
    }
}

void fillBuffer(bool buffer) {
    uint16_t *bufferArray = buffer ? buffer1 : buffer0;
    int increment = granularity / (sampling_rate / frequency);
    for(int i = 0; i < BUFFER_SIZE; i++) {
        bufferArray[i] = pulseWave(globalOffset, amplitude, 50);
        globalOffset = (globalOffset + increment) % granularity;
    }
}

uint16_t pulseWave(uint16_t offset, uint16_t amplitude, uint16_t dutycycle) {
    return offset > (dutycycle * granularity / 100) ? 0 : amplitude;
}

void handleButtonPress() {
    uint32_t buttonPressed = *GPIO_IF;
    // *GPIO_PA_DOUTTGL = buttonPressed << 8;
    switch (buttonPressed) {
        case 1 << 0:
            *GPIO_PA_DOUTTGL = 1 << 8;
            amplitude -= 10;
            break;
        case 1 << 1:
            *GPIO_PA_DOUTTGL = 1 << 9;
            frequency += 20;
            break;
        case 1 << 2:
            *GPIO_PA_DOUTTGL = 1 << 10;
            amplitude += 10;
            break;
        case 1 << 3:
            *GPIO_PA_DOUTTGL = 1 << 11;
            frequency -= 20;
            break;
        case 1 << 4:
            *GPIO_PA_DOUTTGL = 1 << 12;
            granularity += 100;
            break;
        case 1 << 5:
            *GPIO_PA_DOUTTGL = 1 << 13;
            sampling_rate += 1000;
            sample_period = CLOCK_RATE / sampling_rate;
            *TIMER1_TOP = sample_period;
            break;
        case 1 << 6:
            *GPIO_PA_DOUTTGL = 1 << 14;
            granularity -= 100;
            break;
        case 1 << 7:
            *GPIO_PA_DOUTTGL = 1 << 15;
            sampling_rate -= 1000;
            sample_period = CLOCK_RATE / sampling_rate;
            *TIMER1_TOP = sample_period;
            break;
    }
}


/*
 * if other interrupt handlers are needed, use the following names:
 * NMI_Handler HardFault_Handler MemManage_Handler BusFault_Handler
 * UsageFault_Handler Reserved7_Handler Reserved8_Handler
 * Reserved9_Handler Reserved10_Handler SVC_Handler DebugMon_Handler
 * Reserved13_Handler PendSV_Handler SysTick_Handler DMA_IRQHandler
 * GPIO_EVEN_IRQHandler TIMER0_IRQHandler USART0_RX_IRQHandler
 * USART0_TX_IRQHandler USB_IRQHandler ACMP0_IRQHandler ADC0_IRQHandler
 * DAC0_IRQHandler I2C0_IRQHandler I2C1_IRQHandler GPIO_ODD_IRQHandler
 * TIMER1_IRQHandler TIMER2_IRQHandler TIMER3_IRQHandler
 * USART1_RX_IRQHandler USART1_TX_IRQHandler LESENSE_IRQHandler
 * USART2_RX_IRQHandler USART2_TX_IRQHandler UART0_RX_IRQHandler
 * UART0_TX_IRQHandler UART1_RX_IRQHandler UART1_TX_IRQHandler
 * LEUART0_IRQHandler LEUART1_IRQHandler LETIMER0_IRQHandler
 * PCNT0_IRQHandler PCNT1_IRQHandler PCNT2_IRQHandler RTC_IRQHandler
 * BURTC_IRQHandler CMU_IRQHandler VCMP_IRQHandler LCD_IRQHandler
 * MSC_IRQHandler AES_IRQHandler EBI_IRQHandler EMU_IRQHandler 
 */
