
//void ternaryLt0_AddC_AddC_32f(nm32f* srcFlags, nm32f* srcVec, float valueLeft, float valueRight, float* dstVec, int size);
global _ternaryLt0_AddC_AddC_32f: label;

data ".data_demo3d"
	valueLeft: long;
	valueRight: long;
end ".data_demo3d";

begin ".text_demo3d"
<_ternaryLt0_AddC_AddC_32f>
	ar5 = ar7-2;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
	push ar5,gr5;
	push ar6,gr6;
	ar1 = [--ar5];
	ar0 = [--ar5];
	gr2 = [--ar5];
	ar2 = gr2;
	[valueLeft] = ar2,gr2;
	ar2 = valueLeft;
	fpu 0 rep 1 vreg4 = [ar2];
	gr2 = [--ar5];
	ar2 = gr2;
	[valueRight] = ar2,gr2;
	ar2 = valueRight;
	fpu 1 rep 1 vreg4 = [ar2];
	ar6 = [--ar5];
	gr5 = [--ar5];
	gr0 = gr5 << 26;
	gr5>>=6;
	if =0 delayed goto AfterRep32;
		gr0>>=27;
		gr5--;
	fpu 0 rep 32 vreg6 = [ar2];
	fpu 0 .float vreg6 = vreg6 - vreg6;
<NextRep32>
	fpu 0 rep 32 vreg1 = [ar1++];
	fpu 0 .float vreg5 = vreg1 + vreg6, set mask if <;
	fpu 0 rep 32 vreg0 = [ar0++];
	fpu 1 vreg0 = fpu 0 vreg0;
	fpu 0 .float vreg2 = vreg0 + .retrive(vreg4);
	fpu 1 .float vreg2 = vreg0 + .retrive(vreg4);
	fpu 2 vreg3 = fpu 0 vreg2;
	fpu 2 vreg2 = fpu 1 vreg2;
	fp2_lmask = fp0_lmask;
	fp2_hmask = fp0_hmask;
	if > delayed goto NextRep32 with gr5--;
		fpu 2 .float vreg2 = mask ? vreg3: vreg2;
		fpu 2 rep 32 [ar6++] = vreg2;
<AfterRep32>
	gr0--;
	if < delayed goto end_program;
		vlen = gr0;
		nul;
	fpu 0 rep vlen vreg6 = [ar2];
	fpu 0 .float vreg6 = vreg6 - vreg6;
	fpu 0 rep vlen vreg1 = [ar1++];
	fpu 0 .float vreg5 = vreg1 + vreg6, set mask if <;
	fpu 0 rep vlen vreg0 = [ar0++];
	fpu 1 vreg0 = fpu 0 vreg0;
	fpu 0 .float vreg2 = vreg0 + .retrive(vreg4);
	fpu 1 .float vreg2 = vreg0 + .retrive(vreg4);
	fpu 2 vreg3 = fpu 0 vreg2;
	fpu 2 vreg2 = fpu 1 vreg2;
	fp2_lmask = fp0_lmask;
	fp2_hmask = fp0_hmask;
	fpu 2 .float vreg2 = mask ? vreg3: vreg2;
	fpu 2 rep vlen [ar6++] = vreg2;
<end_program>
	pop ar6,gr6;
	pop ar5,gr5;
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	return;
end ".text_demo3d";