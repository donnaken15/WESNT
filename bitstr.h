
//#define NO_INLINES // since i'm normally using TCC for C stuff, force this anyway
// APPARENTLY "pointer expected" WHEN HAVING IT ENABLED HERE
#ifndef NO_INLINES
inline char bitstr_get   (char*bits, unsigned int bit);
inline      bitstr_set   (char*bits, unsigned int bit);
inline      bitstr_clear (char*bits, unsigned int bit);
inline      bitstr_toggle(char*bits, unsigned int bit);
#else
	#define bitstr_get(p,b)   (p[b>>3] >>  (7-(b&7)) & 1)
	#define bitstr_set(p,b)    p[b>>3] |=   1 << (7-(b&7))
	#define bitstr_clear(p,b)  p[b>>3] &= ~(1 << (7-(b&7)))
	#define bitstr_toggle(p,b) p[b>>3] ^=   1 << (7-(b&7))
#endif
int 
bitstr_get_a   (char*bits, unsigned int bit, unsigned int len);
bitstr_set_a   (char*bits, unsigned int bit, unsigned val, unsigned int len);
bitstr_clear_a (char*bits, unsigned int bit, unsigned int len);
bitstr_toggle_a(char*bits, unsigned int bit, unsigned int len);

//#define BITSTR_STDIO
#ifdef BITSTR_STDIO
bitstr_print (char*, unsigned int);
#endif
