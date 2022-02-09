
void dotProdVector4C_Vector4xN(float* srcVector4xN, float* pVec4C, float* dst, int count){
	for(int i=0;i < count; i++){
		dst[i] = srcVector4xN[4 * i + 0] * pVec4C[0] + 
				 srcVector4xN[4 * i + 1] * pVec4C[1] + 
				 srcVector4xN[4 * i + 2] * pVec4C[2] + 
				 srcVector4xN[4 * i + 3] * pVec4C[3];
	}
}