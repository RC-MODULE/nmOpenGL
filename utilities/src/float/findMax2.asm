/*!
 *  \file findMax2.asm
 *  \author Zhilenkov Ivan
 */
//void findMax2(float* src1, float* src2, float* dstMax, int size);
global _findMax2: label;

begin ".text_demo3d"
<_findMax2>
	ar5 = ar7-2;
	push ar0,gr0;
	push ar1,gr1;
	push ar4,gr4;
	ar0 = [--ar5];
	ar1 = [--ar5];
	ar4 = [--ar5];
	gr7 = [--ar5];
	gr7>>=1;
	gr0 = gr7<<27;
	gr7>>=5;
	if =0 delayed goto AfterRep32;
		gr0>>=27;
		gr7--;
<NextRep32>
	fpu 0 rep 32 vreg0 = [ar0++];
	fpu 0 rep 32 vreg1 = [ar1++];
	fpu 0 .float vreg0 - vreg1, set mask if >;
	fp0_lmask = fp0_lmask;
	fpu 0 .float vreg1 = mask ? vreg0;
	if > delayed goto NextRep32 with gr7--;
		fpu 0 rep 32 [ar4++] = vreg1;	
		nul;
<AfterRep32>
	gr0--;
	if < delayed goto end_program;
		nul;
		nul;
	vlen = gr0;
	fpu 0 rep vlen vreg0 = [ar0++];
	fpu 0 rep vlen vreg1 = [ar1++];
	fpu 0 .float vreg0 - vreg1, set mask if >;
	fp0_lmask = fp0_lmask;
	fpu 0 .float vreg1 = mask ? vreg0;
	fpu 0 rep vlen [ar4++] = vreg1;
		
<end_program>
	pop ar4,gr4;
	pop ar1,gr1;
	pop ar0,gr0;
	return;
end ".text_demo3d";
