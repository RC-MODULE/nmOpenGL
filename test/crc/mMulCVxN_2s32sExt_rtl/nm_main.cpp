
#define NMGL_SIZE 2048

#pragma data_section ".data_imu1"
	long long int src[NMGL_SIZE/32];
		
#pragma data_section ".data_imu2"
	nm64s aline;
	int valuesC[32];
	int offsets[32];
	int widths[32];
	int heights[32];
	long long int* ppSrc[32];
	
#pragma data_section ".data_imu3"
	int dst[32768/2];


extern "C" void mMulCVxN_2s32sExt(nm2s** ppSrcTreangle_2s, int* offsets, int* widths, int* heights, nm32s* pDstTreangle, int* valueC,  int count);

int main()
{		
	for(int i =0;i<32;i++){
		valuesC[i] = 1;
		offsets[i] = 0;
		widths[i] = 32;
		heights[i] = 32;
		ppSrc[i] = src;
	}
	
	mMulCVxN_2s32sExt((nm2s**)ppSrc,offsets,widths,heights,(nm32s*)dst, valuesC, 32);
	
	return t1-t0;
}
