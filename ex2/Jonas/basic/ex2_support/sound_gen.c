#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "efm32gg.h"
#include "timer.h"

typedef struct
{
    uint32_t length;
    uint32_t sampling_rate;
    uint16_t *sound_files;
} sample;

void sinGenerate(sample *sample, double accoustFreq, double sampleRate)
{
    sample->length = round(sampleRate/accoustFreq);
    sample->sampling_rate = sampleRate;
    double f = accoustFreq/sampleRate;
    for(int n=0;n<sample->length; n++)
    {
        // 1024: amplitude of sin-wave
        // +2048 to move the sin-wave up so no neg values anymore
        sample->sound_files[n] = round(1024*sin(2*3.14*f*n))+2048;
    }
}

void playSample(sample *sample, int button)
{
    int n = 0;
    startTimer(sample->sampling_rate);
    int k;
    while (k=buttonReadout())
    {
        if(*TIMER1_IFC == 1)
        {
            // sending data to both channels and incrementing n for next round/data
            *DAC0_CH0DATA = sample->sound_files[n];
            *DAC0_CH1DATA = sample->sound_files[n];
            n++;
        }
        if(n>=sample->length)
        {
            // end if end of sample is reached
            break;
        }
    }
    stopTimer();
}