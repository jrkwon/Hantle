/*
 *  파일   : kmain.h
 *  내용   : 편집기 주 함수
 *  기획   : hantle.prj
 *  날짜   : 93.03.16
 *  만든이 : 한양대학교 전자통신공학과 한틀아리 권재락
 *  알림글 : 이 풀그림의 밑그림은 공개를 원칙으로 합니다.
 *           왜냐하면, 나도 다른 밑그림의 도움을 많이 받았으니까요.
 *           따라서, 이 밑그림을 받으신 분은 다른데 사용하셔도 되지만
 *           새로 만든 밑그림도 잘 정리하셔서 공개해 주시기 바랍니다.
 */

#ifndef _KMAIN_H
#define _KMAIN_H

void initusermenu(int fcolor, int bcolor, int init_menu);
int handler(void);
void init_sys_color(void);
int main_loop(int key);

#endif
/*
 *  파일 kedit.h 의 끝
 */
