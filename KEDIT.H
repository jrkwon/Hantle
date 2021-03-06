/*
 *  파일   : kedit.h
 *  내용   : 편집기 전체에 포함되어야 하는 부분
 *  기획   : hantle.prj, nal.prj
 *  날짜   : 93.03.16
 *  만든이 : 한양대학교 전자통신공학과 한틀아리 권재락
 *  알림글 : 이 풀그림의 밑그림은 공개를 원칙으로 합니다.
 *           왜냐하면, 나도 다른 밑그림의 도움을 많이 받았으니까요.
 *           따라서, 이 밑그림을 받으신 분은 다른데 사용하셔도 되지만
 *           새로 만든 밑그림도 잘 정리하셔서 공개해 주시기 바랍니다.
 */

#include "typedefs.h"

#ifndef _KEDIT_H
#define _KEDIT_H

/*
 * 파일 출력창에서 다룰 수 있는 파일의 최대 수
 */
#define  MAX_FILES          (400)          /* 파일상자에서 다룰 수 있는 파일 수 */
#define  MAXLINEBUF         (255)          /* 한줄의 최대 편집가능 길이 */
#define  DEFAULTBUFSIZE     (57200L)       /* 전체 편집용량의 최대크기  */
#define  MAXBLOCKSIZE       (2000)         /* 블럭으로 다룰 수 있는 최대크기 */

/*
 *  편집창의 크기는 최소 (5 x 3), 최대 (80 x 23)
 */
#define  MAX_SCR_COL    80
#define  MAX_SCR_LINE   23
#define  MIN_SCR_COL     5
#define  MIN_SCR_LINE    3

#define  HE_2LARGE  36   /* 파일이 너무 커요 */
#define  HE_NOFIND  37   /* 그런 문자열은 없습니다 */
#define  HE_ILFILE  38   /* 파일이 존재하거나 잘못된 이름입니다 */
#define  HE_NOEXIST 39   /* 더 이상 없습니다 */

enum yesnocan_t { YES_ANS, NO_ANS, CANCEL_ANS };
enum yesno_t { NO, YES };
enum loadmode_t { INIT_LOAD, BLOCK_LOAD };
enum display_mode { REVERSE, NORMAL };

/*
 *  키 입력 부분까지 다시 돌아왔을 때, 우선 해야 할 일
 *  즉, 키 입력이 없을 때 어떤 일을 할 것인가.
 */
#define  TASK_DISPLAYPAGE      0x0100    /* 전 화면 다시 출력 */
#define  TASK_SCROLLUPPAGE     0x0040    /* 전체 위로 화면 스크롤 */
#define  TASK_SCROLLUP         0x0020    /* 현재 커서 아래 부분만 위로 한칸 스크롤 */
#define  TASK_SCROLLDOWN       0x0010    /* 현재 커서 아래 부분만 아래로 한칸 스크롤 */
#define  TASK_DISPLAYUPLINE    0x0008    /* 한줄 위로 이동해서 그 줄을 다시 표시 */
#define  TASK_DISPLAYDOWNLINE  0x0004    /* 한줄 아래로 이동해서 그 줄을 다시 표시 */
#define  TASK_DISPLAYBOTLINE   0x0002    /* 맨 아래줄 표시 */
#define  TASK_DISPLAYLINE      0x0001    /* 현재 줄 다시 표시 */


extern unsigned char *memory;                   /* 시작할 때 편집할 만큼의 공간을 할당받는다 */
extern unsigned char *line_start;               /* 어느 한 줄의 시작 포인터 */
extern unsigned char *current;                  /* 현재 편집 중인 위치 */
extern unsigned char *last_pos;                 /* 바로 이전 위치 */
extern unsigned char *b_b_ptr;                  /* 블럭의 시작 포인터 */
extern unsigned char *b_k_ptr;                  /* 블럭의 끝 포인터 */
extern unsigned char *eofm;                     /* 파일의 마지막 */
extern unsigned char work_file[80];             /* 작업 파일의 이름 */
extern unsigned char block_file[80];            /* 블럭으로 저장/읽어오기 파일의 이름 */
extern unsigned char help_file[80];
extern unsigned char write_file[80];
extern unsigned char line_buffer[MAXLINEBUF+1]; /* 현재 편집 중인 줄의 임시 작업 장소 */

extern bool insert_mode, indent_mode, saved, block_display;
extern unsigned int curx, cury, user_task, max_line, base_y, base_x, block_size;
extern long maxeditbuf;

/*
 *  다음은 편집기의 기능키(저장, 불러오기)에 대한 내정값이다.
 *  단, 한영 토글키는 기본적으로 SHIFT-SPACE이고, 이외에 다른 키도 등록할 수 있다.
 */
extern int save_file_key       ;   /* Save : 저장하기 */
extern int print_file_key      ;   /* Print : 인쇄하기 */
extern int help_key            ;   /* 도움말 키 */
extern int han_toggle_key      ;   /* 한/영 바꿈키 */
extern int hanja_conv_key      ;   /* 한글->한자키 */
extern int kdb_toggle_key      ;   /* 3벌식/2벌식 바꿈키 */
extern int ks_graph_key        ;   /* KS 2바이트 그림문자 입력 */
extern int tg_graph_key        ;   /* TG 2바이트 그림문자 입력 */
extern int menu_key            ;   /* pulldown menu */
extern int quit_key            ;   /* 저장할 것인지 묻고, 편집을 끝내기 */
extern char file_ext[]         ;   /* file extension */
extern char init_file_name[]   ;   /* 초기 파일 이름 - 확장자 빼고 */

extern bool hanja_font_exist;      /* 한자 파일이 존재하는지.. */
extern bool all_jobs_close;        /* 모든 작업이 종료 되었는지 ... */

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
 *  파일 kedit.h 의 끝
 */
