#ifndef SYNTH_H
#define SYNTH_H

#ifdef __cplusplus
extern "C" {
#endif

#define SYNTH_SAMPLE_TYPE			short
#define SYNTH_OUTPUT_SAMPLE_SIZE	sizeof(SYNTH_SAMPLE_TYPE)

#define SYNTH_DURATION		240
#define SYNTH_SAMPLE_RATE	44100
#define SYNTH_NUM_CHANNELS	2
#define SYNTH_NUM_SAMPLES	(SYNTH_DURATION * SYNTH_SAMPLE_RATE)

#define SYNTH_PATTERN_LENGTH 32
#define SYNTH_MAX_COLUMN_COUNT 8
#define SYNTH_MAX_PATTERN_ORDER_COUNT 32
#define SYNTH_MAX_INSTRUMENT_COUNT 8

#define NOTE(x)		(((x)>>4) & 0xF)
#define OCTAVE(x)	((x) & 0x0F)

#define SYNTH_OSC_SINE 1
#define SYNTH_OSC_SAW 2
#define SYNTH_OSC_TRIANGLE 3
#define SYNTH_OSC_SQUARE 4
#define SYNTH_OSC_NOISE 5


// Columns
typedef struct 
{
    // Notes
    unsigned char   notes[SYNTH_PATTERN_LENGTH];          // Notes
} column;

// Instrument
typedef struct 
{
	// General
	unsigned char	pan;					// Pan (0-255) 0 = left, 255 = right

    // Oscillator 1
    char   osc1_detune_octave;				// Detune for octave
	char   osc1_detune_semitone;			// Detune for semitone
    unsigned char   osc1_vol;				// Volume (0-255)
    unsigned char   osc1_waveform;			// Wave form

    // Oscillator 2
    char   osc2_detune_octave;				// Detune for octave
	char   osc2_detune_semitone;			// Detune for semitone
    unsigned char   osc2_vol;				// Volume (0-255)
    unsigned char   osc2_waveform;			// Wave form

	// LFO
	float			lfo_freq;				// LFO frequency
	unsigned char   lfo_amt;				// LFO amount

    // Envelope (in samples)
    unsigned int    env_attack;				// Attack
    unsigned int    env_sustain;			// Sustain
    unsigned int    env_release;			// Release

	// Effects
	unsigned char   fx_delay_time;			// Delay time
    unsigned char   fx_delay_amt;			// Delay amount
	unsigned char   fx_overdrive_amt;		// Overdrive amount

    // Patterns
    char			patternOrder[SYNTH_MAX_PATTERN_ORDER_COUNT];   // Pattern order

    // Columns
    column          columns[SYNTH_MAX_COLUMN_COUNT];    // Columns
} instrument;

// Songs
typedef struct 
{
    // Instruments
    instrument      instruments[SYNTH_MAX_INSTRUMENT_COUNT]; // Instruments
} song;



void synth_init();
void synth_play();

unsigned char synth_get_current_note_for_instrument(int instrument);
unsigned char synth_get_current_envelope_for_instrument(int instrument);

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

#endif
