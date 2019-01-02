/*
 *  Ïa¡‹aœ‘  : ÐeËiœa·¡§aœáŸ¡
 *  ÑÁ·© ·¡Ÿq : HANERRO2.C
 *   e—e ·¡   : Š¥ ¸ œb
 *  i¼a      : 1992. 6. 26
 *  ÑÁ·© ¶w : µ¡ŸAÀáŸ¡ ÑÁ·©
 */

#include <dos.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "hanout.h"
#include "hancoord.h"
#include "hancolor.h"
#include "hanin.h"
#include "ascii.h"
#include "extkey.h"
#include "hanwindw.h"

#define  MAXERRNO   35

/* •¡¯a· µ¡ŸA ¡¢¢ */
byte *hsys_errlist[] = {
  "µ¡ŸA 0",
  "ËiŸ¥ Ðq®¤åÑ¡Š…¶a",
  "‹aœå ÑÁ·©·¡a —¡BÉ¡Ÿ¡“e ´ô´á¶a",
  "‹aœå ‰w¡(path)“e ´ô“e•A¶a",
  "á¢  g·e ÑÁ·©·i µi´ö´á¶a",
  "Ðáˆa–A»¡ ´g·eˆé¶a",
  "ÑÁ·©¤åÑ¡ˆa Ëiv´á¶a",
  "‹¡´â¸w­¡ Šµb·¡ ÌaŠA–A´ö‘A¶a",
  "‹¡´â¸w­¡ˆa ¦¹¢ÐaŠ…¶a",
  "‹¡´â¸w­¡ Šµbº­¡ˆa Ëiv‘A¶a",
  "ÑÅ‰w(environment)·¡ ¸i¡µ–A´ö´á¶a",
  "Ñw¯¢(format)·¡ ¸i¡µ–A´ö‘A¶a",
  "¸ó‹e¦Ñ¡(access cose)ˆa Ëiv´á¶a",
  "¸aža(data)ˆa ËivŠ…¶a",
  "",
  "‹aœå ¸wÃ¡“e ´ô´á¶a",
  "Ñe¸ —¡BÉ¡Ÿ¡Ÿi »¡¶aÐa”a“¡..",
  "ˆ{·e ¸wÃ¡ˆa ´a“¡‘A¶a",
  "”á ·¡¬w ÑÁ·©·¡ ´ô´á¶a",
  "·¥¸a(argument)ˆa Ëiv´á¶a",
  "·¥¸a ¡¢¢·¡ á¢ ‹©´á¶a",
  "¯©Ð— Ñw¯¢·¡ ¸i¡µ–A´ö´á¶a",
  "¸wÃ¡ µe‰i·¡ ¡µv´á¶a",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "‰¬eµA¬á µwµb·¡ ¸i¡µ–A´ö´á¶a",
  "‰i‰Áˆa á¢ ÇaŠ…¶a",
  "ÑÁ·©·¡ ·¡£¡ ·¶“e•A¶a"
};

/* Ã¡¡w¸â·¥ µ¡ŸA - critical error - */
byte *crit_errlist[] = {
  "—¡¯aÇaˆa ³a‹¡¤w»¡ –A´á·¶Š…¶a",    /* Write-protect error   */
  "—aœa·¡§a ·¡Ÿq·¡ Ëiv´á¶a",        /* Invalid drive number  */
  "—aœa·¡§a· ¢…·¡ µia·¶‘A¶a",      /* Drive not ready       */
  "¡ww´áˆa Ëiv´á¶a",               /* Invalid command       */
  "CRC µ¡ŸAˆa v“e•A¶a",             /* CRC error             */
  "Š¹¡ ‹©·¡ ¶aŠˆa Ëiv”aŠ¶a",     /* Bad request structure length  */
  "Seek µ¡ŸAµ¶a",                   /* Seek error            */
  "Îaº… Ñw¯¢(format)·¡ ´a“¡Š…¶a",    /* Unknown medium; disk format not recognized */
  "­BÈáŸi ·ª·i® ´ô‘A¶a",            /* Sector not found      */
  "ÏaŸ¥ÈáµA ¹··¡ˆa ´ô´á¶a",          /* Printer out of paper  */
  "³a“e •·´e µ¡ŸAˆa ¬—‰v‘A¶a",       /* Write error           */
  "·ª“e •·´e µ¡ŸAˆa ¬—‰v‘A¶a",       /* Read error            */
  "¯¡¯aÉQ µ¡ŸA-·©¤e¸â µ¡ŸAµ¶a"      /* General, nonspecific error  */
};

/* ¬a¶w¸a µ¡ŸA ¡¢¢ */
byte *user_errlist[] = {
  "¡A¡¡Ÿ¡ˆa ¦¹¢Ða‘A¶a",
};

/* »¡¸w–E ¶áÃ¡µA ‘A¡¡Ÿi ‹aŸ¥”a·q ¢…¸aµi·i Â‰bÐa‰¡ Ç¡ ·³b·i ‹¡”aŸ¥”a. */

int _Cdecl disp_msg(int x, int y, byte *msg)
{
  bool onoff;
  int ret, width;

  width = (strlen(msg)+4 > 10) ? strlen(msg)+4 : 10;
  if( !wopen(x, y, width, 3))
    return 0;
  wtitle(msg);
  onoff = isreverse();
  hsetreverse(ON);
  wcenter(2, " ÑÂ·¥ ");
  hsetreverse(onoff);
  ret = getxch();
  wclose();
  return ret;
}

/* ÑÁ¡e ˆa¶…•A ¡A¯¡»¡Ÿi Â‰bÐa‰¡ Ç¡ ·³b·i ‹¡”aŸ¥”a. */

int _Cdecl disp_cmsg(byte *msg)
{
  int ret, width;
  bool onoff;

  width = (strlen(msg)+4 > 10) ? strlen(msg)+4 : 10;
  if( !wopen(hgetmaxax()/2-strlen(msg)/2-1, hgetmaxay()/2-1, width, 3))
    return 0;
  wtitle(msg);
  onoff = isreverse();
  hsetreverse(ON);
  wcenter(2, " ÑÂ·¥ ");
  ret = getxch();
  hsetreverse(onoff);
  wclose();
  return ret;
}

/*
 * format·i Àq¹¡Ðaµa bufferµA¬á Ï©—aŠ¦…¸aŸi ¹AˆáÐe Ò dataµA ý‰¡, ™¡
 * dataŸi –A•©Ÿ¥”a.
 */

byte *pickoutdata(byte *buffer, byte *format, byte *data)
{
  byte picture[] = "#CcAa9HhEeKkLlYy!xX*";
  register i, j, len;

  len = strlen(format);

  for(i = 0, j = 0; i < len; i++) {
    if(format[i] & 0x80) {
      data[j++] = buffer[i++];
      data[j++] = buffer[i];
    }
    else {
      if(strchr(picture, format[i]))
        data[j++] = buffer[i];
    }
  }
  data[j] = '\0';
  return data;
}

/*
 * format·i Àq¹¡Ðaµa bufferµA Ï©—aŠ¦…¸aŸi ¡¶¡ý·e Ò dataµA ˆt·i º…”a.
 */

byte *putindata(byte *buffer, byte *format, byte *data)
{
  byte picture[] = "#CcAa9HhEeKkLlYy!xX*";
  register i, j, k, len;

  len = strlen(format);

  for(i = 0, j = 0, k = 0; i < len; i++) {
    if(format[i] & 0x80) {
      data[j++] = format[i++];
      data[j++] = format[i];
    }
    else {
      if(strchr(picture, format[i]))
        data[j++] = buffer[k++];
      else
        data[j++] = format[i];
    }
  }
  return data;
}

/*
 *  Àw¢… µi´á¬á ¢…¸aµi ·³b·i ¤h“e”a.
 *  maxlen :  ·³bÐi ® ·¶“e ÂA” ‹©·¡
 *  title  :  ·³b Àw· ¹A¡¢
 *  str    :  ·³b¤h·i ¢…¸aµi· Í¡·¥Èá
 */
bool _Cdecl win_hgets(int maxlen, byte *title, byte *str)
{
  int width, ans, cnt;
  int allowcanceldelline, restoreline, cuttail, allowfirstdelline;

  if((width = strlen(title)) < maxlen)
    width = maxlen;
  if( !wopen(hgetmaxax()/2-width/2-1, hgetmaxay()/2-1, width+4, 3))
    return 0;
  wtitle(title);
  hgotoxy(2, 2);
  allowcanceldelline = _allowcanceldelline, restoreline = _restoreline;
  cuttail = _cuttail, allowfirstdelline = _allowfirstdelline;

  _cuttail = false, _allowcanceldelline = _restoreline = true;

  ans = hgetln(str, maxlen);
  cnt = strlen(str);
  while(cnt < maxlen)
    str[cnt++] = ' ';
  str[cnt] = '\0';
  _allowcanceldelline = allowcanceldelline, _restoreline = restoreline;
  _cuttail = cuttail, _allowfirstdelline = allowfirstdelline;
  wclose();
  return (ans == '\r') ? true : false;
}

/*
 *  Àw¢… µi´á¬á ¸ažaŸi ·³b·i ¤h“e”a. - ·³b´·¯¢ »¡¸÷ˆa“w
 *  maxlen :  ·³bÐi ® ·¶“e ÂA” ‹©·¡
 *  title  :  ·³b Àw· ¹A¡¢
 *  mask   :  ·³b´·¯¢
 *  str    :  ·³b¤h·i ¢…¸aµi· Í¡·¥Èá
 */

bool _Cdecl win_hgetdata(int maxlen, byte *title, byte *mask, byte *str)
{
  int width, ans, cnt;
  int allowcanceldelline, restoreline, cuttail, allowfirstdelline;
  bool onoff;
  char buf[80];

  if((width = strlen(title)) < maxlen)
    width = maxlen;
  if( !wopen(hgetmaxax()/2-width/2-1, hgetmaxay()/2-1, width+4, 3))
    return 0;
  wtitle(title);
  hgotoxy(2, 2);
  allowcanceldelline = _allowcanceldelline, restoreline = _restoreline;
  cuttail = _cuttail, allowfirstdelline = _allowfirstdelline;

  _cuttail = false, _allowcanceldelline = _restoreline = true;
  putindata(str, mask, buf);    /* ¢…¸aµiµA ·³b´·¯¢·i ¡¶¡ ý“e”a */
  onoff = isreverse();
  hsetreverse(ON);
  ans = hgetdata(buf, mask);    /* ¸ažaŸi ·³b¤h“e”a. */
  hsetreverse(onoff);
  pickoutdata(buf, mask, str);  /* ·³b¤h·e ¢…¸aµiµA¬á ·³b´·¯¢·i ¹AˆáÐe”a */
  cnt = strlen(str);
  while(cnt < maxlen)           /* a á»¡ ¦¦…·e ‰·¤‚·a¡ À¶¡ý“e”a */
    str[cnt++] = ' ';
  str[cnt] = '\0';
  _allowcanceldelline = allowcanceldelline, _restoreline = restoreline;
  _cuttail = cuttail, _allowfirstdelline = allowfirstdelline;
  wclose();
  return (ans == '\r') ? true : false;
}

/*
 * secÁ¡•·´e Ç¡·³b·¡ ´ô·a¡e ESCˆa ’‰Ÿ¥ ˆõ·a¡ ˆeºÐa‰¡
 * ‹a ·¡¸åµA Ç¡·³b·¡ ·¶”a¡e »b¯¡ {…”a.
 */

int _Cdecl wait_getkey(int sec)
{
  time_t time1, time2;
  double diff;
  int ans, isquit = 0;

  time(&time1);
  do {
    diff = difftime( time(&time2), time1 );
    if((int)diff >= sec)
      isquit = true, ans = ESC;
    else {
      ans = getkey();
      if(ans)
        isquit = true;
    }
  } while(!isquit);
  return ans;
}

/* ÑÁ¡e ˆa¶…•A ¡A¯¡»¡Ÿi Â‰bÐa‰¡ Ç¡ ·³b·i secÁ¡ eÇq e ‹¡”aŸ¥”a. */

int _Cdecl disp_cmsg_sec(byte *msg, int sec)
{
  int ret, width;
  bool onoff;

  width = (strlen(msg)+4 > 10) ? strlen(msg)+4 : 10;
  if( !wopen(hgetmaxax()/2-strlen(msg)/2-1, hgetmaxay()/2-1, width, 3))
    return 0;
  wtitle(msg);
  onoff = isreverse();
  hsetreverse(ON);
  wcenter(2, " ÑÂ·¥ ");
  ret = wait_getkey(sec);
  hsetreverse(onoff);
  wclose();
  return ret;
}

/*  ‘A¡¡Ÿi ‹aŸ¡‰¡ µ¡ŸA ¸÷¥¡Ÿi Â‰bÐe Ò ¸aÌe ·³b·i ‹¡”aŸ¥”a. */

void _Cdecl disperror(byte *msg)
{
  int color, width;

  width = (strlen(msg)+4 > 26) ? strlen(msg)+4 : 26;
  if( !wopen(hgetmaxax()/2-strlen(msg)/2-1, hgetmaxay()/2-1, width, 3))
    return;
  wtitle(msg);
  color = hgetcolor();
  hsetcolor(HRED);
  wcenter(2, "´a¢ˆáa ’Ÿa­A¶a");
  getxch();
  hsetcolor(color);
  wclose();
}

/*  ¡A¡¡Ÿ¡ ¦¹¢ Îa¯¡..  ¸aº ³aµa¬á ˜a¡ Ðq®¡  e—q
 *  ¬a¶w¸aˆa Ïa¡‹aœ‘ Á¡‹¡ÑÁÐi˜
 *  ¸â´á•¡ ·¡ µ¡ŸA¸÷¥¡ Îa¯¡Ÿi ¶áÐe ¡A¡¡Ÿ¡ ‰·ˆe·¡ ·¶“e»¡
 *  ˆñ¬aÐa“e ˆõ·¡ ¹½”a.
 */

void _Cdecl memerror(void)
{
  disperror("‹¡´â¸w­¡ˆa ¦¹¢Ða‘A¶a");
}

/*  ¸åµb¥e® errnoµA  x“e µ¡ŸAŸi ÑÁ¡e º—´wµA Îa¯¡ */

void _Cdecl hperror(void)
{
  if(errno > MAXERRNO) /* µ¡ŸA¤åÑ¡ˆa ¸i¡µ »¡¸÷–A´ö·q */
    return;
  disperror(hsys_errlist[errno]);
}

/*
 * ”a¯¡, ™¡“e ‹a e·i ·³b ¤h“e Ðq®
 * ”a¯¡--> 0, ‹a e--> 1, ESC--> 2
 * ".. ÐiŒa¶a?", Á¡‹¡¶áÃ¡
 */

int _Cdecl retry(byte *str, int initpos)
{
  int pos, key, ret, tbl[] = { 11, 20, 29 };
  bool onoff, isquit = 0;
  char *msg2[] = { " ”a¯¡ ", " ‹a e ", " Âá­¡ " };
  char msg[] =  " ”a¯¡     ‹a e     Âá­¡ ";

  pos = initpos;
  if((pos < 0) || (pos > 2))  /* Á¡‹¡¶áÃ¡ˆa ¸i¡µ »¡¸÷–A¡e */
    pos = 0;

  if( !wopen(hgetmaxax()/2-22, hgetmaxay()/2-1, 44, 3))
    return 0;
  onoff = isreverse();   /* ¤e¸å ­¢¬÷ ¥¡‰Å */
  wtitle(str);
  hputsxy(11, 2, msg);
  do {
    hsetreverse(ON);
    hputsxy(tbl[pos], 2, msg2[pos]);
    hsetreverse(OFF);
    key = getxch();
    switch(key) {
      case LEFTARROW :
        hputsxy(tbl[pos], 2, msg2[pos]);
        if(pos > 0)
          pos--;
        else
          pos = 2;
        hsetreverse(ON);
        hputsxy(tbl[pos], 2, msg2[pos]);
        hsetreverse(OFF);
        break;
      case RIGHTARROW :
      case ' ' :
        hputsxy(tbl[pos], 2, msg2[pos]);
        if(pos < 2)
          pos++;
        else
          pos = 0;
        hsetreverse(ON);
        hputsxy(tbl[pos], 2, msg2[pos]);
        hsetreverse(OFF);
        break;
      case ESC :
        isquit = 1;
        ret = 2;  /* Âá­¡ */
        break;
      case '\r' :
        isquit = 1;
        ret = pos;
        break;
    }
  } while( !isquit);
  hsetreverse(onoff);
  wclose();
  return ret;
}

/*
 * µ, ´a“¡µ¡Ÿi ·³b¤h“e Ðq®
 * µ--> 0, ´a“¡µ¡--> 1, ESC--> 2
 * ".. ÐiŒa¶a?", Á¡‹¡¶áÃ¡
 */
int _Cdecl yesno(byte *str, int initpos)
{
  int pos, key, ret, tbl[] = { 11, 20, 29 };
  bool onoff, isquit = 0;
  char *msg2[] = { "  µ  ", "´a“¡µ¡", " Âá­¡ " };
  char msg[] =  "  µ     ´a“¡µ¡    Âá­¡ ";

  pos = initpos;
  if((pos < 0) || (pos > 2))  /* Á¡‹¡¶áÃ¡ˆa ¸i¡µ »¡¸÷–A¡e */
    pos = 0;

  if( !wopen(hgetmaxax()/2-22, hgetmaxay()/2-1, 44, 3))
    return 0;
  onoff = isreverse();   /* ¤e¸å ­¢¬÷ ¥¡‰Å */
  wtitle(str);
  hputsxy(11, 2, msg);
  do {
    hsetreverse(ON);
    hputsxy(tbl[pos], 2, msg2[pos]);
    hsetreverse(OFF);
    key = getxch();
    switch(key) {
      case LEFTARROW :
        hputsxy(tbl[pos], 2, msg2[pos]);
        if(pos > 0)
          pos--;
        else
          pos = 2;
        hsetreverse(ON);
        hputsxy(tbl[pos], 2, msg2[pos]);
        hsetreverse(OFF);
        break;
      case RIGHTARROW :
      case ' ' :
        hputsxy(tbl[pos], 2, msg2[pos]);
        if(pos < 2)
          pos++;
        else
          pos = 0;
        hsetreverse(ON);
        hputsxy(tbl[pos], 2, msg2[pos]);
        hsetreverse(OFF);
        break;
      case ESC :
        isquit = 1;
        ret = 2;  /* Âá­¡ */
        break;
      case '\r' :
        isquit = 1;
        ret = pos;
        break;
    }
  } while( !isquit);
  hsetreverse(onoff);
  wclose();
  return ret;
}

/*
 * Ã¡¡w¸â µ¡ŸA ¹¡¸÷¸a
 * ”a¯¡--> 1, ‹a e--> 2
 */

int _Cdecl criticalhandler(void)
{
  int error, check;
  byte *str;

  error = _DI;
  if(error < 14)
    str = crit_errlist[error];
  else
    str = "¯¡¯aÉQ µ¡ŸA";
  check = retry(str, 0);
  hardresume(2-check);
  return 1;
}

/*
 * ÄåËa© §aA·¡Ça ¹¡¸÷¸a
 * ¥i ·£¡ ´ô·¡ ‹a· 1·i –A•©Ÿ±
 */
int _Cdecl ctrlbrkhandler(void)
{
  return 1;
}

/*
 * ÄåËa© §aA·¡Ça ¹¡¸÷¸aµÁ, Ã¡¡w¸â µ¡ŸA(Ða—a¶Á´á µ¡ŸA)¹¡¸÷¸a —w¢
 */
void _Cdecl seterrhandler(void)
{
  ctrlbrk(ctrlbrkhandler);
  harderr(criticalhandler);
}

/* HANERRO2.C · ÑÁ·© { */
