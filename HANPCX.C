#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <time.h>
#include <bios.h>
#include "hanlib.h"
#include "hanpcx.h"

static void writepcxline(int x, int y, char *s, int n, int fcolor, int bcolor)
{
  char mask[8] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
  int i, j;

  for(i = 0; i < n-2; i++, s++) {
    if(x+i >= hgetmaxx())
      return;
    for(j = 0; j < 8; j++)
      hputpixel((x+i)*8+j, y, (*s & mask[j]) ? fcolor : bcolor);
  }
}

static int readpcxline(char *s, FILE *fpt, int bytes)
{
	int c,i;
	int n=0;

	do {
		c=(fgetc(fpt) & 0xff);
		if ((c&0xc0)==0xc0) {
			i=c&0x3f;
			c=fgetc(fpt);
      while (i--) s[n++]=c;
		}
		else s[n++]=c;
	} while (n<bytes);
	return n;
}

static bool pcx_view(int x, int y, FILE *fp, int fcolor, int bcolor)
{
  PCXHDR hdr;
  unsigned bytes;
  int i, n;
  char *bits;

  fread((void *)&hdr, 128, 1, fp);
  if(hdr.manuf != 0x0a /* &&    ..  */)
    return false;
  x /= 8;
  bytes = hdr.bplin;
  bits = (char *)malloc((size_t)bytes);

  for(i = y; i < y+hdr.y2-hdr.y1+1; i++) {
    if(x > hgetmaxx() || i > hgetmaxpy())
      return true;
    n = readpcxline(bits, fp, bytes);
    writepcxline(x, i, bits, n, fcolor, bcolor);
  }
  free(bits);
  return true;
}

bool view_pcx_file(int x, int y, char *file_name, int fcolor, int bcolor)
{
  FILE *fp;

  fp = fopen(file_name, "rb");
  if(!fp)
    return false;
  pcx_view(x, y, fp, fcolor, bcolor);
  fclose(fp);
  return true;
}


