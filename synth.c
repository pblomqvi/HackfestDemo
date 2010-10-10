#include "synth.h"

#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include "song.h"

#ifdef DEBUG
	#include <stdio.h>
#endif

static int clip(int value) {
	if(value > 32767) return 32767;
	if(value < -32767) return -32767;
	return value;
}

static volatile int bufferPosition = 0;

static SYNTH_SAMPLE_TYPE leftBuffer[SONG_LENGTH_IN_SAMPLES];
static SYNTH_SAMPLE_TYPE rightBuffer[SONG_LENGTH_IN_SAMPLES];

static unsigned char noteBuffer[SONG_LENGTH_IN_SAMPLES * SYNTH_MAX_INSTRUMENT_COUNT];
static unsigned char envelopeBuffer[SONG_LENGTH_IN_SAMPLES * SYNTH_MAX_INSTRUMENT_COUNT];

#define PI 3.1415926536f

static unsigned int randSeed = 1;

static float rand4k() 
{
	return (float) ((randSeed *= 0x15a4e35) % 255) / 255.0f;
}


// Oscillators
static float osc_sin(float value) 
{
    return sin(value);
}

static float osc_square(float value) 
{
    if(osc_sin(value) < 0) return -1.0f;
    return 1.0f;
}

static float osc_saw(float value) 
{
	static float sawPhase = 0;
	float result = 1 - (1 / PI) * sawPhase;

	sawPhase += value;

	if (sawPhase > (2 * PI))
	{
		sawPhase = sawPhase - (2 * PI);
	}

	return result;
}

static float osc_tri(float value) 
{
    float v2 = (osc_saw(value) + .5f) * 4.0f;
    if(v2 < 2.0f) return v2 - 1.0f;
    return 3.0f - v2;
}


static void synthCallback(void *udata, Uint8 *stream, int len) 
{
	short* streamPtr = (short*) stream;

	int i = 0;
	for (i = len / (SYNTH_OUTPUT_SAMPLE_SIZE * SYNTH_NUM_CHANNELS); i > 0 ; i--) 
	{
		bufferPosition++;

		// Loop
		if (bufferPosition > SONG_LENGTH_IN_SAMPLES)
		{
			bufferPosition = 0;
		}
		
		// Copy channel data
		*streamPtr++ = clip(leftBuffer[bufferPosition]);
		*streamPtr++ = clip(rightBuffer[bufferPosition]);
	}
}

void synth_render()
{
    songdata.instruments[0] = ins_bass;
    songdata.instruments[1] = ins_pad;
    songdata.instruments[2] = ins_lead;
    songdata.instruments[3] = ins_kick;
    songdata.instruments[4] = ins_snare;

#ifdef DEBUG
	printf("synth_render\n");
#endif

	// Clear buffers
        /*
	unsigned int pos = SONG_LENGTH_IN_SAMPLES;
    do {
		leftBuffer[pos] = rightBuffer[pos] = 0;
	} while(--pos);
        */

	/*
	pos = SONG_LENGTH_IN_SAMPLES * SYNTH_MAX_INSTRUMENT_COUNT;
    do {
		noteBuffer[pos] = envelopeBuffer[pos] = 0;
	} while(--pos);
	*/
	
	// Loop each instrument
        int instrumentIndex;
        for (instrumentIndex = 0; instrumentIndex < SYNTH_MAX_INSTRUMENT_COUNT; instrumentIndex++)
        { 
                printf("rendering instrument %i / %i \n", instrumentIndex+1, SYNTH_MAX_INSTRUMENT_COUNT);

                instrument ins = songdata.instruments[instrumentIndex];
                instrument* instrument = &ins;

		// Loop each patternOrder in instrument
                int patternOrderIndex;
                for (patternOrderIndex = 0; patternOrderIndex < SYNTH_MAX_PATTERN_ORDER_COUNT; patternOrderIndex++)
		{
			int columnIndex = instrument->patternOrder[patternOrderIndex];

			if (columnIndex == 0)
			{
				// no pattern
				continue;
			}

			columnIndex--; // because pattern orders are not actually indexes (to support 0 pattern)

			// Loop each note in column
                        int noteIndex;
                        for (noteIndex = 0; noteIndex < SYNTH_PATTERN_LENGTH; noteIndex++)
			{
				unsigned char note = instrument->columns[columnIndex].notes[noteIndex];

				if (note == 0)
				{
					continue;
				}

				// Calculate song buffer position for this note
				int bufferStart = (patternOrderIndex) * SYNTH_PATTERN_LENGTH * SONG_SAMPLES_PER_ROW // song position
								+ (noteIndex) * SONG_SAMPLES_PER_ROW; // pattern position

				// Calculate note length from envelope
				unsigned int attack = instrument->env_attack;
                unsigned int sustain = instrument->env_sustain;
                unsigned int release = instrument->env_release;
				unsigned int env = attack + sustain + release;
				int noteLength = env;

				// Calculate pitch from note and octave
				unsigned char osc1Note = NOTE(note) + instrument->osc1_detune_semitone;
				unsigned char osc1Octave = OCTAVE(note) + instrument->osc1_detune_octave;
				float osc1NotePitch = pow(2.0f, (osc1Note-9)/12.0f+(osc1Octave-4)) * 440.0f;

				unsigned char osc2Note = NOTE(note) + instrument->osc2_detune_semitone;
				unsigned char osc2Octave = OCTAVE(note) + instrument->osc2_detune_octave;
				float osc2NotePitch = pow(2.0f, (osc2Note-9)/12.0f+(osc2Octave-4)) * 440.0f;

				float sample = 0;

                                int songPosition;
                                for (songPosition = bufferStart; songPosition < bufferStart+noteLength; songPosition++)
				{
					sample = 0;

					// LFO
					/*
					float lfoAmount = (float) instrument->lfo_amt / 255.0f * 0.01f;
					float lfo = lfoAmount * osc_sin((float)(songPosition-bufferStart) * 2*PI * instrument->lfo_freq / (float)SYNTH_SAMPLE_RATE);

					osc1NotePitch += lfo;
					osc2NotePitch += lfo;
					*/

					// Oscillator 1
					float volume1 = (float) instrument->osc1_vol / 255.0f;

					if (instrument->osc1_waveform == SYNTH_OSC_SINE)
					{
						sample = volume1 * osc_sin((float)songPosition * 2*PI * osc1NotePitch / (float)SYNTH_SAMPLE_RATE);
					}
					else if (instrument->osc1_waveform == SYNTH_OSC_SQUARE)
					{
						sample = volume1 * osc_square((float)songPosition * 2*PI * osc1NotePitch / (float)SYNTH_SAMPLE_RATE);
					}
					else if (instrument->osc1_waveform == SYNTH_OSC_NOISE)
					{
						sample = volume1 * osc_sin(rand4k());
					}
					else if (instrument->osc1_waveform == SYNTH_OSC_SAW)
					{
						sample = volume1 * osc_saw(2*PI * osc1NotePitch / (float)SYNTH_SAMPLE_RATE);
					}					
					else if (instrument->osc1_waveform == SYNTH_OSC_TRIANGLE)
					{
						sample = volume1 * osc_tri(2*PI * osc1NotePitch / (float)SYNTH_SAMPLE_RATE);
					}

					// Oscillator 2
					float volume2 = (float) instrument->osc2_vol / 255.0f;

					if (instrument->osc2_waveform == SYNTH_OSC_SINE)
					{
						sample += volume2 * osc_sin((float)songPosition * 2*PI * osc2NotePitch / (float)SYNTH_SAMPLE_RATE);
					}
					else if (instrument->osc2_waveform == SYNTH_OSC_SQUARE)
					{
						sample += volume2 * osc_square((float)songPosition * 2*PI * osc2NotePitch / (float)SYNTH_SAMPLE_RATE);
					}
					else if (instrument->osc2_waveform == SYNTH_OSC_NOISE)
					{
						sample += volume2 * osc_sin(rand4k());
					}
					else if (instrument->osc2_waveform == SYNTH_OSC_SAW)
					{
						sample += volume2 * osc_saw(2*PI * osc2NotePitch / (float)SYNTH_SAMPLE_RATE);
					}
					else if (instrument->osc2_waveform == SYNTH_OSC_TRIANGLE)
					{
						sample += volume2 * osc_tri(2*PI * osc2NotePitch / (float)SYNTH_SAMPLE_RATE);
					}

					// Envelope
					int notePosition = songPosition - bufferStart;

					float envelope = 1.0f;
                    if (notePosition < attack) 
					{
                        envelope = (float) notePosition / (float) attack;
                    } 
					else if (notePosition >= attack + sustain) 
					{
                        envelope -= (float) (notePosition - attack - sustain) / (float) release;
                    }

					sample *= envelope;

					// Master volume
					sample = sample * (float) SONG_MASTER_VOLUME / 255.0f;

					// Overdrive
					sample *= 1 + instrument->fx_overdrive_amt;

					// Delay
					//unsigned int p = instrument->fx_delay_time * SONG_SAMPLES_PER_ROW / 2;
					//float delayAmount = (float) instrument->fx_delay_amt / 255.0f;

					// Pan
					float pan = instrument->pan / 255.0f;
					SYNTH_SAMPLE_TYPE fl = (SYNTH_SAMPLE_TYPE) 32767 * sample * (1 - pan);
					SYNTH_SAMPLE_TYPE fr = (SYNTH_SAMPLE_TYPE) 32767 * sample * pan;

					// Write to buffers
					leftBuffer[songPosition] += fl;
					rightBuffer[songPosition] += fr;
					
					// Save note and envelope values
					noteBuffer[songPosition + SONG_LENGTH_IN_SAMPLES * instrumentIndex] = note;
					envelopeBuffer[songPosition + SONG_LENGTH_IN_SAMPLES * instrumentIndex] = envelope * 255;
				}
			}
		}
	}
}

void synth_init()
{
#ifdef DEBUG
        printf("synth_init\n");
#endif

        synth_render();

        // SDL audio specification
        SDL_AudioSpec audioSpec;
        audioSpec.freq = SYNTH_SAMPLE_RATE;
        audioSpec.format = AUDIO_S16SYS;
        audioSpec.channels = SYNTH_NUM_CHANNELS;
        audioSpec.silence = 0;
        audioSpec.samples = SYNTH_NUM_SAMPLES;
        audioSpec.size = 0;
        audioSpec.callback = synthCallback;
        audioSpec.userdata = 0;

        // Setup audio
        //if (SDL_OpenAudio(&audioSpec, NULL)) {
        if (Mix_OpenAudio(SYNTH_SAMPLE_RATE, AUDIO_S16SYS, 2, 2048)) {
                SDL_Quit();
        }
}

void synth_play()
{
    // Play music
    //SDL_PauseAudio(0);
    Mix_HookMusic(synthCallback, 0);
}

unsigned char synth_get_current_note_for_instrument(int instrument)
{
        return noteBuffer[bufferPosition + SONG_LENGTH_IN_SAMPLES * instrument];
}

unsigned char synth_get_current_envelope_value_for_instrument(int instrument)
{
	return envelopeBuffer[bufferPosition + SONG_LENGTH_IN_SAMPLES * instrument];
}

float synth_get_current_envelope_for_instrument(int instrument)
{
        return envelopeBuffer[bufferPosition + SONG_LENGTH_IN_SAMPLES * instrument] / 255.0f;
}
