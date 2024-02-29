#include <stdio.h>
#include <stdlib.h>
#include <cbm.h>
#include <conio.h>
#include <cx16.h>
#include <time.h>
#include <unistd.h>

#include "sprite.h"
#include "timer.h"

#define MAX_V                        16

#define	LOAD_TO_MAIN_RAM	           0
#define	LOAD_TO_VERA	              2  

#define  SHIP_1                  0
#define  SHIP_2                  1

#define  SPRITE_ADDR_BEGIN       0x4000
// 8bpp 16x16
#define  STAR_ADDR               0x4000
#define  ASTEROID_ADDR           0x4100
// 8bpp 32x32
#define  SHIP1_ADDR_0             0x4200
#define  SHIP1_ADDR_15            0x4600
#define  SHIP1_ADDR_30            0x4a00
#define  SHIP1_ADDR_45            0x4e00
#define  SHIP1_ADDR_60            0x5200
#define  SHIP1_ADDR_75            0x5600
#define  SHIP1_ADDR_90            0x5a00

#define  SHIP2_ADDR_0             0x5e00
#define  SHIP2_ADDR_15            0x6200
#define  SHIP2_ADDR_30            0x6600
#define  SHIP2_ADDR_45            0x6a00
#define  SHIP2_ADDR_60            0x6e00
#define  SHIP2_ADDR_75            0x7200
#define  SHIP2_ADDR_90            0x7600

#define  STAR_X                  312
#define  STAR_Y                  200

#define  STAR_SPRITE             0
#define  SHIP_1_SPRITE           1
#define  SHIP_2_SPRITE           2
#define  ASTEROID_SPRITE         3

SpriteDefinition star_def, asteroid_def, ship_def[2];
unsigned wait;


void loadFile(char *fname, unsigned address)
{
   cbm_k_setnam(fname);
   cbm_k_setlfs(0,8,0);
   cbm_k_load(LOAD_TO_MAIN_RAM, address);
}

void initSprite(SpriteDefinition *sprdef, unsigned char dimensions, unsigned int address, int x, int y)
{
   sprdef->mode       = SPRITE_MODE_8BPP;
   sprdef->dimensions = dimensions;
   sprdef->block      = address;
   sprdef->collision_mask = 0x0000;
   sprdef->layer      = SPRITE_LAYER_0;
   sprdef->palette_offset = 0;
   sprdef->x          = SPRITE_X_SCALE(x);
   sprdef->y          = SPRITE_Y_SCALE(y);
}

void initSprites()
{
   initSprite(&ship_def[0], SPRITE_32_BY_32, SHIP1_ADDR_0, 100, 200);
   sprite_define(SHIP_1_SPRITE, &ship_def[0]);

   initSprite(&ship_def[1], SPRITE_32_BY_32, SHIP2_ADDR_0, 400, 100);
   sprite_define(SHIP_2_SPRITE, &ship_def[1]);

   initSprite(&star_def, SPRITE_16_BY_16, STAR_ADDR, STAR_X, STAR_Y );
   sprite_define(STAR_SPRITE, &star_def);
   
   initSprite(&asteroid_def, SPRITE_16_BY_16, ASTEROID_ADDR, 50, 50 );
   asteroid_def.dx          = 500;
   asteroid_def.dy          = -200;
   sprite_define(ASTEROID_SPRITE, &asteroid_def);
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
   if (spritenum != SHIP_1_SPRITE && spritenum != SHIP_2_SPRITE)
   {
      obj->flip_horiz = (obj->dx < 0);
      obj->flip_vert  = (obj->dy > 0);
   }
}

void move_asteroid()
{
   gravity(3, &asteroid_def);
}

int ship_theta[2] = { 0, 0 };

unsigned rotation_addr[2][25] = {
   { 
   SHIP1_ADDR_0,  SHIP1_ADDR_15, SHIP1_ADDR_30, SHIP1_ADDR_45, SHIP1_ADDR_60, SHIP1_ADDR_75, 
   SHIP1_ADDR_90, SHIP1_ADDR_75, SHIP1_ADDR_60, SHIP1_ADDR_45, SHIP1_ADDR_30, SHIP1_ADDR_15, 
   SHIP1_ADDR_0,  SHIP1_ADDR_15, SHIP1_ADDR_30, SHIP1_ADDR_45, SHIP1_ADDR_60, SHIP1_ADDR_75, 
   SHIP1_ADDR_90, SHIP1_ADDR_75, SHIP1_ADDR_60, SHIP1_ADDR_45, SHIP1_ADDR_30, SHIP1_ADDR_15,
   SHIP1_ADDR_0 
   },
   {
   SHIP2_ADDR_0,  SHIP2_ADDR_15, SHIP2_ADDR_30, SHIP2_ADDR_45, SHIP2_ADDR_60, SHIP2_ADDR_75, 
   SHIP2_ADDR_90, SHIP2_ADDR_75, SHIP2_ADDR_60, SHIP2_ADDR_45, SHIP2_ADDR_30, SHIP2_ADDR_15, 
   SHIP2_ADDR_0,  SHIP2_ADDR_15, SHIP2_ADDR_30, SHIP2_ADDR_45, SHIP2_ADDR_60, SHIP2_ADDR_75, 
   SHIP2_ADDR_90, SHIP2_ADDR_75, SHIP2_ADDR_60, SHIP2_ADDR_45, SHIP2_ADDR_30, SHIP2_ADDR_15,
   SHIP2_ADDR_0
   }
};

int rotation_vector_x[] = {
   0,  8,  15, 22, 27, 30, 
   31, 30, 27, 22, 16, 8, 
   0,  -8,  -15, -22, -27, -30, 
   -31, -30, -27, -22, -16, -8, 
   0
};

int rotation_vector_y[] = {
   -32, -30, -27, -22, -16, -8, 
   0,  8,  15,  22,  27,  30,
   32, 30, 27, 22, 16, 8, 
   0, -8, -15, -22, -27, -30, 
   -32
};

void ship_accelerate(int shipnum)
{
   SpriteDefinition* shipDef = &ship_def[shipnum];
   int theta = ship_theta[shipnum];

   shipDef->dx += rotation_vector_x[ theta/10 ];
   shipDef->dy += rotation_vector_y[ theta/10 ];
}

void ship_update(int shipnum, int spritenum, int fake_degrees)
{
   SpriteDefinition* shipDef = &(ship_def[shipnum]);

   ship_theta[shipnum] += fake_degrees;

   if (ship_theta[shipnum] < 0)
       ship_theta[shipnum] += 240;
   else
   if (ship_theta[shipnum] >= 240)
       ship_theta[shipnum] -= 240; 

   shipDef->block = rotation_addr[shipnum][ ship_theta[shipnum]/10 ];
   switch(ship_theta[shipnum] / 60)
   {
      case 0: // q1
         shipDef->flip_vert  = 0;
         shipDef->flip_horiz = 0;
         break;

      case 1: // q2
         shipDef->flip_vert  = 1;
         shipDef->flip_horiz = 0;
         break;

      case 2: // q3
         shipDef->flip_vert  = 1;
         shipDef->flip_horiz = 1;
         break;

      case 3: // q4
         shipDef->flip_vert  = 0;
         shipDef->flip_horiz = 1;
         break;
   }
   sprite_changeBlock(spritenum, shipDef);
   sprite_flip(spritenum, shipDef);
}

void move_ship()
{
//   int tan;
//   tan = abs(ship_def.dy * 4 / ship_def.dx);

   if (kbhit())
   {
      switch(cgetc())
      {
         case 'a':
            ship_update(SHIP_1,SHIP_1_SPRITE,-10);
            break;

         case 'd':
            ship_update(SHIP_1,SHIP_1_SPRITE,10);
            break;

         case 's':
            ship_accelerate(SHIP_1);
            break;

         case 'j':
            ship_update(SHIP_2,SHIP_2_SPRITE,-10);
            break;

         case 'l':
            ship_update(SHIP_2,SHIP_2_SPRITE,10);
            break;

         case 'k':
            ship_accelerate(SHIP_2);
            break;
      }
   }

   gravity(SHIP_1_SPRITE, &ship_def[0]);
   gravity(SHIP_2_SPRITE, &ship_def[1]);
}

void main()
{
   unsigned char done = 0;

   cbm_k_bsout(CH_FONT_UPPER);

   bgcolor(0); // lets background layer in
   clrscr();

   // load sprites to vera
   cbm_k_setnam("temp-sprites2.bin");
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
