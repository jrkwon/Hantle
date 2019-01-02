/*
 *  Ìa·©   : kutil.c
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
#include <string.h>
#include <dir.h>
#include <dos.h>
#include "hanin.h"
#include "hanerro2.h"
#include "kedit.h"
#include "kfile.h"
#include "kutil.h"

void beep(void)
{
  int cnt;

  for(cnt = 0; cnt < 5; cnt++)
    sound(cnt*50+500);
  nosound();
}

int get_filename_to_read(unsigned char *name)
{
  if(win_hgetdata(40, " ·ª´á—i·© Ìa·©·¡Ÿq·e ?", "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE", name)) {
    strcuttail(name);
    strupr(name);
    return YES_ANS;
  }
  return CANCEL_ANS;
}

int get_filename_to_write( unsigned char *name)
{
  if(win_hgetdata(40, " ¸á¸wÐi  ÑÁ·©·¡Ÿq·e ?", "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE", name)) {
    strcuttail(name);
    strupr(name);
    return YES_ANS;
  }
  return CANCEL_ANS;
}

int get_number_to_set_tab(unsigned char *name)
{
  if(win_hgetdata(40, " ¬¡ »¡¸÷Ði È“· ì·¡“e ?", "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE", name)) {
    strcuttail(name);
    strupr(name);
    return YES_ANS;
  }
  return CANCEL_ANS;
}

int get_ext_name(unsigned char *name)
{
  if(win_hgetdata(40, " ¸÷ ÑÂ¸w¸a“e ? ", "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE", name)) {
    strcuttail(name);
    strupr(name);
    return YES_ANS;
  }
  return CANCEL_ANS;
}

/*
 *   ‰w¡ º—µA¬á Ìa·©·¡Ÿq e ©³´a…”a.
 */
char *split_filename(char *path)
{
  int index;

  if(*path) {
    index = (strlen(path))-1;
    while(index >= 0 && path[index] != '\\' && path[index] != ':')
      index--;
    return path+index+1;
  }
  return NULL;
}

/*
 *  ‰w¡ º—µA Ìa·©·¡Ÿq·i ¹AˆáÐe”a.
 *  C:\TC\BIN\TC.EXE  --> C:\TC\BIN\
 */
char *truncate_filename(char *path)
{
  int index;

  if(*path) {
    index = (strlen(path))-1;
    while(index >= 0 && path[index] != '\\' && path[index] != ':')
      index--;
    path[index+1] = NULL;
    return path;
  }
  return NULL;
}

/*
 *  ¸á¸wÐi ˆõ·¥»¡ ÑÂ·¥
 *  µ : 0, ´a“¡µ¡ : 1, Âá­¡ : 2
 */
int ask_save(void)
{
  unsigned char msg[40];
  int ans;

  ans = 1;
  if(!saved && memory!=eofm ) {
    sprintf(msg,"%sŸi ¸á¸wÐiŒa¶a?",split_filename(work_file));
    ans = yesno(msg, YES_ANS);
    if(ans == YES_ANS)
      save();
  }
  return ans;
}

bool change_dir(void)
{
  char path[80], drive[3], dir[66], name[13], ext[4];

  sprintf(drive, "%c:", getdisk()+'A');
  getcurdir(0, dir);
  sprintf(path, "%s\\%s", drive, dir);
  if(win_hgetdata(40, " ¬¡¶… ‰w¡“e ? ", "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE", path)) {
    strcuttail(path);
    if(*path != NULL) {
      strupr(path);
      fnsplit(path, drive, dir, name, ext);
      if((*drive) != NULL) 
        setdisk(drive[0]-'A');
      if(chdir(path) == 0) 
        return true;
      else {
        errorf();
        return false;
      }
    }
    else 
      return false;
  }
  return false;  
}

/*
 *  Ìa·© kutil.c · {
 */
