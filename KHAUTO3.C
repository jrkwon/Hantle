/*
 *  �a��   : khauto3.c
 *  ���w   : 3�鯢 �e�i���b ��ɡ�a�a
 *  ���B   : hantle.prj, nal.prj
 *  �i�a   : 93.03.16
 *  �e�e�� : �e�����b�a ��aɷ�����b�� �e�i�a�� �����b
 *  �i���i : �� Ή�a���� ���a���e �����i ��â�a�� �s���a.
 *           �ᐡ�a�e, �a�� �a�e ���a���� �����i �g�� �h�v�a���a�a.
 *           �a�a��, �� ���a���i �h�a�� ���e �a�e�A �a�w�a�a�� �A���e
 *           ���� �e�e ���a���� �i �����a�a�� ����Ё ������ �a�s���a.
 */

#include "hkeydef.h"

#define TRUE  1
#define FALSE 0

enum HAN_STATUS {              /* �b �wȁ�i ���� */
        HS_Start,
        HS_Chosung,   HS_dChosung,
        HS_Joongsung, HS_dJoongsung,
        HS_Jongsung,  HS_dJongsung,
        HS_End
};

enum HAN_CHKIND {              /* ���b�E �a���� ���A */
        CHOSUNG   = 0x00,
        JOONGSUNG = 0x20,
        JONGSUNG  = 0x40
};

/*
** �e ���� ǡ ���b�a�a �}�a �a�a�e ���b �aȂ�� �����i ����
*/
typedef struct {
	int curhanst;		/* ��ɡ�a�a �wȁ    */
	int key;			/* ���b�E ǡ š�a   */
	int charcode;		/* ���s�E ���a š�a */
} INPSTACK;

extern INPSTACK InpStack[];		/* ���b �aȂ�� ���w     */
extern int		InpSP;			/* ���b �aȂ ͡����     */
extern int		OutStack[];		/* �b �aȂ ���i       */
extern int		OutSP;			/* �b �aȂ ͡����     */
extern int		CurHanState;	/* �e���� ��ɡ�a�a �wȁ */

static int CharCode;
static int OldKey;
static int KeyCode;

/*
** ���� š�a�a �w�a�q�i �����e�� ��a�e�a.
*/
static int ChosungPair(void)
{
	static unsigned char dChoTable[][3] = {
		0x82, 0x82, 0x83,		/* ���b, ���b, �w���b */
		0x85, 0x85, 0x86,		/* ���h, ���h, �w���h */
		0x89, 0x89, 0x8a,		/* ���s, ���s, �w���s */
		0x8b, 0x8b, 0x8c,		/* ����, ����, �w���� */
		0x8e, 0x8e, 0x8f		/* ���x, ���x, �w���x */
	};
	int i;

	for (i = 0; i < 5; i++) {
		if (dChoTable[i][0] == OldKey && dChoTable[i][1] == KeyCode)
			return (KeyCode = dChoTable[i][2]);
	}
	return 0;
}

/*
** �s���q�i �����e�� ��a�e�a.
*/
static int JoongsungPair(void)
{
	static unsigned char dJoongTable[][3] = {
		0xad, 0xa3, 0xae,		/* ��, �a, �� */
		0xad, 0xa4, 0xaf,		/* ��, ��, �� */
		0xad, 0xbd, 0xb2,		/* ��, ��, �A */
		0xb4, 0xa7, 0xb5,		/* ��, ��, �� */
		0xb4, 0xaa, 0xb6,		/* ��, �A, �� */
		0xb4, 0xbd, 0xb7, 		/* ��, ��, �� */
		0xbb, 0xbd, 0xbc		/* �a, ��, �� */
	};
	int i;

	for (i = 0; i < 7; i++) {
		if (dJoongTable[i][0] == OldKey && dJoongTable[i][1] == KeyCode)
			return (KeyCode = dJoongTable[i][2]);
	}
	return 0;
}

/*
** �s�hñ�i �����e�� ��a�e�a.
*/
static int JongsungPair(void)
{
	static unsigned char dJongTable[][3] = {
		0xc2, 0xc2, 0xc3,		/* ���b, ���b */
		0xc2, 0xd5, 0xc4,		/* ���b, ���� */
		0xc5, 0xd8, 0xc6,		/* ���e, ���x */
		0xc5, 0xdd, 0xc7,		/* ���e, ӡ�} */
		0xc9, 0xc2, 0xca,		/* ���i, ���b */
		0xc9, 0xd1, 0xcb,		/* ���i, ���q */
		0xc9, 0xd3, 0xcc,		/* ���i, ���s */
		0xc9, 0xd5, 0xcd,		/* ���i, ���� */
		0xc9, 0xdb, 0xce,		/* ���i, ˡ�{ */
		0xc9, 0xdc, 0xcf,		/* ���i, ϡ�| */
		0xc9, 0xdd, 0xd0,		/* ���i, ӡ�} */
		0xd3, 0xd5, 0xd4,		/* ���s, ���� */
		0xd5, 0xd5, 0xd6 		/* ����, ���� */
	};
	int i;

	for (i = 0; i < 13; i++) {
		if (dJongTable[i][0] == OldKey && dJongTable[i][1] == KeyCode)
			return (KeyCode = dJongTable[i][2]);
	}
	return 0;
}

/********************************************************************
	�A�鯢 ��ɡ�a�a
********************************************************************/

int HanAutomata3(int key)

/*--------------------------------------------------------------------
��  ��:	key = ���b�E ǡ š�a
�e�ňt:	�e �i�a�� ���s�� �{�a�e 0���A, ���� ���s�����e 1
		�Ŭ��E �i�a�� š�a�e ���b �aȂ�� �a�w �a���b�A��
		���i �� ���a.
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