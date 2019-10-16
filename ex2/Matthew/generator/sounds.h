typedef struct {
   uint16_t leftChannel;
   uint16_t rightChannel;
} Samples;

typedef struct {
   size_t sampleRate;
   size_t sampleCount;
   Samples samples[];
} Sound;

