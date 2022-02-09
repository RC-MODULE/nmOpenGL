

/*void mulMatrix4x4_Vector4xN(float* srcMatrix4x4, 
							float* srcArrayX, float* srcArrayY, 
							float* srcArrayZ, float* srcArrayW, 
							float* dstVector4xN, int count){
	float temp[4];
	int i=0;
	for(i=0;i<count;i++){
		temp[0] = srcMatrix4x4[0]  * srcArrayX[i] 
				+ srcMatrix4x4[4]  * srcArrayY[i]
				+ srcMatrix4x4[8]  * srcArrayZ[i]
				+ srcMatrix4x4[12] * srcArrayW[i];
		temp[1] = srcMatrix4x4[1]  * srcArrayX[i] 
				+ srcMatrix4x4[5]  * srcArrayY[i]
				+ srcMatrix4x4[9]  * srcArrayZ[i]
				+ srcMatrix4x4[13] * srcArrayW[i];
		temp[2] = srcMatrix4x4[2]  * srcArrayX[i] 
				+ srcMatrix4x4[6]  * srcArrayY[i]
				+ srcMatrix4x4[10] * srcArrayZ[i]
				+ srcMatrix4x4[14] * srcArrayW[i];
		temp[3] = srcMatrix4x4[3]  * srcArrayX[i] 
				+ srcMatrix4x4[7]  * srcArrayY[i]
				+ srcMatrix4x4[11] * srcArrayZ[i]
				+ srcMatrix4x4[15] * srcArrayW[i];
		dstVector4xN[4*i + 0] = temp[0];
		dstVector4xN[4*i + 1] = temp[1];
		dstVector4xN[4*i + 2] = temp[2];
		dstVector4xN[4*i + 3] = temp[3];
	}
}*/

void mulMatrix4x4_Vector4xN(float* srcMatrix4x4, 
							float* srcVector4xN,
							float* dstVector4xN, int count){
	float temp[4];
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
