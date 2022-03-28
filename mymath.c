
#include <stdlib.h>
#include <math.h>

const int sinprec = 2000;
#define __pi_raw 3.14159265359
const float pi     = __pi_raw;
const float pihalf = __pi_raw/2;
#include "sintable_cheapexp.c"
#define sintable ((float*)rawData)
float sinapprox(float x)
{
	if (!x)
		return 0;
	int error = (((float)x/(float)pihalf)*((float)sinprec));
	error %= sinprec << 2;
	if (error <  sinprec) return sintable[error];
	if (error == sinprec) return 1;
	if (error >= sinprec << 1)
	{
		error -= sinprec << 1;
		if (error == sinprec) return -1;
		if (error >  sinprec)
		{
			error -= sinprec;
			error =  sinprec-error;
		}
		return -sintable[error];
	}
	if (error >= sinprec)
	{
		error -= sinprec;
		return sintable[sinprec-error];
	}
}
// sin'(x)
#define cosapprox(x) sinapprox((x)+pihalf)
// simple linear interpolation
//inline float lerp(float a, float x, float y)
// a = fraction, x = point A, y = point B
//{
	//float s = y - x;
	//float o = x + (a*s);
	//return o;
//}
#define _basic_lerp(a,x,y) (x+(a*(y-x)))
float lerp(float x, float f[], unsigned int tsize)
// x = f(x), f /*lol*/ = table
{
	unsigned int i = 0;
	float mod1x = x, floorx = floor(x);
	while (floorx > i && i < tsize)
	{
		i++;
		mod1x--;
	}
	if (floorx >= --tsize)
		return f[tsize]; // fallback if value is too high
	return _basic_lerp(mod1x,f[i],f[i+1]);
}
/*float sinapprox_wlerp(float x) // try to implement, if even necessary */

unsigned int stpdrandfctr = 0xC289BA22;
unsigned short stupidrandom()
{
	if (stpdrandfctr & 2)
		stpdrandfctr ^= 0x992F8AE3;
	stpdrandfctr ^= (stpdrandfctr << 7) ^ (stpdrandfctr >> 9);
	if (stpdrandfctr & 4)
		return stpdrandfctr & 0xFFFF;
	else
		return (stpdrandfctr>>16) & 0xFFFF;
}

#define oldfastmult

#ifdef  oldfastmult

unsigned int fastmult(unsigned int a, unsigned int b)
// definitely not at all
// fastcall / regparm(2)
// don't care to implement here
// since i now did it in assembly
// and clearly not like this
{
	if (!b)
		return 0;
	switch (b)
	{
		case 1:
			return a;
		case 2:
			return (a<<1);
		case 3:
			return (a<<1)+a;
		case 4:
			return (a<<2);
		case 5:
			return (a<<2)+a;
		case 6:
			return (a<<2)+(a<<1);
		case 7:
			return (a<<2)+(a<<1)+a;
		case 8:
			return (a<<3);
		case 9:
			return (a<<3)+a;
		case 10:
			return (a<<3)+(a<<1);
		case 11:
			return (a<<3)+(a<<1)+a;
		case 12:
			return (a<<3)+(a<<2);
		case 13:
			return (a<<3)+(a<<2)+a;
		case 14:
			return (a<<3)+(a<<2)+(a<<1);
		case 15:
			return (a<<3)+(a<<2)+(a<<1)+a;
		case 16:
			return (a<<4);
			// optimize in assembly?
			// find a pattern in these
			// to not rely on this
			// as if making this will be worth it
			// which it wasnt
		// 0b00000000
		//          +1*val
		//         +2*val
		//        +4*val
		//       +8*val
	}
	return a*b; // fallback stupid thing
	// this would be better suited to leave to
	// compiler if using constant (multiplican) values
}

#endif

float makefloat(int val,int xpn,int mts)
{
	int mtsMask = 0b11111111111111111111111 >> 23-mts;
	int xpnMask = 0b11111111 >> 8-xpn << mts;
	return	/* sign     */ pow(-1, val >> (mts + xpn)) *
			/* exponent */ pow(2,((val&xpnMask)>>mts)-(xpnMask>>mts>>1)) *
			/* fraction */ (1+((float)(val&mtsMask)/(1<<mts)));
}

_start()
{
	puts("Hello, World!");
	puts("approximate sin function:");
	float w,x,y,z = 4.0f;
	for (float i = 0.0f, j = 500.0f; i < j; i++)
	{
		w = (i/j)*(pi*z);
		x = sinapprox(w);
		y = sin(w);
		printf("value: % 4.4f, sinapprox: % 4.8f vs. real sin: % 4.8f\n",(i/j),x,y);
	}
	puts("lerping");
	float ltable[] = {0, 0.8415, 0.9093, 0.1411, -0.7568, -0.9589, -0.2794};
	printf("%f\n",_basic_lerp(0.5f,ltable[2],ltable[3]));
	printf("%f\n",lerp(2.5f,ltable,7));
	
	stpdrandfctr ^= 195907 * 0xF27793; // manual seed,
	//stpdrandfctr -= time(0);
	int*lol = (int*)malloc(16); // true way to get random values xd
	// but also mix with nonzero memory
	for (int i = 0; i < 16; i++)
	{
		stpdrandfctr += lol[i] * 3;
	}
	free(lol);
	stpdrandfctr += (int)lol;
	puts("pseudorandom");
	for(int i=0;i<40;i++)
	{
		for(int j=0;j<14;j++)
			printf("%5u ", stupidrandom());
		puts("");
	}
#ifdef  oldfastmult
	puts("\n\"fast\" multiply");
	for(int i=0;i<32;i++)
	{
		printf("2 * %u = %u\n", i, fastmult(2,i));
	}
#endif
	puts("\nsub32-bit floats");
	printf("%f\n", makefloat(0b0011110000000000,5,10));
	printf("%f\n", makefloat(0b0011110010000000,5,10));
	printf("%f\n", makefloat(0b0011110100000000,5,10));
	printf("%f\n", makefloat(0b0011110110000000,5,10));
	printf("%f\n", makefloat(0b0011111000000000,5,10));
	printf("%f\n", makefloat(0b0011111010000000,5,10));
	printf("%f\n", makefloat(0b0011111100000000,5,10));
	printf("%f\n", makefloat(0b0011111110000000,5,10));
	printf("%f\n", makefloat(0b0100000000000000,5,10));
	printf("%f\n", makefloat(0b0100010000000000,5,10));
	printf("%f\n", makefloat(0b0100110000000000,5,10));
	printf("%f\n", makefloat(0b1111110000000000,5,10));
	//printf("%f\n",sinapprox(1.0))
	char ANALOG_absolute[] = "wesleyissuper awesome yeahdudeahhh";
	signed char ANALOG_relative[sizeof(ANALOG_absolute)];
	char ANALOG_absolute_fromrel[sizeof(ANALOG_absolute)];
	puts("\nrelative value data coding");
	ANALOG_relative[0] = ANALOG_absolute[0];
	ANALOG_absolute_fromrel[0] = ANALOG_absolute[0];
	for (int i = 1; i < sizeof(ANALOG_absolute); i++)
	{
		//if (i)
		{
			ANALOG_relative[i] = ANALOG_absolute[i] - ANALOG_absolute[i-1];
			ANALOG_absolute_fromrel[i] = ANALOG_absolute_fromrel[i-1] + ANALOG_relative[i];
		}
		#if 1
			printf(
				"original data: %1c / %3u / 0x%02X, "
				"relative data: %4d / 0x%02X, ",
				ANALOG_absolute[i], ANALOG_absolute[i], ANALOG_absolute[i],
				ANALOG_relative[i], ANALOG_relative[i] & 0xFF);
			if (i)
				printf(
					"%3u + %4d = ",
					ANALOG_absolute_fromrel[i-1], ANALOG_relative[i]);
			printf(
				"turned back: %1c / %3u / 0x%02X\n",
				ANALOG_absolute_fromrel[i], ANALOG_absolute_fromrel[i],
				ANALOG_absolute_fromrel[i]);
		#endif
	}
	return 0;
}


