
#include <cbm.h>

#include "map.h"
#include "universal.h"
#include "common.h"

byte x, y;

void main()
{
   cbm_k_setnam("petfont.bin");
   cbm_k_setlfs(0,8,0);
   cbm_k_load(2, 0x0f800);

   login("spelunk", "22-03-04");
   map_create();
   map_showAll();

   x = 0;
   y = 25;
   
   map_draw(x,y);
}