
//Вычитает из вектора константуж
global _doubleAdd_32f:label;
//void doubleAdd_32f(float* src1, float* src2, float* srcAdd1, float* srcAdd2, float* dst1, float* dst2, int size);

begin ".text_demo3d"
<_doubleAdd_32f>
	ar5 = sp - 2;
	push ar0, gr0	with gr7 = gr5;
	push ar1, gr1;
	push ar2, gr2;
	push ar3, gr3;
	push ar4, gr4;
	push ar6, gr6;
	ar0 = [--ar5];
	ar1 = [--ar5];
	ar2 = [--ar5];
	ar3 = [--ar5];	
	ar6 = [--ar5];
	ar4 = [--ar5];
	gr5 = [--ar5];
	
	gr0 = gr5 << 26;
	gr5 >>= 6;
	if =0 delayed goto AfterRep32;
		gr0 >>= 27;
		gr5--;
<NextRep32>
	fpu 0 rep 32 vreg0 = [ar0++];
	fpu 0 rep 32 vreg1 = [ar2++];
	fpu 1 rep 32 vreg0 = [ar1++];
	fpu 1 rep 32 vreg1 = [ar3++];
	fpu 0 .float vreg7 = vreg0 + vreg1;
	fpu 1 .float vreg7 = vreg0 + vreg1;
	if > delayed goto NextRep32	with gr5--;
		fpu 0 rep 32 [ar6++] = vreg7;
		fpu 1 rep 32 [ar4++] = vreg7;
<AfterRep32>
	gr0--;
	if < delayed goto EndProgram;
		vlen = gr0;
		nul;
	fpu 0 rep vlen vreg0 = [ar0++];
	fpu 0 rep vlen vreg1 = [ar2++];
	fpu 1 rep vlen vreg0 = [ar1++];
	fpu 1 rep vlen vreg1 = [ar3++];
	fpu 0 .float vreg7 = vreg0 + vreg1;
	fpu 1 .float vreg7 = vreg0 + vreg1;
	fpu 0 rep vlen [ar6++] = vreg7;
	fpu 1 rep vlen [ar4++] = vreg7;
<EndProgram>
	pop ar6, gr6;
	pop ar4, gr4;
	pop ar3, gr3;
	pop ar2, gr2;
	pop ar1, gr1;
	pop ar0, gr0	with gr5 = gr7;
	return;
end ".text_demo3d";
