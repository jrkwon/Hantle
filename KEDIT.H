/*
 *  Ìa·©   : kedit.h
 *  ¶w   : Íe»³‹¡ ¸åÁAµA Í¡Ðq–A´á´¡ Ða“e ¦¦…
 *  ‹¡ÒB   : hantle.prj, nal.prj
 *  i¼a   : 93.03.16
 *   e—e·¡ : Ðe´·”ÐbŠa ¸å¸aÉ·¯¥‰·Ðb‰Á ÐeËi´aŸ¡ Š¥¸œb
 *  ´iŸ±‹i : ·¡ Î‰‹aŸ±· £»‹aŸ±·e ‰·ˆŸi ¶¥Ã¢·a¡ Ðs“¡”a.
 *           µá¡Ða¡e, a•¡ ”aŸe £»‹aŸ±· •¡¶‘·i  g·¡ ¤h´v·a“¡Œa¶a.
 *           ˜aœa¬á, ·¡ £»‹aŸ±·i ¤h·a¯¥ ¦…·e ”aŸe•A ¬a¶wÐa­a•¡ –A»¡ e
 *           ¬¡  e—e £»‹aŸ±•¡ ¸i ¸÷Ÿ¡Ða­a¬á ‰·ˆÐ º¯¡‹¡ ¤aœs“¡”a.
 */

#include "typedefs.h"

#ifndef _KEDIT_H
#define _KEDIT_H

/*
 * Ìa·© Â‰bÀwµA¬á ”až‰ ® ·¶“e Ìa·©· ÂA” ®
 */
#define  MAX_FILES          (400)          /* Ìa·©¬w¸aµA¬á ”až‰ ® ·¶“e Ìa·© ® */
#define  MAXLINEBUF         (255)          /* Ðeº‰· ÂA” Íe»³ˆa“w ‹©·¡ */
#define  DEFAULTBUFSIZE     (57200L)       /* ¸åÁA Íe»³¶wœ·· ÂA”Ça‹¡  */
#define  MAXBLOCKSIZE       (2000)         /* §iœâ·a¡ ”až‰ ® ·¶“e ÂA”Ça‹¡ */

/*
 *  Íe»³Àw· Ça‹¡“e ÂA­¡ (5 x 3), ÂA” (80 x 23)
 */
#define  MAX_SCR_COL    80
#define  MAX_SCR_LINE   23
#define  MIN_SCR_COL     5
#define  MIN_SCR_LINE    3

#define  HE_2LARGE  36   /* Ìa·©·¡ á¢ Äá¶a */
#define  HE_NOFIND  37   /* ‹aœå ¢…¸aµi·e ´ô¯s“¡”a */
#define  HE_ILFILE  38   /* Ìa·©·¡ ¹¥¸Ðaˆáa ¸i¡µ–E ·¡Ÿq·³“¡”a */
#define  HE_NOEXIST 39   /* ”á ·¡¬w ´ô¯s“¡”a */

enum yesnocan_t { YES_ANS, NO_ANS, CANCEL_ANS };
enum yesno_t { NO, YES };
enum loadmode_t { INIT_LOAD, BLOCK_LOAD };
enum display_mode { REVERSE, NORMAL };

/*
 *  Ç¡ ·³b ¦¦…Œa»¡ ”a¯¡ •©´aµÖ·i ˜, ¶¬å Ð´¡ Ði ·©
 *  »b, Ç¡ ·³b·¡ ´ô·i ˜ ´á˜å ·©·i Ði ˆõ·¥ˆa.
 */
#define  TASK_DISPLAYPAGE      0x0100    /* ¸å ÑÁ¡e ”a¯¡ Â‰b */
#define  TASK_SCROLLUPPAGE     0x0040    /* ¸åÁA ¶á¡ ÑÁ¡e ¯aÇa© */
#define  TASK_SCROLLUP         0x0020    /* Ñe¸ Äá¬á ´aœ ¦¦… e ¶á¡ ÐeÄe ¯aÇa© */
#define  TASK_SCROLLDOWN       0x0010    /* Ñe¸ Äá¬á ´aœ ¦¦… e ´aœ¡ ÐeÄe ¯aÇa© */
#define  TASK_DISPLAYUPLINE    0x0008    /* Ðeº‰ ¶á¡ ·¡•·Ð¬á ‹a º‰·i ”a¯¡ Îa¯¡ */
#define  TASK_DISPLAYDOWNLINE  0x0004    /* Ðeº‰ ´aœ¡ ·¡•·Ð¬á ‹a º‰·i ”a¯¡ Îa¯¡ */
#define  TASK_DISPLAYBOTLINE   0x0002    /*  … ´aœº‰ Îa¯¡ */
#define  TASK_DISPLAYLINE      0x0001    /* Ñe¸ º‰ ”a¯¡ Îa¯¡ */


extern unsigned char *memory;                   /* ¯¡¸bÐi ˜ Íe»³Ði  eÇq· ‰·ˆe·i Ði”w¤h“e”a */
extern unsigned char *line_start;               /* ´á“a Ðe º‰· ¯¡¸b Í¡·¥Èá */
extern unsigned char *current;                  /* Ñe¸ Íe»³ º—·¥ ¶áÃ¡ */
extern unsigned char *last_pos;                 /* ¤a¡ ·¡¸å ¶áÃ¡ */
extern unsigned char *b_b_ptr;                  /* §iœâ· ¯¡¸b Í¡·¥Èá */
extern unsigned char *b_k_ptr;                  /* §iœâ· { Í¡·¥Èá */
extern unsigned char *eofm;                     /* Ìa·©·  a»¡ b */
extern unsigned char work_file[80];             /* ¸b´ó Ìa·©· ·¡Ÿq */
extern unsigned char block_file[80];            /* §iœâ·a¡ ¸á¸w/·ª´áµ¡‹¡ Ìa·©· ·¡Ÿq */
extern unsigned char help_file[80];
extern unsigned char write_file[80];
extern unsigned char line_buffer[MAXLINEBUF+1]; /* Ñe¸ Íe»³ º—·¥ º‰· ·±¯¡ ¸b´ó ¸w­¡ */

extern bool insert_mode, indent_mode, saved, block_display;
extern unsigned int curx, cury, user_task, max_line, base_y, base_x, block_size;
extern long maxeditbuf;

/*
 *  ”a·q·e Íe»³‹¡· ‹¡“wÇ¡(¸á¸w, ¦‰œáµ¡‹¡)µA ”Ðe ¸÷ˆt·¡”a.
 *  ”e, Ðeµw É¡‹iÇ¡“e ‹¡¥¥¸â·a¡ SHIFT-SPACE·¡‰¡, ·¡¶AµA ”aŸe Ç¡•¡ —w¢Ði ® ·¶”a.
 */
extern int save_file_key       ;   /* Save : ¸á¸wÐa‹¡ */
extern int print_file_key      ;   /* Print : ·¥­áÐa‹¡ */
extern int help_key            ;   /* •¡¶‘ i Ç¡ */
extern int han_toggle_key      ;   /* Ðe/µw ¤aŽ‘Ç¡ */
extern int hanja_conv_key      ;   /* Ðe‹i->Ðe¸aÇ¡ */
extern int kdb_toggle_key      ;   /* 3¤é¯¢/2¤é¯¢ ¤aŽ‘Ç¡ */
extern int ks_graph_key        ;   /* KS 2¤a·¡Ëa ‹aŸ±¢…¸a ·³b */
extern int tg_graph_key        ;   /* TG 2¤a·¡Ëa ‹aŸ±¢…¸a ·³b */
extern int menu_key            ;   /* pulldown menu */
extern int quit_key            ;   /* ¸á¸wÐi ˆõ·¥»¡ ¢ˆ‰¡, Íe»³·i {‹¡ */
extern char file_ext[]         ;   /* file extension */
extern char init_file_name[]   ;   /* Á¡‹¡ Ìa·© ·¡Ÿq - ÑÂ¸w¸a ¨‰¡ */

extern bool hanja_font_exist;      /* Ðe¸a Ìa·©·¡ ¹¥¸Ða“e»¡.. */
extern bool all_jobs_close;        /* ¡¡—e ¸b´ó·¡ ¹·ža –A´ö“e»¡ ... */

extern void (*menu_process)(int key);         /* for the main menu process */

void block_decision(unsigned char **block_ptr);
void block_copy(void);
void block_move(void);
void block_delete(void);
void block_write(void);
void block_read(void);
void to_pointer(unsigned char *destin);
void find_condition(int replace);
void writef(int sx, int ex, int y, unsigned char *msg, int mode);
void errorf(void);
bool set_tabsize(int size);
void set_max_width_height(int width, int height);
void draw_border(int x, int y, int width, int height, int border);
int  get_tabsize(void);
void init_editor_variable(void);
bool init_editor(char *filename, char *help_filename, long bufsize, int kdb_kind, bool han_eng);
bool editor(int x, int y, int width, int height, int border);
unsigned int line_count(unsigned char *start, unsigned char *finish);
void disp_kbd_status(void);
void disp_indent_status(void);
void disp_file_status(void);
void disp_status(void);

#endif

/*
 *  Ìa·© kedit.h · {
 */
