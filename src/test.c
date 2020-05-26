#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327
#endif // M_PI

int main()
{
	for (int i = 0; i < 8; i++) {
		printf("tan %dpi/4: %f, cos %dpi/4: %f, sin %dpi/4: %f\n", i, tan(i*M_PI/4), i, cos(i*M_PI/4), i, sin(i*M_PI/4));

	}
 
	return EXIT_SUCCESS;
}
