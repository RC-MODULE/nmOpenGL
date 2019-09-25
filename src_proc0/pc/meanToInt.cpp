extern "C" {
	void meanToInt3(
		float* src1,
		float* src2,
		float* src3,
		int* result,
		int amm
	)
	{
		for (int i = 0; i < amm; i++) {
			result[i] = (int)((src1[i] + src2[i] + src3[i])*0.33333333);
		}

	}
	
	void meanToInt2(
		float* src1,
		float* src2,
		int* result,
		int amm
	)
	{
		for (int i = 0; i < amm; i++) {
			result[i] = (int)((src1[i] + src2[i]) * 0.5);
		}

	}
}