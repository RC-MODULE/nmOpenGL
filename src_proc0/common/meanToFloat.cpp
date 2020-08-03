#ifdef TEXTURE_ENABLED
extern "C" {
	void meanToFloat3(
		float* src1,
		float* src2,
		float* src3,
		float* result,
		int amm
	)
	{
		for (int i = 0; i < amm; i++) {
			result[i] = ((src1[i] + src2[i] + src3[i])*0.33333333);
		}

	}
}
#endif //TEXTURE_ENABLED
