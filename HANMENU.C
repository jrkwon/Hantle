/*
 *  œaù°ãaúë  : –eÀiúa∑°ßaú·ü°
 *  —¡∑© ∑°üq : HANMENU.C
 *  †eóe ∑°   : ä• ∏Å úb
 *  êiºa      : 1992. 8. 16
 *  —¡∑© êÅ∂w : Õi¡aêÅü± ¿aü±Œa â≈ùe —¡∑©
 */

#include <stdio.h>
#include <alloc.h>
#include <ctype.h>
#include <string.h>
#include "hanmenu.h"
#include "hanwindw.h"
#include "hancolor.h"
#include "hanin.h"
#include "ascii.h"
#include "extkey.h"


/*
 *  ¿aü±Œa(menu)ìe àaù°§w–∑, –w°¢(item)∑e ≠Aù° §w–∑
 */
int nmenu;                      /* ¿aü±Œa àïÆÅ */
int themenu, theitem;           /* ¨Â»ÇñE ¿aü±Œaµ¡ –w°¢§Â—° */
static int curmenu;             /* —e∏Å ¿aü±Œa */
static int curitem[MAXMENU];    /* —e∏Å –w°¢ */
static int ispulldown;          /* Õi¡aêÅü±¿aü±Œa(pull-down menu)àa ŒaØ°ñA¥·∑∂êa? */
static char menuflag, itemflag; /* ¿aü±Œaµ¡ –w°¢ §e∏Â∑i ∂·–e œiúÅãa */
static unsigned char menufcolor, menubcolor;

HMENU pmenu[MAXMENU];

static int _Cdecl maxitemlength(int curmenu_num);
void _Cdecl definemenu(char fcolor, char bcolor, int init_menu, int gap);
void _Cdecl drawmenubar(void);
static void _Cdecl highlightmenu(void);
static void _Cdecl highlightitem(void);
/*static bool _Cdecl selectmenu(int menunum);*/
/*static bool _Cdecl selectitem(int itemnum);*/
static void _Cdecl erasemenubox(void);
static bool _Cdecl findshortcut(unsigned int keycode);
static bool _Cdecl drawmenubox(void);
static bool _Cdecl beforemenu(void);
static bool _Cdecl nextmenu(void);
static void _Cdecl beforeitem(void);
static void _Cdecl nextitem(void);
static void _Cdecl firstitem(void);
static void _Cdecl lastitem(void);
int _Cdecl getitem(unsigned int keycode);

/*
 * –e ¿aü±ŒaµA¨· àa∏w ã• –w°¢∑Å ã©∑°üi äÅ–eîa
 */
static int _Cdecl maxitemlength(int curmenu_num)
{
	int i, max, length;

	max = 0;
	for(i = 0; i < pmenu[curmenu_num].nitem; i++)
		if((length = strlen(pmenu[curmenu_num].item[i])) > max)
			max = length;
	return max;
}

/*
 *  Õi¡aêÅü± ¿aü±Œaüi ∏˜∑Å–eîa
 *  fcolor    : ãi∏a¨Ç
 *  bcolor    : §Åâw¨Ç
 *  init_menu : ¡°ã°¨w»Å∑Å °AìA§Â—° (∂EΩ¢µA¨·¶Å»· 0, 1, 2...)
 *  gap       : °AìA¨a∑° àeâb
 */
void _Cdecl definemenu(char fcolor, char bcolor, int init_menu, int gap)
{
  char altkey[] = {
    81, 87, 69, 82, 84, 89, 85, 73, 79, 80, 0, 0, 0, 0,
    65, 83, 68, 70, 71, 72, 74, 75, 76,  0, 0, 0, 0, 0,
    90, 88, 67, 86, 66, 78, 77
  };
	register i, j;
  char tmp_fcolor, tmp_bcolor;

  tmp_fcolor = hgetcolor();
  tmp_bcolor = hgetbkcolor();
  hsetcolor(fcolor);
  hsetbkcolor(bcolor);
  menufcolor = fcolor, menubcolor = bcolor;
  menuflag = itemflag = ON;
  themenu = theitem = -1;   /* ¨Â»ÇñE ¿aü±Œaµ¡ –w°¢§Â—° ¡°ã°—¡ */
	for(i = 0; i < nmenu; i++) {
		pmenu[i].menuable = true;
		for(j = 0; j < pmenu[i].nitem; j++)
			pmenu[i].itemable[j] = (!strcmp(pmenu[i].item[j], "-")) ? false : true;
		pmenu[i].menuwidth = strlen(pmenu[i].menu)+2;
		pmenu[i].itemwidth = maxitemlength(i)+3+2;
    /* ¿aü±Œa ¿wµA¨· Ø©πA ãi∏aàa ∞·ª°ìe ¶Å¶Ö∑Å π¡Œa */
  	pmenu[i].left = 2;
		for(j = 0; j < i; j++)
			pmenu[i].left += pmenu[j].menuwidth+gap;
		pmenu[i].top = 3;
		pmenu[i].width = pmenu[i].itemwidth;
		pmenu[i].height = pmenu[i].nitem;
	} /* for i */
	curmenu = init_menu;
	for(i = 0; i < nmenu; i++)
		curitem[i] = 0; /* °°óe –w°¢§Â—° ¡°ã°—¡ */
	for(i = 0; i < nmenu; i++) {
		for(j = 0; j < pmenu[i].nitem; j++) {
			if(pmenu[i].shortcut[j] == 0) { /* îe¬ÇãiÆA ¥Ù∑q */
				sprintf(pmenu[i].shortcutchar[j], "%c%c", ' ', 0);
      }
      else {  /* îe¬ÇãiÆA ãi∏a †eóiã° */
				if(pmenu[i].shortcut[j] < 27) { /* îe¬ÇãiÆAàa Ctrlπ°–s∑°°e */
					sprintf(pmenu[i].shortcutchar[j], "%c%c%c",
                  '^', pmenu[i].shortcut[j]+64, 0);
				}	else if(pmenu[i].shortcut[j] < 97) { /* îe¬ÇãiÆAàa µw¢Ö∏a */
					sprintf(pmenu[i].shortcutchar[j], "%c%c", pmenu[i].shortcut[j], 0);
				}	else if(pmenu[i].shortcut[j] >= F1 && pmenu[i].shortcut[j] < F10) {
          /* F1 .. F9 åaª° */
					sprintf(pmenu[i].shortcutchar[j], "%c%c%c",
                  'F', ((pmenu[i].shortcut[j]-F1) >> 8)+'1', 0);
        } else if(pmenu[i].shortcut[j] == F10) { /* F10 ∑°°e */
					sprintf(pmenu[i].shortcutchar[j], "%c%c%c", 'F', '0', 0);
        } else if(pmenu[i].shortcut[j] >= CTRL_F1 && pmenu[i].shortcut[j] < CTRL_F10) {
          /* ^F1 .. ^F9 åaª° */
					sprintf(pmenu[i].shortcutchar[j], "%c%c%c%c",
                  '^', 'F', ((pmenu[i].shortcut[j]-CTRL_F1) >> 8) +'1', 0);
        } else if(pmenu[i].shortcut[j] == CTRL_F10) { /* ^F10 ∑°°e */
					sprintf(pmenu[i].shortcutchar[j], "%c%c%c%c", '^', 'F', '0', 0);
        } else if(pmenu[i].shortcut[j] >= ALT_F1 && pmenu[i].shortcut[j] < ALT_F10) {
          /* @F1 .. @F9 åaª° */
					sprintf(pmenu[i].shortcutchar[j], "%c%c%c%c",
                  '@', 'F', ((pmenu[i].shortcut[j]-ALT_F1) >> 8) +'1', 0);
        } else if(pmenu[i].shortcut[j] == ALT_F10) { /* @F10 ∑°°e */
					sprintf(pmenu[i].shortcutchar[j], "%c%c%c%c", '@', 'F', '0', 0);
        } else if(pmenu[i].shortcut[j] >= ALT_1 && pmenu[i].shortcut[j] < ALT_0) {
          /* @1 .. @9 åaª° */
					sprintf(pmenu[i].shortcutchar[j], "%c%c%c",
                  '@', ((pmenu[i].shortcut[j]-ALT_1)>>8) +'1', 0);
        } else if(pmenu[i].shortcut[j] == ALT_0) { /* @0 ∑°°e */
					sprintf(pmenu[i].shortcutchar[j], "%c%c%c", '@', '0', 0);
        } else { /* ALT π°–s ãiÆA */
          sprintf(pmenu[i].shortcutchar[j], "%c%c%c", '@', ' ', 0);
					pmenu[i].shortcutchar[j][1]=altkey[(pmenu[i].shortcut[j]-0x1000)>>8];
				} /* else */
			} /* else */
		} /* for j */
	} /* for i */
  hsetcolor(tmp_fcolor);
  hsetbkcolor(tmp_bcolor);
}

/*
 *  Õi¡aêÅü± ¿aü±Œa∑Å àaù°§w–∑¶Å¶Ö∑i ãaü•îa
 */
void _Cdecl drawmenubar(void)
{
	register i, j;
  bool onoff, onoff2;
  char fcolor, bcolor;

  fcolor = hgetcolor();
  bcolor = hgetbkcolor();
  hsetcolor(menufcolor);
  hsetbkcolor(menubcolor);
  onoff = isreverse(); onoff2 = isunder();
  hsetreverse(ON);     hsetunder(ON);
  eputchs(-1, -1, 80, ' ');
	if(!(pmenu[curmenu].menuable)) /* ¿aü±Œa àaìwµa¶ÅµA òaúa */
		for(i = nmenu-1; i >= 0; i--)  /* ¿aü±Œa ¡°ã°§Â—° π°∏˜ */
			if(pmenu[i].menuable)
				curmenu = i;
	for(i = 0; i < nmenu; i++) /* àb ¿aü±Œa∑Å –w°¢ àaìwµa¶ÅµA òaúa */
		if(!(pmenu[i].itemable[curitem[i]])) /* –w°¢§Â—° ¡°ã°—¡ */
			for(j = pmenu[i].nitem-1; j >= 0; j--)
				if(pmenu[i].itemable[j] == true)
					curitem[i] = j;
	for(i = 0; i < nmenu; i++)
    hprintfxy(-pmenu[i].left, -1, " %s ", pmenu[i].menu);
  hsetreverse(onoff);  hsetunder(onoff2);
  menuflag = OFF;
  hsetcolor(fcolor);
  hsetbkcolor(bcolor);
}

/*
 *  ¿aü±Œa¶Å¶Ö∑i §e∏Â
 */
static void _Cdecl highlightmenu(void)
{
  bool onoff, onoff2;

  onoff = isreverse(); onoff2 = isunder();
  if(menuflag) {
    hsetreverse(ON); hsetunder(ON);
  }
	hprintfxy(-pmenu[curmenu].left, -1, " %s ", pmenu[curmenu].menu);
  if(menuflag) {
    hsetreverse(onoff); hsetunder(onoff2);
  }
  menuflag = !menuflag;
}

/*
 *  –w°¢ ¶Å¶Ö∑i §e∏Â
 */
static void _Cdecl highlightitem(void)
{
  register limit;
  int offset, onoff;

  onoff = isreverse();
  if(itemflag)
    hsetreverse(ON);
	hprintfxy(1, curitem[curmenu]+1, " %s", pmenu[curmenu].item[curitem[curmenu]]);
  offset = strlen(pmenu[curmenu].item[curitem[curmenu]])+1;
  limit = pmenu[curmenu].width-3-offset;
  eputchs(offset+1, curitem[curmenu]+1, limit, ' ');
  hprintfxy(pmenu[curmenu].width-2, curitem[curmenu]+1, "%-3s", pmenu[curmenu].shortcutchar[curitem[curmenu]]);
  if(itemflag)
    hsetreverse(onoff);
  itemflag = !itemflag;
}

/*
 *  §Â—°«°üi íâú·¨· °AìAüi ¨Â»Ç.. ∏aÃe∑i ∑°∂w–aìeïA ¨a∂w–aã° ∂·–Å¨·úaã°•°îaìe
 *  †a∂ÅØaù° °AìAüi ¨Â»Ç–aìe âw∂Åüi ∂·–Å †eói¥· ëΩ¥v∑q
 *  ∂EΩ¢µA¨·¶Å»· 1, 2, 3, ...
 */
/*static bool _Cdecl selectmenu(int menunum)
{
  if(!pmenu[curmenu].menuable || !(menunum >= 0 && menunum <= 9) || !(menunum < nmenu))
    return false;
  erasemenubox();
  curmenu = menunum;
  return drawmenubox() ? true : false;
} */

/*
 *  °AìA∑Å –w°¢∑i ¨Â»Ç∑i –eîa.
 *  ∑° –qÆÅìe †a∂ÅØaù° ¿aü±Œaüi ¨Â»Ç–aìe âw∂Åüi ∂·–Å¨· †eói¥ˆîa
 */
/*static int _Cdecl selectitem(int itemnum)
{
  if(!pmenu[curmenu].itemable[itemnum] || !(itemnum < pmenu[curmenu].nitem))
    return false;
  highlightitem();
  themenu = curmenu;
  theitem = curitem[curmenu] = itemnum;
  highlightitem();
  return true;
} */

/* µiü• –w°¢¨w∏aüi ª°∂Öîa */
static void _Cdecl erasemenubox(void)
{
  highlightitem();
	wclose();
	highlightmenu();
}

/* keycodeàa îe¬Ç∏aÃe∑aù° ¨È∏˜ñA¥· ∑∂ìeª° àÒ¨a */
static bool _Cdecl findshortcut(unsigned int keycode)
{
	register i, j;

  keycode = ((keycode > 256) ? keycode : toupper(keycode));
	for(i = 0; i < nmenu; i++) {
		if(pmenu[i].menuable) {
			for(j = 0; j < pmenu[i].nitem; j++) {
				if(keycode == pmenu[i].shortcut[j] && pmenu[i].itemable[j]) {
					if(ispulldown)
						erasemenubox();
					curmenu = themenu = i;
					curitem[i] = theitem = j;
					return true; /* îe¬ÇãiÆA ¿x¥vîa */
				} /* if keycode */
			} /* for j */
		} /* if */
	} /* for i */
	return false; /* îe¬ÇãiÆA °µ¿x¥vîa */
}

/* –w°¢ ¨w∏aüi ãaü•îa */
static bool _Cdecl drawmenubox(void)
{
	register i;
  int left, top, width, height;
  int limit, offset, onoff;

  left = pmenu[curmenu].left, top = pmenu[curmenu].top;
  width = pmenu[curmenu].width, height = pmenu[curmenu].height;

	highlightmenu();
	if(wopen(left, top, width, height) == 0)
    return false;
	for(i = 0; i < pmenu[curmenu].nitem; i++) {
		if(!strcmp(pmenu[curmenu].item[i], "-")) {
      eputchs(1, i+1, width, 'ƒ');
		} else {
			if(!(pmenu[curmenu].itemable[i])) {
        onoff = isfaint();
        hsetfaint(ON);
			  hprintfxy(1, i+1, " %s", pmenu[curmenu].item[i]);
        offset = strlen(pmenu[curmenu].item[i])+1;
        limit = width-2-offset;
        eputchs(offset+1, i+1, limit, ' ');
			  hprintfxy(width-2, i+1, "%-3s", pmenu[curmenu].shortcutchar[i]);
        hsetfaint(onoff);
			} else {
			  hprintfxy(1, i+1, " %s", pmenu[curmenu].item[i]);
        offset = strlen(pmenu[curmenu].item[i])+1;
        limit = width-2-offset;
        eputchs(offset+1, i+1, limit, ' ');
			  hprintfxy(width-2, i+1, "%-3s", pmenu[curmenu].shortcutchar[i]);
      } /* ¥eΩ¢ else */
		} /* §aå{Ω¢ else */
	} /* for */
	highlightitem();
  return true;
}

static bool _Cdecl beforemenu(void)
{
	erasemenubox();
	do {
		curmenu--;
		if(curmenu < 0)
			curmenu = nmenu-1;
	} while(!pmenu[curmenu].menuable);
	return drawmenubox() ? true : false;
}

static bool _Cdecl nextmenu(void)
{
	erasemenubox();
	do {
		curmenu++;
		if(curmenu >= nmenu)
			curmenu = 0;
	} while(!pmenu[curmenu].menuable);
	return drawmenubox() ? true : false;
}

static void _Cdecl beforeitem(void)
{
	highlightitem();
	do {
		curitem[curmenu]--;
		if(curitem[curmenu] < 0)
			curitem[curmenu] = pmenu[curmenu].nitem-1;
	} while(!pmenu[curmenu].itemable[curitem[curmenu]]);
	highlightitem();
}

static void _Cdecl nextitem(void)
{
	highlightitem();
	do {
		curitem[curmenu]++;
		if(curitem[curmenu] >= pmenu[curmenu].nitem)
			curitem[curmenu] = 0;
	} while(!pmenu[curmenu].itemable[curitem[curmenu]]);
	highlightitem();
}

static void _Cdecl firstitem(void)
{
	register i;

	highlightitem();
	i = 0;
	while(!pmenu[curmenu].itemable[i])
		i++;
	curitem[curmenu] = i;
	highlightitem();
}

static void _Cdecl lastitem(void)
{
	register i;

	highlightitem();
	i = pmenu[curmenu].nitem-1;
	while(!pmenu[curmenu].itemable[i])
		i--;
	curitem[curmenu] = i;
	highlightitem();
}

int _Cdecl getitem(unsigned int keycode)
{
  int fcolor, bcolor;

  fcolor = hgetcolor();
  bcolor = hgetbkcolor();
  hsetcolor(menufcolor);
  hsetbkcolor(menubcolor);
	ispulldown = OFF;
	if(keycode != F10) {             /* F10∑° íâü°ª° ¥g¥v∑a°e */
    hsetcolor(fcolor);
    hsetbkcolor(bcolor);
		return findshortcut(keycode);   /* îe¬ÇãiÆA∑•ª° àÒ¨a–eîa */
  }
	else {                            /* F10∑° íâùv∑a°e */
		ispulldown = ON;
		if(!drawmenubox()) {
      hsetcolor(fcolor);
      hsetbkcolor(bcolor);
      return false;
    }
		do {
			keycode = getxch();
			switch(keycode) {
				case LEFTARROW  :
          beforemenu();
					break;
				case RIGHTARROW :
          nextmenu();
					break;
				case UPARROW    :
          beforeitem();
					break;
        case ' '    :
				case DOWNARROW  :
          nextitem();
					break;
				case HOMEKEY  :
          firstitem();
          break;
				case ENDKEY   :
          lastitem();
					break;
				case '\r'   :
          themenu = curmenu;           /* ¿aü±Œa §Â—° */
					theitem = curitem[curmenu]; /* –w°¢ §Â—° */
					erasemenubox();
          ispulldown = OFF;
          hsetcolor(fcolor);
          hsetbkcolor(bcolor);
					return true; /* ¨Â»Ç */
				case ESC    :
          erasemenubox();
          ispulldown = OFF;
          hsetcolor(fcolor);
          hsetbkcolor(bcolor);
					return false; /* ¬·≠° */
				default     :
          if(findshortcut(keycode)) { /* îe¬ÇãiÆAàa ∑∂∑a°e */
            ispulldown = OFF;
            hsetcolor(fcolor);
            hsetbkcolor(bcolor);
					  return true; /* ¨Â»Ç */
          }
			}/* switch */
		} while(true); /* ¢Å–e ûÅœa */
	}/* else */
}

/* HANMENU.C—¡∑©∑Å è{ */
