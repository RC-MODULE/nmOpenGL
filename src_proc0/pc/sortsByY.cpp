
extern "C" {
	void sortByY3(float* srcXY0, 
							float* srcXY1,
							float* srcXY2,
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
			}
			if(srcXY0[2 * i + 1] > srcXY1[2 * i + 1]){
				temp[0] = srcXY0[2 * i];
				temp[1] = srcXY0[2 * i + 1];
				srcXY0[2 * i] = srcXY1[2 * i];
				srcXY0[2 * i + 1] = srcXY1[2 * i + 1];
				srcXY1[2 * i] = temp[0];
				srcXY1[2 * i + 1] = temp[1];
			}
			if(srcXY1[2 * i + 1] > srcXY2[2 * i + 1]){
				temp[0] = srcXY1[2 * i];
				temp[1] = srcXY1[2 * i + 1];
				srcXY1[2 * i] = srcXY2[2 * i];
				srcXY1[2 * i + 1] = srcXY2[2 * i + 1];
				srcXY2[2 * i] = temp[0];
				srcXY2[2 * i + 1] = temp[1];
			}
		}
	}
	
	void sortByY2(float* srcXY0, 
							float* srcXY1,
							int size){
		float temp[2];
		for(int i=0;i<size;i++){
			if(srcXY0[2 * i + 1] > srcXY1[2 * i + 1]){
				temp[0] = srcXY0[2 * i];
				temp[1] = srcXY0[2 * i + 1];
				srcXY0[2 * i] = srcXY1[2 * i];
				srcXY0[2 * i + 1] = srcXY1[2 * i + 1];
				srcXY1[2 * i] = temp[0];
				srcXY1[2 * i + 1] = temp[1];
			}
		}
	}
}