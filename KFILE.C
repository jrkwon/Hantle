/*
 *  Ìa·©   : kfile.c
 *  ¶w   : ¢…¬á ¸á¸w‰Á ¦‰œáµ¡‹¡
 *  ‹¡ÒB   : hantle.prj, nal.prj
 *  i¼a   : 93.03.16
 *   e—e·¡ : Ðe´·”ÐbŠa ¸å¸aÉ·¯¥‰·Ðb‰Á ÐeËi´aŸ¡ Š¥¸œb
 *  ´iŸ±‹i : ·¡ Î‰‹aŸ±· £»‹aŸ±·e ‰·ˆŸi ¶¥Ã¢·a¡ Ðs“¡”a.
 *           µá¡Ða¡e, a•¡ ”aŸe £»‹aŸ±· •¡¶‘·i  g·¡ ¤h´v·a“¡Œa¶a.
 *           ˜aœa¬á, ·¡ £»‹aŸ±·i ¤h·a¯¥ ¦…·e ”aŸe•A ¬a¶wÐa­a•¡ –A»¡ e
 *           ¬¡  e—e £»‹aŸ±•¡ ¸i ¸÷Ÿ¡Ða­a¬á ‰·ˆÐ º¯¡‹¡ ¤aœs“¡”a.
 */
#include <stdio.h>
#include <io.h>
#include <string.h>
#include <stdlib.h>
#include <mem.h>
#include <dir.h>
#include <dos.h>
#include <ctype.h>
#include "hanout.h"
#include "hanwindw.h"
#include "hanerro2.h"
#include "ascii.h"
#include "extkey.h"
#include "kedit.h"
#include "kfile.h"
#include "kutil.h"

static unsigned int ctrl_z_delete(unsigned index);
static int search( char ch, char fn[][13], int start, int all);
static int get_response(char filename[][13], int count, int dir_count);
static char *get_modir(char *dir);
static int display_name(int topi, int count, char filename[][13]);

int file_or_dir_exist(char *fname)
{
  return (access(fname, 0) == 0);
}

void new_file(void)
{
  int ans;

  ans = ask_save();
  if(ans != CANCEL_ANS) {
    init_editor_variable();
    disp_status();
  }
}

int load(void)
{
  static char load_name[80];
  char temp_name[80], temp_load[80];
  int  success = NO;

  if(get_filename_to_read(load_name) == YES_ANS) {
    strcpy(temp_name, load_name);
    if(files(load_name) != NULL) {
      new_file();           /* 93.07 */
      if(temp_name[0] == NULL) {
        sprintf(temp_name, "*%s", file_ext);
/*      if(!strchr(temp_name, '.')) {
        strcat(temp_name, file_ext);
        strcpy(load_name, temp_name); */
      }
      if(!file_or_dir_exist(load_name)) {
        disp_cmsg_sec(" ¬¡¶… Ìa·©·³“¡”a ", 5);
        strcpy(work_file, load_name);
        user_task = TASK_DISPLAYPAGE;
        disp_status();
        success = YES;
      }
      else if(load_file(load_name, INIT_LOAD)) {
        strcpy(work_file, load_name);
        if(strchr(temp_name, '*') || strchr(temp_name, '?')) {
          truncate_filename(load_name);
          sprintf(temp_load, "%s%s", load_name, split_filename(temp_name));
          strcpy(load_name, temp_load);
        }
        else if(temp_name[0] == NULL) {
          sprintf(temp_name, "*%s", file_ext);
          truncate_filename(load_name);
          sprintf(temp_load, "%s%s", load_name, temp_name);
          strcpy(load_name, temp_load);
        }
        user_task = TASK_DISPLAYPAGE;
        disp_status();
        success = YES;
      }
    }
    else {
      if(temp_name[0] == NULL)
        sprintf(load_name, "*%s", file_ext);
      else if(!strchr(temp_name, '.')) { /* no ext */
        strcat(temp_name, file_ext);
        strcpy(load_name, temp_name);
      }
      else
        strcpy(load_name, temp_name);
    }
  }
  return success;
}

static unsigned int ctrl_z_delete(unsigned index)
{
  if((*(current+index-1)) == 0x1a) {
    movmem(current+index, current+index-1, (size_t )(eofm-current+1));
    index -= 1;
  }
  return index;
}

unsigned int load_file(char *filename, int load_mode)
{
  int handle;
  long     l_size;
  unsigned size;

  size = 0;
  errno = 0;
  user_task = TASK_DISPLAYPAGE;

  if((handle = _open(filename, 0x8001)) != -1) {
    l_size =filelength(handle);
    if(load_mode == INIT_LOAD) {
      if(ask_save() != CANCEL_ANS) {
        if(l_size > maxeditbuf) {
          if(yesno("¡A¡¡Ÿ¡ˆa ¦¹¢Ðs“¡”a. –õ¦¦…·i ¸aŸiŒa¶a?", 0) == YES_ANS)
            l_size = maxeditbuf;
          else
            l_size = -1L;
        }
      }
      else
        l_size = -1L;
    }
    else {
      if(l_size > (maxeditbuf-(long)(eofm-memory))) {
        memerror();
        l_size = -1L;
      }
    }
    if(l_size != -1L) {
      size = (size_t)(l_size);
      movmem(current, current+size, (size_t)(eofm-current)+1);
      if(_read(handle, current, size)== -1)
        movmem(current+size, current,(size_t)(eofm-current)+1);
      else {
        size = ctrl_z_delete(size);
        max_line += line_count(current, current+size);
        eofm += size;
        strncpy(line_buffer, line_start, MAXLINEBUF);
        line_buffer[MAXLINEBUF] = NULL;
      }
    }
    _close(handle);
  }
/*  else
    errno = 38;

  if(errno)
    errorf();   */

  return size;
}

int save(void)
{
  char temp_name[80];

  if(memory != eofm) {
    sprintf(temp_name, "%s%s", init_file_name, file_ext);
    if(!strcmp(work_file, temp_name)) {
/*      strcpy(temp_name, INIT_FILE); */
      if(get_filename_to_write(temp_name) == YES_ANS)
        if(!strchr(temp_name, '.'))
          strcat(temp_name, file_ext);
        strcpy(work_file, temp_name);
        disp_status();  /* disp_file_status(); */
    }
    if(save_file(memory, eofm, work_file)) {
      saved = YES;
      return YES;
    }
    else
      return NO;
  }
  else
    return YES; /* length 0 error */
}

bool save_file(unsigned char *start, unsigned char *finish, unsigned char *filename)
{
  unsigned char back_up_name[80];
  unsigned char *f_n_pt = filename, *b_n_pt = back_up_name;
  int handle;
  bool success = true;

  while( *f_n_pt && *f_n_pt !=  '.')
    *b_n_pt++ = *f_n_pt++;

  strcpy(b_n_pt , ".BAK");
  if(file_or_dir_exist(filename)) {
    if(file_or_dir_exist(back_up_name)) {
      if(strcmp(filename, back_up_name) == 0) {
        unlink( back_up_name);
      }
      rename(filename, back_up_name);
    }
  }
  errno = 0;
  if((handle = creat(filename, 0x0180)) != -1) {
    /* ¶wœ··¡ ¦¹¢Ða¡e ´á˜âÐa»¡.. */
    _write(handle, start, (unsigned int)(finish-start));
    close(handle);
  }
  if(errno) {
    errorf();
    success = false;
  }
  return success;
}

bool save_as(void)
{
  char temp_name[80];
  bool success;

  success = false;
  /* strcpy(temp_name, write_name); */
  /* ... "save as" is rename function */
  strcpy(temp_name, "");
  if(get_filename_to_write(temp_name) == YES_ANS) {
    if(!strchr(temp_name, '.'))  /* no ext */
      strcat(temp_name, file_ext);
    if(files(temp_name) != NULL) {
      if(file_or_dir_exist(temp_name)) {
        if(yesno(" Ìa·©·¡ ·¡£¡ ·¶Š…¶a, ”ü´á ³iŒa¶a ? ", NO_ANS) != YES_ANS)
          return false;
      }
      if(save_file(memory, eofm, temp_name)) {
        strcpy(work_file, temp_name);
        disp_status();  /* disp_file_status(); */
        saved = YES;
        success = true;
      }
    }
    else {
      /* strcpy(write_name, temp_name); */
    }
  }
  return success;
}

static int search( char ch, char fn[][13], int start, int all)
{
  int i = start+1;

  if(i < all) {
    while( i < all) {
      if((ch == fn[i][0]))
        return i;
      else
        i++;
    }
  }
  else
    start = all;
  i = 0;
  while(i < start) {
    if(ch == fn[i][0])
      return i;
    else
      i++;
  }
  return -1;
}

static int get_response(char filename[][13], int count, int dir_count)
{
  unsigned   ch;
  int   select_done;
  int i, topi, last, temp;
  unsigned ox, oy, barx, bary;
  bool onoff;

  select_done = i = topi = 0, barx = bary = 1;
  last = display_name(0, count, filename);
  while(!select_done) {
    ox = barx, oy = bary;
    onoff = isreverse();
    if(!onoff)
      hsetreverse(ON);
    hprintfxy(ox, oy, " %-12s ", filename[topi+i]);
    hsetreverse(onoff);
    ch = getxch();
    hprintfxy(ox, oy, " %-12s ", filename[topi+i]);
    switch(ch) {
      case LEFTARROW :
        i--;
        if(i < 0) {
          if(topi-4 <0 ) {
            i = 0;
          }
          else {
            topi -= 4;
            i = 3;
            last = display_name(topi, count, filename);
          }
        }
        break;
      case RIGHTARROW :
        i++;
        if(i > last) {
          if(last == 35) {
            if(topi+36 < count) {
              topi += 4;
              i = 32;
              last = display_name(topi, count, filename);
            }
            else {
              i = last;
            }
          }
          else
            i = last;
        }
        break;
      case  UPARROW :
        if(i-4 < 0) {
          if(topi-4 >= 0) {
            topi -= 4;
            last = display_name(topi, count, filename);
          }
        }
        else
          i -=4;
        break;
      case  DOWNARROW :
        if(i+4 > last) {
          if(topi+36 <count) {
            topi +=4;
            last = display_name(topi, count, filename);
            if(i>last) i-=4;
          }
        }
        else
          i += 4;
        break;
      case  HOMEKEY :
        if(topi != 0) {
          topi=0;
          last = display_name(topi, count, filename);
        }
        i = 0;
        break;
      case  ENDKEY  :
        if(topi+36 < count) {
          i = 31+(count & 0x03);
          topi = count-(i+1);
          last = display_name(topi, count, filename);
        }
        else
          i = last;
        break;
      case  PGUPKEY :
        if(topi != 0) {
          topi -= 36;
          if(topi < 0)
            topi = 0;
          last = display_name(topi, count, filename);
        }
        else
          i = i & 0x03;
        break;
      case  PGDNKEY :
        temp = topi+36;
        if(temp < count) {
          if((temp+(i&0x03)) < count)
            topi = temp;
          else
            topi += 32;
          last = display_name(topi, count, filename);
          if(i > last)
          i = last-((last-i)&0x03);
        }
        else
          i = last-((last-i)&0x03);
        break;
      case  CR   :
        select_done = 1;
        break;
      case  ESC  :
        return -1;
      default    :
        temp = search(toupper(ch), filename, topi+i, count-dir_count);
        if(temp >= 0) {
          if(topi <= temp && topi+36 > temp)
            i = (temp-topi)%36;
          else {
            topi = temp & 0xfffc;
            i = temp & 0x0003;
            last = display_name(topi, count, filename);
          }
        }
    }
    barx = (i&0x0003)*15+1;
    bary = (i/4) +1;
  }/* while */
  return topi+i;
}

/*
 *  ¡¡ —¡BÉ¡Ÿ¡(..)Ÿi ŠÐe”a.
 */
static char *get_modir(char *dir)
{
  int index;

  if(*dir) {
    index=(strlen(dir))-2;
    while(index >= 0 && dir[index] != '\\' && dir[index] != ':')
      index--;
    dir[index+1] = NULL;
  }
  return dir;
}

static int display_name(int topi, int count, char filename[][13])
{
  unsigned index, last, x, y;

  last = (count-topi) > 35 ? 35 : count-topi-1;

  for(index = 0; index < 36; index++) {
    x = (index & 0x0003)*15+1;
    y = (index / 4) +1;
    if(index > last)
      hprintfxy(x, y, " %-12s ", " ");
    else
      hprintfxy(x, y, " %-12s ", filename[topi+index]);
  }
  return last;
}

static char *split_filename(char *path)
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

char temp_file[66];

char *files(char *name)
{
  struct ffblk ffblk;
  int  index, dir_count, count, response, select_done = NO;
  char path_name[66], search_name[13], drive[3], dir[66], file[9], ext[5];
  char filename[MAX_FILES][13], otherdir[20][13];

  while(*name) {
    if((*name)==' ')
      name++;
    else
      break;
  }
  fnsplit(name, drive, dir, file, ext);
  if((*drive) == NULL)
    sprintf(drive, "%c:", getdisk()+'A');
  if((*dir) == NULL) {
    if(getcurdir(drive[0]-'A'+1, path_name) == -1) {
      errorf();
      return NULL;
    }
    sprintf(dir, "\\%s%c", path_name, (*path_name) ? '\\' : '' );
  }
  else
    strcat(dir, (dir[strlen(dir)-1]=='\\') ? "" : "\\");
  sprintf(search_name, "%s%s", (*file) ? file : "*", (*ext) ? ext : file_ext);

  if(strchr(search_name,'*') || strchr(search_name,'?') ) {
    if(wopen(11, 10, 59, 9)) {
      do {
        sprintf(temp_file, "%s%s%s", drive, dir, "*");
        dir_count=0;
        errno = 0;
        findfirst(temp_file, &ffblk, FA_DIREC);
        while(errno == 0 && dir_count < 20) {
          if(ffblk.ff_attrib==16)
            sprintf(otherdir[dir_count++], "%s\\", ffblk.ff_name);
          findnext(&ffblk);
        }
        sprintf(temp_file, "%s%s%s", drive, dir, search_name);
        wtitle(temp_file);
        count = 0;
        errno = 0;
        findfirst(temp_file, &ffblk, 0x00);
        while(errno == 0 && count < MAX_FILES) {
          sprintf(filename[count++], "%s", ffblk.ff_name);
          findnext(&ffblk);
        }
        if(count > 0)
          qsort(filename,count,13,strcmp);
        if(dir_count)
          qsort(otherdir, dir_count, 13, strcmp);
        index=0;
        response=0;
        if(dir_count && otherdir[0][0]=='.') {
          index = 2, dir_count--, response = 1;
        }
        while (count < MAX_FILES && index < dir_count+response) {
          sprintf(filename[count++],"%s",otherdir[index++]);
        }
        if(response)
          sprintf(filename[count++], "%s", otherdir[0]);
        if(count > 0) {
          response = get_response(filename, count, dir_count);
          if(response != -1)
            sprintf(name, "%s%s%s", drive, dir, filename[response]);
          else {
            wclose();
            return NULL;
          }
        }
        else {
/*          errno=38;
          errorf(); */
          wclose();
          return NULL;
        }
        if(strchr(filename[response],'\\') == NULL)
          select_done = YES;
        else {
          if(filename[response][0]=='.')
            strcpy(dir, get_modir(dir));
          else
            strcat(dir, filename[response]);
        }
      }while( !select_done );
      wclose();
      return name;
    }
    else {
      return NULL;
    }
  }
  else
    sprintf(temp_file, "%s%s%s", drive, dir, search_name);
  return temp_file;
}

/*
 *  Ìa·© kfile.c · {
 */
