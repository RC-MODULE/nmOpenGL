
extern "C"{
	void loadIdentify(float* matrix){
		for(int i=0;i<4;i++){
			for (int j = 0; j < 4; j++) {
				matrix[i * 4 + j] = (i == j) ? 1 : 0;
			}
		}
	}
}