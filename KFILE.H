/*
 *  �a��   : kfile.h
 *  ���w   : �a��
 *  ���B   : hantle.prj, nal.prj
 *  �i�a   : 93.03.16
 *  �e�e�� : �e�����b�a ��aɷ�����b�� �e�i�a�� �����b
 *  �i���i : �� Ή�a���� ���a���e �����i ��â�a�� �s���a.
 *           �ᐡ�a�e, �a�� �a�e ���a���� �����i �g�� �h�v�a���a�a.
 *           �a�a��, �� ���a���i �h�a�� ���e �a�e�A �a�w�a�a�� �A���e
 *           ���� �e�e ���a���� �i �����a�a�� ����Ё ������ �a�s���a.
 */

int file_or_dir_exist(char *fname);
void new_file(void);
int load(void);
unsigned int load_file(char *filename, int load_mode);
int save(void);
bool save_file(unsigned char *start, unsigned char *finish, unsigned char *filename);
bool save_as(void);
char *files(char *name);

/*
 *  �a�� kfile.h �� �{
 */
