#ifndef _MISSILES_H_
#define _MISSILES_H_


unsigned char trackMissiles(int epoch);
void addInvaderMissile(unsigned char x, unsigned char y);
void fireCannon(unsigned char x);
void maybeBlowSomethingUp(unsigned char target);
unsigned char whatDidMissileHit();



#endif