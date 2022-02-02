/*!
 *  \file copyOddOffset.asm
 *  \author Zhilenkov Ivan
 */
//void copyOddOffset(float* src, float* dst, int first, int size);

data ".data_demo3d"
	zero_one: word[2] = (0, float(1));
	one_zero: word[2] = (float(1), 0);
end ".data_demo3d";
global _copyOddOffset:label;
begin ".text_demo3d"
<_copyOddOffset>
	ar5 = ar7-2;
	push ar0,gr0	with gr7 = gr5;
	push ar1,gr1;
	push ar6,gr6;
	ar0 = [--ar5];
	ar6 = [--ar5];
	gr1 = [--ar5];
	gr5 = [--ar5]	with gr0 = gr1>>1;
	gr0 <<= 1;
	ar0 += gr0	with 	gr5>>=1;
	
	ar1 = zero_one	with gr1 = gr5 << 27;
	fpu 1 rep 32 vreg4 = [ar1];
	fpu 1 .float vreg5 = vreg4 - vreg4;
	fpu 0 vreg2 = fpu 1 vreg5;
	ar1 = one_zero	with gr5>>=5;
	fpu 0 rep 32 vreg3 = [ar1];	
	if =0 delayed goto AfterRep32Odd	with gr1>>=27;
		gr1--;
		vlen = gr1;
	ar1 = ar0 + 2	with gr5--;
<NextRep32Odd>
	fpu 0 rep 32 vreg0 = [ar0++];
	fpu 0 .matrix vreg6 = vreg0 * (vreg2, vreg3);
	fpu 1 vreg6 = fpu 0 vreg6;
	fpu 1 rep 32 vreg0 = [ar1++];
	if > delayed goto NextRep32Odd	with gr5--;
		fpu 1 .matrix vreg7 = vreg0 * (vreg4, vreg5) + vreg6;
		fpu 1 rep 32 [ar6++] = vreg7;	
		//fpu 0 rep 32 [ar6++] = vreg3;	
<AfterRep32Odd>		
	gr1;
	if < delayed goto endProgram;
		ar1 = zero_one;
	fpu 1 rep vlen vreg4 = [ar1];
	fpu 1 .float vreg5 = vreg4 - vreg4;
	fpu 0 vreg2 = fpu 1 vreg5;
	ar1 = one_zero;
	fpu 0 rep vlen vreg3 = [ar1];	
	
	ar1 = ar0 + 2;
	fpu 0 rep vlen vreg0 = [ar0++];
	fpu 0 .matrix vreg6 = vreg0 * (vreg2, vreg3);
	fpu 1 vreg6 = fpu 0 vreg6;
	fpu 1 rep vlen vreg0 = [ar1++];
	fpu 1 .matrix vreg7 = vreg0 * (vreg4, vreg5) + vreg6;
	fpu 1 rep vlen [ar6++] = vreg7;	
<endProgram>
	
	pop ar6,gr6	with gr5 = gr7;
	pop ar1,gr1;
	pop ar0,gr0;
	return;
end ".text_demo3d";