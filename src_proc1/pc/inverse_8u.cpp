
extern "C"
{
	
void inverse_8u(const unsigned char *srcArray,  unsigned char *dstArray, int size){
	for(int i = 0; i < size; i++){
		unsigned char temp = 0;
		unsigned char srcTemp = srcArray[i];
		for(int j = 0; j < 8; j++){
			temp <<= 1;
			temp |= srcTemp & 1;			
			srcTemp >>= 1;
		}
		dstArray[i] = temp;
	}
}

void inverse_v4nm8u(const v4nm8u *srcArray, v4nm8u *dstArray, int count){
	unsigned char *src = (unsigned char*)srcArray;
	unsigned char *dst = (unsigned char*)dstArray;
	for(int i = 0; i < count; i++){
		dst[4 * i + 0] = src[4 * i + 0];
		dst[4 * i + 1] = src[4 * i + 1];
		dst[4 * i + 2] = src[4 * i + 2];
		dst[4 * i + 3] = src[4 * i + 3];
	}
	
}

}

