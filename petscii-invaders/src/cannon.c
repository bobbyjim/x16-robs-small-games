



#include <conio.h>

#include "cannon.h"
#include "missiles.h"

int x = 40;

void drawCannon()
{
    textcolor(COLOR_GREEN);
    revers(0);
    cputcxy(x-1, CANNON_Y+1, ' ');
    revers(1);
    cputc(161);
    cputs("   ");
    revers(0);
    cputc(' ');
    cputcxy(x, CANNON_Y, ' ');
    cputc(172);
    revers(1);
    cputc(188);
    revers(0);
    cputc(' ');
}

void initCannon()
{
    drawCannon();
}

void showCannon()
{
    if (kbhit())
    {
       switch(cgetc())
       {
          // left
          case 'j':
          case 'a':
          case 157:
            if (x>CANNON_MIN_X) --x;
            break;

          // right
          case 'l':
          case 'd':
          case 29:
            if (x<CANNON_MAX_X) ++x;
            break;

          // fire
          case 'z':
          case '/':
          case ' ':
            fireCannon(x+1);
            break;
       }
       drawCannon();
    }
}
