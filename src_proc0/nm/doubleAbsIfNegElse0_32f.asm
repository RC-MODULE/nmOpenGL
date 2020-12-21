/*!
 *  \file doubleAbsIfNegElse0_32f.asm
 *  \author Zhilenkov Ivan
 */
//вычисляет модуль если число отрицательное, иначе записывает ноль
global _doubleAbsIfNegElse0_32f:label;
//void doubleAbsIfNegElse0_32f(float* src1, float* src2, float* dst1, float* dst2, int size);

data ".data_demo3d"
	zero_zero: word[2] = (float(0), float(0)); 
end ".data_demo3d";


begin ".text_demo3d"
<_doubleAbsIfNegElse0_32f>
	ar5 = sp - 2;
	push ar0, gr0	with gr7 = gr5;
	push ar1, gr1;
	push ar4, gr4;
	push ar6, gr6;
	ar0 = [--ar5];
	ar1 = [--ar5];
	ar6 = [--ar5];
	ar4 = [--ar5];
	gr5 = [--ar5];
	gr0 = gr5 << 26;
	ar5 = zero_zero	with gr5>>=6;
	if =0 delayed goto AfterRep32;
		gr0 >>= 27;
		nul;
	fpu 0 rep 32 vreg1 = [ar5];		//vreg1 = 0		
	fpu 1 vreg1 = fpu 0 vreg1;		//vreg1 = 0		
<NextRep32>
	fpu 0 rep 32 vreg0 = [ar0++];
	fpu 1 rep 32 vreg0 = [ar1++];
	fpu 0 .float vreg2 = vreg1 - vreg0, set mask if <;		//-src
	fpu 1 .float vreg2 = vreg1 - vreg0, set mask if <;		//-src
	fp0_lmask = fp0_lmask;
	fp1_lmask = fp1_lmask	with gr5--;
	fpu 0 .float vreg2 = mask ? vreg1: vreg2;				//(src < 0) ? -src: 0;
	fpu 1 .float vreg2 = mask ? vreg1: vreg2;				//(src < 0) ? -src: 0;
	if > delayed goto NextRep32;
		fpu 0 rep 32 [ar6++] = vreg2;	
		fpu 1 rep 32 [ar4++] = vreg2;	
<AfterRep32>
	gr0--;
	if < delayed goto EndProgram;
		vlen = gr0;
		nul;
		
	fpu 0 rep vlen vreg1 = [ar5];		//vreg1 = 0		
	fpu 1 vreg1 = fpu 0 vreg1;		//vreg1 = 0		
	fpu 0 rep vlen vreg0 = [ar0++];
	fpu 1 rep vlen vreg0 = [ar1++];
	fpu 0 .float vreg2 = vreg1 - vreg0, set mask if <;		//-src
	fpu 1 .float vreg2 = vreg1 - vreg0, set mask if <;		//-src
	fp0_lmask = fp0_lmask;
	fp1_lmask = fp1_lmask;
	fpu 0 .float vreg2 = mask ? vreg1: vreg2;				//(-src > 0) ? -src: 0;
	fpu 1 .float vreg2 = mask ? vreg1: vreg2;				//(-src > 0) ? -src: 0;
	fpu 0 rep vlen [ar6++] = vreg2;
	fpu 1 rep vlen [ar4++] = vreg2;
		
<EndProgram>	
	pop ar6, gr6;
	pop ar4, gr4;
	pop ar1, gr1;
	pop ar0, gr0	with gr5 = gr7;
	return;
end ".text_demo3d";
