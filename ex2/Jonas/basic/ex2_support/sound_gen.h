#include <stdint.h>
#include "efm32gg.h"

typedef struct
{
    uint32_t length;
    uint32_t sampling_rate;
    uint16_t *sound_files;
} sample;

void sinGenerate(sample *sample, double accoustFreq, double sampleRate);

void playSample(sample *sample, int button);
