

void mul_mat4nm32f_v4nm32f(float* srcMatrix4x4, 
							float* srcVector4xN,
							float* dstVector4xN, int count){
	float dstTmp[4];
	float matTmp[16];
	for (int i = 0; i < 16; i++) {
		matTmp[i] = srcMatrix4x4[i];
	}
	int i = 0;
	for(i = 0; i < count; i++){
		dstTmp[0] = matTmp[0]  * srcVector4xN[4 * i + 0] +
				  + matTmp[4]  * srcVector4xN[4 * i + 1] + 
				  + matTmp[8]  * srcVector4xN[4 * i + 2] + 
				  + matTmp[12] * srcVector4xN[4 * i + 3];
		dstTmp[1] = matTmp[1]  * srcVector4xN[4 * i + 0] +
				  + matTmp[5]  * srcVector4xN[4 * i + 1] + 
				  + matTmp[9]  * srcVector4xN[4 * i + 2] + 
				  + matTmp[13] * srcVector4xN[4 * i + 3];
		dstTmp[2] = matTmp[2]  * srcVector4xN[4 * i + 0] +
				  + matTmp[6]  * srcVector4xN[4 * i + 1] + 
				  + matTmp[10] * srcVector4xN[4 * i + 2] + 
				  + matTmp[14] * srcVector4xN[4 * i + 3];
		dstTmp[3] = matTmp[3]  * srcVector4xN[4 * i + 0] +
				  + matTmp[7]  * srcVector4xN[4 * i + 1] + 
				  + matTmp[11] * srcVector4xN[4 * i + 2] + 
				  + matTmp[15] * srcVector4xN[4 * i + 3];
		dstVector4xN[4 * i + 0] = dstTmp[0];
		dstVector4xN[4 * i + 1] = dstTmp[1];
		dstVector4xN[4 * i + 2] = dstTmp[2];
		dstVector4xN[4 * i + 3] = dstTmp[3];
	}
}
