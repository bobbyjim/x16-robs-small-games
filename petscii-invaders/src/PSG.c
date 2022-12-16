#include <cx16.h>
//#include <conio.h>

#include "PSG.h"
#include "timer.h"

int tunedNotes[] = {
//  C2   , Cs2  , d2    , ds2  , e2   , f2     
    175  , 186  , 197   , 208  , 221  , 234 ,
//  fs2  , g2   , gs2   , a2   , as2  , b2     
    248  , 263  , 278   , 295  , 312  , 331 ,
//  C3   , Cs3  , d3    , ds3  , e3   , f3     
    351  , 372  , 394   , 417  , 442  , 468 ,
//  fs3  , g3   , gs3   , a3   , as3  , b3     
    496  , 526  , 557   , 590  , 625  , 662 ,
//  c4   , cs4  , d4    , ds4  , e4   , f4     
    702  , 744  , 788   , 835  , 884  , 937 ,
//  fs4  , g4   , gs4   , a4   , as4  , b4     
    993  , 1052 , 1114  , 1181 , 1251 , 1325,
//  c5   , cs5  , d5    , ds5  , e5   , f5     
    1404 , 1488 , 1576  , 1670 , 1769 , 1875,
//  fs5  , g5   , gs5   , a5   , as5  , b5     
    1986 , 2104 , 2229  , 2362 , 2502 , 2651,
//  c6   , cs6  , d6    , ds6  , e6   , f6     
    2809 , 2976 , 3153  , 3340 , 3539 , 3750
};

int i;

int  getTunedNote( unsigned index )
{
    return tunedNotes[ index ];
}

void runVoice( unsigned voiceNumber, Voice* voice )
{
    VERA.control    = 0; // port 0
    VERA.address    = PSG_ADDRESS_VOICE(voiceNumber);
    VERA.address_hi = VERA_INC_1 + 1; // from cx16.h

    VERA.data0 = voice->frequency & 0xff;
    VERA.data0 = voice->frequency >> 8;
    VERA.data0 = (3<<6) + voice->volume; //voice->channel  + voice->volume;
    VERA.data0 = voice->waveform + voice->pulseWidth; 
}

void setVolume( unsigned voiceNumber, Voice* voice )
{
    VERA.control = 0;
    VERA.address = PSG_ADDRESS_VOICE(voiceNumber) + 2; // channel + volume
    VERA.address_hi = VERA_INC_1 + 1; // from cx16.h
    VERA.data0 = (3<<6) + voice->volume; //voice->channel  + voice->volume;
}

#define     HI_RES(x)       (x << 10)
#define     HI_RES2(x)      (x << 9)
#define     LO_RES(x)       (x >> 10)

void runVoiceWithEnvelope( unsigned voiceNumber, Voice* voice )
{
    unsigned maxVol        = HI_RES(voice->volume);
    unsigned halfMaxVol    = HI_RES2(voice->volume);
    unsigned curVolPercent = 0;
    unsigned deltaPercent;

    voice->volume = 0;
    runVoice(voiceNumber, voice);

    //
    //  Attack rise is 0 to volume setting.
    //
    deltaPercent = maxVol / (1+ADSR_ENVELOPE(voiceNumber)->attack);
    for(i=0; i<=ADSR_ENVELOPE(voiceNumber)->attack; ++i)
    {
        curVolPercent += deltaPercent;
//        cprintf("att ----- %05u %05u %05u\r", deltaPercent, curVolPercent, voice->volume);
        voice->volume = LO_RES(curVolPercent);
        setVolume( voiceNumber, voice );
        pause_jiffies(1);
    }

    //
    //  What is the decay rate??  half volume??
    //
    deltaPercent = halfMaxVol / (1+ADSR_ENVELOPE(voiceNumber)->decay);
    for(i=0; i<=ADSR_ENVELOPE(voiceNumber)->decay; ++i)
    {
        curVolPercent -= deltaPercent;
//        cprintf("dec ----- %05u %05u %05u\r", deltaPercent, curVolPercent, voice->volume);
        voice->volume = LO_RES(curVolPercent);
        setVolume( voiceNumber, voice );
        pause_jiffies(1);
    }

    //
    //  Sustain is pure time.
    //
//    cprintf("sus ----- ----- ----- %05u\r", voice->volume);
    pause_jiffies( ADSR_ENVELOPE(voiceNumber)->sustain );

    //
    //  And release is curVolPercent down to zero.
    //
    deltaPercent = halfMaxVol / (1+ADSR_ENVELOPE(voiceNumber)->release);
    for(i=0; i<ADSR_ENVELOPE(voiceNumber)->release; ++i)
    {
        curVolPercent -= deltaPercent;
//        cprintf("rel ----- %05u %05u %05u\r", deltaPercent, curVolPercent, voice->volume);
        voice->volume = LO_RES(curVolPercent);
        setVolume( voiceNumber, voice );
        pause_jiffies(1);
    }
}

void bang( unsigned frequency )
{
   Voice voice;
   voice.frequency  = frequency;
   voice.channel    = PSG_CHANNEL_BOTH;
   voice.volume     = PSG_VOLUME_KNOB_10;
   voice.waveform   = PSG_WAVE_NOISE;
   voice.pulseWidth = 0;
   ADSR_ENVELOPE(1)->attack  = 0;
   ADSR_ENVELOPE(1)->decay   = 10;
   ADSR_ENVELOPE(1)->sustain = 0;
   ADSR_ENVELOPE(1)->release = 10;
   runVoiceWithEnvelope( 1, &voice );
}