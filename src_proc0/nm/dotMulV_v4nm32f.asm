
//void dotMulC_Add_v4nm32f(v2nm32f* srcVec, v4nm32f* mulVec, v4nm32f* dst, int size);
global _dotMulV_v4nm32f: label;

begin ".text_demo3d"
<_dotMulV_v4nm32f>
	ar5 = ar7-2	;
	push ar0,gr0;
	push ar1,gr1	with gr1 = false;
	push ar2,gr2	with gr1++;
	push ar5,gr5	with gr1 <<= 2;
	push ar6,gr6	with gr2 = gr1;
	ar0 = [--ar5]	with gr5 = gr1;
	ar1 = [--ar5]	with gr6 = gr1;
	ar6 = [--ar5];
	gr0 = [--ar5];

	ar2 = ar1 + 2;
	ar5 = ar6 + 2;
	
	gr7 = gr0 << 27;
	gr0 >>= 5;
	if =0 delayed goto AfterRep32;
		gr7 >>= 27;
		gr0--;
<NextRep32>
	fpu 0 rep 32 vreg0 = [ar0++];
	fpu 1 vreg0 = fpu 0 vreg0;
	fpu 0 rep 32 vreg1 = [ar1++gr1];	
	fpu 1 rep 32 vreg1 = [ar2++gr2];
	fpu 0 .float vreg7 = vreg0 * vreg1;
	fpu 1 .float vreg7 = vreg0 * vreg1;
	if > delayed goto NextRep32	with gr0--;
		fpu 0 rep 32 [ar6++gr6] = vreg7;
		fpu 1 rep 32 [ar5++gr5] = vreg7;	
<AfterRep32>	
	gr7--;
	if < delayed goto EndProgram;
		vlen = gr7;
		nul;
	fpu 0 rep vlen vreg0 = [ar0++];
	fpu 1 vreg0 = fpu 0 vreg0;
	fpu 0 rep vlen vreg1 = [ar1++gr1];	
	fpu 1 rep vlen vreg1 = [ar2++gr2];
	fpu 0 .float vreg7 = vreg0 * vreg1;
	fpu 1 .float vreg7 = vreg0 * vreg1;
	fpu 0 rep vlen [ar6++gr6] = vreg7;
	fpu 1 rep vlen [ar5++gr5] = vreg7;	
<EndProgram>	
	pop ar6,gr6;
	pop ar5,gr5;
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	return;
end ".text_demo3d";