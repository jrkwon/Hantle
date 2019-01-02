typedef struct {
  char manuf;     /* Ðw¬w 10 (0x0a) */
  char ver;       /*                */
  char encod;     /* RLE ·¡¡e 1     */
  char bitpx;     /* Ðe¸ñ·i Îa¯¡Ða“e §¡Ëa ® */
  int  x1;        /* ‹aŸ±· ¹ÁÎa    */
  int  y1;
  int  x2;
  int  y2;
  int  hres;      /* Ð¬w•¡         */
  int  vres;
  char palette[48];
  char reserved;     /* ‹aŸ±·i ¤h·e §¡—¡µ¡ ¡¡—a ? */
  char nplanes;   /* ¬a¶w–E ÏiA·¥® */
  int  bplin;     /* Ðeœa·¥”w ¤a·¡Ëa ® */
  int  palinfo;   /* 1=COLOR, 0=GREY */
  int  shres;     /* ¯aÄá Ð¬w•¡?*/
  int  svres;
  char xtra[54];  /* ¬a¶w ´g·q */
} PCXHDR;    /* PCX Ñw¯¢ ÑÁ·©· ÑA”á */

bool view_pcx_file(int x, int y, char *file_name, int fcolor, int bcolor);
