



#include <conio.h>
#include <stdlib.h>

#include "invader.h"
#include "PSG.h"
#include "timer.h"
#include "missiles.h"
#include "score.h"

Voice voiceSettings;

unsigned freq[] = { 150, 160, 170, 180 };

#define     CH_REVERSE      64
#define     INVADER_ROWS    5
#define     INVADER_COLS    11
#define     STANCE_1        0
#define     STANCE_2        10
#define     MARGIN_LEFT     10
#define     MARGIN_RIGHT    70

int x1      = MARGIN_LEFT;
int x2      = MARGIN_LEFT + 44;
int dx      = 1;

int y1      = 5;
int y2      = 15;
int speed   = 50;
int v       = 4;

int stance  = STANCE_1;

char invaderType[INVADER_ROWS][INVADER_COLS] = {

   { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 },
   { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
   { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
   { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
   { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }

};

int invader1[20] = {

      32, (191 + CH_REVERSE),  (163 + CH_REVERSE),  191 ,               32, 
      32, (191 + CH_REVERSE),  (185 + CH_REVERSE),  191 ,               32,

      32, (191 + CH_REVERSE),  (163 + CH_REVERSE),  191 ,               32,
      32, (181 + CH_REVERSE),  (162 + CH_REVERSE),  (182 + CH_REVERSE), 32

};

int invader2[20] = {

     32, 188 + CH_REVERSE, 25,  190 + CH_REVERSE, 32,
     32, 187 + CH_REVERSE, 32,  172 + CH_REVERSE, 32,

     32, 184 + CH_REVERSE, 25,  184 + CH_REVERSE, 32,
     32, 172 + CH_REVERSE, 162, 187 + CH_REVERSE, 32 

};

int invader3[20] = {

     32, 169 + CH_REVERSE, 178 + CH_REVERSE, 159  + CH_REVERSE, 32,
     32, 88,               32,               88,                32,  
     32, 169 + CH_REVERSE, 178 + CH_REVERSE, 159  + CH_REVERSE, 32,
     32, 60,               32,               62,                32
};

void initInvaders()
{
    voiceSettings.frequency  = 400;
    voiceSettings.channel    = PSG_CHANNEL_BOTH;
    voiceSettings.volume     = PSG_VOLUME_KNOB_5;
    voiceSettings.waveform   = PSG_WAVE_PULSE;
    voiceSettings.pulseWidth = PSG_PULSE_SQUARE;

   //  runVoice(1, &voiceSettings);

   //  voiceSettings.frequency  = 401;
   //  voiceSettings.channel    = PSG_CHANNEL_BOTH;
   //  voiceSettings.volume     = PSG_VOLUME_KNOB_5;
   //  voiceSettings.waveform   = PSG_WAVE_PULSE;
   //  voiceSettings.pulseWidth = PSG_PULSE_SQUARE;

   //  runVoice(2, &voiceSettings);
}

unsigned char attackInvaderAt(int i, int j)
{
   unsigned char thisInvader;

   // invaders have origin [x1,y1] and occupy a 44 x 15 grid on screen, including whitespace.
   // First, translate [i,j] to the [x1,y1] origin.
   i -= x1;
   j -= y1 + 1; // +1 to adjust for bottom of invader.

   // Now scale to fit the 11 x 5 invader grid.
   i /= 5;
   j /= 3;

   gotoxy(0,40);
   cprintf("%2u,%2u", i, j);

   if (i<0 || i>10 || j<0 || j>4) 
      return 0; // out of range = automatic miss

   // [i,j] now points to a cell in the invader's space.
   thisInvader = invaderType[j][i];
   if (thisInvader > 0) // we got one!
   {
      addPoints(thisInvader);
      invaderType[i][j] = 0; // kill it
      --speed;
      // voiceSettings.frequency += 10;
      // runVoice(1, &voiceSettings);  
      // voiceSettings.frequency += 1;
      // runVoice(2, &voiceSettings);
   }
   return thisInvader;
}

void showInvaders(int row)
{
   int col,k;
   int *invader = invader1;

   textcolor(COLOR_WHITE);

   for(col=0; col<INVADER_COLS; ++col)
   {
      int x = col * 4;
      int y = row * 3 + 5;
      
      int t = invaderType[row][col];
      if (t == 0) continue; // this invader is dead. do not draw.
      if (t == 1) invader = invader1;
      if (t == 2) invader = invader2;
      if (t == 3) invader = invader3;
      revers(0);
      for(k=0; k<5; ++k)
      {
         cputcxy(x1 + x + k, y1 + y - 1, ' ');
         if (invader[stance+k] > 0xbf) revers(1);
         cputcxy(x1 + x + k, y1 + y,invader[stance+k]);
         revers(0);
         if (invader[stance+k+5] > 0xbf) revers(1);
         cputcxy(x1 + x + k, y1 + y+1,invader[stance+k+5]);
         revers(0);
      }

      if (rand() % 200 == 1)
         addInvaderMissile( x1 + x + 2, y1 + y );
   }
}

void updateInvaderPosition(int epoch)
{
   if (epoch % speed > 0)
      return;

   stance = (stance == STANCE_1)? STANCE_2 : STANCE_1;
   x1 += dx;
   x2 += dx;

   if (x1 < MARGIN_LEFT && dx == -1) 
   {
       dx = 1;
       ++x1;
       ++x2;
       ++y1;
       ++y2;
   }
   else if (x2 > MARGIN_RIGHT)
   {
       dx = -1;
       --x2;
       --x1;
       ++y1;
       ++y2;
   }

   showInvaders(0);
   showInvaders(1);
   showInvaders(2);
   showInvaders(3);
   showInvaders(4);

   voiceSettings.frequency = freq[v];
   runVoice(1, &voiceSettings);   
   voiceSettings.frequency++;
   runVoice(2, &voiceSettings);   
   v = (v+1) % 4;
}
