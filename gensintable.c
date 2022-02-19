
#include <stdio.h>
#include <math.h>

// use for an approximate sin function

FILE*sinTable;
double x, PI = 3.14159265359, y, z = 1024;
float  w;
int _start()
{
	sinTable = fopen("sintable.bin","wb");
	for(y = 0; y < z; y++)
	{
		x = sin(PI*((y)/z));
		//fwrite(&x,8,1,sinTable); // write doubles
		w = x;
		fwrite(&w,4,1,sinTable); // write floats
	}
	fclose(sinTable);
}

