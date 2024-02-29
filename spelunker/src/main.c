#include <6502.h>
#include <cbm.h>

#include "map.h"
#include "universal.h"
#include "common.h"

byte x, y;

void set_PET_font()
{
   struct regs fontregs;
   fontregs.a = 4; // PET-like
   fontregs.pc = 0xff62;
   _sys(&fontregs);
}

void main()
{
   set_PET_font();

   login("spelunk", "22-03-04");
   map_create();
   map_showAll();

   x = 0;
   y = 25;
   
   map_draw(x,y);
}