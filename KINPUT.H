/*
 *  Ìa·©   : khinput.c
 *  ¶w   : Íe»³‹¡· Ðe‹i ·³b¦  áŸ¡
 *  ‹¡ÒB   : hantle.prj, nal.prj
 *  i¼a   : 93.03.16
 *   e—e·¡ : Ðe´·”ÐbŠa ¸å¸aÉ·¯¥‰·Ðb‰Á ÐeËi´aŸ¡ Š¥¸œb
 *  ´iŸ±‹i : ·¡ Î‰‹aŸ±· £»‹aŸ±·e ‰·ˆŸi ¶¥Ã¢·a¡ Ðq“¡”a.
 *           µá¡Ða¡e, a•¡ ”aŸe £»‹aŸ±· •¡¶‘·i  g·¡ ¤h´v·a“¡Œa¶a.
 *           ˜aœa¬á, ·¡ £»‹aŸ±·i ¤h·a¯¥ ¦…·e ”aŸe•A ¬a¶wÐa­a•¡ –A»¡ e
 *           ¬¡  e—e £»‹aŸ±•¡ ¸i ¸÷Ÿ¡Ða­a¬á ‰·ˆÐ º¯¡‹¡ ¤aœs“¡”a.
 */

#define   LSHIFT_SPACE  512
#define   RSHIFT_SPACE	513

enum HANINPRESULTS {
	HR_NOKEY,
	HR_ASC,
	HR_HANST,
	HR_HANIN,
	HR_HANEND,
	HR_HANBS,
        HR_NOHAN
};

enum { HANKBD2, HANKBD3 };

extern int HanKbdState;    /* Ðe/µw ¬wÈ, Ðe‹i¬wÈ·©˜ Àq */
extern int HanKbdKind;     /* Ðe‹i¸aÌe¹·ŸA 2(HANKBD2), 3¤é¯¢(HANKBD3) */

int GetKey(int inphan);
void PutKey(int key);
int InKeyBuf(void);

void DisplayCursor(int x, int y, int bch);
void EraseCursor(int x, int y);
int GetHanCh(int x, int y, int *ch, int bch);
void EraseKeyBuf(void);
/*
 *  Ìa·© khinput.c · {
 */
