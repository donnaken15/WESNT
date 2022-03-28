
#include <stdio.h>
#include <stdlib.h>

char*load(char*fname)
{
	FILE*lfile = fopen(fname,"rb");
	fseek(lfile,0,SEEK_END);
	size_t size = ftell(lfile);
	rewind(lfile);
	char*out = calloc(++size,1);
	out[--size] = 0;
	fread(out,1,size,lfile);
	fclose(lfile);
	return  out;
}
// make this inline or macro
void*copy(void*addr,size_t size)
{
	// wtf warning                  v
	return memcpy(malloc(size),addr,size);
}
char*append(char*a,char*b)
{
	char*c = (char*)malloc(strlen(a)+strlen(b)+1);
	if (c)
		if (strcpy(c,a))
			return strcat(c,b);
}

// though this type of stuff could be more wasteful
// in a way that doesn't need to, except certain
// cases i had where i don't want to do however
// i'm required to, like sprintf, i just want a formatted
// string and not have to manually allocate space first

#if 0

_start()
{
	puts("load file into char*");
	puts(load(__FILE__));
	
	puts("copy piece of memory");
	static char*string1 = "String A";
	static char*string2;
	string2 = copy(string1,strlen(string1)+1);
	string2[7] = 'B';
	puts(string1);
	puts(string2);
	puts("");
	
	puts("append string");
	static char*string3 = "I hope this sent";
	static char*string4 = "ence will be finished.";
	puts(append(string3,string4));
}

#endif

