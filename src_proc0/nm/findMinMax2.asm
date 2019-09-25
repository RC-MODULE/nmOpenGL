
//void findMinMax2(float* src1, float* src2, float* dstMin, float* dstMax, int size);
global _findMinMax2: label;

begin ".text_demo3d"
<_findMinMax2>
	ar5 = ar7-2;
	push ar0,gr0;
	push ar1,gr1;
	push ar4,gr4;
	push ar6,gr6;
	ar0 = [--ar5];
	ar1 = [--ar5];
	ar6 = [--ar5];
	ar4 = [--ar5];
	gr7 = [--ar5];
	gr7>>=1;
	gr0 = gr7<<27;
	gr7>>=5;
	if =0 delayed goto AfterRep32;
		gr0>>=27;
		gr7--;
<NextRep32>
	//fpu0 
	fpu 0 rep 32 vreg0 = [ar0++];
	fpu 1 vreg0 = fpu 0 vreg0;
	fpu 1 rep 32 vreg1 = [ar1++];
	fpu 0 vreg1 = fpu 1 vreg1;
	fpu 0 .float vreg0 - vreg1, set mask if <;
	fp0_lmask = fp0_lmask;
	fpu 1 .float vreg0 - vreg1, set mask if >;
	fp1_lmask = fp1_lmask;
	fpu 0 .float vreg1 = mask ? vreg0;
	fpu 1 .float vreg1 = mask ? vreg0;
	if > delayed goto NextRep32 with gr7--;
		fpu 0 rep 32 [ar6++] = vreg1;
		fpu 1 rep 32 [ar4++] = vreg1;	
<AfterRep32>
	gr0--;
	if < delayed goto end_program;
		nul;
		nul;
	vlen = gr0;
	fpu 0 rep vlen vreg0 = [ar0++];
	fpu 1 vreg0 = fpu 0 vreg0;
	fpu 1 rep vlen vreg1 = [ar1++];
	fpu 0 vreg1 = fpu 1 vreg1;
	fpu 0 .float vreg0 - vreg1, set mask if <;
	fp0_lmask = fp0_lmask;
	fpu 1 .float vreg0 - vreg1, set mask if >;
	fp1_lmask = fp1_lmask;
	fpu 0 .float vreg1 = mask ? vreg0;
	fpu 1 .float vreg1 = mask ? vreg0;
	fpu 0 rep vlen [ar6++] = vreg1;
	fpu 1 rep vlen [ar4++] = vreg1;
		
<end_program>
	pop ar6,gr6;
	pop ar4,gr4;
	pop ar1,gr1;
	pop ar0,gr0;
	return;
end ".text_demo3d";