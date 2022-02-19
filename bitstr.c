
/*
 * WESLEY'S NT
 *
 * Title: Bitstreams
 *            or just bit arrays
 * Author: Wesley
 * License: GPLv3
 *
 * Dependencies: None
 *
 */

// bitstream functions
// bit set from left to right
// nth : 01234567 89
// data: 00000000 00
// set bit 0:
//       10000000
// set bit 6:
//       00000010

//#define NO_INLINES
// THANKS FABRICE
// though this isn't a major blow tbh

// TODO: use bit test ops if possible (BT,BTS,BTR,BTC)
// actually that's apparently slower than AND,OR,XOR
// wtf, but i'd kind of understand why

// massive TODO:
// dont operate bitwise on every single bit for multibyte functions
// only per byte and adjust with where bit changes start

/*
 * Common usage:
 * bits - byte array
 * bit  - Nth bit, from left to right disregarding endianness
 *                                    (not sure about multibyte funcs though)
 *                                    (tested only with little endian ints)
 *
 * Other args:
 * len  - bit length of int to get
 * val  - set bits in bitstream to this value
 *
 */
#ifndef NO_INLINES
//   BITSTREAM: GET
inline char bitstr_get   (char*bits, unsigned int bit) {
	return (bits[bit>>3] >> (7-(bit&7)) & 1);
}
//   BITSTREAM: SET
inline void bitstr_set   (char*bits, unsigned int bit) {
	bits[bit>>3] |= (1) << (7-(bit&7));
}
//   BITSTREAM: CLEAR
inline void bitstr_clear (char*bits, unsigned int bit) {
	bits[bit>>3] &= ~(1 << (7-(bit&7)));
}
//   BITSTREAM: TOGGLE
inline void bitstr_toggle(char*bits, unsigned int bit) {
	bits[bit>>3] ^= 1 << (7-(bit&7));
}
#endif
//
// which way should bits start from
// like for 0b11001010:
// 11001010_00000000_00000000_00000000
// 00000000_00000000_00000000_01010011
// first method looks better or something
//
//   BITSTREAM: GET MULTIBYTE
int  bitstr_get_a   (char*bits, unsigned int bit, unsigned int len) {
	int retval = 0;
	int curbit = 0;
	for (int i = 0; i < len; i++)
	{
		curbit = bit + i;
		retval |= (bits[curbit>>3] >> (7-(curbit&7)) & 1) << (7-(i&7)) << (i&~7);
	}
	return retval;
}
//   BITSTREAM: SET MULTIBYTE
void bitstr_set_a   (char*bits, unsigned int bit, unsigned val, unsigned int len) {
	int curbit = 0;
	int onoroff = 0;
	int bitplace = 0;
	for (int i = 0; i < len; i++)
	{
		curbit = bit + i;
		onoroff = (val >> (i&~7) >> (7-(i&7)) & 1);
		bitplace = 1 << (7-(i&7));
		if (onoroff)
			bits[curbit>>3] |=  bitplace;
		else
			bits[curbit>>3] &= ~bitplace;
	}
}
//   BITSTREAM: CLEAR MULTIBYTE
void bitstr_clear_a (char*bits, unsigned int bit, unsigned int len) {
	int curbit = 0;
	for (int i = 0; i < len; i++)
	{
		curbit = bit + i;
		bits[curbit>>3] &= ~(1 << (7-(curbit&7)));
	}
}
//   BITSTREAM: TOGGLE MULTIBYTE
void bitstr_toggle_a(char*bits, unsigned int bit, unsigned int len) {
	// should add argument to xor using val
	int curbit = 0;
	for (int i = 0; i < len; i++)
	{
		curbit = bit + i;
		bits[curbit>>3] ^= 1 << (7-(curbit&7));
	}
}

//#define BITSTR_STDIO
#ifdef BITSTR_STDIO
void bitstr_print (char*bits, unsigned int count)
{
	for (int i = 0; i < count; i++)
		printf("%u",bitstr_get(bits,i));
}
#endif
