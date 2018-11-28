#define _USE_MATH_DEFINES
#include <cmath>

double	
				// »сходные значени¤ параметров L, R, B, T
	L=-M_PI,
	R=M_PI,
	B=-1.5,
	T=1.5;

double Parabola(double x)
{
	return x*x-2;
}

double Sinusoid(double x)
{
	return sin(x);
}


double polarSpiral(double phi)
{
	return 0.125*phi;
}

double polarRose(double phi)
{
	return sin(4 * phi);
}

