#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "efm32gg.h"

typedef struct {
	float toneFrequency;
	float durationMs;
} note_t;

typedef struct {
	note_t* sequence;
	size_t length;
} sequence_t;

void setFallbackSequence(sequence_t sequence);
void setSequence(sequence_t sequence);
void play(float delta);
float getFrequency();
