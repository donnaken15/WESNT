
/*
 * WESLEY'S NT
 *
 * Title: ComPatU
 * Author: Wesley
 * License: GPLv3
 *
 * Enable or disable use of
 * CPU extension methods by
 * detecting if your PC
 * supports them
 *
 * Dependencies: CPUID
 *
 */

// TODO: write methods that CPUs with or without (i.e. SSE or MMX) can do
// so if one CPU doesn't support SSE and another one does,
// the first will use a compatible part of the method,
// and the other will use the SSE part of the method
//
// or however i imagine it...

// im acting like ill write full on 3D games for Windows 95
// you never know what cheap laptop will have these days
// or i don't

unsigned int
	__VENDOR [3] = { 0x38786E49, 0x54655736, 0x74737572 };
//char ____vendpad = 0;
unsigned int
	__CID    [2],
	__EXTNS  [2],
	__EXTNS_ [3],
	__TLB    [4],
	__EXTEXTS[4];

#include <stdio.h>

// CPUID workaround
// source: https://wiki.osdev.org/CPUID
int CPUIDOK = 0;
// check for if the CPUID instruction is valid to run
int EFB21()
{
	/* INSTRUCTIONS:
		pushfd
		pushfd
		xor dword[esp],200000h
		popfd
		pushfd
		pop eax
		xor eax,[esp]
		popfd
		and eax,200000h
		shr eax,21
		ret
		
		BYTES:
		9C 9C 81 34 24 00 00 20
		00 9D 9C 58 33 04 24 9D
		25 00 00 20 00 C1 E8 15
		C3 */
	__asm__ __volatile__ (
		// lol
		// GAS won't let me use pushfd or whatever
		// it threw an error so here's this + pointless overhead
		"  .byte    0x9c, 0x9c, 0x81, 0x34, 0x24, 0x00, 0x00, 0x20,"
					"0x00, 0x9d, 0x9c, 0x58, 0x33, 0x04, 0x24, 0x9d,"
					"0x25, 0x00, 0x00, 0x20, 0x00, 0xc1, 0xe8, 0x15\n"
		:
		:
		:"memory", "eax"
	);
}

enum {
	CPUID_FEAT_ECX_SSE3         = 1 << 0, 
	CPUID_FEAT_ECX_PCLMUL       = 1 << 1,
	CPUID_FEAT_ECX_DTES64       = 1 << 2,
	CPUID_FEAT_ECX_MONITOR      = 1 << 3,  
	CPUID_FEAT_ECX_DS_CPL       = 1 << 4,  
	CPUID_FEAT_ECX_VMX          = 1 << 5,  
	CPUID_FEAT_ECX_SMX          = 1 << 6,  
	CPUID_FEAT_ECX_EST          = 1 << 7,  
	CPUID_FEAT_ECX_TM2          = 1 << 8,  
	CPUID_FEAT_ECX_SSSE3        = 1 << 9,  
	CPUID_FEAT_ECX_CID          = 1 << 10,
	CPUID_FEAT_ECX_SDBG         = 1 << 11,
	CPUID_FEAT_ECX_FMA          = 1 << 12,
	CPUID_FEAT_ECX_CX16         = 1 << 13, 
	CPUID_FEAT_ECX_XTPR         = 1 << 14, 
	CPUID_FEAT_ECX_PDCM         = 1 << 15, 
	CPUID_FEAT_ECX_PCID         = 1 << 17, 
	CPUID_FEAT_ECX_DCA          = 1 << 18, 
	CPUID_FEAT_ECX_SSE4_1       = 1 << 19, 
	CPUID_FEAT_ECX_SSE4_2       = 1 << 20, 
	CPUID_FEAT_ECX_X2APIC       = 1 << 21, 
	CPUID_FEAT_ECX_MOVBE        = 1 << 22, 
	CPUID_FEAT_ECX_POPCNT       = 1 << 23, 
	CPUID_FEAT_ECX_TSC          = 1 << 24, 
	CPUID_FEAT_ECX_AES          = 1 << 25, 
	CPUID_FEAT_ECX_XSAVE        = 1 << 26, 
	CPUID_FEAT_ECX_OSXSAVE      = 1 << 27, 
	CPUID_FEAT_ECX_AVX          = 1 << 28,
	CPUID_FEAT_ECX_F16C         = 1 << 29,
	CPUID_FEAT_ECX_RDRAND       = 1 << 30,
	CPUID_FEAT_ECX_HYPERVISOR   = 1 << 31,
	
	CPUID_FEAT_EDX_FPU          = 1 << 0,  
	CPUID_FEAT_EDX_VME          = 1 << 1,  
	CPUID_FEAT_EDX_DE           = 1 << 2,  
	CPUID_FEAT_EDX_PSE          = 1 << 3,  
	CPUID_FEAT_EDX_TSC          = 1 << 4,  
	CPUID_FEAT_EDX_MSR          = 1 << 5,  
	CPUID_FEAT_EDX_PAE          = 1 << 6,  
	CPUID_FEAT_EDX_MCE          = 1 << 7,  
	CPUID_FEAT_EDX_CX8          = 1 << 8,  
	CPUID_FEAT_EDX_APIC         = 1 << 9,  
	CPUID_FEAT_EDX_SEP          = 1 << 11, 
	CPUID_FEAT_EDX_MTRR         = 1 << 12, 
	CPUID_FEAT_EDX_PGE          = 1 << 13, 
	CPUID_FEAT_EDX_MCA          = 1 << 14, 
	CPUID_FEAT_EDX_CMOV         = 1 << 15, 
	CPUID_FEAT_EDX_PAT          = 1 << 16, 
	CPUID_FEAT_EDX_PSE36        = 1 << 17, 
	CPUID_FEAT_EDX_PSN          = 1 << 18, 
	CPUID_FEAT_EDX_CLFLUSH      = 1 << 19, 
	CPUID_FEAT_EDX_DS           = 1 << 21, 
	CPUID_FEAT_EDX_ACPI         = 1 << 22, 
	CPUID_FEAT_EDX_MMX          = 1 << 23, 
	CPUID_FEAT_EDX_FXSR         = 1 << 24, 
	CPUID_FEAT_EDX_SSE          = 1 << 25, 
	CPUID_FEAT_EDX_SSE2         = 1 << 26, 
	CPUID_FEAT_EDX_SS           = 1 << 27, 
	CPUID_FEAT_EDX_HTT          = 1 << 28, 
	CPUID_FEAT_EDX_TM           = 1 << 29, 
	CPUID_FEAT_EDX_IA64         = 1 << 30,
	CPUID_FEAT_EDX_PBE          = 1 << 31
};
enum CPUID_MODES { // set eax to these
	CPUID_GETVENDORSTRING,
	CPUID_GETFEATURES,
	CPUID_GETTLB, // what to get from this???
	CPUID_GETSERIAL,
	CPUID_INTELTHREADTPLGY,
	CPUID_THERMALPOWMGMT = 6,
	CPUID_EXTENDEDFEATURES = 7, // ECX=0
	CPUID_INTELCACHETPOLGY = 0xB,
	
	CPUID_INTELEXTENDED=0x80000000,
	CPUID_INTELFEATURES, // same as 0x01
	CPUID_INTELBRANDSTRING,
	CPUID_INTELBRANDSTRINGMORE,
	CPUID_INTELBRANDSTRINGEND,
};

int GetExts()
{
	if (!EFB21())
		return;
	CPUIDOK = 1;
	// string
	__asm__(
		"xor %eax, %eax\n\t"
		"cpuid\n\t");
	__asm__("mov %%ebx, %0\n\t":"=r" (__VENDOR[0]));
	__asm__("mov %%edx, %0\n\t":"=r" (__VENDOR[1]));
	__asm__("mov %%ecx, %0\n\t":"=r" (__VENDOR[2]));
	// features (EAX, EBX, EDX, ECX)
	// read https://en.wikipedia.org/wiki/CPUID#EAX=1:_Processor_Info_and_Feature_Bits
	__asm__(
		"mov $0x1, %eax\n\t"
		"cpuid\n\t");
	__asm__("mov %%eax, %0\n\t":"=r" (__CID  [0]));
	__asm__("mov %%ebx, %0\n\t":"=r" (__CID  [1]));
	__asm__("mov %%edx, %0\n\t":"=r" (__EXTNS[0]));
	__asm__("mov %%ecx, %0\n\t":"=r" (__EXTNS[1]));
	// cache and tlb
	__asm__(
		"mov $0x2, %eax\n\t"
		"cpuid\n\t");
	__asm__("mov %%eax, %0\n\t":"=r" (__TLB  [0]));
	__asm__("mov %%ebx, %0\n\t":"=r" (__TLB  [1]));
	__asm__("mov %%ecx, %0\n\t":"=r" (__TLB  [2]));
	__asm__("mov %%edx, %0\n\t":"=r" (__TLB  [3]));
	// extended features
	__asm__(
		"mov $0x7, %eax\n\t"
		//"xor %ecx, %ecx\n\t"
		"cpuid\n\t");
	//__asm__("mov %%eax, %0\n\t":"=r" (__EXTEXTS[0])); // lol
	__asm__("mov %%ebx, %0\n\t":"=r" (__EXTEXTS[0]));
	__asm__("mov %%ecx, %0\n\t":"=r" (__EXTEXTS[1]));
	__asm__("mov %%edx, %0\n\t":"=r" (__EXTEXTS[2]));
	// extended features
	__asm__(
		"mov $0x80000001, %eax\n\t"
		"cpuid\n\t");
	__asm__("mov %%eax, %0\n\t":"=r" (__EXTNS_[0]));
	__asm__("mov %%ebx, %0\n\t":"=r" (__EXTNS_[1]));
	return 1;
}

typedef struct {
	float x, y, z;
} __VEC3_tag;

// ez wikipedia example thing
typedef struct {
	float x, y, z, w;
} __VEC4_tag;
#define Vec4 __VEC4_tag __attribute__((aligned(16)))
Vec4 A,B,C;
Vec4*Vec4Add(Vec4*VecRes,Vec4*V1,Vec4*V2)
{
	if (!VecRes || !V1 || !V2)
		return 0;
	if (!(__EXTNS[0] & CPUID_FEAT_EDX_SSE)
		|| ((int)VecRes & 0xF || (int)V1 & 0xF || (int)V2 & 0xF))
	{
		for (int i = 0; i < 4; i++)
		{
			// try to not depend on names in struct,
			// as long as struct is only 4 floats
			// also align 16 for some reason
			((float*)(VecRes))[i] = ((float*)(V1))[i] + ((float*)(V2))[i];
		}
	}
	else
	{
		__asm__ __volatile__ (
		/*
			mov eax, [ebp+12] ; V1
			movaps xmm0, [eax]
			mov eax, [ebp+16] ; V2
			addps  xmm0, [eax]
			mov eax, [ebp+8]  ; VecRes
			movaps [eax], xmm0
		*/
			"  .byte    0x8b, 0x45, 0xc, 0xf, 0x28, 0x0, 0x8b, 0x45,"
						"0x10, 0xf, 0x58, 0x0, 0x8b, 0x45, 0x8, 0xf, 0x29, 0x0\n"
			: // unknown opcode "movaps" smh
			:
			:"memory", "eax"
		);
	}
	return VecRes;
}

#if (1)
#define BITSTR_STDIO
#include "bitstr.c" // ????????
// i guess this will have to be done
// if this code is not in a project
// which it isnt

int _start()
{
	GetExts();
	if (CPUIDOK)
	{
		puts("EFLAGS & 0x200000 = OK");
	}
	printf(" EAX = 0 : CPU: %.12s\n", __VENDOR);
	printf(" EAX = 1 | 80000000 :\n");
	bitstr_print(&__EXTNS [0], 32);
	puts("");
	bitstr_print(&__EXTNS [1], 32);
	puts("");
	bitstr_print(&__EXTNS_[1], 32);
	puts("");
	printf(" EAX = 7 , ECX = 0 :\n");
	bitstr_print(&__EXTEXTS[0], 32);
	puts("");
	bitstr_print(&__EXTEXTS[1], 32);
	puts("");
	bitstr_print(&__EXTEXTS[2], 32);
	puts("");
	if (__EXTNS[0] & CPUID_FEAT_EDX_SSE)
		puts("got sse");
	B.x = 0.2; C.x = 0.8;
	B.y = 0.3; C.y = 0.7;
	B.z = 0.4; C.z = 0.6;
	B.w = 0.1; C.w = 0.9;
	Vec4Add(&A,&B,&C);
	return 0;
}

#endif
