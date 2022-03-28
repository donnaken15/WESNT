
/*
 * WESLEY'S NT
 *
 * Title: UTF-8 to Wide char
 * Author: Wesley
 * License: GPLv3
 *
 * Dependencies: C library
 *
 */

#include <stdio.h>
#include <stdlib.h>

wchar_t* utf8stowcs(char*str,int len) // len = character count - extra code bytes
{
	wchar_t*out = (wchar_t*)malloc((len+1)<<1);
	out[len] = 0;
	wchar_t tmp;
	char    tmp2;
	int h = 0; // :P
	for (int i = 0; i < len; i++)
	{
		if (!(str[h] & 0x80))
		{
			out[i] = str[h];
			//puts("0");
		}
		else
		{
			char j, k = 0;
			tmp2 = str[h];
			// use bitstream |:/
			k++;
			for (j = 1; j < 6; j++)
			{
				if (!(tmp2 >> (7-j) & 1))
					break;
				k++;
			}
			tmp = str[h] & (255 >> k);
			//printf("%u %02X\n  %02X\n", k, str[h], tmp);
			for (j = 0; j < k-1; j++)
			{
				tmp <<= 6;
				//printf("| %02X\n", str[++h] & 0b00111111);
				tmp |= (str[++h] & 0b00111111);
			}
			//printf("%04X\n",tmp);
			//putwchar(tmp);
			//puts("");
			out[i] = tmp;
		}
		h++;
	}
	return out;
}

#if 1

#include "ez.c"
_start()
{
	//printf("Hello, World!\n");
	char*utf8str = load("utf8test.txt");
	wchar_t*newwide = utf8stowcs(utf8str,27);
	//printf("%u", wcslen(newwide));
	//puts("");
	FILE*  widetest =
	fopen("widetest.txt","wb");
	fwprintf(widetest,newwide);
	fclose(widetest);
}

#endif

