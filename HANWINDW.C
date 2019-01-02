/*
 *  œaù°ãaúë  : –eãiúa∑°ßaú·ü°
 *  —¡∑© ∑°üq : HANWINDW.C
 *  †eóe ∑°   : ä• ∏Å úb
 *  êiºa      : 1992.  6. 24
 *  —¡∑© êÅ∂w : ¿w¢Ö(WINDOW) â≈ùe –qÆÅ
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

#define MAXSTACK   10   /* –eå·§ÂµA µiü© ÆÅ ∑∂ìe ¿w∑Å ¬AîÅ ÆÅ */

typedef struct {
  int left, top, width, height;
  void far *buf;
} windw;

windw stack[MAXSTACK];

static int sp = 0;   /* ¿w∑Å Øa»Ç Õ°∑•»· */

/* àaù°ù° à{∑e ¢Ö∏a µaú·àÅüi ¬âùb */

void _Cdecl eputchs(int left, int top, int width, byte ch)
{
  byte buf[81];
  register i;

  /* chù° ∑°ûÅ¥·ª• ¢Ö∏aµi †eóiã° */

  for(i = 0; i < width; i++)
    buf[i] = ch;
  buf[i] = NULL;

  /* ¨Â ¬âùb */

  eputsxy(left, top, buf);
}

/*
 * ëA°° ãaü°ã° .. π¡Œaìe ∏ÈîÅ π¡Œa
 * clrfalgàa ON∑°°e ëA°° êÅ¶Åüi ª°∂ë
 */
void _Cdecl drawbox(int left, int top, int width, int height, int clrflag)
{
  register y;

  eputsxy(left, top, "⁄");
  eputsxy(left, top+height-1, "¿");

  eputchs(left+1, top, width-2, 'ƒ');
  eputchs(left+1, top+height-1, width-2, 'ƒ');

  eputsxy(left+width-1, top, "ø");
  eputsxy(left+width-1, top+height-1, "Ÿ");
  for(y = top+1; y < top+height-1; y++) {
    eputsxy(left, y, "≥");
    eputsxy(left+width-1, y, "≥");
  }
  if(clrflag) hclrscrxy(left+1, top+1, -(width-2), -(height-2));
}

/*
 *  ¿w∑i ª°∏˜–i âw∂Å, wopen()â¡ wclose()µ¡∑Å π°—¡üi ∂·–Å
 *  hwindow()–qÆÅ îÅØ• ¨a∂w–eîa.
 */
void _Cdecl sethwindow(int left, int top, int right, int bottom)
{
  stack[sp].left = left, stack[sp].top = top;
  stack[sp].width = right-left+1, stack[sp].height = bottom-top+1;
  hwindow(left, top, right, bottom);
}

/*
 * §aù° ∑°∏ÂµA ówù¢–e ¿w∑i ãa ∑°∏Â∑aù° ï©ùa ëΩìeîa.
 */
void _Cdecl releasehwindow(void)
{
  hwindow(stack[sp].left, stack[sp].top, -stack[sp].width, -stack[sp].height);
}

/*
 *  "–eúa"∑Å ¿w –qÆÅµ¡ à{∑° ï∑∏b–aã°∂·–Å¨·
 *  ¡°ã°—¡òÅ ¬Åàa–Å∫Å¥·¥°–aìe –qÆÅ
 */
void _Cdecl inithwindow(char fcolor, char bcolor)
{
  stack[0].left = 1, stack[0].top = 1;
  stack[0].width = hgetmaxax(), stack[0].height = hgetmaxay();
  hsetcolor(fcolor);
  hsetbkcolor(bcolor);
}

/*
 * ¿w¢Ö µiã°,
 * ¨˜â∑ --> 1, Ø©ÃÅ : °A°°ü° ¶Åπ¢. --> 0
 * π¡Œa§Ò∂· àÒ¨aüi –aª° ¥g∑aì° π°Ø±–Å¥° –q
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
  if(stack[sp].buf == NULL) {  /* °A°°ü° ¶Åπ¢ */
    sp--;
    releasehwindow();
    return 0;
  }
  /* ∏ÈîÅπ¡Œaù° ¿·ü°–aã°∂·–Å ¿w∑i ∏Â¡Aù° ¨È∏˜ */
  hgettext(left-1, top-1, -(width+2), -(height+2), stack[sp].buf);
  stack[sp].left = left, stack[sp].top = top;
  stack[sp].width = width, stack[sp].height = height;
  drawbox(left-1, top-1, width+2, height+2, ON);
  hwindow(left, top, -width, -height);
  hgotoxy(1, 1);
  return 1;
}

/* ¿w¢Ö îhã° */

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
 *  ¿w¢Ö∑Å ¨w∏a …AñÅü°µA πA°¢∑i ¶õ∑•îa.
 */

void _Cdecl wtitle(byte *str)
{
  bool onoff, onoff2;

  onoff = isreverse();
  onoff2 = isunder();
  hsetunder(ON);
  hsetreverse(ON);

  /* ∏ÈîÅπ¡ŒaµA ¬âùb */
  eputchs(-(stack[sp].left-1), -(stack[sp].top-1), stack[sp].width+2, ' ');
  hputsxy(-(stack[sp].left+(stack[sp].width+2)/2-strlen(str)/2-1), -(stack[sp].top-1), str);

  /* ∂•úÅ ¨w»Å •¢äÅ */
  hsetreverse(onoff);
  hsetunder(onoff2);
}

/* —e∏Å ¿w¢Ö êÅ ¨wîÅπ¡ŒaµA ¢Ö∏aµi∑i àa∂ÖïA ∏˜ùiØ°≈a ¬âùb–eîa. */

void _Cdecl wcenter(int y, byte *str)
{
  hputsxy((stack[sp].width-strlen(str))/2+1, y, str);
}

void _Cdecl wputs(int y, byte *str)
{
  hputsxy(1, y, str);
}

/* HANWINDW.C ∑Å —¡∑© è{ */
