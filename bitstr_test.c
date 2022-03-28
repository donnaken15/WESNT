
#define BITSTR_STDIO
#include "bitstr.c"
#include "bitstr.h"
#include <stdio.h>

int bitfield_0 = 0b10001110 | (0b10010100 << 8) | (0b10101011 << 16) | (0b01001000 << 24);
int bitfield_1 = 0;
int bitfield_2 = 0b00110100 | (0b01110001 << 8) | (0b01100100 << 16) | (0b00111000 << 24);

main()
{
	printf("Hello, World!\n");
	printf("shiver in eternal darkness\n");
	bitstr_print(&bitfield_0,32);
	puts("");
	bitstr_toggle(&bitfield_0,1);
	bitstr_print(&bitfield_0,32);
	puts("");
	bitstr_toggle(&bitfield_0,2);
	bitstr_print(&bitfield_0,32);
	puts("");
	bitstr_toggle(&bitfield_0,3);
	bitstr_print(&bitfield_0,32);
	puts("");
	puts("");
	bitstr_print(&bitfield_1,32);
	puts("");
	bitfield_1 = bitstr_get_a(&bitfield_0,0,32);
	bitstr_print(&bitfield_1,32);
	puts("");
	bitstr_set_a(&bitfield_0,0,0b00101111100000000000000011000001,32);
	bitstr_print(&bitfield_0,32);
	puts("");
	bitstr_set_a(&bitfield_1,0,bitfield_2,32);
	bitstr_print(&bitfield_1,32);
	puts("");
	bitstr_set_a(&bitfield_1,0,0b11111111111111111111111111111111,32);
	bitstr_print(&bitfield_1,32);
	puts("");
	bitstr_clear_a(&bitfield_1,0,8);
	bitstr_print(&bitfield_1,32);
	puts("");
	bitstr_clear_a(&bitfield_1,0,12);
	bitstr_print(&bitfield_1,32);
	puts("");
	bitstr_clear_a(&bitfield_1,0,16);
	bitstr_print(&bitfield_1,32);
	puts("");
	bitstr_clear_a(&bitfield_1,0,24);
	bitstr_print(&bitfield_1,32);
	puts("");
	bitstr_clear_a(&bitfield_1,0,32);
	bitstr_print(&bitfield_1,32);
	puts("");
	bitstr_toggle_a(&bitfield_1,0,32);
	bitstr_print(&bitfield_1,32);
	puts("");
	return 0;
}


