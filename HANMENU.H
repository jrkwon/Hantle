/*
 *  Ïa¡‹aœ‘  : ÐeËiœa·¡§aœáŸ¡
 *  ÑÁ·© ·¡Ÿq : HANMENU.H
 *   e—e ·¡   : Š¥ ¸ œb
 *  i¼a      : 1992. 8. 16
 *  ÑÁ·© ¶w : ¡A“A ‰Åe  áŸ¡ ÑÁ·©
 */

#if !defined(__HANMENU)
#define __HANMENU

#if __STDC__
#define _Cdecl
#else
#define _Cdecl cdecl
#endif

#define  MAXMENU    10   /* ÂA” ÀaŸ±Îa ˆ•® */
#define  MAXITEM    16   /* Ðe ÀaŸ±ÎaµA¬á ÂA” Ðw¡¢ ˆ•® */
#define  MAXLENGTH  20   /* Ðe ÀaŸ±Îa ·¡Ÿq· ÂA”‹©·¡ ? */

#define  getcurmenu()   (themenu)
#define  getcuritem()   (theitem)

typedef struct {
	unsigned char	*menu;                    /* ÀaŸ±Îa ·¡Ÿq */
	unsigned char nitem;                    /* Ðw¡¢ ˆ•® */
	unsigned char	*item[MAXITEM];           /* Ðw¡¢ ·¡Ÿq */
	unsigned int  menuable;                 /* ÀaŸ±Îa ˆa“w Îa¯¢ */
	unsigned int  itemable[MAXITEM];        /* Ðw¡¢ ˆa“w Îa¯¢ */
	unsigned int  shortcut[MAXITEM];        /* ”eÂ‚ ‹i®A */
	char  shortcutchar[MAXITEM][4];         /* ÀaŸ±ÎaµA aÈai ”eÂ‚ ‹i®A· ¢…¸aµi */
	unsigned int  top, left, width, height; /* ÀaŸ±Îa· ¹ÁÎa */
	unsigned int  menuwidth;                /* ÀaŸ±Îa· ì·¡ */
  unsigned int  itemwidth;                /* Ðw¡¢· ì·¡ */
} HMENU;

extern int themenu, theitem, nmenu;
extern HMENU pmenu[MAXMENU];

void _Cdecl definemenu(char fcolor, char bcolor, int init_menu, int gap);
void _Cdecl drawmenubar(void);
int _Cdecl getitem(unsigned int keycode);

#endif

/* HANMENU.H ÑÁ·©· { */