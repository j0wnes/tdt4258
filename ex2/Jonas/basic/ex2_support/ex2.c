#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "efm32gg.h"
// #include "sound_gen.h"
#include "dac.h"
#include "gpio.h"
#include "timer.h"


/*
 * TODO calculate the appropriate sample period for the sound wave(s) you 
 * want to generate. The core clock (which the timer clock is derived
 * from) runs at 14 MHz by default. Also remember that the timer counter
 * registers are 16 bits. 
 */
/*
 * The period between sound samples, in clock cycles 
 */
#define   SAMPLE_PERIOD   317       // about 44.1 kHz (14M/44,1k)

typedef struct
{
    uint32_t length;
    uint32_t sampling_rate;
    uint16_t *sound_files;
} sample;


/*
 * Declaration of peripheral setup functions 
 */
void setupTimer(uint16_t period);
void setupDAC();
void setupNVIC();
void sinGenerate(sample *sample, double accoustFreq, double sampleRate);
void playSample(sample *sample);
/*
 * Your code will start executing here 
 */
int main(void)
{
	/*
	 * Call the peripheral setup functions 
	 */
	setupGPIO();
	setupDAC();
	setupTimer(SAMPLE_PERIOD);

	/*
	 * Enable interrupt handling 
	 */
	setupNVIC();

	/*
	 * TODO for higher energy efficiency, sleep while waiting for
	 * interrupts instead of infinite loop for busy-waiting 
	 */
	 
	 
	 


	sample sample1;

	while (1)
    {
	    int b = (int) buttonReadout();
	    // lighting LEDs according to pressed butotn
	    updateLED();
	    // actions for each button
	    switch (b)
        {
            case 0: break;
            case 1: {
                        sinGenerate(&sample1, 400.0, 44100);                  
                        break;
                    }
            case 2: break;
            case 4: break;
            case 8: break;
            case 16: break;
            case 32: break;
            case 64: break;
            default: break;
        }
        playSample(&sample1);

    }
	return 0;
}

void setupNVIC()
{
	/*
	 * TODO use the NVIC ISERx registers to enable handling of
	 * interrupt(s) remember two things are necessary for interrupt
	 * handling: - the peripheral must generate an interrupt signal - the
	 * NVIC must be configured to make the CPU handle the signal You will
	 * need TIMER1, GPIO odd and GPIO even interrupt handling for this
	 * assignment. 
	 */


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
 
 
void sinGenerate(sample *sample, double accoustFreq, double sampleRate)
{
    sample->length = round(sampleRate/accoustFreq);
    sample->sampling_rate = sampleRate;
    double f = accoustFreq/sampleRate;		// f * length = 1
    for(int n=0;n<(sample->length); n++)
    {
        // 1024: amplitude of sin-wave
        // +2048 to move the sin-wave up so no neg values anymore
        sample->sound_files[n] = round(1024*sin(2*3.14*f*n))+2048;
    }
}

void playSample(sample *dsamp)
{
    startTimer(dsamp->sampling_rate);
    while((int) buttonReadout())
    {
    	int n = 0;
        if(*TIMER1_CNT == 1 && n<dsamp->length)
        {
            // sending data to both channels and incrementing n for next round/data
            *DAC0_CH0DATA = dsamp->sound_files[n];
            *DAC0_CH1DATA = dsamp->sound_files[n];
            n++;
        }
        if(n>=dsamp->length)
        {
            // end if end of sample is reached
            break;
        }
    }
    stopTimer();
}
