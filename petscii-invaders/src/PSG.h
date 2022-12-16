#ifndef _PSG_H_
#define _PSG_H_

#define     ADSR_ENVELOPE(x)        ((Envelope *)(0x0400 + (x * 12)))
#define     PSG_ADDRESS             0xf9c0
#define     PSG_ADDRESS_VOICE(x)    (PSG_ADDRESS + (x << 4))

#define     PSG_CHANNEL_OFF         0
#define     PSG_CHANNEL_LEFT        (1 << 6)
#define     PSG_CHANNEL_RIGHT       (2 << 6)
#define     PSG_CHANNEL_BOTH        (3 << 6)

#define     PSG_VOLUME_KNOB_11      63
#define     PSG_VOLUME_KNOB_10      58
#define     PSG_VOLUME_KNOB_9       52
#define     PSG_VOLUME_KNOB_8       46
#define     PSG_VOLUME_KNOB_7       40
#define     PSG_VOLUME_KNOB_6       35
#define     PSG_VOLUME_KNOB_5       29
#define     PSG_VOLUME_KNOB_4       23
#define     PSG_VOLUME_KNOB_3       17
#define     PSG_VOLUME_KNOB_2       11
#define     PSG_VOLUME_KNOB_1       6
#define     PSG_VOLUME_KNOB_0       0

#define     PSG_WAVE_PULSE          0
#define     PSG_WAVE_SAWTOOTH       64
#define     PSG_WAVE_TRIANGLE       128
#define     PSG_WAVE_NOISE          196

#define     PSG_PULSE_SQUARE        63
#define     PSG_PULSE_QUARTER       32
#define     PSG_PULSE_EIGHTH        16
#define     PSG_PULSE_SIXTEENTH     8

typedef struct {
    int frequency  :16;
    int channel    :8;
    int volume     :6;
    int n1         :2;
    int waveform   :8;
    int pulseWidth :6;
    int n2         :2;
} Voice;

typedef struct {
    long phase; // along the ADSR envelope
    int attack     :16;
    int decay      :16;
    int sustain    :16;
    int release    :16;
} Envelope;

void runVoice( unsigned voiceNumber, Voice* voice );
void runVoiceWithEnvelope( unsigned voiceNumber, Voice* voice );
int  getTunedNote( unsigned index );
void bang(unsigned frequency);

#endif