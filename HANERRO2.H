/*
 *  Ïa¡‹aœ‘  : ÐeËiœa·¡§aœáŸ¡
 *  ÑÁ·© ·¡Ÿq : HANERRO2.H
 *   e—e ·¡   : Š¥ ¸ œb
 *  i¼a      : 1992. 6. 26
 *  ÑÁ·© ¶w : µ¡ŸA ÀáŸ¡  áŸ¡ ÑÁ·©
 */

#ifndef _HANERRO2_H_
#define _HANERRO2_H_

#include "typedefs.h"

#if __STDC__
#define _Cdecl
#else
#define _Cdecl cdecl
#endif

#define MAXERRNUMBER   35  /* ¯¡¯aÉQ· ˆa¸w Çe µ¡ŸA ¤åÑ¡ */

extern int errno, _doserrno;

/*  •¡¯a µ¡ŸA ¦Ñ¡ */
#define HEZERO     0    /* Error 0			*/
#define HEINVFNC	 1		/* Invalid function number	*/
#define HENOFILE	 2		/* File not found		*/
#define HENOPATH	 3		/* Path not found		*/
#define HECONTR	   7		/* Memory blocks destroyed	*/
#define HEINVMEM	 9		/* Invalid memory block address */
#define HEINVENV  10		/* Invalid environment		*/
#define HEINVFMT  11		/* Invalid format		*/
#define HEINVACC  12		/* Invalid access code		*/
#define HEINVDAT  13		/* Invalid data			*/
#define HEINVDRV  15		/* Invalid drive specified	*/
#define HECURDIR  16		/* Attempt to remove CurDir	*/
#define HENOTSAM  17		/* Not same device		*/
#define HENMFILE  18		/* No more files		*/

#define HENOENT	  2		  /* No such file or directory	*/
#define HEMFILE	  4		  /* Too many open files		*/
#define HEACCES	  5		  /* Permission denied		*/
#define HEBADF	  6		  /* Bad file number		*/
#define HENOMEM	  8		  /* Not enough core		*/
#define HENODEV	 15		  /* No such device		*/
#define HEINVAL	 19		  /* Invalid argument		*/
#define HE2BIG	 20		  /* Arg list too long		*/
#define HENOEXEC 21		  /* Exec format error		*/
#define HEXDEV	 22		  /* Cross-device link		*/
#define HEDOM	   33		  /* Math argument		*/
#define HERANGE	 34		  /* Result too large		*/
#define HEEXIST	 35		  /* File already exists		*/

#define HEFAULT	 -1		  /* Unknown error		*/

void _Cdecl memerror(void);
void _Cdecl hperror(void);
int  _Cdecl disp_msg(int x, int y, byte *msg);
int  _Cdecl disp_cmsg(byte *msg);
int  _Cdecl disp_cmsg_sec(byte *msg, int sec);
bool _Cdecl win_hgets(int maxlen, byte *title, byte *str);
bool _Cdecl win_hgetdata(int maxlen, byte *title, byte *mask, byte *str);
void _Cdecl disperror(byte *msg);
int  _Cdecl retry(byte *str, int initpos);
int  _Cdecl yesno(byte *str, int initpos);
int  _Cdecl criticalhandler(void);
int  _Cdecl ctrlbrkhandler(void);
void _Cdecl seterrhandler(void);
int _Cdecl wait_getkey(int sec);
void _Cdecl memerror(void);
void _Cdecl hperror(void);

extern byte *hsys_errlist[];

#endif

/* HANERRO2.H · ÑÁ·© { */