#include "demo3d_common.h"
#include "nmblas.h"
#include "demo3d_nm0.h"
#include "nmgltex_nm0.h"
#include "nmgltex_common.h"

namespace nm0_version {

SECTION(".data_imu0") float initVecx [32] = {0.5,   1.5,  2.5,  3.5,  4.5,  5.5,  6.5,  7.5,  8.5,  9.5, 
										10.5, 11.5, 12.5, 13.5, 14.5, 15.5, 16.5, 17.5, 18.5, 19.5,
										20.5, 21.5, 22.5, 23.5, 24.5, 25.5, 26.5, 27.5, 28.5, 29.5,
										30.5, 31.5};
SECTION(".data_imu0") float initVecy [32] = {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 
										0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 
										0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 
										0.5, 0.5};
SECTION(".data_imu0") float vecxf [32];
SECTION(".data_imu0") float vecyf [32];
SECTION(".data_imu0") float vecx [32];
SECTION(".data_imu0") float vecy [32];


SECTION(".data_imu0") float ones [32] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 
										1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 
										1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 
										1.0, 1.0};

SECTION(".data_imu0") float oneOverDenominator [32];
SECTION(".data_imu0") float derivOneOverDenom [32];

SECTION(".data_imu0") float buf0 [32];

SECTION(".text")
int minf(float a, float b)
{
	return (b < a) ? b : a;
}

SECTION(".text")
int maxf(float a, float b)
{
	return (b > a) ? b : a;
}


SECTION(".text")
void edgeFunction(float x0, float y0, float x1, float y1, float x2, float y2, float* res)
{
    *res = (x2 - x0) * (y1 - y0) - (y2 - y0) * (x1 - x0);
}

} //end of namespace nm0_version

