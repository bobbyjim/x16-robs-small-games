#ifndef _INVADER_H_
#define _INVADER_H_

void initInvaders();
void showInvaders(int row);
//void stepInvaders(int epoch);
void updateInvaderPosition(int epoch);
unsigned char attackInvaderAt(int x, int y);

#endif
