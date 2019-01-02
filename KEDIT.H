/*
 *  ����   : kedit.h
 *  ����   : ������ ��ü�� ���ԵǾ�� �ϴ� �κ�
 *  ��ȹ   : hantle.prj, nal.prj
 *  ��¥   : 93.03.16
 *  ������ : �Ѿ���б� ������Ű��а� ��Ʋ�Ƹ� �����
 *  �˸��� : �� Ǯ�׸��� �ر׸��� ������ ��Ģ���� �մϴ�.
 *           �ֳ��ϸ�, ���� �ٸ� �ر׸��� ������ ���� �޾����ϱ��.
 *           ����, �� �ر׸��� ������ ���� �ٸ��� ����ϼŵ� ������
 *           ���� ���� �ر׸��� �� �����ϼż� ������ �ֽñ� �ٶ��ϴ�.
 */

#include "typedefs.h"

#ifndef _KEDIT_H
#define _KEDIT_H

/*
 * ���� ���â���� �ٷ� �� �ִ� ������ �ִ� ��
 */
#define  MAX_FILES          (400)          /* ���ϻ��ڿ��� �ٷ� �� �ִ� ���� �� */
#define  MAXLINEBUF         (255)          /* ������ �ִ� �������� ���� */
#define  DEFAULTBUFSIZE     (57200L)       /* ��ü �����뷮�� �ִ�ũ��  */
#define  MAXBLOCKSIZE       (2000)         /* �������� �ٷ� �� �ִ� �ִ�ũ�� */

/*
 *  ����â�� ũ��� �ּ� (5 x 3), �ִ� (80 x 23)
 */
#define  MAX_SCR_COL    80
#define  MAX_SCR_LINE   23
#define  MIN_SCR_COL     5
#define  MIN_SCR_LINE    3

#define  HE_2LARGE  36   /* ������ �ʹ� Ŀ�� */
#define  HE_NOFIND  37   /* �׷� ���ڿ��� �����ϴ� */
#define  HE_ILFILE  38   /* ������ �����ϰų� �߸��� �̸��Դϴ� */
#define  HE_NOEXIST 39   /* �� �̻� �����ϴ� */

enum yesnocan_t { YES_ANS, NO_ANS, CANCEL_ANS };
enum yesno_t { NO, YES };
enum loadmode_t { INIT_LOAD, BLOCK_LOAD };
enum display_mode { REVERSE, NORMAL };

/*
 *  Ű �Է� �κб��� �ٽ� ���ƿ��� ��, �켱 �ؾ� �� ��
 *  ��, Ű �Է��� ���� �� � ���� �� ���ΰ�.
 */
#define  TASK_DISPLAYPAGE      0x0100    /* �� ȭ�� �ٽ� ��� */
#define  TASK_SCROLLUPPAGE     0x0040    /* ��ü ���� ȭ�� ��ũ�� */
#define  TASK_SCROLLUP         0x0020    /* ���� Ŀ�� �Ʒ� �κи� ���� ��ĭ ��ũ�� */
#define  TASK_SCROLLDOWN       0x0010    /* ���� Ŀ�� �Ʒ� �κи� �Ʒ��� ��ĭ ��ũ�� */
#define  TASK_DISPLAYUPLINE    0x0008    /* ���� ���� �̵��ؼ� �� ���� �ٽ� ǥ�� */
#define  TASK_DISPLAYDOWNLINE  0x0004    /* ���� �Ʒ��� �̵��ؼ� �� ���� �ٽ� ǥ�� */
#define  TASK_DISPLAYBOTLINE   0x0002    /* �� �Ʒ��� ǥ�� */
#define  TASK_DISPLAYLINE      0x0001    /* ���� �� �ٽ� ǥ�� */


extern unsigned char *memory;                   /* ������ �� ������ ��ŭ�� ������ �Ҵ�޴´� */
extern unsigned char *line_start;               /* ��� �� ���� ���� ������ */
extern unsigned char *current;                  /* ���� ���� ���� ��ġ */
extern unsigned char *last_pos;                 /* �ٷ� ���� ��ġ */
extern unsigned char *b_b_ptr;                  /* ������ ���� ������ */
extern unsigned char *b_k_ptr;                  /* ������ �� ������ */
extern unsigned char *eofm;                     /* ������ ������ */
extern unsigned char work_file[80];             /* �۾� ������ �̸� */
extern unsigned char block_file[80];            /* �������� ����/�о���� ������ �̸� */
extern unsigned char help_file[80];
extern unsigned char write_file[80];
extern unsigned char line_buffer[MAXLINEBUF+1]; /* ���� ���� ���� ���� �ӽ� �۾� ��� */

extern bool insert_mode, indent_mode, saved, block_display;
extern unsigned int curx, cury, user_task, max_line, base_y, base_x, block_size;
extern long maxeditbuf;

/*
 *  ������ �������� ���Ű(����, �ҷ�����)�� ���� �������̴�.
 *  ��, �ѿ� ���Ű�� �⺻������ SHIFT-SPACE�̰�, �̿ܿ� �ٸ� Ű�� ����� �� �ִ�.
 */
extern int save_file_key       ;   /* Save : �����ϱ� */
extern int print_file_key      ;   /* Print : �μ��ϱ� */
extern int help_key            ;   /* ���� Ű */
extern int han_toggle_key      ;   /* ��/�� �ٲ�Ű */
extern int hanja_conv_key      ;   /* �ѱ�->����Ű */
extern int kdb_toggle_key      ;   /* 3����/2���� �ٲ�Ű */
extern int ks_graph_key        ;   /* KS 2����Ʈ �׸����� �Է� */
extern int tg_graph_key        ;   /* TG 2����Ʈ �׸����� �Է� */
extern int menu_key            ;   /* pulldown menu */
extern int quit_key            ;   /* ������ ������ ����, ������ ������ */
extern char file_ext[]         ;   /* file extension */
extern char init_file_name[]   ;   /* �ʱ� ���� �̸� - Ȯ���� ���� */

extern bool hanja_font_exist;      /* ���� ������ �����ϴ���.. */
extern bool all_jobs_close;        /* ��� �۾��� ���� �Ǿ����� ... */

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
 *  ���� kedit.h �� ��
 */