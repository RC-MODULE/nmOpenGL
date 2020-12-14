
extern "C" {
    
    #define SWAP2(x0, y0, x1, y1) temp[0] = x0[i]; \
                                  temp[1] = y0[i]; \
                                  x0[i] = x1[i]; \
                                  y0[i] = y1[i]; \
                                  x1[i] = temp[0]; \
                                  y1[i] = temp[1];

    #define SWAP1(x0, x1) temp[0] = x0[i]; \
                          x0[i] = x1[i]; \
                          x1[i] = temp[0]; \

    
	void sortByY5(float* srcXY0, 
							float* srcXY1,
							float* srcXY2,
                            float* srcS0,
                            float* srcT0,
                            float* srcS1,
                            float* srcT1,
                            float* srcS2,
                            float* srcT2,
                            float* srcW0,
                            float* srcW1,
                            float* srcW2,
							int size){
		float temp[2];
		for(int i=0;i<size;i++){
			if(srcXY0[2 * i + 1] > srcXY2[2 * i + 1]){
				temp[0] = srcXY0[2 * i];
				temp[1] = srcXY0[2 * i + 1];
				srcXY0[2 * i] = srcXY2[2 * i];
				srcXY0[2 * i + 1] = srcXY2[2 * i + 1];
				srcXY2[2 * i] = temp[0];
				srcXY2[2 * i + 1] = temp[1];
                
                SWAP2(srcS0, srcT0, srcS2, srcT2)
                SWAP1(srcW0, srcW2)
                
			}
			if(srcXY0[2 * i + 1] > srcXY1[2 * i + 1]){
				temp[0] = srcXY0[2 * i];
				temp[1] = srcXY0[2 * i + 1];
				srcXY0[2 * i] = srcXY1[2 * i];
				srcXY0[2 * i + 1] = srcXY1[2 * i + 1];
				srcXY1[2 * i] = temp[0];
				srcXY1[2 * i + 1] = temp[1];
                
                SWAP2(srcS0, srcT0, srcS1, srcT1)
                SWAP1(srcW0, srcW1)
			}
			if(srcXY1[2 * i + 1] > srcXY2[2 * i + 1]){
				temp[0] = srcXY1[2 * i];
				temp[1] = srcXY1[2 * i + 1];
				srcXY1[2 * i] = srcXY2[2 * i];
				srcXY1[2 * i + 1] = srcXY2[2 * i + 1];
				srcXY2[2 * i] = temp[0];
				srcXY2[2 * i + 1] = temp[1];
                
                SWAP2(srcS1, srcT1, srcS2, srcT2)
                SWAP1(srcW1, srcW2)
			}
		}
	}	
}