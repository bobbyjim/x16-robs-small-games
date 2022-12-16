

#include <conio.h>

#include "missiles.h"
#include "cannon.h"
#include "invader.h"

#define     MAX_INVADER_MISSILES    4

unsigned char missile_x[MAX_INVADER_MISSILES];
unsigned char missile_y[MAX_INVADER_MISSILES];
unsigned char currentMissile = 0;
unsigned char cannon_x = 0;
unsigned char cannon_y = 0;

unsigned char trackMissiles(int epoch)
{
   int i;

   if (epoch % 5 > 0) return 0;

   for(i=0; i<MAX_INVADER_MISSILES; ++i)
   {
      if (missile_y[i] == 0)
         continue;
      
      cputcxy(missile_x[i], missile_y[i], ' ');
      ++missile_y[i];
      if (missile_y[i] > 55) 
        missile_y[i] = 0;
      else // draw it
        cputcxy(missile_x[i], missile_y[i], '$');
   }

   cputcxy(cannon_x+1, cannon_y, ' ');
   if (cannon_y < 10)
      cannon_y = 0;
   else
   {
      --cannon_y;
      // has it hit an invader?
      if (attackInvaderAt(cannon_x, cannon_y)) // got one!
         cannon_y = 0; // remove it from the field.
      else
      {
         cputcxy(cannon_x+1, cannon_y, 199);
         gotoxy(40,0);
      }
   }
   
   return 0;
}

void addInvaderMissile(unsigned char x, unsigned char y)
{
    if (missile_y[ currentMissile ] > 0)
        return; // already in use

    // create new missile
    missile_x[currentMissile] = x;
    missile_y[currentMissile] = y;

    // next missile
    currentMissile = (currentMissile+1) % MAX_INVADER_MISSILES;

}

void fireCannon(unsigned char x)
{
    if (cannon_y > 0)
        return; // already in use

    cannon_x = x;
    cannon_y = CANNON_Y-1;
}

void maybeBlowSomethingUp(unsigned char target)
{
    if (target == 0)
        return;
}

unsigned char whatDidMissileHit()
{
    return 0;
}
