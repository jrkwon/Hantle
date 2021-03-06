/*
 *  �a���a��  : �e�i�a���a�១
 *  ���� ���q : HANWINDW.C
 *  �e�e ��   : �� �� �b
 *  �i�a      : 1992.  6. 24
 *  ���� ���w : �w��(WINDOW) �ŝe �q��
 */

#include <stdlib.h>
#include <alloc.h>
#include <stdarg.h>
#include <string.h>
#include "hanout.h"
#include "hancolor.h"
#include "hancoord.h"
#include "hanwindw.h"
#include "hanerro2.h"

#define MAXSTACK   10   /* �e���A �i�� �� ���e �w�� �A�� �� */

typedef struct {
  int left, top, width, height;
  void far *buf;
} windw;

windw stack[MAXSTACK];

static int sp = 0;   /* �w�� �aȂ ͡���� */

/* �a���� �{�e ���a �a�ሁ�i �b */

void _Cdecl eputchs(int left, int top, int width, byte ch)
{
  byte buf[81];
  register i;

  /* ch�� �����ụ ���a�i �e�i�� */

  for(i = 0; i < width; i++)
    buf[i] = ch;
  buf[i] = NULL;

  /* �� �b */

  eputsxy(left, top, buf);
}

/*
 * �A�� �a���� .. ���a�e �锁 ���a
 * clrfalg�a ON���e �A�� �����i ����
 */
void _Cdecl drawbox(int left, int top, int width, int height, int clrflag)
{
  register y;

  eputsxy(left, top, "�");
  eputsxy(left, top+height-1, "�");

  eputchs(left+1, top, width-2, '�');
  eputchs(left+1, top+height-1, width-2, '�');

  eputsxy(left+width-1, top, "�");
  eputsxy(left+width-1, top+height-1, "�");
  for(y = top+1; y < top+height-1; y++) {
    eputsxy(left, y, "�");
    eputsxy(left+width-1, y, "�");
  }
  if(clrflag) hclrscrxy(left+1, top+1, -(width-2), -(height-2));
}

/*
 *  �w�i �����i �w��, wopen()�� wclose()���� �����i ��Ё
 *  hwindow()�q�� ���� �a�w�e�a.
 */
void _Cdecl sethwindow(int left, int top, int right, int bottom)
{
  stack[sp].left = left, stack[sp].top = top;
  stack[sp].width = right-left+1, stack[sp].height = bottom-top+1;
  hwindow(left, top, right, bottom);
}

/*
 * �a�� ����A �w���e �w�i �a ����a�� ���a ���e�a.
 */
void _Cdecl releasehwindow(void)
{
  hwindow(stack[sp].left, stack[sp].top, -stack[sp].width, -stack[sp].height);
}

/*
 *  "�e�a"�� �w �q���� �{�� ���b�a����Ё��
 *  �������� �aЁ���ᴡ�a�e �q��
 */
void _Cdecl inithwindow(char fcolor, char bcolor)
{
  stack[0].left = 1, stack[0].top = 1;
  stack[0].width = hgetmaxax(), stack[0].height = hgetmaxay();
  hsetcolor(fcolor);
  hsetbkcolor(bcolor);
}

/*
 * �w�� �i��,
 * ���� --> 1, ��́ : �A���� ����. --> 0
 * ���a��� ��a�i �a�� �g�a�� ����Ё�� �q
 */
int _Cdecl wopen(int left, int top, int width, int height)
{
  long bufsize;

  if(sp > (MAXSTACK-1))
    return 0;
  hwindow(1, 1, hgetmaxax(), hgetmaxay());
  sp++;
  bufsize = htextsize(left-1, top-1, -(width+2), -(height+2));
  stack[sp].buf  = farmalloc(bufsize);
  if(stack[sp].buf == NULL) {  /* �A���� ���� */
    sp--;
    releasehwindow();
    return 0;
  }
  /* �锁���a�� �១�a����Ё �w�i ���A�� ��� */
  hgettext(left-1, top-1, -(width+2), -(height+2), stack[sp].buf);
  stack[sp].left = left, stack[sp].top = top;
  stack[sp].width = width, stack[sp].height = height;
  drawbox(left-1, top-1, width+2, height+2, ON);
  hwindow(left, top, -width, -height);
  hgotoxy(1, 1);
  return 1;
}

/* �w�� �h�� */

void _Cdecl wclose(void)
{
  if(sp < 1)
    return;
  hwindow(1, 1, hgetmaxax(), hgetmaxay());
  hputtext(stack[sp].left-1, stack[sp].top-1, -(stack[sp].width+2), -(stack[sp].height+2), stack[sp].buf);
  farfree(stack[sp].buf);
  sp--;
  hwindow(stack[sp].left, stack[sp].top, -stack[sp].width, -stack[sp].height);
}

/*
 *  �w���� �w�a �A�����A �A���i �����a.
 */

void _Cdecl wtitle(byte *str)
{
  bool onoff, onoff2;

  onoff = isreverse();
  onoff2 = isunder();
  hsetunder(ON);
  hsetreverse(ON);

  /* �锁���a�A �b */
  eputchs(-(stack[sp].left-1), -(stack[sp].top-1), stack[sp].width+2, ' ');
  hputsxy(-(stack[sp].left+(stack[sp].width+2)/2-strlen(str)/2-1), -(stack[sp].top-1), str);

  /* ���� �wȁ ���� */
  hsetreverse(onoff);
  hsetunder(onoff2);
}

/* �e�� �w�� �� �w�����a�A ���a�i�i �a���A ���i���a �b�e�a. */

void _Cdecl wcenter(int y, byte *str)
{
  hputsxy((stack[sp].width-strlen(str))/2+1, y, str);
}

void _Cdecl wputs(int y, byte *str)
{
  hputsxy(1, y, str);
}

/* HANWINDW.C �� ���� �{ */
