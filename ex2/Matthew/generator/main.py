import librosa


def convert_note_to_frequency(note):
    # A few of the most useful notes
    notes = {
        "C3": 130.81,
        "C#3/Db3": 138.59,
        "D3": 146.83,
        "D#3/Eb3": 155.56,
        "E3": 164.81,
        "F3": 174.61,
        "F#3/Gb3": 185.00,
        "G3": 196.00,
        "G#3/Ab3": 207.65,
        "A3": 220.00,
        "A#3/Bb3": 233.08,
        "B3": 246.94,
        "C4": 261.63,
        "C#4/Db4": 277.18,
        "D4": 293.66,
        "D#4/Eb4": 311.13,
        "E4": 329.63,
        "F4": 349.23,
        "F#4/Gb4": 369.99,
        "G4": 392.00,
        "G#4/Ab4": 415.30,
        "A4": 440.00,
        "A#4/Bb4": 466.16,
        "B4": 493.88,
        "C5": 523.25,
        "C#5/Db5": 554.37,
        "D5": 587.33,
        "D#5/Eb5": 622.25,
        "E5": 659.25,
        "F5": 698.46,
        "F#5/Gb5": 739.99,
        "G5": 783.99,
        "G#5/Ab5": 830.61,
        "A5": 880.00,
        "A#5/Bb5": 932.33,
        "B5": 987.77,
        "C6": 1046.50,
        "C#6/Db6": 1108.73,
        "D6": 1174.66,
        "D#6/Eb6": 1244.51,
        "E6": 1318.51,
        "F6": 1396.91,
        "F#6/Gb6": 1479.98,
        "G6": 1567.98,
        "G#6/Ab6": 1661.22,
        "A6": 1760.00,
        "A#6/Bb6": 1864.66,
        "B6": 1975.53,
    }

    return notes.get(note, 1.0)


def generate_smoke_on_the_water_sequence():

    # D4 F4 G4 - | D4 F4 G#4 G4 | D4 F4 G4 - | F4 D4
    sequence = [("D4", 300), ("X", 200), ("F4", 300), ("X", 200), ("G4", 600), ("X", 200),
                ("D4", 300), ("X", 200), ("F4", 300), ("X", 200), ("G#4/Ab4", 300), ("G4", 600), ("X", 400),
                ("D4", 300), ("X", 200), ("F4", 300), ("X", 200), ("G4", 600), ("X", 200),
                ("F4", 300), ("X", 200), ("D4", 600), ("X", 800)]

    return sequence


def generate_square_wave_from_sequence(sequence, sample_rate, amplitude):

    mono_channel = []
    counter = 0

    for note, duration in sequence:

        # Convert note to frequency
        frequency = convert_note_to_frequency(note)
        frequency_in_whole_range = frequency * (duration / 1000.0)
        total_sample_count = sample_rate * (duration / 1000.0)
        square_wave_half_length = (total_sample_count / frequency_in_whole_range) / 2.0

        # Generate samples
        flip = False
        for i in range(int(total_sample_count)):
            if counter >= square_wave_half_length:
                flip = not flip
                counter = 0

            if flip:
                mono_channel.append(amplitude)
            else:
                mono_channel.append(0)

            counter += 1

    # Return samples and
    return mono_channel, sample_rate


def convert_wav_to_samples(filename, sampling_rate):

    # Read file and resample data
    data, new_sampling_rate = librosa.load(filename, sr=sampling_rate, mono=True)
    return data, new_sampling_rate


def scale_samples(mono_channel, sample_rate, amplitude):

    # Scaling to range [0, amplitude]
    # Scaling to range [a,b]: (b−a) * (x − min(x)) / (max(x) − min(x)) + a
    mono_channel = amplitude * (mono_channel - mono_channel.min()) / (mono_channel.max() - mono_channel.min())

    # Cast values to int16
    mono_channel = mono_channel.astype("int16")

    return mono_channel, sample_rate


def write_struct_type_to_file(header_file):

    sound_type_name = "Sound"
    header_file = open(header_file, 'w')

    header_file.write("typedef struct {\n")
    header_file.write("   uint16_t sampleRate;\n")
    header_file.write("   uint16_t sampleCount;\n")
    header_file.write("   uint8_t samples[];\n")
    header_file.write("} " + str(sound_type_name) + ";\n\n")

    header_file.close()


def save_samples_as_struct(mono_channel, sample_rate, header_file, struct_name):
    sound_type_name = "Sound"
    line_wrap = 10

    header_file = open(header_file, 'a')
    header_file.write(str(sound_type_name) + " " + str(struct_name) + " = {\n")

    # Write metadata
    header_file.write("  " + str(sample_rate) + ",\n")
    header_file.write("  " + str(len(mono_channel)) + ",\n")

    # Write samples
    header_file.write("  {")
    for i in range(len(mono_channel)):
        header_file.write(str(mono_channel[i]) + ",")

        if i % line_wrap == 0:
            header_file.write("\n  ")

    header_file.write("}};\n")
    header_file.close()


def main():

    # Parameters
    sampling_rate = 8000
    amplitude = 100

    # Create header file with types
    write_struct_type_to_file("sounds.h")

    # Add wav file samples
    mono_channel, sample_rate = convert_wav_to_samples("explo.wav", sampling_rate)
    mono_channel, sample_rate = scale_samples(mono_channel, sample_rate, amplitude)
    save_samples_as_struct(mono_channel, sample_rate, "sounds.h", "chord")

    # Add generated samples
    sequence = generate_smoke_on_the_water_sequence()
    mono_channel, sample_rate = generate_square_wave_from_sequence(sequence, sampling_rate, amplitude)
    save_samples_as_struct(mono_channel, sample_rate, "sounds.h", "smoke")


main()
