/*
 *  Ìa·©   : khinput.c
 *  ¶w   : 2, 3 ¤é¯¢ Ðe‹i·³b ÀáŸ¡ ¦¦… 
 *  ‹¡ÒB   : hantle.prj, nal.prj
 *  i¼a   : 93.03.16
 *   e—e·¡ : Ðe´·”ÐbŠa ¸å¸aÉ·¯¥‰·Ðb‰Á ÐeËi´aŸ¡ Š¥¸œb
 *  ´iŸ±‹i : ·¡ Î‰‹aŸ±· £»‹aŸ±·e ‰·ˆŸi ¶¥Ã¢·a¡ Ðs“¡”a.
 *           µá¡Ða¡e, a•¡ ”aŸe £»‹aŸ±· •¡¶‘·i  g·¡ ¤h´v·a“¡Œa¶a.
 *           ˜aœa¬á, ·¡ £»‹aŸ±·i ¤h·a¯¥ ¦…·e ”aŸe•A ¬a¶wÐa­a•¡ –A»¡ e
 *           ¬¡  e—e £»‹aŸ±•¡ ¸i ¸÷Ÿ¡Ða­a¬á ‰·ˆÐ º¯¡‹¡ ¤aœs“¡”a.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mem.h>
#include <bios.h>

#include "hanlib.h"
#include "hanout.h"
#include "typedefs.h"
#include "hinput.h"
#include "hkeydef.h"

#define TRUE   1
#define FALSE  0

typedef struct {
  int curhanst;
  int key;
  int charcode;
} INPSTACK;

INPSTACK InpStack[10];         /* µ¡É¡ aÈa ·³b ¯aÈ‚          */
int OutStack[5];               /* µ¡É¡ aÈa Â‰b ¯aÈ‚          */
int InpSP = 0;                 /* µ¡É¡ aÈa ·³b ¯aÈ‚ Í¡·¥Èá */
int OutSP = 0;                 /* µ¡É¡ aÈa Â‰b ¯aÈ‚ Í¡·¥Èá */
int CurHanState = 0;           /* µ¡É¡ aÈa· Ñe¸ ¬wÈ      */
int CursorCode;

int HanAutomata2(int key);
int HanAutomata3(int key);

/*
** Ñe¸ ¬é¸÷–E Ðe‹i ¸aÌe ¹·ŸAµA ˜aœa Ð”wÐa“e µ¡É¡ aÈaŸi ¦‰œá º…”a.
*/
int HanAutomata(int key)
{
  return (HanKbdKind == HANKBD2) ? HanAutomata2(key) : HanAutomata3(key);
}

#define BLINKCOUNT 7                   /* Äá¬á Œq¤b·± º‹¡           */

static int CursorState = 0;            /* Ñe¸· Äá¬á ¬wÈ                         */
                                       /*     0 = Îa¯¡–A´á ·¶»¡ ´g·q               */
                                       /*     1 = µw¢… Ça‹¡ ( 8 x 16)                      */
                                       /*     2 = Ðe‹i Ça‹¡ (16 x 16)                      */
static int CursorBlinkTime;            /* Äá¬áˆa ¤e¸å–A´á´¡ Ði µ¸÷ ¯¡ˆb      */
static bool ReverseType;      /* Ñe¸ Â‰bÀw· Â‰b­¢¬÷; ¤e¸å·¡¡e Àq */
static bool BlinkOnOff;

/*
** Äá¬áŸi ¬¡ Îa¯¡Ðe”a.  ·¡£¡ Îa¯¡–A´á ·¶·a¡e Œq¤b·© ¯¡ˆe·¡
** –A´ö“e»¡ ˆñ¬aÐ¬á Äá¬á ¡¡´··i ¤e¸å¯¡Ç¥”a.
 ! ¹ÁÎa“e Ï©¶a ´ô·i ˆõ ˆ{‹¡•¡ Ðe•A...
   µá¡Ða¡e ·¡ Äá¬á Îa¯¡Ðq®“e ´å¹Aa Ñe¸ ¶áÃ¡µA¬á ¦‰Ÿ© ˆõ ˆ{·e•A..
*/
void DisplayCursor(int x, int y, int bch)
{
  int i;
  hgotoxy(x+1, y+1);
  if (CursorState == 0) {     /* Îa¯¡–A´á ·¶»¡ ´g·a¡e ¬¡ Îa¯¡ */
    if (CurHanState) {
      CursorCode = InpStack[InpSP-1].charcode;
      CursorState = 2;
    } else {
      if (bch & 0x8000) {
        CursorCode = bch;
        CursorState = 2;
      } else {
        CursorCode = (((bch == '\r') || (bch == '\0') || (bch == '\t')) ? ' ' : bch);
        CursorState = 1;
      }
    }
    CursorBlinkTime = (int)biostime(0,0);
  }
  /*
  ** Œq¤b·± ÀáŸ¡
  */
  if ((i =(int) biostime(0,0)) >= CursorBlinkTime) {
    CursorBlinkTime = i + BLINKCOUNT;
    if(CursorState) {
      BlinkOnOff = !BlinkOnOff;
      hsetreverse(BlinkOnOff);
    }
    if(CursorState == 2) {
      _hputch((CursorCode>>8) & 0xFF, CursorCode & 0xFF);
    }
    else {
      _eputch(CursorCode); /* , hwherex(), hwherey());
      hgotoxy(hwherex()+1, hwherey());                */
    }
  }
}

/*
** Äá¬áŸi »¡¶…”a.
*/
void EraseCursor(int x, int y)
{
  if (CursorState == 0)
    return;
  if(CursorState) {
    hgotoxy(x+1, y+1);
    hsetreverse(ReverseType);
    if(CursorState == 2)
      _hputch((CursorCode>>8) & 0xFF, CursorCode & 0xFF);
    else {
      _eputch(CursorCode); /*, hwherex(), hwherey());
      hgotoxy(hwherex()+1, hwherey()); */
    }
  }
  CursorState = 0;
}

/********************************************************************
        Ç¡¥¡—a¡¦Èá Ðeˆ· ¢…¸aŸi ·³b¤h“e”a.
********************************************************************/

int GetHanCh(int x, int y, int *ch, int bch)

/*--------------------------------------------------------------------

·¥  ®: x           Äá¬áŸi Îa¯¡Ði ¶áÃ¡· x ¹ÁÎa (¤a·¡Ëa ”e¶á)
               y           Äá¬áŸi Îa¯¡Ði ¶áÃ¡· y ¹ÁÎa (•¡Ëa ”e¶á)
               ch       ·³b–E ¢…¸a· Å¡—aŸi ¸á¸wÐi ¡A¡¡Ÿ¡ º­¡
               bch      Äá¬áŸi Îa¯¡Ði ¶áÃ¡µA Ñe¸ Îa¯¡–E ¢…¸a Å¡—a

¤eÑÅˆt: ·³b–E ‰i‰Á
               HR_ASC     ¢…¸a Å¡—a 1-127¤å· ·©¤e ´a¯aÇ¡ ¢…¸a ™¡“e
                                ¢…¸a Å¡—a 256 ·¡¬w· Ëb® Ç¡ˆa ·³b–Q.
               HR_HANST   Ðe‹i ·³b·¡ ¯¡¸b–A´ö·q·i aÈa…”a.
               HR_HANIN   Ðe‹i ·³b·¡ ‰­¢–A‰¡ ·¶·q·i aÈa…”a.
               HR_HANEND  Ðe‹i ·³b·¡ {v·q·i aÈa…”a.
               HR_HANBS   Ðe‹i ·³b º—µA <BS>ˆa ·³b–A´ö·q·i aÈa…”a.
                                ‹aœáa Ðe‹i·e ‰­¢ ¹¡Ðs º—·¡”a.
               HR_NOHAN   Ðe‹i ·³bº—µA <BS>¡ ¡¡– »¡¶¡»¡‰¡ ´a¢œå
                                ¶w•¡ q»¡ ´g´v·q·i aÈa…”a.
--------------------------------------------------------------------*/
{
  int i, key, result, ar;
  static int hanstart = FALSE;
  int noctrlcode;

  noctrlcode = _noctrlcode;
  _noctrlcode = true;

  BlinkOnOff = ReverseType = isreverse();
  for (;;) {
    if (hanstart) {
      hanstart = FALSE;
      result = HR_HANST;
      *ch = InpStack[InpSP-1].charcode;
      break;      /* for žÏaŸi ¨a¹a aˆe”a. */
      /*
      ** hanstart ¥e®“e ¤a¡¸åµA Ðe‹i Ðe ‹i¸aˆa µÅ¬÷–A´ö‰¡,
      ** µÅ¬÷–A‰¡ q·e ¸a¡¡Ÿi ˆa»¡‰¡ ‰­¢ ”a·q ‹i¸aŸi ¹¡ÐsÐ
      ** aˆi Ï©¶aˆa ·¶·i ˜ TRUE¡ ¬é¸÷–E”a.  ·¡ ¥e®“e
      ** ·¡œáÐe ‰w¶µA HR_HANSTŸi •©a º‰ ¡¢¸â·a¡  ae–A´ö”a.
      */
    }
    DisplayCursor(x, y, bch);
    if (!InKeyBuf()) {
      continue;   /* for žÏaŸi ¤e¥¢ (Ç¡¥¡—a ·³b·i ‹¡”aŸ±) */
    }
    EraseCursor(x, y);
    key = *ch = GetKey(TRUE);
    if (key == '\b') {
      if (InpSP) {
        InpSP--;
        if (InpSP) {
          i = InpSP - 1;
          CurHanState = InpStack[i].curhanst;
          *ch = InpStack[i].charcode;
          result = HR_HANBS;
        } else {
          result = HR_NOHAN;
          CurHanState = 0;
        }
      } else {
        result = HR_ASC;
      }
    } else if (key >= 128 && key <= 255) {
      i = CurHanState;
      ar = HanAutomata(key);
      *ch = InpStack[InpSP-1].charcode;
      if (ar) {               /* Ðe‹i ·³b· { */
        CurHanState = 0;
        InpSP = 0;
        result = HR_HANEND;
        if (OutSP) {
          hanstart = TRUE;
          while (OutSP) HanAutomata(OutStack[--OutSP]);
        }
      } else if (i == 0) {
        /*
        ** ¤a¡ ¸å· µ¡É¡ aÈa ¬wÈˆa 0·¡´ö‰¡, ¤w‹q Ðe‹i
        ** ·³b·¡ —i´á µÖ”a¡e ¬¡¶… Ðe‹i· ¯¡¸b·¡”a.
        */
        result = HR_HANST;
      } else {
        result = HR_HANIN;
      }
    } else if (CurHanState) {
      /*
      ** Ñe¸ Ðe‹i·i ¹¡Ðsº—·¥ ¬wÈµA¬á Ðe‹i ¸a¡¡ ·¡¶A· Ç¡ˆa
      ** ·³b–A´ö”a¡e »¡‹qŒa»¡ ¹¡Ðs–E Ðe‹i·i µÅ¬÷–E ˆõ·a¡ Â‰bÐa‰¡,
      ** ¤w‹q ·³b–E Ç¡“e ¤áÌáµA ý´á ”a·q·a¡ ÀáŸ¡–A•¡¢ ¬é¸÷.
      */
      PutKey(key);
      *ch = InpStack[InpSP-1].charcode;
      result = HR_HANEND;
      CurHanState = InpSP = 0;
    } else {
      result = HR_ASC;
    }
    break;  /* for žÏaŸi ¤õ´áq */
  }
  _noctrlcode = noctrlcode;
  return result;
}

/*
** Ç¡¥¡—a ¤áÌáŸi »¡¶…”a.   e´¢ Ñe¸ ¹¡Ðsº—·¥ Ðe‹i·¡ ·¶·a¡e
** ‹aˆõŒa»¡ »¡¶…”a.
*/
void EraseKeyBuf(void)
{
  CurHanState = InpSP = OutSP = 0;
  while (InKeyBuf()) GetKey(FALSE);
}

/*
 *  khinput.c
 */