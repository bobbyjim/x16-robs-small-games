/*

    PETSCII Invaders: a space invaders clone
    Copyright (C) 2022 Robert Eaglestone

    This file is part of PETSCII Invaders.

    PETSCII Invaders is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PETSCII Invaders is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with PETSCII Invaders.  If not, see <https://www.gnu.org/licenses/>.

*/

#include <stdlib.h>
#include <conio.h>
#include <cx16.h>

#include "common.h"
#include "invader.h"
#include "score.h"
#include "timer.h"
#include "cannon.h"
#include "missiles.h"

#define  VERSION     "0.01"

#define  HOUSE_X     15
#define  HOUSE_Y     48

char house[2][8] = {
   { 0xa9, 32, 32, 32, 32, 32, 0x7f, 0x00 },
   { 32,   32, 32, 32, 32, 32, 32,   0x00 }
};

void showHouses()
{
   int i;
   int x;

   for(i=0; i<4; ++i)
   {
      x = HOUSE_X + i * 15;
      revers(1);
      cputsxy(x,HOUSE_Y,house[0]);
      cputsxy(x,HOUSE_Y+1,house[1]);
      gotoxy(x,HOUSE_Y+2);
      cputc(32);
      cputc(32);
      revers(0);
      cputc(0xa9);
      cputc(32);
      cputc(0x7f);
      revers(1);
      cputc(32);
      cputc(32);
   
      cputcxy(x,HOUSE_Y+3,32);
      cputcxy(x+1,HOUSE_Y+3,32);
      cputcxy(x+5,HOUSE_Y+3,32);
      cputcxy(x+6,HOUSE_Y+3,32);
   }
}

void main() 
{
   unsigned epoch = 0;
   char offx = 4;
   char offy = 4;

   cbm_k_bsout(CH_BLACK);
   cbm_k_bsout(0x01);  // #define TO_BACKGROUND	0x01
   clrscr();

   // load PET font
   cbm_k_bsout(0x8E); // revert to primary case
   common_loadCharacterSet("petfont.bin");

   textcolor(COLOR_WHITE);
   cputsxy(2,1,"score                 high");
   textcolor(COLOR_GREEN);
   chlinexy(0,59,80);

   initInvaders();
   showHouses();
   initCannon();

   for(;;)
   {
//      showScore();
      showCannon();

      if (trackMissiles(epoch))
         maybeBlowSomethingUp( whatDidMissileHit() );

      updateInvaderPosition(epoch);

      pause_jiffies(1);
      epoch = (epoch + 1) % 1000;
   }
}
