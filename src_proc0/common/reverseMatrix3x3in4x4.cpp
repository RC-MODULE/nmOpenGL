#include "demo3d_common.h"

//SECTION(".data_imu6") float a[3][3];
//float a[3][3];

SECTION(".text_demo3d")
void reverseMatrix3x3in4x4(mat4nm32f* src, mat4nm32f* dst){
	float det = src->matr[0] * src->matr[5] * src->matr[10] + 
				src->matr[1] * src->matr[6] * src->matr[8] + 
				src->matr[2] * src->matr[4] * src->matr[9] - 
				src->matr[2] * src->matr[5] * src->matr[8] -
				src->matr[0] * src->matr[6] * src->matr[9] -
				src->matr[1] * src->matr[4] * src->matr[10];
	float a[3][3];
	a[0][0] =  src->matr[5] * src->matr[10] - src->matr[6] * src->matr[9];
	a[0][1] = -src->matr[4] * src->matr[10] + src->matr[6] * src->matr[8];
	a[0][2] =  src->matr[4] * src->matr[9]  - src->matr[5] * src->matr[8];
	
	a[1][0] = -src->matr[1] * src->matr[10] + src->matr[2] * src->matr[9];
	a[1][1] =  src->matr[0] * src->matr[10] - src->matr[2] * src->matr[8];
	a[1][2] = -src->matr[0] * src->matr[9]  + src->matr[1] * src->matr[8];
	
	a[2][0] =  src->matr[1] * src->matr[6] - src->matr[2] * src->matr[5];
	a[2][1] = -src->matr[0] * src->matr[6] + src->matr[2] * src->matr[4];
	a[2][2] =  src->matr[0] * src->matr[5] - src->matr[1] * src->matr[4];

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