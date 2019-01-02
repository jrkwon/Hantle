/*
 *  파일   : kedit.c
 *  내용   :
 *  기획   : hantle.prj, nal.prj
 *  날짜   : 93.03.16
 *  만든이 : 한양대학교 전자통신공학과 한틀아리 권재락
 *  알림글 : 이 풀그림의 밑그림은 공개를 원칙으로 합니다.
 *           왜냐하면, 나도 다른 밑그림의 도움을 많이 받았으니까요.
 *           따라서, 이 밑그림을 받으신 분은 다른데 사용하셔도 되지만
 *           새로 만든 밑그림도 잘 정리하셔서 공개해 주시기 바랍니다.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <alloc.h>
#include <mem.h>
#include <ctype.h>
#include "hanout.h"
#include "hanin.h"
#include "hanhanja.h"
#include "hancoord.h"
#include "hanmenu.h"
#include "kedit.h"
#include "kfile.h"
#include "kutil.h"
#include "khelp.h"
#include "ascii.h"
#include "extkey.h"
#include "hanerro2.h"
#include "hanwindw.h"
#include "kinput.h"
#include "kprn.h"

void (*menu_process)(int key);  /* function pointer for the main menu routine */

static unsigned char *pre_ptr(unsigned char *ptr, int count);
static unsigned char *next_ptr(unsigned char *ptr, int count);
static unsigned int linewidth(unsigned char *ptr);
static void down_line(int delta);
static void insert_line(int y);
static void delete_line(int y);
static void user_func(int kbd_check);
static unsigned int get_key(int x, int y, int *state);
static unsigned int get_key2(int indicator);
static bool insert(unsigned int size, unsigned char content);
static bool insert_ptr(unsigned int size, unsigned char content,  unsigned char *ptr);
static unsigned int block_line_count(unsigned char *start, unsigned char *finish);
static unsigned int delete(unsigned int size);
static int in_block(unsigned char *ptr);
static unsigned char *one_line(unsigned char *start, int y);
static void display_page(int kbd_check);
static void hor_locate(unsigned int pos);
static void ver_locate(unsigned int line_no,unsigned char *destin);
static void left(void);
static int is_eol(unsigned char *ptr);
static int is_eof(unsigned char *ptr);
static void han_right(void);
static void right(void);
static void del(void);
static void english(int ch);
static void hanja(void);
static unsigned int get_ks_graphic_char(void);
static void ks_graphic_char(void);
static void hangul(void);
static void up_down(int delta);
static void del_eol(void);
static void del_word(void);
static void ins_return(void);
static int in_word(unsigned char *ch);
static void word_left(void);
static void word_right( void );
static void restore_line( void );
static int compare(unsigned char *st1);
static int find( void );
static int change_line(int mem_size, int buf_size, unsigned char *buf_ptr);
static void find_replace( void );
static void find_string(int repeat_last_find);
static void ctrl_k(int key);
static void ctrl_q(int key);

/*
 *  편집기 운영에 사용되는 변수들..
 */
bool hanja_font_exist;                    /* 한자 글꼴 파일이 존재하는 지 */

bool all_jobs_close;                      /* all jobs close */

/*
 *  편집에 직접 관련된 변수들
 */
unsigned char *memory;                   /* 시작할 때 편집할 만큼의 공간을 할당받는다 */
unsigned char *line_start;               /* 어느 한 줄의 시작 포인터 */
unsigned char *current;                  /* 현재 편집 중인 위치 */
unsigned char *last_pos;                 /* 바로 이전의 위치 */
unsigned char *b_b_ptr;                  /* 블럭의 시작 포인터 */
unsigned char *b_k_ptr;                  /* 블럭의 끝 포인터 */
unsigned char *eofm;                     /* 파일의 마지막 */
unsigned char work_file[80];             /* 작업 파일의 이름 */
unsigned char block_file[80];            /* 블럭으로 저장/읽어오기 파일의 이름 */
unsigned char write_file[80];            /* 새이름으로 파일의 이름 */
unsigned char help_file[80];             /* 도움말 파일의 이름 */
unsigned char line_buffer[MAXLINEBUF+1]; /* 현재 편집 중인 줄의 임시 작업 장소 */

bool insert_mode = true, indent_mode = true, saved = true, block_display = true;
unsigned int curx, cury, user_task, max_line, base_y, base_x, block_size;
long maxeditbuf;

static unsigned int maxwidth, maxheight;
/* 커서를 위아래로 움직일 경우 이전의 커서 위치가 지금 커서위치보다 크다면 이를 기억 */
static unsigned int cur_most_x;
/*
 *  다음은 편집기의 기능키(저장, 불러오기)에 대한 내정값이다.
 *  단, 한영 토글키는 기본적으로 SHIFT-SPACE이고, 이외에 다른 키도 등록할 수 있다.
 */
int save_file_key       = ALT_S;         /* Save : 저장하기 */
int print_file_key      = ALT_P;         /* Print : 인쇄하기 */
int help_key            = F1;            /* 도움말 키 */
int han_toggle_key      = F2;            /* 한/영 바꿈키 */
int hanja_conv_key      = F9;            /* 한글->한자키 */
int kbd_toggle_key      = ALT_K;         /* 3벌식/2벌식 바꿈키 */
int ks_graph_key        = F5;            /* KS 2바이트 그림문자 입력 */
int tg_graph_key        = F6;            /* TG 2바이트 그림문자 입력 */
int menu_key            = F10;           /* pulldown menu */
int quit_key            = ALT_X;         /* 저장할 것인지 묻고, 편집을 끝내기 */
char file_ext[5]        = ".TXT";        /* default file extension */
char init_file_name[9]  = "UNTITLED";    /* 초기 파일 이름 */

static unsigned char *pre_ptr(unsigned char *ptr, int count)
{
 int line = 0;

 while(line <= count && ptr > memory)
   if(*(--ptr) == LF && *(ptr-1) == CR) line++;
 return((line <= count) ? memory : ptr+1);
}

static unsigned char *next_ptr(unsigned char *ptr, int count)
{
 int line = 0;

 while(line < count && ptr < eofm) {
   if(*ptr++ == CR && *ptr == LF) {
     ++ptr, ++line;
   }
 }
 return(ptr);
}

/*
 * 현재 포인터에서 그 줄 끝까지의 길이
 */
static unsigned int linewidth(unsigned char *ptr)
{
  unsigned int size = 0;

  while(eofm > ptr && !(*ptr++ == CR && *ptr == LF))
    ++size;
  return(size);
}

/*
 *  msg 문자열을 (sx, y)좌표에 출력하되 ex까지의 나머지 부분은 공백으로 채운다
 */
static void writef(int sx, int ex, int y, unsigned char *msg, int mode)
{
  bool onoff;

  if(mode == REVERSE) {
    onoff = isreverse();
    hsetreverse(ON);
  }
  hputsxy(-(sx+1), -(y+1), msg);
  sx += strlen(msg);

  while(sx++ <= ex)
    eputsxy(-(sx), -(y+1), " ");
  if(mode == REVERSE)
    hsetreverse(onoff);
}

/*
 *  delta만큼 아래 줄로
 */
static void down_line(int delta)
{
  if(base_y + cury + delta <= max_line)
    one_line(next_ptr(line_start, delta), cury + delta);
}

/*
 *  한줄 삽입
 */
static void insert_line(int y)
{
  hbackscrollxy(1, y+1, -maxwidth, -(maxheight-y), _CW->linespace);
}

/*
 *  한줄 지우기
 */
static void delete_line(int y)
{
  hscrollxy(1, y+1, -maxwidth, -(maxheight-y), _CW->linespace);
}

/*
 *  자판입력이 없을 때 하는 일...
 */
static void user_func(int kbd_check)
{
  if(user_task & TASK_DISPLAYPAGE)
    display_page(kbd_check);
  else {
    if(user_task & TASK_SCROLLUPPAGE)
      delete_line(0);
    else if(user_task & TASK_SCROLLUP)
      delete_line(cury);
    else if(user_task & TASK_SCROLLDOWN)
      insert_line(cury);
    if((user_task & TASK_DISPLAYUPLINE) && cury)
      one_line(pre_ptr(line_start, 1), cury-1);
    if(user_task & TASK_DISPLAYDOWNLINE)
      down_line(1);
    else if(user_task & TASK_DISPLAYBOTLINE)
      down_line(maxheight-cury-1);
    if(user_task & TASK_DISPLAYLINE)
      one_line(line_start, cury);
    user_task = 0;
 }
}

static unsigned int get_key(int x, int y, int *state)
{
  int bch, key;
  bool onoff;

  if(user_task) /*  && !keypressed()) 93.07 */
    user_func(true);
  if(block_display && in_block(current)) {
    onoff = isreverse();
    hsetreverse(!onoff);
  }
  if(*current & 0x80)
    bch = (*current << 8) | *(current+1);
  else
    bch = *current;
  *state = GetHanCh(x, y, &key, bch);
  if(block_display && in_block(current)) {
    hsetreverse(onoff);
  }
  return key;
}

/*
 *  두개의 키의 조합으로 이루어진 것에서 그 두번째 것을 입력받는다.
 *  Ctrl-KB, Ctrl-KK, ...
 */
static unsigned int get_key2(int indicator)
{
  byte far *buf;
  long bufsize;
  bool onoff;
  int key;

  bufsize = htextsize(1, 1, -3, -1);
  buf = farmalloc(bufsize);
  if(!buf) {
    memerror();
    return ESC;
  }
  hgettext(1, 1, -3, -1, buf);
  onoff = isreverse();
  hsetreverse(ON);
  hprintfxy(1, 1, "^%c ", (char)indicator);
  key = getxch();
  hprintfxy(3, 1, "%c", key);
  if(('A' <= key && key <= 'Z') || ('a' <= key && key <= 'z'))
    key &= 31;
  hputtext(1, 1, -3, -1, buf);
  hsetreverse(onoff);
  farfree(buf);
  return(key);
}

static bool insert(unsigned int size, unsigned char content)
{
  int i;

  if(maxeditbuf-(unsigned int)(eofm-memory) < size)
    return false;
  movmem(current, current+size, (unsigned int)(eofm-current+1));
  for(i = 0; i < size; i++)
    *(current+i) = content;
  if(*current == CR) {
    *(current+1) = LF,  ++max_line;
  }
  else
    user_task |= TASK_DISPLAYLINE;
  eofm += size;
  saved = false;
  if(last_pos > current)
    last_pos += size;
  if(block_size) {
    if(b_b_ptr > current)
      b_b_ptr += size;
    if(b_k_ptr > current)
      b_k_ptr += size;
    block_size = (unsigned int)(b_k_ptr - b_b_ptr);
  }
  return true;
}

static bool insert_ptr(unsigned int size, unsigned char content,  unsigned char *ptr)
{
  int i;

  if(maxeditbuf-(unsigned int)(eofm-memory) < size)
    return false;
  movmem(ptr, ptr+size, (unsigned int)(eofm-ptr+1));
  for(i = 0; i < size; i++)
    *(ptr+i) = content;
  if(*ptr == CR) {
    *(ptr+1) = LF,  ++max_line;
  }
  else
    user_task |= TASK_DISPLAYLINE;
  eofm += size;
  if(last_pos > ptr)
    last_pos += size;
  if(block_size) {
    if(b_b_ptr > ptr)
      b_b_ptr += size;
    if(b_k_ptr > ptr)
      b_k_ptr += size;
    block_size = (unsigned int)(b_k_ptr - b_b_ptr);
  }
  return true;
}

/*
 *  start에서 finish까지 몇 줄?
 */
unsigned int line_count(unsigned char *start, unsigned char *finish)
{
  unsigned int line = 0;

  while( finish > start)
    if(*start++ == CR && *start == LF)
      ++line;
  return(line);
}

/*
 */
static unsigned int block_line_count(unsigned char *start, unsigned char *finish)
{
  if((unsigned int)(finish-start) < (unsigned int)(eofm-(finish-start)))
    return(line_count(start, finish));
  else
    return(max_line-line_count(memory, start)-line_count(finish, eofm));
}

/*
 *  현재 포인터에서 size만큼 지운다.
 */
static unsigned int delete(unsigned int size)
{
  unsigned char *del_e;
  unsigned int line = 0;

  if(eofm == current || size == 0)
    return false;
  del_e = current + size;
  if((line = block_line_count(current, del_e)) != 0)
    max_line -= line;
  else
    user_task |= TASK_DISPLAYLINE;
  movmem(del_e, current, (unsigned int)(eofm-del_e+1));
  eofm -= size;
  saved = false;
  if(last_pos > current)
    last_pos = (last_pos > del_e) ? last_pos-size : current;
  if(block_size) {
    if(current<=b_b_ptr && b_k_ptr <= del_e) {
      b_b_ptr = b_k_ptr = current;
    }
    else {
      if(del_e<= b_b_ptr) {
        b_b_ptr -= size, b_k_ptr -= size;
      }
      else if(current <= b_b_ptr) {
        b_b_ptr=current, b_k_ptr -= size;
      }
      else if(del_e <= b_k_ptr)
        b_k_ptr -= size;
      else if(current<=b_k_ptr)
        b_k_ptr = current;
    }
    block_size = (unsigned int)(b_k_ptr - b_b_ptr);
  }
  return true;
}

/*
 *  상태줄
 *           1         2         3         4         5         6         7         8
 *  12345678901234567890123456789012345678901234567890123456789012345678901234567890
 *   한글 2벌식 |0000줄 000칸 |  삽입 |  들여쓰기 |  FILENAME.EXT
 */

void disp_kbd_status(void)
{
  unsigned char kbd_msg[40];

  strcpy(kbd_msg, (HanKbdKind == HANKBD2) ? "한글 2벌식" : "한글 3벌식");
  writef(1, 1+strlen(kbd_msg)-1, hgetmaxax()-1, HanKbdState ? kbd_msg : "영문  미국", REVERSE);
}

static void disp_coord_status(void)
{
  unsigned char msg[40];

  sprintf(msg, "%4u줄%4u칸", base_y+cury+1, base_x+curx+1);
  writef(13, 13+strlen(msg)-1, hgetmaxax()-1, msg, REVERSE);
}

static void disp_ins_status(void)
{
  writef(29, 29+strlen("삽입")-1, hgetmaxax()-1, insert_mode ? "삽입" : "수정", REVERSE);
}

void disp_indent_status(void)
{
  writef(37, 37+strlen("들여쓰기")-1, hgetmaxax()-1, indent_mode ? "들여쓰기" : "내어쓰기", REVERSE);
}


void disp_file_status(void)
{
  writef(49, 49+8+4-1, hgetmaxax()-1, split_filename(work_file), REVERSE);
}

void disp_status(void)
{
  unsigned char msg[80], kbd_msg[40];

  strcpy(kbd_msg, (HanKbdKind == HANKBD2) ? "한글 2벌식" : "한글 3벌식");
  sprintf(msg, " %s│%4u줄 %3u칸 │ %s │ %s │ %s",
      HanKbdState ? kbd_msg : "영문  미국",
      base_y+cury+1, base_x+curx+1,
      (insert_mode) ? "삽입" : "수정",
      (indent_mode) ? "들여쓰기" : "내어쓰기", split_filename(work_file));
  writef(0, hgetmaxax()-1, hgetmaxay()-1, msg, REVERSE);
}

static int in_block(unsigned char *ptr)
{
  return(b_b_ptr <= ptr && ptr < b_k_ptr);
}

void block_decision( unsigned char **block_ptr)
{
  *block_ptr = current;
  block_size = (b_b_ptr >= b_k_ptr) ? 0 : (unsigned int)( b_k_ptr-b_b_ptr);
  block_display = YES;
  user_task = TASK_DISPLAYPAGE;
}

void block_copy( void)
{
  unsigned char *temp_ptr;

  if(!in_block(current) && insert(block_size,' ')) {
    max_line += block_line_count(b_b_ptr,b_k_ptr) ;
    movmem(b_b_ptr, current, block_size);
    b_b_ptr = current;
    b_k_ptr = b_b_ptr+ block_size;
    user_task = TASK_DISPLAYPAGE;
  }
  else if(current == b_b_ptr) {
    temp_ptr = current;
    current = b_k_ptr;
    block_copy();
    b_k_ptr = b_b_ptr;
    current = b_b_ptr = temp_ptr;
  }
}

static void line_after_block_move( void )
{
  unsigned char *homep;
  homep = pre_ptr( line_start, cury );
  if( homep <= b_b_ptr )
    cury  -=  line_count( b_b_ptr , b_k_ptr );
  else if ( homep <= b_k_ptr ) {
    base_y  -= line_count( b_b_ptr , homep );
    cury = line_count( b_k_ptr , current);
  }
  else base_y -= line_count(b_b_ptr , b_k_ptr);
  saved = NO;
}

void block_move(void)
{
  unsigned char buffer[2001], *start, *mid, *endp;
  unsigned int count, len, size;

  if( in_block( current ) ) return ;
  if( b_k_ptr <= current ) {
    start =  b_b_ptr; mid = b_k_ptr; endp =current;
    line_after_block_move();
    b_b_ptr += (current - b_k_ptr);
  }
  else {
    start = current;
    mid = b_b_ptr;
    endp = b_k_ptr;
    b_b_ptr = current;
  }
  len = ( unsigned int)(endp - start );
  count = (unsigned int) (mid - start );
  while( count > 0) {
    size = ( count >= MAXBLOCKSIZE )? MAXBLOCKSIZE :count ;
    count -= size;
    strncpy( buffer, start, size );
    movmem(start + size, start, len - size );
    strncpy(endp - size, buffer, size );
  }
  b_k_ptr = b_b_ptr + block_size;
  line_start = pre_ptr( b_b_ptr, 0);
  cur_most_x = (unsigned int)(b_b_ptr-line_start);
  hor_locate(cur_most_x);
  user_task = TASK_DISPLAYPAGE;
  saved = NO;
}

void block_delete( void )
{
  unsigned char *temp;

  if( in_block( current ) )
    to_pointer( b_b_ptr );
  else if( b_k_ptr  <= current ) {
    line_after_block_move();
    current -= block_size;
  }
  temp = current;
  current = b_b_ptr;
  delete( block_size );
  current = temp ;
  user_task = TASK_DISPLAYPAGE;
  line_start = pre_ptr( current , 0 );
  cur_most_x = (unsigned int)(current - line_start);
  hor_locate(cur_most_x);
}

void block_write(void)
{
  char temp_name[80], temp_file[80], temp_block[80];

  if(b_k_ptr > b_b_ptr) {
    strcpy(temp_name, block_file);
    if(get_filename_to_write(temp_name) != CANCEL_ANS) {
      strcpy(temp_file, temp_name);
      if(files(temp_name) != NULL) {
        if(!strchr(temp_name, '.'))
          strcat(temp_name, file_ext);
        if(file_or_dir_exist(temp_name)) {
          if(yesno(" 파일이 이미 있는데, 덮어 쓸까요 ? ", NO_ANS) == YES_ANS)
            save_file(b_b_ptr, b_k_ptr, temp_name);
        }
      }
      if(strchr(temp_file, '*') || strchr(temp_file, '?')) {
        truncate_filename(block_file);
        sprintf(temp_block, "%s%s", block_file, split_filename(temp_file));
        strcpy(block_file, temp_block);
      }
      else if(temp_file[0] == NULL) {
        sprintf(temp_file, "*%s", file_ext);
        truncate_filename(block_file);
        sprintf(temp_block, "%s%s", block_file, temp_file);
        strcpy(block_file, temp_block);
      }
    }
  }
}

void block_read(void)
{
  char temp_name[80], temp_file[80], temp_block[80];
  char *temp_b_ptr = b_b_ptr;
  unsigned temp_size = block_size;

  strcpy(temp_name, block_file);
  if(get_filename_to_read(temp_name) == YES_ANS) {
    strcpy(temp_file, temp_name);
    if(files(temp_name) != NULL) {
      if(!strchr(temp_name, '.'))
        strcat(temp_name, file_ext);
      b_b_ptr = current;
      if(!file_or_dir_exist(temp_name)) {
        disperror(" 그런 파일은 없는데요 ");
        return;
      }
      block_size = load_file(temp_name, BLOCK_LOAD);
      if(block_size) {
        b_k_ptr = current+block_size;
        user_task = TASK_DISPLAYPAGE;
        block_display = YES;
        saved = NO;
      }
      else {
        b_b_ptr = temp_b_ptr;
        block_size = temp_size;
      }
    }
    if(strchr(temp_file, '*') || strchr(temp_file, '?')) {
      truncate_filename(block_file);
      sprintf(temp_block, "%s%s", block_file, split_filename(temp_file));
      strcpy(block_file, temp_block);
    }
    else if(temp_file[0] == NULL) {
      sprintf(temp_file, "*%s", file_ext);
      truncate_filename(block_file);
      sprintf(temp_block, "%s%s", block_file, temp_file);
      strcpy(block_file, temp_block);
    }
  }
}

static unsigned char *one_line(unsigned char *start, int y)
{
  unsigned char *ptr = start;
  int pos = 0, limit, block_inst, tmp= 0;
  unsigned int size;
  bool onoff;

  size = linewidth(start);
  block_inst = block_size && block_display;
  hgotoxy(1, y+1);
  if(base_x < size) {
    ptr += base_x;
    limit = (base_x+maxwidth-1 >= size) ? size-base_x : maxwidth;
    for( ;pos < limit; ++pos) {
      if(block_inst && b_b_ptr <= ptr && ptr < b_k_ptr) {
        onoff = isreverse();
        hsetreverse(ON);
        if(*ptr == '\t') {
          *ptr = ' ';
          insert_ptr(_CW->tabsize-((pos) % _CW->tabsize)-1, ' ', ptr);
          limit += _CW->tabsize-((pos) % _CW->tabsize);
          tmp += _CW->tabsize-((pos) % _CW->tabsize)-1;
        }
        else {
          if(*ptr & 0x80) {
            if(pos != limit-1) {
              _hputchxy(*ptr, *(ptr+1), hwherex(), hwherey());
              hgotoxy(hwherex()+2, hwherey());
              ptr += 2;
              pos++;
            }
            else {
              _eputch(' ');
            }
          }
          else {
            if(is_eol(ptr)) {
              pos++;
              ptr += 2;
            }
            else {
              _eputchxy(*ptr++, hwherex(), hwherey());
              hgotoxy(hwherex()+1, hwherey());
            }
          }
        }
        hsetreverse(onoff);
      }
      else {
        if(*ptr == '\t') {
          *ptr = ' ';
          insert_ptr(_CW->tabsize-((pos) % _CW->tabsize)-1, ' ', ptr);
          limit += _CW->tabsize-((pos) % _CW->tabsize);
          tmp += _CW->tabsize-((pos) % _CW->tabsize)-1;
        }
        else {
          if(*ptr & 0x80) {
            if(pos != limit-1) {
              _hputchxy(*ptr, *(ptr+1), hwherex(), hwherey());
              hgotoxy(hwherex()+2, hwherey());
              ptr += 2;
              pos++;
            }
            else {
              _eputch(' ');
            }
          }
          else {
            if(is_eol(ptr)) {
              ptr += 2; pos++;
            }
            else {
              _eputchxy(*ptr++, hwherex(), hwherey());
              hgotoxy(hwherex()+1, hwherey());
            }
          }
        }
      }
    }
  }
  while(pos++ < maxwidth) {
    if(block_inst && in_block(start+size)) {
      onoff = isreverse();
      hsetreverse(ON);
      _eputch(' ');
      hsetreverse(onoff);
    }
    else {
      _eputch(' ');
    }
  }
  start += (size+2)+tmp;
  hputch(-1);  /* hputch buffer clear */
  return start;
}

static void display_page(int kbd_check)
{
  unsigned char *ptr;
  int y, last;

  user_task = TASK_DISPLAYPAGE;
  one_line(line_start, cury);
  if(base_y+maxheight-2 < max_line)
    last = maxheight - 1;
  else {
    last = max_line - base_y;
    hclrscrxy(1, last+1, -maxwidth, -(maxheight-last));
  }
  ptr = pre_ptr(line_start, cury);
  for(y = 0; y <= last; y++) {
    if(kbd_check && keypressed())
      return;
    ptr = one_line(ptr, y);
  }
  user_task = 0;
}

static void hor_locate(unsigned int pos)
{
  if (pos < base_x || base_x+maxwidth-1 <= pos) {
    curx = (pos < base_x) ? 0 : maxwidth-2;
    base_x = pos - curx;
    user_task = TASK_DISPLAYPAGE;
  }
  else curx = pos - base_x;
  last_pos = current;
  current = line_start + pos;
  disp_coord_status();
}

static void ver_locate(unsigned int line_no,unsigned char *destin)
{
  if(line_no < base_y || base_y+maxheight-2 < line_no) {
    if(cury == 0 && line_no+1 == base_y) {
      user_task |= TASK_DISPLAYLINE+TASK_SCROLLDOWN;
    }
    else if(cury == maxheight-2 && base_y+maxheight-1 == line_no) {
      user_task |= TASK_SCROLLUPPAGE+TASK_DISPLAYLINE+TASK_DISPLAYDOWNLINE;
    }
    else
      user_task = TASK_DISPLAYPAGE;
    if(line_no < cury && line_no < base_y)
      cury = line_no;
    base_y = line_no - cury;
  }
  else
    cury = line_no - base_y;
  strncpy(line_buffer, line_start, MAXLINEBUF);
  hor_locate((unsigned int)(destin-line_start));
}

void to_pointer(unsigned char *destin)
{
  unsigned int line_no;

  line_no = base_y + cury;
  if(destin <= current) {
    if((unsigned int)(destin-memory) < (unsigned int)(current-destin))
      line_no = line_count(memory, destin);
    else
      line_no -= line_count(destin, current);
  }
  else {
    if((unsigned int)(destin-current) < (unsigned int)(eofm-destin))
      line_no += line_count(current, destin);
    else
      line_no = max_line - line_count(destin, eofm);
  }
  line_start = pre_ptr(destin, 0);
  cur_most_x = (unsigned int)(destin - line_start);
  ver_locate(line_no,destin);
}

static void left(void)
{
  if(memory < current) {
    if(current == line_start)
      to_pointer(current-2);
    else {
      if(ishangul2nd(line_start, curx+base_x-1)) {
        cur_most_x = base_x+curx-2;
        hor_locate(cur_most_x);
      }
      else {
        cur_most_x = base_x+curx-1;
        hor_locate(cur_most_x);
      }
    }
  }
}

static int is_eol(unsigned char *ptr)
{
  return (*ptr==CR && *(ptr+1)==LF);
}

static int is_eof(unsigned char *ptr)
{
  return (*ptr == 0x1a || *ptr == 0 || ptr == eofm);
}

static void han_right(void)
{
  if(current < eofm) {
    cur_most_x = base_x+curx+2;
    hor_locate(cur_most_x);
  }
}

static void right(void)
{
  if(current < eofm) {
    if(is_eol(current))
      to_pointer(current+2);
    else {
      if(ishangul1st(line_start, curx+base_x))
        han_right();
      else {
        cur_most_x = base_x+curx+1;
        hor_locate(cur_most_x);
      }
    }
  }
}

static void del(void)
{
  if(is_eol(current)) {
    if(delete(2))
      user_task |= TASK_SCROLLUP+TASK_DISPLAYLINE+TASK_DISPLAYBOTLINE;
  }
  else {
    if(ishangul1st(line_start, curx+base_x))
      delete(2);
    else
      delete(1);
  }
}

static void english(int ch)
{
  if(insert_mode || is_eol(current) || is_eof(current)) {
    if(insert(1, ' ') == false)
      return;
  }
  if(eofm >= current) {
    if(ishangul1st(line_start, curx+base_x))
      *(current+1) = ' ';
    *current = (unsigned char)ch;
    user_task |= TASK_DISPLAYLINE;
    saved = false;
    right();
  }
}

static void hanja(void)
{
  unsigned int hangulcode, hanjacode;

  if(isalnum(*current) || is_eol(current) || is_eof(current))
    return;
  if(eofm >= current) {
    if(ishangul1st(line_start, curx+base_x)) {
      hangulcode = ((*current) << 8) + *(current+1);
      hanjacode = findhanja(hgetmaxax()/2 - 14, 8, *current, *(current+1));
      if(hangulcode != hanjacode) {  /* 한자가 선택 되었음 */
        *current = (hanjacode & 0xFF00) >> 8;
        *(current+1) = hanjacode & 0x00FF;
        user_task |= TASK_DISPLAYLINE;
        saved = false;
        han_right();
      }
    }
  }
}

/*
 *  KS Graphic Character
 *
 *  blank is 217:49
 *
 *  217:49 ~ 126, 145 ~ 160, blank, blank, 161 ~ 254, blank
 *  218:49 ~ 126, 145 ~ 160, blank, blank, 161 ~ 254, blank
 *  ....
 *  ....
 *  222:49 ~ 126, 145 ~ 160, blank, blank, 161 ~ 254, blank
 */
static unsigned int get_ks_graphic_char(void)
{
  int x, y, isquit, flag;
  int blank[] = { 217, 49 };
  unsigned int key, int_bcode, ans;
  unsigned char bcode1, bcode2;
  static int bx = 0, by = 0;
  static unsigned int code1 = 217, code2;
  bool onoff;

  isquit = ans = 0;
  flag = 1;

  if(wopen(hgetmaxax()/2-16, hgetmaxay()/2-4, 32, 12))
    wtitle("▣ ㉿ 그림 문자 입력 ▣");
  else
    return 0;
  do {
    ans = 0;
    if( flag) {
      code2 = 49;
      _hputchxy(blank[0], blank[1], 1, 1);

      for(y = 0; y < 12; y++) {
        for(x = 0; x < 32; x+=2, code2++) {
          if(x == 0 && y == 0)
            x = 2;
          if(code2 == 127)
            code2 = 145;
          else if(code2 == 161) {
            _hputchxy(blank[0], blank[1], x+1, y+1);
            x+=2;
            if(x == 32)
              x = 0, y++;
            _hputchxy(blank[0], blank[1], x+1, y+1);
            x+=2;
            if(x == 32)
              x = 0, y++;
          }
          else if(code2 == 255) {
            _hputchxy(blank[0], blank[1], x+1, y+1);
            break;
          }
          _hputchxy(code1, code2, x+1, y+1);
        }
      }
    }
    if((bx == 0 && by == 0) || (bx == 30 && by == 11)
        || (bx == 30 && by == 5) || (bx == 0 && by == 6))
      bcode1 = blank[0], bcode2 = blank[1];
    else {
      bcode1 = code1, int_bcode = bx/2 + by*16 + 48;
      int_bcode = (int_bcode >= 127) ? int_bcode + 18 : int_bcode;
      bcode2 = (int_bcode > 160) ? int_bcode - 2 : int_bcode;
    }
    onoff = isreverse();
    if(!onoff)
      hsetreverse(ON);
    _hputchxy(bcode1, bcode2, bx+1, by+1);
    key = getxch();
    hsetreverse(onoff);
    _hputchxy(bcode1, bcode2, bx+1, by+1);
    flag = 0;
    switch(key) {
      case LEFTARROW :
        if(bx == 0) {
          if(by > 0)
            by--, bx = 30;
        }
        else
          bx-=2;
        break;
      case RIGHTARROW :
        if(bx == 30) {
          if(by < 11)
            by++, bx = 0;
        }
        else
          bx+=2;
        break;
      case UPARROW :
        if(by > 0)
          by--;
        break;
      case DOWNARROW :
        if(by < 11)
          by++;
        break;
      case PGUPKEY :
        if(code1 > 217)
          code1--;
        else
          code1 = 222;
        flag = 1;
        break;
      case PGDNKEY :
        if(code1 < 222)
          code1++;
        else
          code1 = 217;
        flag = 1;
        break;
      case ESC :
        isquit = 1;
        break;
      case '\r' :
        ans = ((bcode1 << 8) & 0xFF00) + (bcode2);
        isquit = 1;
        break;
    }
  } while (!isquit);
  wclose();
  return ans;
}

static void ks_graphic_char(void)
{
  unsigned int ans, flag;

  flag = 0;
  if(insert_mode || is_eol(current) || is_eof(current)) {
    if(insert(2, ' ') == false)
      return;
    flag = 1;
  }
  if(eofm >= current) {
    if((ans = get_ks_graphic_char()) != 0) {
      if(ishangul1st(line_start, curx+base_x+1))
        *(current+2) = ' ';
      *current = (ans & 0xFF00) >> 8;
      *(current+1) = ans & 0x00FF;
      user_task |= TASK_DISPLAYLINE;
      saved = false;
      han_right();
    }
    else if(flag) {
      delete(2);
    }
  }
}

/*
 *  TG Graphic Character
 *
 *  212:128 ~ 212:255
 */
static unsigned int get_tg_graphic_char(void)
{
  int x, y, isquit;
  unsigned int key, ans;
  int code1, code2;
  static int bx = 0, by = 0;
  static unsigned int bcode1 = 212, bcode2 = 218;
  bool onoff;

  ans = isquit = 0;

  if(wopen(hgetmaxax()/2-16, hgetmaxay()/2-4, 32, 8))
    wtitle("▣ ∴ 그림 문자 입력 ▣");
  else
    return 0;

  code1 = 212, code2 = 128;
  for(y = 0; y < 8; y++) {
    for(x = 0; x < 32; x+=2, code2++) {
      _hputchxy(code1, code2, x+1, y+1);
    }
  }

  do {
    bcode1 = code1, bcode2 = bx/2 + by*16 + 128;
    onoff = isreverse();
    if(!onoff)
      hsetreverse(ON);
    _hputchxy(bcode1, bcode2, bx+1, by+1);
    key = getxch();
    hsetreverse(onoff);
    _hputchxy(bcode1, bcode2, bx+1, by+1);
    switch(key) {
      case LEFTARROW :
        if(bx == 0) {
          if(by > 0)
            by--, bx = 30;
        }
        else
          bx-=2;
        break;
      case RIGHTARROW :
        if(bx == 30) {
          if(by < 7)
            by++, bx = 0;
        }
        else
          bx+=2;
        break;
      case UPARROW :
        if(by > 0)
          by--;
        break;
      case DOWNARROW :
        if(by < 7)
          by++;
        break;
      case ESC :
        isquit = 1;
        break;
      case '\r' :
        ans = ((bcode1 << 8) & 0xFF00) + (bcode2);
        isquit = 1;
        break;
    }
  } while(!isquit);
  wclose();
  return ans;
}

static void tg_graphic_char(void)
{
  unsigned int ans, flag;

  flag = 0;
  if(insert_mode || is_eol(current) || is_eof(current)) {
    if(insert(2, ' ') == false)
      return;
    flag = 1;
  }
  if(eofm >= current) {
    if((ans = get_tg_graphic_char()) != 0) {
      if(ishangul1st(line_start, curx+base_x+1))
        *(current+2) = ' ';
      *current = (ans & 0xFF00) >> 8;
      *(current+1) = ans & 0x00FF;
      user_task |= TASK_DISPLAYLINE;
      saved = false;
      han_right();
    }
    else if(flag) {
      delete(2);
    }
  }
}

static void hangul(void)
{
  if(insert_mode || is_eol(current) || is_eof(current)) {
    if(insert(2, ' ') == false)
      return;
  }
  else if(!insert_mode)
    user_task |= TASK_DISPLAYLINE;
  if(eofm >= current) {
    if(ishangul1st(line_start, curx+base_x+1)) {
      *(current+1) = *(current+2) = ' ';   /* 93.07.05 */
      user_task |= TASK_DISPLAYLINE;
      user_func(0);
    }
    saved = false;
  }
}

static void up_down(int delta)
{
  unsigned char *destin;
  unsigned int line_no, size;

  if(delta <= 0) {
    delta = -delta;
    line_no = (delta > base_y+cury) ? 0 : base_y+cury-delta;
    line_start = pre_ptr(line_start, delta);
  }
  else {
    line_no = base_y+cury;
    if(delta > max_line-line_no)
      delta = max_line-line_no;
    line_no += delta;
    line_start = next_ptr(line_start, delta);
  }
  size = linewidth(line_start);
/*  destin = line_start + ((base_x+curx > size) ? size : base_x+curx);
  if(ishangul2nd(line_start, (base_x+curx > size) ? size : base_x+curx))
    destin--;
 */
  destin = line_start + ((cur_most_x > size) ? size : cur_most_x);
  if(ishangul2nd(line_start, (cur_most_x > size) ? size : cur_most_x))
    destin--;
  ver_locate(line_no,destin);
}

static void del_eol(void)
{
  delete(linewidth(current));
}

static void del_word(void)
{
  while(eofm > current && in_word(current))
    del();
  while(eofm > current && !in_word(current))
    del();
}

static void ins_return(void)
{
  unsigned int pos = 0;

  if(indent_mode) {
    pos = strspn(line_start, " ");
    if(pos >= base_x+curx)
      pos = base_x;
  }
  if(insert(pos+2, CR)) {
    user_task |= TASK_SCROLLDOWN+TASK_DISPLAYLINE+TASK_DISPLAYUPLINE;
    memset(current+2, ' ', pos);
    to_pointer(current+pos+2);
  }
}

static int in_word(unsigned char *ch)
{
  return (strchr(" \r\n<>,:.()[]^'*+-/$[", *ch) == NULL);
}

static void word_left(void)
{
  unsigned char *ptr =  current;

  while(memory < ptr && !in_word(ptr-1))
    --ptr;
  while(memory < ptr &&  in_word(ptr-1))
    --ptr;
  to_pointer(ptr);
}

static void word_right( void )
{
  unsigned char *ptr = current;

  while(eofm > ptr && in_word(ptr))
    ++ptr;
  while(eofm > ptr && !in_word(ptr))
    ++ptr;
  to_pointer(ptr);
}

static void restore_line( void )
{
  unsigned char *pt = line_buffer;
  unsigned int buf_size = 0;

  cur_most_x = 0;
  hor_locate(0);
  while(buf_size < MAXLINEBUF && *pt && !is_eol(pt)) {
    ++buf_size;
    ++pt;
  }
  change_line(linewidth(current), buf_size, line_buffer);
}

/*
 *  찾기 부분
 */
static unsigned char first_key = 0xFF, keys[41], object[41], option[41];
static unsigned char replace_str[41], word_f = NO, find_flag = NO;
static unsigned char replace = NO, len = 0;
static int sense = 1, f_count = 1;

static int compare(unsigned char *st1)
{
  int count = 1, same = YES;
  unsigned char *st2 = object, *key = keys;

  while( count++ < len )
    if( (*++st1 - *++st2) & *++key ) {
      same = NO; break;
    }
  return (same);
}

static int find( void )
{
  unsigned char *pt = current;
  int left, right, success;
  unsigned int line = base_y+cury;
  static int found = 0;

  do {
    success = NO;
    while(*pt) {
      if(*pt == LF)
        line += sense;
      else if( !((*pt - *object)&first_key ) && compare(pt) ) {
        left = right = YES;
        if( word_f ) {
          left = (*(pt-1) <= ' ' );
          right = (*(pt+len) <= ' ');
        }
        if(left && right) {
          success = YES;
          ++found;
          break;
        }
      }
      pt+=sense;
    }
    if(found > f_count) {
      success = NO;
      break;
    }
    if(!*pt || success == YES)
      break;
    pt += sense;
  } while( YES );

  if(success) {
    line_start = pre_ptr( pt ,0 );
    ver_locate( line , pt);
    find_flag = YES;
  }
  else {
    if(found <= f_count) {
      errno = (find_flag) ? 39 : 37;  /* 더이상 없다 : 그런거 없다 */
      errorf();
    }
    found = 0;
    find_flag = NO;
  }
  return success;
}

static int change_line(int mem_size, int buf_size, unsigned char *buf_ptr)
{
  int success;

  success = ( buf_size < mem_size ) ? delete( (unsigned int) (mem_size-buf_size) ) :
            insert( (unsigned int) (buf_size-mem_size) , ' ');
  if(success)
    strncpy(current, buf_ptr, buf_size);
  return success;
}

static void find_replace( void )
{
  #define  ASK         1
  #define  NOT_ASK     2
  int repeat , key = 0 , replace_len , delta, mode;
  bool onoff;

  if(strchr(option, 'N'))
    replace = NOT_ASK;
  if(strchr(option, 'G')) {
    sense = 1;
    repeat = YES;
    to_pointer(memory);
    user_task = TASK_DISPLAYPAGE;
  }
  repeat = YES;
  replace_len = strlen(replace_str);
  while(find()) {
    switch( replace ) {
      case ASK :
        if(user_task)
          user_func(true);
        delta = len ;
        writef(0, 16, hgetmaxay()-1, "[바꿀까요? (Y/N)]", NORMAL);
        mode = HanKbdState, HanKbdState = false;
        onoff = isreverse();
        hsetreverse(ON);
        hprintfxy(curx+1, cury+1, "%s", object);
        hsetreverse(onoff);
        key = toupper(getxch());
        HanKbdState = mode;
        hprintfxy(curx+1, cury+1, "%s", object);
        disp_status();
        if( key != 'Y' )
          break;
      case NOT_ASK :
        delta = replace_len;
        if(change_line(len, replace_len, replace_str) == NO)
          repeat = NO;
        user_func(true);
    }
    if(keypressed()) key = getxch();
    if(repeat==NO || key==ESC) break;
    to_pointer(current+delta);
  }
}

static void find_string( int repeat_last_find )
{
  if(*object) {
    if( sense == 1 ) {
      if( repeat_last_find)
        right();
    }
    else if( repeat_last_find || current == eofm)
      left();
    if(replace)
      find_replace();
    else
      find();
  }
}

void  find_condition(int is_replace)
{
  unsigned char *op; int i;

  if(win_hgets(40, "찾을 문자열은 ?", object))  {
    if(is_replace && !win_hgets(40, "바꿀 문자열은 ?", replace_str))
      return;
    strcuttail(object);
    strcuttail(replace_str);
    /* 93.07.18 */
    replace = is_replace;
    if(win_hgetdata(40, "선택사항 (G B U W N 숫자)", "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE", option)) {
      strcuttail(option);
      strupr( option );
      first_key = 0xFF;
      if(strchr(option, 'U') && !(object[0] & 0x80)) {
        strupr(object); /* When object string is NOT hangul */
      }
      len = (unsigned char)strlen(object);
      for(i = 0; i < len; i++ )
        keys[i] = (object[i] == 1) ? 0 : first_key;
      first_key = keys[0];
      f_count = 1;  /* find count */
      for(op = option; *op && !isdigit(*op); op++)
        ;
      f_count = atoi(op);
      if(f_count == 0)
        f_count = 1;
      sense = (strchr(option, 'B')) ? -1 :1 ; /* 탐색 방향 */
      word_f = (strchr(option , 'W')) ? YES :NO;
      find_string(NO);
    }
  }
}

static void ctrl_k(int key)
{
  if(strchr("CVMWY", (unsigned char) key +'@' ) \
      && ( block_size ==0 || block_display == NO) )
    return;
  switch( key ) {
    case  2:    /* B */
      block_decision( &b_b_ptr);
      break;
    case 11:    /* K */
      block_decision( &b_k_ptr );
      break;
    case 8 :    /* H */
      block_display = !block_display;
      user_task = TASK_DISPLAYPAGE;
      break;
    case 3 :    /* C */
      block_copy();
      break;
    case 22 :   /* V */
      block_move();
      break;
    case 25 :   /* Y */
      block_delete();
      break;
    case 18 :   /* R */
      block_read();
      break;
    case 23 :   /* W */
      block_write();
      break;
  }
}

static void ctrl_q(int key)
{
  switch( key ) {
    case 2 :    /* B */
      to_pointer(b_b_ptr);
      break;
    case 11:    /* K */
      to_pointer(b_k_ptr);
      break;
    case 16:    /* P */
      to_pointer(last_pos);
      break;
    case 12:    /* L */
      restore_line();
      break;
    case 6:     /* F */
      find_condition(NO); /* replace NO */
      break;
    case 1:     /* A */
      find_condition(YES); /* replace YES */
      break;
    case 9:     /* I */
      indent_mode = !(indent_mode);
      disp_indent_status();
      break;
    case 25:    /* Y */
      del_eol();
      break;
  }
}

void errorf( void )
{
  unsigned char *errmsg[] = {
    "파일이 너무 커요",
    "그런 문자열이 없습니다",
    "새로운 문서입니다",
    "더 이상 없습니다"
  };

  disperror((errno>=36) ? errmsg[errno-36] : hsys_errlist[errno]);
  errno = 0;
}

bool set_tabsize(int size)
{
  if(size > 0 && size <= 8) {
    _CW->tabsize = size;
    return true;
  }
  return false;
}

int get_tabsize(void)
{
  return _CW->tabsize;
}

void set_max_width_height(int width, int height)
{
  maxwidth = width, maxheight = height;
}

void draw_border(int x, int y, int width, int height, int border)
{
  releasehwindow();
  hwindow(1, 1, hgetmaxax(), hgetmaxay());
  if(border)
    drawbox((x-1), (y-1), (width+2), (height+2), 1);
  sethwindow(x, y, -width, -height);
}

void init_editor_variable(void)
{
  line_start = current = last_pos = b_b_ptr = b_k_ptr = eofm = memory;
  *memory = *line_buffer = curx = cury = 0;
/*  *replace_str = *object = *option = *block_file = 0; */
  max_line = base_y = base_x = block_size = 0;
  sprintf(work_file, "%s%s", init_file_name, file_ext);
  saved = block_display = true;
  user_task = TASK_DISPLAYPAGE;
}

/*
 *  편집기에 쓰이는 각 변수 초기화
 *  filename : 편집할 파일이름, NULL이면 "UNTITLED.TXT"라는 이름을 준다.
 *  bufsize  : 최대 편집크기, 0이면 DEFAULTBUFSIZE를 설정한다.
 *  kbd_kind : 3벌식; HANKBD3   2벌식; HANKBD2
 *  hangul   : 한글상태로 시작; true   영문상태로 시작; false
 */
bool init_editor(char *filename, char *help_file_name, long bufsize, int kbd_kind, bool hangul)
{
  if(bufsize == 0L)
    bufsize = DEFAULTBUFSIZE;
  maxeditbuf = bufsize;
  if((memory = (unsigned char *)farmalloc(bufsize+2)) == NULL) {
    memerror();
    return false;
  }
  *memory++ = 0;    /* 편집버퍼의 처음에 NULL을 넣어준다 */

  line_start = current = last_pos = b_b_ptr = b_k_ptr = eofm = memory;
  *memory = *line_buffer = curx = cury = 0;
  *replace_str = *object = *option = *block_file = 0;
  max_line = base_y = base_x = block_size = 0;
  saved = block_display = true;
  user_task = TASK_DISPLAYPAGE;

  if(filename[0] == NULL)
    sprintf(work_file, "%s%s", init_file_name, file_ext);
  else {
    strcpy(work_file, filename);
    load_file(work_file, INIT_LOAD);
  }

  HanKbdKind = kbd_kind;
  HanKbdState = hangul;
  strcpy(help_file, help_file_name);

  return true;
}

/*
 *  편집창의 크기는 최소 (5 x 3), 최대 (80 x 23)
 */

bool editor(int x, int y, int width, int height, int border)
{
  int maxx, maxy, tmp_tabsize;
  int key, state, tmp_state, done;
  int c_break;

  tmp_state = done = 0;

  maxx = hgetmaxax();
  maxy = hgetmaxay();
  /*
   *  적당하게 좌표가 지정되었는지 검사한다.
   */
  if((x < (border ? 1 : 0)) || (x > (border ? maxx-MIN_SCR_COL : maxx-MIN_SCR_COL+1)) ||\
     (y < (border ? 1 : 0)) || (y > (border ? maxy-MIN_SCR_LINE : maxy-MIN_SCR_LINE+1)) ||\
     (width < MIN_SCR_COL) || (width > (border ? MAX_SCR_COL-2 : MAX_SCR_COL)) ||\
     (height < MIN_SCR_LINE) || (height > (border ? MAX_SCR_LINE-2 : MAX_SCR_LINE))) {
    return false;
  }

  set_max_width_height(width, height);

  draw_border(x, y, width, height, border);

  disp_status();
/*  sethwindow(x, y, -width, -height); */

  c_break = getcbrk();
  if(c_break) /* BREAK is ON */
    setcbrk(0);  /* make BREAK is OFF */

  do {
    if(all_jobs_close)
      key = quit_key;
    else
      key = get_key(curx, cury, &state);

    /*
     *  사용자 정의 키를 처리
     */
    if(key == hanja_conv_key) {            /* hanja convert key */
      if(hanja_font_exist) {
        if(tmp_state == HR_HANEND)
          left();
        tmp_state = 0;
        hanja();
      }
      else
        disperror("한자 글꼴 파일이 없습니다");
    }
    else if(key == ks_graph_key) {         /* KS-5601 graphic character */
      ks_graphic_char();
    }
    else if(key == tg_graph_key) {         /* TG-graphic character */
      tg_graphic_char();
    }
    else if(key == kbd_toggle_key && HanKbdState) {
      beep();
      if(HanKbdKind == HANKBD2) {
        hsethan390board(1 /* split ?? */);   /* for halla */
        HanKbdKind = HANKBD3;                /* for user(=JaeRock) define */
      }
      else {
        hsethan2board(1 /* split */);         /* for halla */
        HanKbdKind = HANKBD2;                 /* for user(=JaeRock) define */
      }
      disp_kbd_status();
    }
    else if(key == quit_key) {
      if(ask_save() != CANCEL_ANS)
        done = 1;
    }
    else if(key == han_toggle_key) {          /* hangul <--> english */
      beep();
      HanKbdState = ! HanKbdState;
      disp_kbd_status();
    }
    else if(key == save_file_key) {
      save();
    }
    else if(key == help_key) {
      help_manager(help_file);
    }
    else if(key == print_file_key) {
      data_print(memory, eofm);
    }
    else if(key == menu_key) {
      (*menu_process)(menu_key);
    }
    else {
      if(key != hanja_conv_key)
        tmp_state = 0;
      switch(key) {
        case CTRL_K :
          ctrl_k(get_key2('K'));
          break;
        case CTRL_Q :
          ctrl_q(get_key2('Q'));
          break;
        case CTRL_B:  /* 제어문자 입력 */
          key = get_key2('B');
          if( (key <' ') && key && \
              (strchr("\r\n\x1A", (unsigned char) key) == NULL))
            english(key);
          break;
        case LEFTARROW :
          left();
          break;
        case RIGHTARROW :
          right();
          break;
        case CTRL_LEFT :
          word_left();
          break;
        case CTRL_RIGHT :
          word_right();
          break;
        case UPARROW :
          up_down(-1);
          break;
        case DOWNARROW :
          up_down(1);
          break;
        case PGUPKEY :
          up_down(-maxheight);
          break;
        case PGDNKEY :
          up_down(maxheight);
          break;
        case HOMEKEY :
          cur_most_x = 0;
          hor_locate(0);
          break;
        case ENDKEY :
          cur_most_x = (unsigned int)linewidth(line_start);
          hor_locate(cur_most_x);
          break;
        case CTRL_HOME :
          up_down(-cury) ;
          break;
        case CTRL_END :
          up_down(maxheight-2-cury);
          break;
        case CTRL_PGUP :
          to_pointer(memory) ;
          break;
        case CTRL_PGDN :
          to_pointer(eofm) ;
          break;
        case CTRL_L :
          find_string(YES);
          break;
        case CTRL_T :
          del_word();
          break;
        case CR :
          if(insert_mode)
            ins_return();
          else {
            cur_most_x = 0;
            hor_locate(0);
            up_down(1);
          }
          break;
        case CTRL_N :
          if(insert(2, CR))
            user_task |= TASK_SCROLLDOWN+TASK_DISPLAYDOWNLINE+TASK_DISPLAYLINE;
          break;
        case INSKEY :
          insert_mode = !insert_mode;
          disp_ins_status();
          break;
        case BS :
          if(state == HR_NOHAN) {
            delete(2);
            break;
          }
          if(current == memory)
            break;
          left();
          if(user_task > 1)
            user_func(true);
        case DELKEY :
          del();
          break;
        case CTRL_Y :
          cur_most_x = 0;
          hor_locate(0);
          if(base_y+cury == max_line)
            del_eol();
          else if(delete(linewidth(current)+2))
            user_task |= TASK_SCROLLUP+TASK_DISPLAYBOTLINE;
          break;
        case LSHIFT_SPACE : case RSHIFT_SPACE :
          HanKbdState = ! HanKbdState;
          disp_kbd_status();
          break;
        default :
          if(state == HR_ASC && ' ' <= key && key < 128) {
            if(!insert_mode && (*current & 0x80))
              *(current + 1) = ' ';
            english(key);
          }
          else if(state == HR_ASC  && key == '\t') {
            if(insert_mode)
              insert(_CW->tabsize-((curx+base_x) % _CW->tabsize), ' ');
            tmp_tabsize = _CW->tabsize-((curx+base_x) % _CW->tabsize);
            while(tmp_tabsize--)
              right();
          }
          else if(state == HR_HANST) { /* 한글 입력이 시작 */
            hangul();
          }
          else if(state == HR_HANIN || state == HR_HANBS) {
            *current = (key >> 8) & 0xFF;
            *(current + 1) = key & 0xFF;
          }
          else if(state == HR_HANEND) {
            *current = (key >> 8) & 0xFF;
            *(current + 1) = key & 0xFF;
            right();
            tmp_state = state;
          }
          else {
            if(getitem(key))
              (*menu_process)(key);
          }
      } /* switch */
    } /* else */
  }while( !done );
  releasehwindow();
  setcbrk(c_break);
  return true;
}

/*
 *  파일 kedit.c 의 끝
 */
