/*
 *  ����   : kmain.h
 *  ����   : ������ �� �Լ�
 *  ��ȹ   : hantle.prj
 *  ��¥   : 93.03.16
 *  ������ : �Ѿ���б� ������Ű��а� ��Ʋ�Ƹ� �����
 *  �˸��� : �� Ǯ�׸��� �ر׸��� ������ ��Ģ���� �մϴ�.
 *           �ֳ��ϸ�, ���� �ٸ� �ر׸��� ������ ���� �޾����ϱ��.
 *           ����, �� �ر׸��� ������ ���� �ٸ��� ����ϼŵ� ������
 *           ���� ���� �ر׸��� �� �����ϼż� ������ �ֽñ� �ٶ��ϴ�.
 */

#ifndef _KMAIN_H
#define _KMAIN_H

void initusermenu(int fcolor, int bcolor, int init_menu);
int handler(void);
void init_sys_color(void);
int main_loop(int key);

#endif
/*
 *  ���� kedit.h �� ��
 */