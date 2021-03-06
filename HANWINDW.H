/*
 *  �a���a��  : �e�� �e�i�a���a�១
 *  �a�� ���q : HANWINDW.H
 *  �e�e ��   : �� �� �b
 *  �i�a      : 1992. 6. 26
 *  �a�� ���w : �w��(WINDOW) �ŝe �១
 */

#ifndef _HANWINDW_H_
#define _HANWINDW_H_

#ifndef _HANOUT_H_
#include "hanout.h"
#endif

#ifndef _TYPES_H_
#include "typedefs.h"
#endif

#if __STDC__
#define _Cdecl
#else
#define _Cdecl cdecl
#endif

void _Cdecl eputchs(int left, int top, int width, byte ch);

void _Cdecl drawbox(int left, int top, int width, int height, int clrflag);

void _Cdecl sethwindow(int left, int top, int width, int height);
void _Cdecl releasehwindow(void);
void _Cdecl inithwindow(char fcolor, char bcolor);
int  _Cdecl wopen(int left, int top, int width, int height);
void _Cdecl wclose(void);

void _Cdecl wtitle(byte *str);
void _Cdecl wcenter(int y, byte *str);
void _Cdecl wputs(int y, byte *str);

#endif

/* HANWINDW.H �� ���� �{ */
