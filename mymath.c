
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
	tsize--;
	if (floorx >= tsize)
		return f[tsize]; // fallback if value is too high
	return _basic_lerp(mod1x,f[i],f[i+1]);
}
/*float sinapprox_wlerp(float x) // try to implement, if even necessary */

unsigned int stpdrandfctr = 0xC289BA22;
unsigned short stupidrandom()
{
	if (stpdrandfctr & 2)
		stpdrandfctr ^= 0x992F8AE3;
	stpdrandfctr += (stpdrandfctr << 7) | (stpdrandfctr >> 9);
	if (stpdrandfctr & 4)
		return stpdrandfctr & 0xFFFF;
	else
		return (stpdrandfctr>>16) & 0xFFFF;
}

int main()
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
	printf("%f\n",_basic_lerp(0.5f,0.9093f,0.1411f));
	printf("%f\n",lerp(2.5f,ltable,7));
	
	stpdrandfctr ^= 195907 * 0xF27793; // manual seed,
	stpdrandfctr -= time(0);
	void*lol = malloc(1); // true way to get random values xd
	free(lol);
	stpdrandfctr += (int)lol;
	puts("pseudorandom");
	for(int i=0;i<40;i++)
	{
		for(int j=0;j<14;j++)
			printf("%5u ", stupidrandom());
		puts("");
	}
	//printf("%f\n",sinapprox(1.0));
	return 0;
}


