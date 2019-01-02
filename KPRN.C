/*
 *  Ìa·©   : kprn.c
 *  ¶w   :
 *  ‹¡ÒB   : hantle.prj, nal.prj
 *  i¼a   : 93.03.16
 *   e—e·¡ : Ðe´·”ÐbŠa ¸å¸aÉ·¯¥‰·Ðb‰Á ÐeËi´aŸ¡ Š¥¸œb
 *  ´iŸ±‹i : ·¡ Î‰‹aŸ±· £»‹aŸ±·e ‰·ˆŸi ¶¥Ã¢·a¡ Ðs“¡”a.
 *           µá¡Ða¡e, a•¡ ”aŸe £»‹aŸ±· •¡¶‘·i  g·¡ ¤h´v·a“¡Œa¶a.
 *           ˜aœa¬á, ·¡ £»‹aŸ±·i ¤h·a¯¥ ¦…·e ”aŸe•A ¬a¶wÐa­a•¡ –A»¡ e
 *           ¬¡  e—e £»‹aŸ±•¡ ¸i ¸÷Ÿ¡Ða­a¬á ‰·ˆÐ º¯¡‹¡ ¤aœs“¡”a.
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <bios.h>
#include "hanlib.h"
#include "ascii.h"
#include "kedit.h"
#include "hanerro2.h"
#include "hanwindw.h"
#include "kprn.h"

int prn_port = 0;   /* 0 = lpt1, 1 = lpt2 ... */
int prn_OK = true;
int formfeed_mode = 0;

static bool isOKprn(void)
{
  int res;

  res = biosprint(2, '*', prn_port) & 0x29;
  prn_OK = (res) ? false : true;
  return prn_OK;
}

static bool lputc(unsigned char c)
{
  int res;

  while( 1 ) {
    if(!isOKprn()) {
      if(retry("ÏaŸ¥Èáˆa Œá¹a ·¶ˆáa, µe‰i–A´á·¶»¡ ´g¯s“¡”a", 0) == 0)
        break;
      else
        return false;
    }
    else
      break;
  }
  while( 1 ) {
    res = biosprint(2, c, prn_port) & 0x29;
    if(res) {
      while( 1 ) {
        if(retry("·¥­áÐi ®ˆa ´ô´á¶a", 0) == 0)
          break;
        else
          return false;
      }
    }
    if(res == 0) {  /* ¬÷‰· */
      biosprint(0, c, prn_port);
      return true;
    }
  }
  return false;
}

/* ************
static bool lputs(unsigned char *s)
{
  if(!prn_OK)
    return false;
  while( 1 ) {
    if(!isOKprn()) {
      if(retry("ÏaŸ¥Èáˆa Œá¹a ·¶ˆáa, µe‰i–A´á·¶»¡ ´g¯s“¡”a", 0) == 0)
        break;
      else
        return false;
    }
    else
      break;
  }
  while(*s) {
    if(!lputc(*s++))
      return false;
  }
  return true;
}


static void lprintf(va_list arg_list, ...)
{
  va_list arg_ptr;
  char *format;
  char prtbuf[300];

  va_start(arg_ptr, arg_list);
  format = arg_list;
  vsprintf(prtbuf, format, arg_ptr);
  lputs(prtbuf);
}
************* */

bool data_print(unsigned char *start, unsigned char *end)
{
  char msg[80];
  int width, line_cnt, line_no;

  if(!prn_OK)
    return false;
  while( 1 ) {
    if(!isOKprn()) {
      if(retry("ÏaŸ¥Èáˆa Œá¹a ·¶ˆáa, µe‰i–A´á·¶»¡ ´g¯s“¡”a", 0) == 0)
        break;
      else
        return false;
    }
    else
      break;
  }
  sprintf(msg, " %s ·¥­á º— ... ", strupr(work_file));
  width = (strlen(msg)+4 > 10) ? strlen(msg)+4 : 10;
  if(!wopen(hgetmaxax()/2-strlen(msg)/2-1, hgetmaxay()/2-1, width, 3))
    return 0;
  wtitle(msg);
  line_cnt = line_count(start, end);
  line_no = 0;
  while(*start && start < end) {
    if(*start == CR && *(start+1) == LF)
      line_no++;
    sprintf(msg, "%3.0f Ú5", (float)100/line_cnt*line_no);
    wcenter(2, msg);
    if(!lputc(*start++)) {
      wclose();
      return false;
    }
  }
  if(formfeed_mode)
    lputc(FF);
  wait_getkey(2);
  wclose();
  return true;
}

/*
 *  Ìa·© kprn.c · {
 */
