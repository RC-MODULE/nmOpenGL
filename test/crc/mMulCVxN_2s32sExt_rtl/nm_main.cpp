#ifdef __GNUC__
	#define setHeap(n) nmc_malloc_set_heap(n) 
#else
	#define setHeap(n)
#endif


#define SIZE 2048

SECTION(".data_imu1") long long int src[SIZE/32];
		
SECTION(".data_imu2") nm64s aline;
SECTION(".data_imu2") int valuesC[32];
SECTION(".data_imu2") int offsets[32];
SECTION(".data_imu2") int widths[32];
SECTION(".data_imu2") int heights[32];
SECTION(".data_imu2") long long int* ppSrc[32];
	
SECTION(".data_imu3") int dst[32768/2];


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
