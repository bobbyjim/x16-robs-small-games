#ifndef _common_h_
#define _common_h_
/*

    PETSCII Invaders: a space invaders clone
    Copyright (C) 2022 Robert Eaglestone

    This file is part of PETSCII Invaders.

    PETSCII Invaders is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PETSCII Invaders is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with PETSCII Invaders.  If not, see <https://www.gnu.org/licenses/>.

*/

#define  PETSCII_BALL      209
#define  PETSCII_DIAMOND   218
#define  PETSCII_SAIL1     223
#define  PETSCII_SAIL2     169
#define  PETSCII_SQUARE    172

void common_loadCharacterSet(char* name);

void setBank(int bank);
int  getBank();
       
void redline();
void greenline();
void titleLine();
void statusLine();

void toDefaultColor();
void loadFile(char* name, unsigned address);
void loadFileToBank(char* name, char bankNum, unsigned address);
char pressReturnAndClear();

#endif
