#include "map.h"
#include "universal.h"
#include "common.h"

byte x, y;

void main()
{
   setPetFont();

   login("spelunk", "22-03-04");
   map_create();
   map_showAll();

   x = 0;
   y = 25;
   
   map_draw(x,y);
}