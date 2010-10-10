// Music

#ifndef SONG_H
#define SONG_H

#ifdef __cplusplus
extern "C" {
#endif


#include "synth.h"


// Notes
/*
	0- C
	1- C#/Db
	2- D
	3- D#/Eb
	4- E
	5- F
	6- F#/Gb
	7- G
	8- G#/Ab
	9- A
	10- A#/Bb
	11- B
*/

static instrument ins_lead = {
        // General
        35, // Pan
        // Oscillator 1
        0, // Detune for octave
        0, // Detune for semitone
        40, // Volume
        SYNTH_OSC_TRIANGLE, // Wave form
        // Oscillator 2
        0, // Detune for octave
        0, // Detune for semitone
        30, // Volume
        SYNTH_OSC_SQUARE, // Wave form
        // LFO
        1, // LFO frequency
        0, // LFO amount
        // Envelope
        100, // Attack
        2727, // Sustain
        22727, // Release
        100, // Delay time
    100, // Delay amount
        0, // Overdrive amount
        // Patterns
        {0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0}, // Pattern order
        // Columns
        {
                {{0x94, 0, 0, 0, 0, 0, 0x05, 0, 0x54, 0, 0, 0, 0, 0, 0x05, 0, 0x44, 0, 0, 0, 0, 0, 0x05, 0, 0x44, 0, 0, 0, 0, 0, 0, 0}},
        }
};

static instrument ins_bass = {
        // General
        128, // Pan
        // Oscillator 1
        -2, // Detune for octave
        0, // Detune for semitone
        100, // Volume
        SYNTH_OSC_SINE, // Wave form
        // Oscillator 2
        -2, // Detune for octave
        0, // Detune for semitone
        30, // Volume
        SYNTH_OSC_SAW, // Wave form
        // LFO
        0.1, // LFO frequency
        10, // LFO amount
        // Envelope
        100, // Attack
        50000, // Sustain
        40000, // Release
        0, // Delay time
    0, // Delay amount
        0, // Overdrive amount
        // Patterns
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}, // Pattern order
        // Columns
        {
                {{0x24, 0, 0, 0, 0, 0, 0, 0, 0xA3, 0, 0, 0, 0, 0, 0, 0, 0x93, 0, 0, 0, 0, 0, 0, 0, 0x93, 0, 0, 0, 0, 0, 0, 0}}
        }
};

static instrument ins_pad = {
        // General
        230, // Pan
        // Oscillator 1
        -2, // Detune for octave
        0, // Detune for semitone
        170, // Volume
        SYNTH_OSC_SINE, // Wave form
        // Oscillator 2
        -2, // Detune for octave
        0, // Detune for semitone
        100, // Volume
        SYNTH_OSC_SINE, // Wave form
        // LFO
        0.1, // LFO frequency
        10, // LFO amount
        // Envelope
        100, // Attack
        30000, // Sustain
        20000, // Release
        0, // Delay time
    0, // Delay amount
        0, // Overdrive amount
        // Patterns
        {0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1}, // Pattern order
        // Columns
        {
                {{0x06, 0, 0x06, 0, 0x26, 0, 0x46, 0, 0x56, 0, 0, 0, 0x46, 0, 0x06, 0, 0x95, 0, 0, 0, 0, 0, 0x06, 0, 0x95, 0, 0, 0, 0x75, 0, 0, 0}}
        }
};

static instrument ins_kick = {
        // General
        128, // Pan
        // Oscillator 1
        -3, // Detune for octave
        -8, // Detune for semitone
        100, // Volume
        SYNTH_OSC_SINE, // Wave form
        // Oscillator 2
        -2, // Detune for octave
        -8, // Detune for semitone
        100, // Volume
        SYNTH_OSC_SINE, // Wave form
        // LFO
        0, // LFO frequency
        0, // LFO amount
        // Envelope
        10, // Attack
        3000, // Sustain
        200, // Release
        0, // Delay time
                0, // Delay amount
        0, // Overdrive amount
        // Patterns
        {0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0}, // Pattern order
        // Columns
        {
                {{0x94, 0, 0, 0, 0, 0x94, 0, 0x94, 0x94, 0, 0, 0, 0x94, 0, 0, 0x94, 0x94, 0, 0, 0, 0, 0x94, 0, 0x94, 0x94, 0, 0, 0, 0x94, 0, 0, 0x94}},
        }
};

static instrument ins_snare = {
        // General
        128, // Pan
        // Oscillator 1
        0, // Detune for octave
        0, // Detune for semitone
        150, // Volume
        SYNTH_OSC_NOISE, // Wave form
        // Oscillator 2
        0, // Detune for octave
        0, // Detune for semitone
        20, // Volume
        SYNTH_OSC_NOISE, // Wave form
        // LFO
        0, // LFO frequency
        0, // LFO amount
        // Envelope
        100, // Attack
        3000, // Sustain
        4000, // Release
        0, // Delay time
    0, // Delay amount
        2, // Overdrive amount
        // Patterns
        {0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0}, // Pattern order
        // Columns
        {
                {{0, 0, 0x94, 0, 0, 0, 0x94, 0, 0, 0, 0x94, 0, 0, 0, 0x94, 0, 0, 0, 0x94, 0, 0, 0, 0x94, 0, 0, 0, 0x94, 0, 0, 0, 0x94, 0}},
        }
};

static song songdata;

#define SONG_BPM 50
#define SONG_MASTER_VOLUME 160 // 0-255
#define SONG_SAMPLES_PER_ROW	(SYNTH_SAMPLE_RATE*4*60 / (SONG_BPM*16))
#define SONG_LENGTH_IN_SAMPLES (SYNTH_MAX_PATTERN_ORDER_COUNT * SYNTH_PATTERN_LENGTH * SYNTH_SAMPLE_RATE)

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif


#endif
