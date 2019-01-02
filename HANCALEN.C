#include <stdio.h>
#include <dos.h>
#include <math.h>
#include "hanout.h"
#include "hancoord.h"
#include "hancolor.h"
#include "hanwindw.h"
#include "ascii.h"
#include "extkey.h"

static int find_firstday(int year, int month)
{
	int n, l, p, y1, y2,
			z1, z2, z3, z4, firstday;
	float c;

	n=(int)floor(0.6+1./(float)month);
	l=year-n;
	p=month+12*n;
	c=(float)l/100.;
	y1=(int)floor(c);
	z1=(int)floor(c/4.);
	z2=(int)floor(l/4.);
	z3=13*(p+1);
	z4=(int)floor(z3/5.);
	firstday=l+z1+z2+z4-y1;
	y2=(int)floor((firstday-1)/7.);
	firstday-=y2*7;
	if(firstday==7)
		firstday=0;
	return firstday;
}

static int arrange_days(int year, int month)
{
	int endday;

	if(month==4 || month==6 || month==9 || month==11)
		endday=30;
	else if(month==2)
	 endday=(year%4==0) ? 29 : 28;
	else endday=31;
	return endday;
}

/*
 *  ∂a∑© äÅ–aã° (0 ~ 6 : ∑© ∂© —¡ ÆÅ °¢ ãq …°)
 */
int get_yoil(int year, int month, int today)
{
  int first_yoil;

  first_yoil = find_firstday(year, month);
  return first_yoil + (today % 7) - 1;
}

static void print_calendar(int year, int month, int firstday, int endday)
{
	register i, j;
  char title[40];
  int onoff;

  onoff = isoutline();
  hclrscr();
  hsetoutline(ON);
	sprintf(title, "%4dëe %2d∂©", year, month);
  wcenter(1, title);
  hprintfxy(3, 3, "∑© ∂© —¡ ÆÅ °¢ ãq …°");
  hsetoutline(onoff);
  eputchs(1, 2, 24, 'ƒ');
	for(i = 0, j = 3; i < endday+firstday; i++) {
    if(i && !(i % 7))
      j++;
    if(i < firstday)
      hprintfxy(i*3+3, j+1, "   ");
    else
      hprintfxy((i % 7)*3+3, j+1, "%2d ", i+1-firstday);
	}
}

static void make_calendar(int year, int month)
{
	int firstday, endday;

	firstday=find_firstday(year, month);
	endday=arrange_days(year, month);
	print_calendar(year, month, firstday, endday);
}

void calendar(void)
{
  struct date datep;
	int isquit;
  int year, month, t_year, t_month;

	isquit = 0;
	getdate(&datep);
  year=datep.da_year, month=datep.da_mon;
  if(!wopen(hgetmaxax()/2-11, hgetmaxay()/2-5, 24, 9))
    return;
  t_year = year, t_month = month;
  do {
	  make_calendar(t_year, t_month);
    switch(getxch()) {
      case UPARROW :
        t_year++;
        break;
      case DOWNARROW :
        if(t_year)
          t_year--;
        break;
      case LEFTARROW :
        if(t_month > 1)
          t_month--;
        else {
          if(t_year)
            t_year--;
          t_month = 12;
        }
        break;
      case RIGHTARROW :
        if(t_month < 12)
          t_month++;
        else {
          t_year++;
          t_month = 1;
        }
        break;
      case HOMEKEY :
        t_year = year, t_month = month;
        break;
      case ESC :
        isquit = 1;
        break;
    }
	} while(!isquit);
  wclose();
}
