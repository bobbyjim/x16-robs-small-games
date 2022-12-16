#ifndef _PSG_H_
#define _PSG_H_

#define     PSG_ADDRESS             0xf9c0
#define     PSG_ADDRESS_VOICE(x)    (PSG_ADDRESS + (x << 4))

#define     PSG_CHANNEL_OFF         0
#define     PSG_CHANNEL_LEFT        (1 << 6)
#define     PSG_CHANNEL_RIGHT       (2 << 6)
#define     PSG_CHANNEL_BOTH        (3 << 6)

#define     PSG_VOLUME_MAX          63
#define     PSG_VOLUME_HALF         32

#define     PSG_WAVE_PULSE          0
#define     PSG_WAVE_SAWTOOTH       64
#define     PSG_WAVE_TRIANGLE       128
#define     PSG_WAVE_NOISE          196

#define     PSG_PULSE_SQUARE        63
#define     PSG_PULSE_QUARTER       32
#define     PSG_PULSE_EIGHTH        16
#define     PSG_PULSE_SIXTEENTH     8

typedef struct {
    int frequency: 16;
    int channel:    8;
    int volume:     6;
    int waveform:   8;
    int pulseWidth: 6;

    // int attack:  8;
    // int decay:   8;
    // int sustain: 8;
    // int release: 8;
} Voice;

void defineVoice( unsigned int voiceNumber, Voice* voice );
int  getTunedNote( unsigned int index );

#endif