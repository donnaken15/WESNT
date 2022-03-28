
#include <windows.h>
#include <malloc.h>
#include <stdio.h>

#define _stdcall	 __attribute__ ((stdcall))
#define FASTCALL_A	 __attribute__ ((regparm(1)))

HANDLE __HEAP = 0;

heap() // evade this by putting the function return in main only once
{
    if (!__HEAP) // save time
    {
        __HEAP = GetProcessHeap();
    }
    return __HEAP;
}

#define malloc(s)   HeapAlloc(heap(), 0, s)
#define calloc(c,s) HeapAlloc(heap(), HEAP_ZERO_MEMORY, c*s)
#define   free(p)   HeapAlloc(heap(), 0, p)
#define  msize(p)   HeapSize (heap(), 0, p)
#define   exit(i)   ExitProcess(i)
//#define exit ExitProcess

/*
inline void* __cdecl malloc(size_t size)
{
	return HeapAlloc(heap(), 0, size);
}
inline void* __cdecl calloc(size_t nitems, size_t size)
{
	return HeapAlloc(heap(), HEAP_ZERO_MEMORY, nitems * size);
}
inline void __cdecl free(void* p)
{
	HeapFree(heap(), 0, p);
}
inline size_t __cdecl msize(void* p)
{
	return HeapSize(heap(), 0, p);
}
*/
void* __cdecl realloc(void* p, size_t size)
{
	if (p)
		return HeapReAlloc(heap(), 0, p, size);
	else
		return HeapAlloc(heap(), 0, size);
}
__cdecl puts(const char *s)
{
	DWORD cbWritten;
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	WriteFile(hStdOut, s, lstrlen(s), &cbWritten, 0);
	WriteFile(hStdOut, "\r\n", 2, &cbWritten, 0);

	return (int)(cbWritten ? cbWritten : EOF);
}

/*
main(int argc, char*argv[])
{
	
}
*/
