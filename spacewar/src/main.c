#include <stdio.h>
#include <stdlib.h>
#include <cbm.h>
#include <conio.h>
#include <cx16.h>
#include <time.h>
#include <unistd.h>

#include "sprite.h"
#include "PSG.h"
#include "timer.h"

#define MAX_V                        16

#define	LOAD_TO_MAIN_RAM	           0
#define	LOAD_TO_VERA	              2  

#define  SPRITE_ADDR_BEGIN       0x4000
// 8bpp 16x16
#define  STAR_ADDR               0x4000
#define  ASTEROID_ADDR           0x4100
// 8bpp 32x32
#define  SHIP_ADDR_0             0x4200
#define  SHIP_ADDR_22            0x4600
#define  SHIP_ADDR_45            0x4a00
#define  SHIP_ADDR_68            0x4e00
#define  SHIP_ADDR_90            0x5200

#define  STAR_X                  312
#define  STAR_Y                  200

SpriteDefinition star_def, asteroid_def, ship_def;
unsigned wait;


void loadFile(char *fname, unsigned address)
{
   cbm_k_setnam(fname);
   cbm_k_setlfs(0,8,0);
   cbm_k_load(LOAD_TO_MAIN_RAM, address);
}

void initSprites()
{
   ship_def.mode            = SPRITE_MODE_8BPP;
   ship_def.dimensions      = SPRITE_32_BY_32;
   ship_def.block           = SHIP_ADDR_0;
   ship_def.collision_mask  = 0x0000;
   ship_def.layer           = SPRITE_LAYER_0;
   ship_def.palette_offset  = 0;
   ship_def.x               = SPRITE_X_SCALE(100);
   ship_def.y               = SPRITE_Y_SCALE(200);

   sprite_define(1, &ship_def);

   star_def = ship_def;
   star_def.dimensions      = SPRITE_16_BY_16;
   star_def.block           = STAR_ADDR;
   star_def.x               = SPRITE_X_SCALE(STAR_X);
   star_def.y               = SPRITE_Y_SCALE(STAR_Y);

   sprite_define(2, &star_def);
   
   asteroid_def = star_def;
   asteroid_def.block       = ASTEROID_ADDR;
   asteroid_def.x           = SPRITE_X_SCALE(50);
   asteroid_def.y           = SPRITE_Y_SCALE(50);
   asteroid_def.dx          = 500;
   asteroid_def.dy          = -200;

   sprite_define(3, &asteroid_def);
}

int x_delta, y_delta;

void gravity(uint8_t spritenum, SpriteDefinition *obj)
{
   sprite_pos(spritenum, obj);
   obj->x += obj->dx >> 4;
   obj->y += obj->dy >> 4;

   //
   // deltas range -320 to +320.
   //
   x_delta = (obj->x >> 5) - STAR_X; 
   y_delta = (obj->y >> 5) - STAR_Y;

   //
   // downshift to pull slower
   //
   obj->dx -= x_delta >> 6;
   obj->dy -= y_delta >> 6; 

   //
   // flip the sprite if necessary
   //
   obj->flip_horiz = (obj->dx < 0);
   obj->flip_vert  = (obj->dy > 0);
}

void move_asteroid()
{
   gravity(3, &asteroid_def);
}

typedef struct {
   int t_x;
   int t_y;
   int theta : 8; // wraps
} Ship;

Ship ship_data;

void ship_rotate(int fake_degrees)
{
   int arc;

   ship_data.theta += fake_degrees;

   // adjust so that the vertical arc begins at 0
   arc = ship_data.theta + 8;

   switch((arc % 64) / 16)
   {
      case 0: ship_def.block = SHIP_ADDR_0; break;
      case 1: ship_def.block = SHIP_ADDR_22; break;
      case 2: ship_def.block = SHIP_ADDR_45; break;
      case 3: ship_def.block = SHIP_ADDR_68; break;
   }

   gotoxy(0,0);
   cprintf("%4x", ship_def.block);

   // switch(arc / 64)
   // {
   //    case 0: // q1
   //       ship_def.flip_vert  = 0;
   //       ship_def.flip_horiz = 0;
   //       break;

   //    case 1: // q2
   //       ship_def.flip_vert  = 1;
   //       ship_def.flip_horiz = 0;
   //       break;

   //    case 2: // q3
   //       ship_def.flip_horiz = 1;
   //       ship_def.flip_vert  = 1;
   //       break;

   //    case 3: // q4
   //       ship_def.flip_horiz = 1;
   //       ship_def.flip_vert  = 0;
   //       break;
   // }
}

void move_ship()
{
//   int tan;
//   tan = abs(ship_def.dy * 4 / ship_def.dx);

   if (kbhit())
   {
      switch(cgetc())
      {
         case 'j':
         case 'a':
            ship_rotate(-15);
            break;

         case 'l':
         case 'd':
            ship_rotate(15);
            break;
      }
   }

   gravity(1, &ship_def);
}

void main()
{
   unsigned char done = 0;

   cbm_k_bsout(CH_FONT_UPPER);

   bgcolor(0); // lets background layer in
   clrscr();

   // load sprites to vera
   cbm_k_setnam("sw-sprites.bin");
   cbm_k_setlfs(0,8,0);
   cbm_k_load(LOAD_TO_VERA, SPRITE_ADDR_BEGIN);

   vera_sprites_enable(1); // cx16.h 
   initSprites();

   while(!done)
   {
      move_ship();
      move_asteroid();
      pause_jiffies(2);
   }
}
