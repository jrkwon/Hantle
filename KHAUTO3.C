/*
 *  Ìa·©   : khauto3.c
 *  ¶w   : 3¤é¯¢ Ðe‹i·³b µ¡É¡ aÈa
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
        HS_Chosung,   HS_dChosung,
        HS_Joongsung, HS_dJoongsung,
        HS_Jongsung,  HS_dJongsung,
        HS_End
};

enum HAN_CHKIND {              /* ·³b–E ¸a¡¡· ¹·ŸA */
        CHOSUNG   = 0x00,
        JOONGSUNG = 0x20,
        JONGSUNG  = 0x40
};

/*
** Ðe ˆ· Ç¡ ·³b a”a °}µa aˆa“e ·³b ¯aÈ‚· Š¹¡Ÿi ¸÷·
*/
typedef struct {
	int curhanst;		/* µ¡É¡ aÈa ¬wÈ    */
	int key;			/* ·³b–E Ç¡ Å¡—a   */
	int charcode;		/* ¹¡Ðs–E ¢…¸a Å¡—a */
} INPSTACK;

extern INPSTACK InpStack[];		/* ·³b ¯aÈ‚· ¶w     */
extern int		InpSP;			/* ·³b ¯aÈ‚ Í¡·¥Èá     */
extern int		OutStack[];		/* Â‰b ¯aÈ‚ ¤µi       */
extern int		OutSP;			/* Â‰b ¯aÈ‚ Í¡·¥Èá     */
extern int		CurHanState;	/* Ñe¸· µ¡É¡ aÈa ¬wÈ */

static int CharCode;
static int OldKey;
static int KeyCode;

/*
** Á¡¬÷ Å¡—aˆa °w¸a·q·i ·¡ž“e»¡ ˆñ¬aÐe”a.
*/
static int ChosungPair(void)
{
	static unsigned char dChoTable[][3] = {
		0x82, 0x82, 0x83,		/* ‹¡µb, ‹¡µb, °w‹¡µb */
		0x85, 0x85, 0x86,		/* —¡‹h, —¡‹h, °w—¡‹h */
		0x89, 0x89, 0x8a,		/* §¡·s, §¡·s, °w§¡·s */
		0x8b, 0x8b, 0x8c,		/* ¯¡µµ, ¯¡µµ, °w¯¡µµ */
		0x8e, 0x8e, 0x8f		/* »¡·x, »¡·x, °w»¡·x */
	};
	int i;

	for (i = 0; i < 5; i++) {
		if (dChoTable[i][0] == OldKey && dChoTable[i][1] == KeyCode)
			return (KeyCode = dChoTable[i][2]);
	}
	return 0;
}

/*
** ‰s¡¡·q·i ·¡ž“e»¡ ˆñ¬aÐe”a.
*/
static int JoongsungPair(void)
{
	static unsigned char dJoongTable[][3] = {
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
	static unsigned char dJongTable[][3] = {
		0xc2, 0xc2, 0xc3,		/* ‹¡µb, ‹¡µb */
		0xc2, 0xd5, 0xc4,		/* ‹¡µb, ¯¡µµ */
		0xc5, 0xd8, 0xc6,		/* “¡·e, »¡·x */
		0xc5, 0xdd, 0xc7,		/* “¡·e, Ó¡·} */
		0xc9, 0xc2, 0xca,		/* Ÿ¡·i, ‹¡µb */
		0xc9, 0xd1, 0xcb,		/* Ÿ¡·i, £¡·q */
		0xc9, 0xd3, 0xcc,		/* Ÿ¡·i, §¡·s */
		0xc9, 0xd5, 0xcd,		/* Ÿ¡·i, ¯¡µµ */
		0xc9, 0xdb, 0xce,		/* Ÿ¡·i, Ë¡·{ */
		0xc9, 0xdc, 0xcf,		/* Ÿ¡·i, Ï¡·| */
		0xc9, 0xdd, 0xd0,		/* Ÿ¡·i, Ó¡·} */
		0xd3, 0xd5, 0xd4,		/* §¡·s, ¯¡µµ */
		0xd5, 0xd5, 0xd6 		/* ¯¡µµ, ¯¡µµ */
	};
	int i;

	for (i = 0; i < 13; i++) {
		if (dJongTable[i][0] == OldKey && dJongTable[i][1] == KeyCode)
			return (KeyCode = dJongTable[i][2]);
	}
	return 0;
}

/********************************************************************
	­A¤é¯¢ µ¡É¡ aÈa
********************************************************************/

int HanAutomata3(int key)

/*--------------------------------------------------------------------
·¥  ®:	key = ·³b–E Ç¡ Å¡—a
¤eÑÅˆt:	Ðe ‹i¸a· ¹¡Ðs·¡ {a¡e 0·¡¶A, ‰­¢ ¹¡Ðsº—·¡¡e 1
		µÅ¬÷–E ‹i¸a· Å¡—a“e ·³b ¯aÈ‚· ˆa¸w  a»¡ bµA¬á
		ŠÐi ® ·¶”a.
--------------------------------------------------------------------*/
{
	int ChKind = key & 0x60;

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
			switch (ChKind) {
				case CHOSUNG:
					CurHanState = HS_Chosung;
					break;
				case JOONGSUNG:
					CurHanState = HS_Joongsung;
					break;
				case JONGSUNG:
					CurHanState = HS_Jongsung;
					break;
			}
			break;
		case HS_Chosung:
			switch (ChKind) {
				case CHOSUNG:
					if (ChosungPair())
						CurHanState = HS_dChosung;
					else
						CurHanState = HS_End;
					break;
				case JOONGSUNG:
					CurHanState = HS_Joongsung;
					break;
				case JONGSUNG:
					CurHanState = HS_Jongsung;
					break;
			}
			break;
		case HS_dChosung:
			switch (ChKind) {
				case CHOSUNG:
					CurHanState = HS_End;
					break;
				case JOONGSUNG:
					CurHanState = HS_Joongsung;
					break;
				case JONGSUNG:
					CurHanState = HS_Jongsung;
					break;
			}
			break;
		case HS_Joongsung:
			switch (ChKind) {
				case CHOSUNG:
					CurHanState = HS_End;
					break;
				case JOONGSUNG:
					if (JoongsungPair())
						CurHanState = HS_dJoongsung;
					else
						CurHanState = HS_End;
					break;
				case JONGSUNG:
					CurHanState = HS_Jongsung;
					break;
			}
			break;
		case HS_dJoongsung:
			switch (ChKind) {
				case CHOSUNG:
				case JOONGSUNG:
					CurHanState = HS_End;
					break;
				case JONGSUNG:
					CurHanState = HS_Jongsung;
					break;
			}
			break;
		case HS_Jongsung:
			switch (ChKind) {
				case CHOSUNG:
				case JOONGSUNG:
					CurHanState = HS_End;
					break;
				case JONGSUNG:
					if (JongsungPair())
						CurHanState = HS_dJongsung;
					else
						CurHanState = HS_End;
					break;
			}
		case HS_dJongsung:
			CurHanState = HS_End;
			break;
	}
	switch (CurHanState) {
		case HS_Chosung:
		case HS_dChosung:
			CharCode = (CharCode & 0x83FF) | ((KeyCode - 0x80) << 10);
			break;
		case HS_Joongsung:
		case HS_dJoongsung:
			CharCode = (CharCode & 0xFC1F) | ((KeyCode - 0xA0) << 5);
			break;
		case HS_Jongsung:
		case HS_dJongsung:
			CharCode = (CharCode & 0xFFE0) | (KeyCode - 0xC0);
			break;
		case HS_End:
			OutStack[OutSP++] = key;
			return TRUE;
	}
	InpStack[InpSP].curhanst = CurHanState;
	InpStack[InpSP].charcode = CharCode;
        InpStack[InpSP++].key   = key;
        return FALSE;
}

/*
 *  khauto3.c
 */