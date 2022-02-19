
//#define NO_INLINES
#ifndef NO_INLINES
inline char bitstr_get   (char*bits, unsigned int bit);
inline void bitstr_set   (char*bits, unsigned int bit);
inline void bitstr_clear (char*bits, unsigned int bit);
inline void bitstr_toggle(char*bits, unsigned int bit);
#else
	#define bitstr_get(p,b)   (p[b>>3] >>  (7-(b&7)) & 1)
	#define bitstr_set(p,b)    p[b>>3] |=   1 << (7-(b&7))
	#define bitstr_clear(p,b)  p[b>>3] &= ~(1 << (7-(b&7)))
	#define bitstr_toggle(p,b) p[b>>3] ^=   1 << (7-(b&7))
#endif
int  bitstr_get_a   (char*bits, unsigned int bit, unsigned int len);
void bitstr_set_a   (char*bits, unsigned int bit, unsigned val, unsigned int len);
void bitstr_clear_a (char*bits, unsigned int bit, unsigned int len);
void bitstr_toggle_a(char*bits, unsigned int bit, unsigned int len);

//#define BITSTR_STDIO
#ifdef BITSTR_STDIO
void bitstr_print (char*bits, unsigned int count);
#endif
