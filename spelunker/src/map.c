
#include <stdlib.h>
#include <conio.h>
#include <cx16.h>

#include "common.h"
#include "map.h"


#define     ROWS        50
#define     COLS        50

#define     CORNER_NE       202
#define     CORNER_NW       203
#define     CORNER_SE       213
#define     CORNER_SW       201
#define     T_HORIZ_N       177
#define     T_HORIZ_S       178
#define     T_VERT_E        171
#define     T_VERT_W        179
#define     VERT            192
#define     HORIZ           221
#define     CROSSING        219
#define     ROOM            215

#define     WALL            166

//                 Corners    T-Junct        Vert, Cross, Horiz   T-Junct      Corners     Room
byte mapchar[] = { 213, 201,  171, 179,      192, 219, 221,      177, 178,     202, 203,   215 };

void map_create()
{
   int i, j;
   _randomize();

   cputs("warning!  not r39+ safe!\r\n\r\n");
   RAM_BANK = 1;            // r38 !!!!
   for(j=1; j<ROWS-1; ++j)
   {
      BANK_RAM[j * COLS] = 219;      // outer box = cross 
      BANK_RAM[j * COLS + ROWS - 1] = 219;
      for(i=1; i<COLS-1; ++i)
         BANK_RAM[i + j * COLS] = mapchar[ rand() % 6 + rand() % 7 ];
   }
   for(i = 0; i < COLS; ++i) // outer box = cross
   {
      BANK_RAM[i] = 219;         
      BANK_RAM[i + (ROWS-1) * COLS] = 219;
   }
}

void map_showAll()
{
    byte i, j;

    clrscr();
    RAM_BANK = 1;
    for(j=0; j<ROWS; ++j)
    {
       gotoxy(14,j+4);
       for(i=0; i<COLS; ++i)
          cputc( BANK_RAM[j * COLS + i] );
    }

    cgetc();
}

char solid[] = {
    166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
    166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
    166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
    166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 
    0
};

char center_passage[] = {
    166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
    166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 32, 32, 32, 32, 32, 32, 32, 32, 32,
    32, 32, 32, 32, 32, 32, 32, 32, 32, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
    166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
    0
};

char center_open1[] = {
    166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
    166, 166, 166, 166, 166, 166, 166, 166, 166, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 166, 166, 166, 166, 166, 166, 166, 166, 166,
    166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
    0
};

char center_open2[] = {
    166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
    166, 166, 166, 166, 166, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 166, 166, 166, 166, 166,
    166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
    0
};

char center_open3[] = {
    166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
    166, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 166,
    166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
    0
};

char center_open4[] = {
    166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 32, 32, 32, 32,
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
    32, 32, 32, 32, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
    0
};

char left_open[] = {
    32, 32, 32, 32, 32, 32, 32, 32, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
    166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
    166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
    166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 
    0
};

char right_open[] = {
    166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
    166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
    166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
    166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 166, 32, 32, 32, 32, 32, 32, 32, 32, 
    0
};

void map_draw(byte x, byte y)
{
    byte current;
    byte i;

    RAM_BANK = 1;
    current = BANK_RAM[ x + y * COLS ];

    clrscr();

    // cputs(solid);
    // cputs(center_passage);
    // cputs(center_open1);
    // cputs(center_open2);
    // cputs(center_open3);
    // cputs(center_open4);
    // cputs(left_open);
    // cputs(right_open);

    switch(current)
    {
        case 213:   // se corner
        case 201:   // sw corner
        case 171:   // vert-east T
        case 179:   // vert-west T
        case 192:   // vert
        case 221:   // horiz
        case 177:   // horiz-north T
        case 178:   // horiz-south T
        case 202:   // ne corner
        case 203:   // nw corner
        case 215:   // room
            for(i=0; i<10; ++i)
               cputs(center_passage);
            cputs(center_open1);
            cputs(center_open1);
            cputs(center_open2);
            cputs(center_open2);
            cputs(center_open3);
            cputs(center_open3);
            cputs(center_open4);
            cputs(center_open4);
            cputs(center_open4);
            cputs(center_open4);
            gotoxy(0,40);
            cputs(center_open4);
            cputs(center_open4);
            cputs(center_open4);
            cputs(center_open4);
            cputs(center_open3);
            cputs(center_open3);
            cputs(center_open2);
            cputs(center_open2);
            cputs(center_open1);
            cputs(center_open1);
            for(i=0; i<10; ++i)
               cputs(center_passage);
            break;

        case 219:   // cross
        default:
            for(i=0; i<20; ++i)
                cputs(center_passage);
            gotoxy(0,40);
            for(i=0; i<20; ++i)
                cputs(center_passage);
            break;

    }
}
