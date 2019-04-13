#ifndef __STRING__EMBEDDED__
#define __STRING__EMBEDDED__

#define __isalpha(c) (c > '9')
#define __isupper(c) !(c & 0x20)

unsigned int _strlen(const char * str);
int			__atoi(char *p);
int			_atoi(char * pString);
int			__toupper(int c);
void		_str_toupper(char * text);
unsigned	_str_split(char * string, char charSep, int maxArgs, char * argv);
const char	* _str_field(const char * string, int field);
void		_str_leftShift(char * string, int index);
void		_str_simplified(char * string);
int			_strcmp(const char * s1, const char * s2);
char		* _strcpy(char * pDst, const char * pSrc);
int			_strEndsWith(const char * src, const char * txt);

#endif