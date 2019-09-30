
//вычисляет модуль если число отрицательное, иначе записывает ноль
global _absIfNegElse0_32f:label;
//void absIfNegElse0_32f(float* src, float* dst, int size);



begin ".text_demo3d"
<_absIfNegElse0_32f>
	ar5 = sp - 2;
	push ar0, gr0	with gr7 = gr5;
	push ar6, gr6;
	ar0 = [--ar5];
	ar6 = [--ar5];
	gr5 = [--ar5];
	gr7 = gr5 << 26;
	gr5>>=6;
	if =0 delayed goto AfterRep32;
		gr7 >>= 27;
		nul;
	
	fpu 0 .float vreg1 = vreg0 - vreg0;		//vreg1 = 0
<NextRep32>
	fpu 0 rep 32 vreg0 = [ar0++];
	fpu 0 .float vreg2 = vreg1 - vreg0, set mask if >;		//-src
	fp0_lmask = fp0_lmask	with gr5--;
	if > delayed goto NextRep32;
		fpu 0 .float vreg1 = mask ? vreg2: vreg1;				//(src < 0) ? -src: 0;
		fpu 0 rep 32 [ar6++] = vreg1;	
<AfterRep32>
	gr7--;
	if > delayed goto EndProgram;
		vlen = gr7;
		nul;
		
	fpu 0 rep vlen vreg0 = [ar0++];
	fpu 0 .float vreg2 = vreg1 - vreg0, set mask if >;		//-src
	fp0_lmask = fp0_lmask;
	fpu 0 .float vreg1 = mask ? vreg2: vreg1;				//(-src > 0) ? -src: 0;
	fpu 0 rep vlen [ar6++] = vreg1;
		
<EndProgram>	
	pop ar6, gr6;
	pop ar0, gr0	with gr5 = gr7;
	return;
end ".text_demo3d";
