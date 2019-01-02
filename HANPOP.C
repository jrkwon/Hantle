#include <string.h>
#include <ctype.h>
#include "hanlib.h"
#include "ascii.h"
#include "extkey.h"
#include "hanin.h"
#include "hanwindw.h"
#include "hanerro2.h"

static void disp_menu(char *menu[], int cnt);
static int is_in(char *s, char c);
static int get_resp(char *menu[], char *keys, int cnt, int flag);

static void disp_menu(char *menu[], int cnt)
{
  int i;

  for(i = 0; i < cnt; i++)
    hputsxy(1, 1+i, menu[i]);
}

static int is_in(char *s, char c)
{
  int i;

  for(i = 0; *s; i++)
    if(*s++ == c)
      return i;
  return -1;
}

static int get_resp(char *menu[], char *keys, int cnt, int flag)
{
  bool onoff;
  int y = 1, ans, hot;

  onoff = isreverse();

  hgotoxy(1, 1);
  hsetreverse(ON);
  hputsxy(1, y, menu[0]);
  hsetreverse(onoff);
  do {
    ans = getxch();
    hputsxy(1, y, menu[y-1]);
    switch(ans) {
      case '\r' :
        return y-1;
      case DOWNARROW :
      case ' ' :
        if(y < cnt) y++;
        else y = 1;
        break;
      case UPARROW :
        if(y > 1) y--;
        else y = cnt;
        break;
      case ESC :
        if(flag)
          return -1;
        else
          break;
    }
    hsetreverse(ON);
    hputsxy(1, y, menu[y-1]);
    hsetreverse(onoff);
    hot = is_in(keys, tolower(ans));
    if(hot != -1)
      return hot;
  } while( 1);
}

/*
 *  x, y  : •©Â‰¡A“A· ¶E½¢ ¶á ¹ÁÎa (¹A¡¢Ða‰¡ ÉA–Ÿ¡ ¨‰¡)
 *  cnt   : ¡A“A ˆ•®
 *  menu  : ¡A“A· ¢…¸aµi Í¡·¥Èá
 *  title : ¹A¡¢
 *  keys  : »¡Ÿq‹©
 *  flag  : ESC¡ {‰ ® ·¶”a(1), ´ô”a(0)..
 */

int popup(int x, int y, int cnt, char *menu[], char *title, char *keys, int flag)
{
  int len, i, choice;

  len = 0;
  for(i = 0; i < cnt; i++)
    if(strlen(menu[i]) > len)
      len = strlen(menu[i]);
  if(!wopen(x, y, len, cnt)) {
    memerror();
    return -1;
  }
  wtitle(title);
  disp_menu(menu, cnt);
  choice = get_resp(menu, keys, cnt, flag);
  wclose();
  return choice;
}

