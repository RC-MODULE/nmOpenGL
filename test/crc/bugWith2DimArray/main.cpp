#include "demo3d_common.h"

SECTION(".data_imu6") float a[3][3];
//float a[3][3];

void reverseMatrix3x3in4x4(float* src, float* dst){
	float det = src[0] * src[5] * src[10] + 
				src[1] * src[6] * src[8] + 
				src[2] * src[4] * src[9] - 
				src[2] * src[5] * src[8] -
				src[0] * src[6] * src[9] -
				src[1] * src[4] * src[10];
	//float a[3][3];
	a[0][0] =  src[5] * src[10] - src[6] * src[9];
	a[0][1] = -src[4] * src[10] + src[6] * src[8];
	a[0][2] =  src[4] * src[9]  - src[5] * src[8];
	
	a[1][0] = -src[1] * src[10] + src[2] * src[9];
	a[1][1] =  src[0] * src[10] - src[2] * src[8];
	a[1][2] = -src[0] * src[9]  + src[1] * src[8];
	
	a[2][0] =  src[1] * src[6] - src[2] * src[5];
	a[2][1] = -src[0] * src[6] + src[2] * src[4];
	a[2][2] =  src[0] * src[5] - src[1] * src[4];

	//write
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			dst->matr[4 * i + j] = a[i][j]/det;
		}
	}
	for(int i = 0; i < 4; i++){
		dst->matr[4 * i + 3] = 0;
		dst->matr[4 * 3 + i] = 0;
	}
	return;
}
int main()
{
	
};

