

void mul_v4nm32f_mat4nm32f(float* srcVector4xN,
							float* srcMatrix4x4, 
							float* dstVector4xN, int count){
	static float temp[4];
	int i=0;
	for(i=0;i<count;i++){
		temp[0] = srcMatrix4x4[0]  * srcVector4xN[4 * i + 0] 
				+ srcMatrix4x4[1]  * srcVector4xN[4 * i + 1]
				+ srcMatrix4x4[2]  * srcVector4xN[4 * i + 2]
				+ srcMatrix4x4[3]  * srcVector4xN[4 * i + 3];
		temp[1] = srcMatrix4x4[4]  * srcVector4xN[4 * i + 0] 
				+ srcMatrix4x4[5]  * srcVector4xN[4 * i + 1]
				+ srcMatrix4x4[6]  * srcVector4xN[4 * i + 2]
				+ srcMatrix4x4[7]  * srcVector4xN[4 * i + 3];
		temp[2] = srcMatrix4x4[8]  * srcVector4xN[4 * i + 0] 
				+ srcMatrix4x4[9]  * srcVector4xN[4 * i + 1]
				+ srcMatrix4x4[10] * srcVector4xN[4 * i + 2]
				+ srcMatrix4x4[11] * srcVector4xN[4 * i + 3];
		temp[3] = srcMatrix4x4[12] * srcVector4xN[4 * i + 0] 
				+ srcMatrix4x4[13] * srcVector4xN[4 * i + 1]
				+ srcMatrix4x4[14] * srcVector4xN[4 * i + 2]
				+ srcMatrix4x4[15] * srcVector4xN[4 * i + 3];
		dstVector4xN[4*i + 0] = temp[0];
		dstVector4xN[4*i + 1] = temp[1];
		dstVector4xN[4*i + 2] = temp[2];
		dstVector4xN[4*i + 3] = temp[3];
	}
}
