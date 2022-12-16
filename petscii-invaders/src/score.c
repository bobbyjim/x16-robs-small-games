


#include <conio.h>

#include "score.h"

int score = 0;
int high  = 0;
int lives = 3;

//
//  draws a mini cannon in the "lives" section
//
void drawMiniCannon(char x)
{
    cputcxy(x+1,1,172);
    revers(1);
    cputc(188);
    revers(0);
}

void showScore()
{
    char x;
    
    textcolor(COLOR_GREEN);
    gotoxy(9,1);
    cprintf("%03d00", score);
    gotox(30);
    cprintf("%03d00", high);

    for(x=0; x<lives; ++x)
    {
       drawMiniCannon(75 - x * 3);
    }
}

void addPoints(int points)
{
    score += points;
    gotoxy(9,1);
    textcolor(COLOR_GREEN);
    cprintf("%03d00", score);
}

void addLife()
{
    ++lives;
}