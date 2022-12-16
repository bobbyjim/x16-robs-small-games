
#include <cx16.h>

#include "PSG.h"

// typedef struct {

//     int frequency: 16;
//     int channel:    2;
//     int volume:     6;
//     int waveform:   2;
//     int pulseWidth: 6;

// } Voice;

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

int  getTunedNote( unsigned int index )
{
    return tunedNotes[ index ];
}

void defineVoice( unsigned int voiceNumber, Voice* voice )
{
    VERA.control    = 0; // port 0
    VERA.address    = PSG_ADDRESS_VOICE(voiceNumber);
    VERA.address_hi = VERA_INC_1 + 1; // from cx16.h

    VERA.data0 = voice->frequency & 0xff;
    VERA.data0 = voice->frequency >> 8;
    VERA.data0 = (3<<6) + voice->volume; //voice->channel  + voice->volume;
    VERA.data0 = voice->waveform + voice->pulseWidth; 
}
