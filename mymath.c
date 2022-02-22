
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
	//printf("%u\n",error);
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
// simple linear interpolation
//inline float lerp(float a, float x, float y)
// a = fraction, x = lowest point, h = highest point
//{
	//float s = y - x;
	//float o = x + (a*s);
	//return o;
//}
#define _basic_lerp(a,x,y) (x+(a*(y-x)))
float ltable[] = {0, 0.8415, 0.9093, 0.1411, -0.7568, -0.9589, -0.2794};
float lerp(float x, float f[], unsigned int tsize)
// x = f(x), f = table
{
	unsigned int i;
	while (x < i && i < tsize)
	{
		i++;
	}
	if (i == tsize)
		return f[i-1]; // fallback if value is too high
	return 0;
	//float y = f[i] - f[i-1];
	//x - f[i-1];
	// WORK ON THIS
	//float o;
	//return y;
}

int main()
{
	puts("Hello, World!");
	puts("approximate sin function:");
	float x,y,z = 2.0f;
	for (float i = 0.0f, j = 500.0f; i < j; i++)
	{
		x = sinapprox((i/j)*(pi*z));
		y = sin((i/j)*(pi*z));
		printf("value: % 4.4f, sinapprox: % 4.8f vs. real sin: % 4.8f\n",(i/j),x,y);
	}
	printf("%f\n",_basic_lerp(0.5f,0.9093f,0.1411f));
	printf("%f\n",lerp(2.5f,ltable,7));
	return 0;
}


