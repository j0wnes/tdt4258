#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "efm32gg.h"

// Data structures for the player
typedef enum { SQUARE = 0, SINE = 1} WaveType; 

typedef struct {
	float toneFrequency;
	float durationMs;
	WaveType waveType;
} note_t;

typedef struct {
	note_t* sequence;
	size_t length;
} sequence_t;

void setFallbackSequence(sequence_t sequence);
void setSequence(sequence_t sequence);
void play(float delta);
float getFrequency();

// External variables with frequency and wave type
//extern float globalCurrentFrequency;
//extern WaveType globalCurrentWaveType;
