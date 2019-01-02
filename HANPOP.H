/*
 *      File HANPOP.H
 *      A Header for Popup menu
 *      Written by Kwon, J. R
 */


#ifndef _HANPOP_H_
#define _HANPOP_H_

#ifndef _TYPES_H_
    #include "typedefs.h"
#endif

#ifndef _CHKVER_H_
    #include "chkver.h"
#endif

int popup(int x, int y, int cnt, char *menu[], char *title, char *keys, int flag);

#endif