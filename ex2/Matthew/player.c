#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "efm32gg.h"
#include "player.h"

// Global variables
//float globalCurrentFrequency = 0;
//WaveType globalCurrentWaveType = SQUARE;

// Sequences
sequence_t currentSequence;
sequence_t fallbackSequence;
bool sequenceChanged = true;

float currentFrequency = 0;
size_t currentNoteIndex = 0;
float currentNoteTime = 0;


void setFallbackSequence(sequence_t sequence)
{
	fallbackSequence = sequence;
	currentSequence = sequence;
	sequenceChanged = true;
}

void setSequence(sequence_t sequence)
{
	currentSequence = sequence;
	sequenceChanged = true;
}

float getFrequency()
{
	return currentFrequency;
}

void play(float delta)
{
		// Test if we got a new sequence to play
		if(sequenceChanged)
		{
			currentNoteIndex = 0;
			currentNoteTime = 0;
			sequenceChanged = false;
		}
		
		// Sample current note
		note_t currentNote = currentSequence.sequence[currentNoteIndex];
		currentFrequency = currentSequence.sequence[currentNoteIndex].toneFrequency;
		//globalCurrentWaveType = currentSequence.sequence[currentNoteIndex].waveType;

		// Maybe change to a new note
		if (currentNoteTime > currentNote.durationMs)
		{
			currentNoteIndex++;
			
			// Increase repeat count
			if(currentNoteIndex == currentSequence.length)
			{
				// Always fallback to the background music
				currentSequence = fallbackSequence;
				sequenceChanged = true;
			}
		}

		// Step time
		currentNoteTime += delta;	
}