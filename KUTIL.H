/*
 *  Ìa·©   : kutil.h
 *  ¶w   :
 *  ‹¡ÒB   : hantle.prj, nal.prj
 *  i¼a   : 93.03.16
 *   e—e·¡ : Ðe´·”ÐbŠa ¸å¸aÉ·¯¥‰·Ðb‰Á ÐeËi´aŸ¡ Š¥¸œb
 *  ´iŸ±‹i : ·¡ Î‰‹aŸ±· £»‹aŸ±·e ‰·ˆŸi ¶¥Ã¢·a¡ Ðs“¡”a.
 *           µá¡Ða¡e, a•¡ ”aŸe £»‹aŸ±· •¡¶‘·i  g·¡ ¤h´v·a“¡Œa¶a.
 *           ˜aœa¬á, ·¡ £»‹aŸ±·i ¤h·a¯¥ ¦…·e ”aŸe•A ¬a¶wÐa­a•¡ –A»¡ e
 *           ¬¡  e—e £»‹aŸ±•¡ ¸i ¸÷Ÿ¡Ða­a¬á ‰·ˆÐ º¯¡‹¡ ¤aœs“¡”a.
 */

#ifndef _KUTIL_H
#define _KUTIL_H

#include "typedefs.h"

void beep(void);
int get_filename_to_read(unsigned char *name);
int get_filename_to_write(unsigned char *name);
int get_number_to_set_tab(unsigned char *name);
int get_ext_name(unsigned char *name);
char *split_filename(char *path);
char *truncate_filename(char *path);
int ask_save(void);
void new_file(void);
bool change_dir(void);

#endif

/*
 *  Ìa·© kutil.h · {
 */
