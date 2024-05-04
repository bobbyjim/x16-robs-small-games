#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <cx16.h>
#include <6502.h>

#include "universal.h"


#define     ADDR_VERSION      ((unsigned char*)0xff80)
#define     REVERSE_ON      0x12
#define     REVERSE_OFF     0x92

#define     CH_DOWN         17
#define     CH_LEFT         157
#define     CH_REVERSE_ON   18
#define     CH_REVERSE_OFF  146
#define     CH_FILL_ANGLE   169

void setPetFont()
{
   struct regs fontregs;
   fontregs.a = 4; // PET-like
   fontregs.pc = 0xff62;
   _sys(&fontregs);
}

char isProto()
{
    return ADDR_VERSION[0] > 0xf8? 'p' : ' ';
}

unsigned char getVersion()
{
    return isProto()? 0x100 - ADDR_VERSION[0] : ADDR_VERSION[0];
}

char petsciiLogo[] = {
    CH_LIGHTBLUE, CH_REVERSE_ON, CH_FILL_ANGLE, ' ', ' ', CH_LEFT, CH_LEFT, CH_LEFT, CH_DOWN,
    ' ', CH_REVERSE_OFF, ' ', ' ', CH_REVERSE_ON, ' ', CH_REVERSE_OFF, CH_FILL_ANGLE, CH_LEFT, CH_LEFT, CH_LEFT, CH_LEFT, CH_LEFT, CH_DOWN,
    CH_REVERSE_ON, ' ', CH_REVERSE_OFF, ' ', ' ', CH_RED, CH_REVERSE_ON, 163, 223, CH_LEFT, CH_LEFT, CH_LEFT, CH_LEFT, CH_LEFT, CH_DOWN,
    CH_LIGHTBLUE, REVERSE_OFF, 223, REVERSE_ON, ' ', ' ', REVERSE_OFF
};

void logo(unsigned char x, unsigned char y)
{
    int i;
    gotoxy(x,y);
    for(i=0; i<45; ++i)
          cbm_k_bsout(petsciiLogo[i]);
}

void login(char *name, char *date)
{
   unsigned char version = getVersion();
   char proto = isProto();

   bgcolor(COLOR_BLACK);
   clrscr();
   logo(2,8);

   textcolor(5);

   gotoxy(0,2);
   cprintf("  r%03d%c                             %s                               ",version,proto, name);
   cputsxy(2,4,  "****************************************************************************");
   cputsxy(9,7,  "not a warning!  all are welcome to access this system; there are no");
   cputsxy(9,9, "penalties, nor any system security policy,  and no applicable state");
   cputsxy(9,11, "or federal laws. sessions & e-mail are not monitored, as the system"); 
   cputsxy(9,13, "has neither.     * * * note: system will be down if it is shut off.");
   cputsxy(2,16, "****************************************************************************");
   gotoxy(2,18);
   cprintf("                        commander x16 | %s", date);

   textcolor(10);
   cputsxy(2,25,"                    * * *   press a key to login   * * * ");
   cgetc();
   cclearxy(2,25,70);
   textcolor(5);

   gotoxy(0,20);
}