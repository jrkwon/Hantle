/*
 *  Ìa·©   : khauto2.c
 *  ¶w   : 2¤é¯¢ Ðe‹i·³b µ¡É¡ aÈa
 *  ‹¡ÒB   : hantle.prj, nal.prj
 *  i¼a   : 93.03.16
 *   e—e·¡ : Ðe´·”ÐbŠa ¸å¸aÉ·¯¥‰·Ðb‰Á ÐeËi´aŸ¡ Š¥¸œb
 *  ´iŸ±‹i : ·¡ Î‰‹aŸ±· £»‹aŸ±·e ‰·ˆŸi ¶¥Ã¢·a¡ Ðs“¡”a.
 *           µá¡Ða¡e, a•¡ ”aŸe £»‹aŸ±· •¡¶‘·i  g·¡ ¤h´v·a“¡Œa¶a.
 *           ˜aœa¬á, ·¡ £»‹aŸ±·i ¤h·a¯¥ ¦…·e ”aŸe•A ¬a¶wÐa­a•¡ –A»¡ e
 *           ¬¡  e—e £»‹aŸ±•¡ ¸i ¸÷Ÿ¡Ða­a¬á ‰·ˆÐ º¯¡‹¡ ¤aœs“¡”a.
 */

#include "hkeydef.h"

#define TRUE  1
#define FALSE 0

enum HAN_STATUS {              /* ˆb ¬wÈŸi ¸÷· */
        HS_Start,
        HS_Chosung,
        HS_Joongsung, HS_dJoongsung,
        HS_Jongsung,  HS_dJongsung,
        HS_End1,         HS_End2
};

enum HAN_CHKIND { CONSONANT,  VOWEL };

/*
** Ðe ˆ· Ç¡ ·³b a”a °}µa aˆa“e ·³b ¯aÈ‚· Š¹¡Ÿi ¸÷·
*/
typedef struct {
        int curhanst;         /* µ¡É¡ aÈa ¬wÈ    */
        int key;              /* ·³b–E Ç¡ Å¡—a   */
        int charcode;         /* ¹¡Ðs–E ¢…¸a Å¡—a */
} INPSTACK;

extern INPSTACK InpStack[];   /* ·³b ¯aÈ‚· ¶w     */
extern int InpSP;             /* ·³b ¯aÈ‚ Í¡·¥Èá     */
extern int OutStack[];        /* Â‰b ¯aÈ‚ ¤µi       */
extern int OutSP;             /* Â‰b ¯aÈ‚ Í¡·¥Èá     */
extern int CurHanState;       /* Ñe¸· µ¡É¡ aÈa ¬wÈ */

static int CharCode;
static int OldKey;
static int KeyCode;

/*
** ‰s¡¡·q·i ·¡ž“e»¡ ˆñ¬aÐe”a.
*/
static int JoongsungPair(void)
{
	static unsigned char dJoongTable[7][3] = {
		0xad, 0xa3, 0xae,		/* µ¡, ´a, µÁ */
		0xad, 0xa4, 0xaf,		/* µ¡, ´, µá */
		0xad, 0xbd, 0xb2,		/* µ¡, ·¡, ¶A */
		0xb4, 0xa7, 0xb5,		/* ¶, ´á, ¶¡ */
		0xb4, 0xaa, 0xb6,		/* ¶, µA, ¶Á */
		0xb4, 0xbd, 0xb7, 		/* ¶, ·¡, ¶á */
		0xbb, 0xbd, 0xbc		/* ·a, ·¡, · */
	};
	int i;

	for (i = 0; i < 7; i++) {
		if (dJoongTable[i][0] == OldKey && dJoongTable[i][1] == KeyCode)
			return (KeyCode = dJoongTable[i][2]);
	}
	return 0;
}

/*
** ‰s¤hÃ±·i ·¡ž“e»¡ ˆñ¬aÐe”a.
*/
static int JongsungPair(void)
{
	static unsigned char dJongTable[11][3] = {
		0x82, 0x8b, 0xc4,		/* ‹¡µb, ¯¡µµ */
		0x84, 0x8e, 0xc6,		/* “¡·e, »¡·x */
		0x84, 0x94, 0xc7,		/* “¡·e, Ó¡·} */
		0x87, 0x82, 0xca,		/* Ÿ¡·i, ‹¡µb */
		0x87, 0x88, 0xcb,		/* Ÿ¡·i, £¡·q */
		0x87, 0x89, 0xcc,		/* Ÿ¡·i, §¡·s */
		0x87, 0x8b, 0xcd,		/* Ÿ¡·i, ¯¡µµ */
		0x87, 0x92, 0xce,		/* Ÿ¡·i, Ë¡·{ */
		0x87, 0x93, 0xcf,		/* Ÿ¡·i, Ï¡·| */
		0x87, 0x94, 0xd0,		/* Ÿ¡·i, Ó¡·} */
		0x89, 0x8b, 0xd4,		/* §¡·s, ¯¡µµ */
	};
	int i;

	for (i = 0; i < 11; i++) {
		if (dJongTable[i][0] == OldKey && dJongTable[i][1] == KeyCode)
			return (KeyCode = dJongTable[i][2]);
	}
	return 0;
}

/********************************************************************
	–¤é¯¢ µ¡É¡ aÈa
********************************************************************/

int HanAutomata2(int key)

/*--------------------------------------------------------------------
·¥  ®:	key = ·³b–E Ç¡ Å¡—a
¤eÑÅˆt:	Ðe ‹i¸a· ¹¡Ðs·¡ {a¡e 0·¡¶A, ‰­¢ ¹¡Ðsº—·¡¡e 1
		µÅ¬÷–E ‹i¸a· Å¡—a“e ·³b ¯aÈ‚· ˆa¸w  a»¡ bµA¬á
		ŠÐi ® ·¶”a.
--------------------------------------------------------------------*/
{
	int ChKind, CanBeJongsung = FALSE;
	static unsigned char Cho2Jong[] = {	/* Á¡¬÷ Å¡—aµA ”·wÐa“e ¹·¬÷ Å¡—a */
		0xc2,	/*  ‹¡µb				*/
		0xc3,	/*  °w‹¡µb				*/
		0xc5,	/*  “¡·e				*/
		0xc8,	/*  —¡‹h				*/
		0x00,	/*  °w—¡‹h (Ð”w ´ô·q)	*/
		0xc9,	/*  Ÿ¡·i				*/
		0xd1,	/*  £¡·q				*/
		0xd3,	/*  §¡·s				*/
		0x00,	/*  ¬w§¡·s (Ð”w ´ô·q)	*/
		0xd5,	/*  ¯¡µµ				*/
		0xd6,	/*  °w¯¡µµ				*/
		0xd7,	/*  ·¡·w				*/
		0xd8,	/*  »¡·x				*/
		0x00,	/*  °w»¡·x (Ð”w ´ô·q)	*/
		0xd9,	/*  Ã¡·y				*/
		0xda,	/*  Ç¡·z				*/
		0xdb,	/*  Ë¡·{				*/
		0xdc,	/*  Ï¡·|				*/
		0xdd	/*  Ó¡·}				*/
    };

	if ((key & 0x60) == 0x20) {
		ChKind = VOWEL;
	} else {
		ChKind = CONSONANT;
		if (!(key == 0x86 || key == 0x8A || key == 0x8F))
			CanBeJongsung = TRUE;
	}
	if (CurHanState) {
		CharCode = InpStack[InpSP-1].charcode;
		OldKey   = InpStack[InpSP-1].key;
	} else {
		CharCode = 0x8441;
		OldKey   = 0;
	}
	KeyCode = key;
	switch (CurHanState) {
		case HS_Start:
			if (ChKind == CONSONANT)
				CurHanState = HS_Chosung;
			else
				CurHanState = HS_Joongsung;
			break;
		case HS_Chosung:
			if (ChKind == VOWEL)
				CurHanState = HS_Joongsung;
			else
				CurHanState = HS_End1;
			break;
		case HS_Joongsung:
			if (CanBeJongsung)
				CurHanState = HS_Jongsung;
			else if (JoongsungPair())
				CurHanState = HS_dJoongsung;
			else
				CurHanState = HS_End1;
			break;
		case HS_dJoongsung:
			if (CanBeJongsung)
				CurHanState = HS_Jongsung;
			else
				CurHanState = HS_End1;
			break;
		case HS_Jongsung:
			if (ChKind == CONSONANT && JongsungPair())
				CurHanState = HS_dJongsung;
			else if (ChKind == VOWEL)
				CurHanState = HS_End2;
			else
				CurHanState = HS_End1;
			break;
		case HS_dJongsung:
			if (ChKind == VOWEL)
				CurHanState = HS_End2;
			else
				CurHanState = HS_End1;
			break;
	}
	switch (CurHanState) {
		case HS_Chosung:
			CharCode = (CharCode & 0x83FF) | ((KeyCode - 0x80) << 10);
			break;
		case HS_Joongsung:
		case HS_dJoongsung:
			CharCode = (CharCode & 0xFC1F) | ((KeyCode - 0xA0) << 5);
			break;
		case HS_Jongsung:
			KeyCode = Cho2Jong[KeyCode - 0x82];
		case HS_dJongsung:
			CharCode = (CharCode & 0xFFE0) | (KeyCode - 0xC0);
			break;
		case HS_End1:
			OutStack[OutSP++] = key;
			return TRUE;
		case HS_End2:
			OutStack[OutSP++] = key;
                      OutStack[OutSP++] = OldKey;
                      InpSP--;
                      return TRUE;
        }
        InpStack[InpSP].curhanst = CurHanState;
        InpStack[InpSP].charcode = CharCode;
        InpStack[InpSP++].key   = key;
        return FALSE;
}

/*
 * khauto2.c
 */
