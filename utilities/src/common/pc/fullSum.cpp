
extern "C"{
	int totalSum(int* pVec, int size){
		int result = 0;
		int i;
		for(i = 0; i < size; i++){
			result += pVec[i];
		}
		return result;
	}
}
