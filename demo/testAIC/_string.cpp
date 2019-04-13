#include "_string.h"

unsigned int _strlen(const char * str)
{
	unsigned int slen;
	if(str == 0)
		return 0;
	slen = 0;
	while(*str != 0) 
	{
		slen++;
		str++;
	}
	return slen;
}

int __atoi(char *p) 
{
	int k = 0;
	while(*p) 
	{
		k = (k << 3) + (k << 1) + (*p) - '0';
		p++;
	}
	return k;
}

int _atoi(char * pString)
{
	int   	wBase = 10;
	int   	wMinus = 0;
	int   	wLastIndex;
	int   	wResult = 0;
	int   	wI;

	if(pString[0] == '-')
	{
		wMinus = 1;
		pString++;
	}

	if(pString[0] == '0' && (pString[1] == 'x' || pString[1] == 'X'))
	{
		wBase = 16;
		pString += 2;
	}

	wLastIndex = _strlen(pString) - 1;
	if(pString[wLastIndex] == 'h' || pString[wLastIndex] == 'H')
	{
		wBase = 16;
		pString[wLastIndex] = 0;
		wLastIndex--;
	}

	if(wBase == 10)
	{
		wResult = __atoi(pString);
		wResult = wMinus ? (-1 * wResult) : wResult;
	}
	else
	{
		for(wI = 0; wI <= wLastIndex; wI++)
		{
			if(__isalpha(pString[wI]))
			{
				if(__isupper(pString[wI]))
					wResult = (wResult << 4) + pString[wI] - 'A' + 10;	
				else
					wResult = (wResult << 4) + pString[wI] - 'a' + 10;
			}
			else
			{
				wResult = (wResult << 4) + pString[wI] - '0';
			}
		}
		wResult = wMinus ? (-1 * wResult) : wResult;
	}
	return wResult;
}

inline int __toupper(int c)
{
	return ( c >= 'a' && c <= 'z' ) ? (c-32) : c;
}

void _str_toupper(char * text)
{
	int     i = 0;
	while(text[i])
	{
		text[i] = __toupper(text[i]);
		i++;
	}
}

unsigned _str_split(char * string, char charSep, int maxArgs, char * argv)
{
	volatile unsigned	i, n, argc = 0;	// Clear string counter

	i = n = 0;
	// String split
	while(string[n])
	{
		if(string[n] != charSep)
			argv[i++] = string[n++];
		else
		{
			n++;
			// Skip all spaces
			while(string[n] == ' ') n++;
			// Add terminator
			argv[i] = 0;
			argc++;
			if(argc == maxArgs)
				break;
			i++;
		}
	}
	// Add terminator
	argv[i] = 0;
	argc++;
	return argc;
}

const char * _str_field(const char * string, int field)
{
	int     i, n = 0, fp = 0;

	for(i = 0; i < field; i++)
	{

		while(string[n]) 
			n++;
		fp = ++n;
	}
	return &string[fp];
}

void _str_leftShift(char * string, int index)
{
	int     n = 0;
	while(string[index + n])
	{
		string[index + n] = string[index + n + 1];
		n++;
	}
}

void _str_simplified(char * string)
{
	int     i = 0;
	while(string[i])
	{
		// Begin 
		if(string[i] == ' ' && i == 0)
			_str_leftShift(string, i);
		// Last
		if(string[i] == ' ' && string[i + 1] == 0)
			_str_leftShift(string, i);
		if(string[i] == ' ' && string[i + 1] == ' ')
			_str_leftShift(string, i);
		i++;
	}
}

int _strcmp(const char * s1, const char * s2)
{
	while(*s1 == *s2++)
		if(*s1++ == 0)
			return (0);
	return(*s1 - *--s2);
}

char * _strcpy(char * pDst, const char * pSrc)
{
	char	* dst = pDst;
	while ((*pDst++ = *pSrc++) != '\0')
		continue;
	return dst;
}

int _strEndsWith(const char * src, const char * txt)
{
	int     sl = _strlen(src);
	int     tl = _strlen(txt);
	if(tl > sl) return 1;
	return _strcmp(&src[sl - tl], txt);
}
