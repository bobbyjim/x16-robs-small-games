#include <stdio.h>
#include <stdlib.h>
#include <cbm.h>
#include <conio.h>
#include <cx16.h>
#include <time.h>
#include <unistd.h>

#include "sprite.h"
#include "timer.h"
#include "gradients.h"

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

#define  X_GRADIENT     ((int*)(0x8000))
#define  Y_GRADIENT     ((int*)(0x9000))

void gravity(uint8_t spritenum, SpriteDefinition *obj)
{
   int *x = X_GRADIENT;
   int *y = Y_GRADIENT; 
   int  col = obj->x >> (SPRITE_POSITION_FRACTIONAL_BITS+4); // 0..39
   int  row = obj->y >> (SPRITE_POSITION_FRACTIONAL_BITS+4); // 0..29
   int i, j;

   sprite_pos(spritenum, obj);
   obj->x += obj->dx >> 4;
   obj->y += obj->dy >> 4;

   if (col < 39 && row < 29)
   {
      //
      //  Use the gradient maps
      //
      RAM_BANK = 1;
      i = x[row*40+col];
      j = y[row*40+col];
   
      obj->dx += i;
      obj->dy += j;
      //if (obj->dx < 16) obj->dx = abs(obj->dx);
      //if (obj->dy < 16) obj->dy = abs(obj->dy);
   }
   else
   {
      //if (obj->dx > SPRITE_X_SCALE(600)) obj->dx = -abs(obj->dx);     
      //if (obj->dy > SPRITE_Y_SCALE(440)) obj->dy = -abs(obj->dy);
   }

   /*   
   
   This was the old way to do something vaguely gravity-like.

   //
   // deltas range -320 to +320.
   //
   //x_delta = (obj->x >> SPRITE_POSITION_FRACTIONAL_BITS) - STAR_X; 
   //y_delta = (obj->y >> SPRITE_POSITION_FRACTIONAL_BITS) - STAR_Y;

   //
   // downshift to pull slower
   //
   //obj->dx -= x_delta >> 5;
   //obj->dy -= y_delta >> 5;
   */

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
   gravity(ASTEROID_SPRITE, &asteroid_def);
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

   shipDef->dx += rotation_vector_x[ theta/10 ] << 2;
   shipDef->dy += rotation_vector_y[ theta/10 ] << 2;
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

void load_gradients()
{
   int i,j;
   int row, col;
   int *x = X_GRADIENT;
   int *y = Y_GRADIENT; 
   char c;

   RAM_BANK = 1;
   cbm_k_setnam("gradient-x.bin");
   cbm_k_setlfs(0,8,0);
   cbm_k_load(LOAD_TO_MAIN_RAM, 0x8000); // bank 1
   cbm_k_setnam("gradient-y.bin");
   cbm_k_setlfs(0,8,0);
   cbm_k_load(LOAD_TO_MAIN_RAM, 0x9000); // bank 1, 2nd half

   for(row=0; row<30; ++row)
   {
      for(col=0; col<40; ++col)
      {
         i = (x[row*40+col]);
         j = (y[row*40+col]);
         gotoxy(col,row);
         if (i >= 0)
            cputc(48+i);
         else
            cputc(64+48+i);
         
         gotoxy(col+40,row);
         if (j >= 0)
            cputc(48+j);
         else
            cputc(64+48+j);
      }
   }

   gotoxy(0,40);
   cprintf("press a key to begin");
   c = cgetc();
}

void main()
{
   unsigned char done = 0;

   cbm_k_bsout(CH_FONT_UPPER); // cbm.h

   // load sprites to vera
   cbm_k_setnam("temp-sprites.bin");
   cbm_k_setlfs(0,8,0);
   cbm_k_load(LOAD_TO_VERA, SPRITE_ADDR_BEGIN);

   load_gradients();

   bgcolor(0); // lets background layer in
   clrscr();

   vera_sprites_enable(1); // cx16.h 
   initSprites();

   while(!done)
   {
      move_ship();
      move_asteroid();
      pause_jiffies(2000);
   }
}
