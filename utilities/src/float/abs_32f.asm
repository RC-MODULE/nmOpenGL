/*!
 *  \file abs_32f.asm
 *  \author Zhilenkov Ivan
 */
global _abs_32f:label;
//void abs_32f(float* src, float* dst, int size);

begin ".text_demo3d"
<_abs_32f>
	ar5 = sp - 2;
	push ar0, gr0	with gr7 = gr5;
	push ar6, gr6;
	ar0 = [--ar5];
	ar6 = [--ar5];
	gr5 = [--ar5];
	gr0 = gr5 << 26;
	gr5>>=6;
	if =0 delayed goto AfterRep32;
		gr0 >>= 27;
		nul;
<NextRep32>
	fpu 0 rep 32 vreg0 = [ar0++];
	fpu 0 .float vreg2 = /vreg0/;//abs(src)
	gr5--;
	if > delayed goto NextRep32;
		fpu 0 rep 32 [ar6++] = vreg2;
		nul;
<AfterRep32>
	gr0--;
	if < delayed goto EndProgram;
		vlen = gr0;
		nul;
	fpu 0 rep vlen vreg0 = [ar0++];
	fpu 0 .float vreg2 = /vreg0/;		//abs(src)
	fpu 0 rep vlen [ar6++] = vreg2;
		
<EndProgram>	
	pop ar6, gr6;
	pop ar0, gr0	with gr5 = gr7;
	return;
end ".text_demo3d";
