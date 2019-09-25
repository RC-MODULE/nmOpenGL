
//void subCRev_v4nm32f(v4nm32f* pSrcV, v4nm32f* pSrcC, v4nm32f* pDst, int size);

begin ".text_demo3d"
global _subCRev_v4nm32f: label;
<_subCRev_v4nm32f>
	ar5 = ar7-2	;
	push ar0,gr0	with gr0 = false;
	push ar1,gr1	with gr0++;
	push ar2,gr2	with gr0++;
	push ar5,gr5	with gr1 = gr0;
	push ar6,gr6	with gr6 = gr0;
	ar1 = [--ar5];
	ar0 = [--ar5];
	ar2 = ar1+gr1	with gr1<<=1;
	ar6 = [--ar5]	with gr2 = gr1;
	gr7 = [--ar5]	with gr5 = gr1;
	ar5 = ar6+gr6	with gr6<<=1;
	
	fpu 0 rep 32 vreg0 = [ar0];
	ar0+=gr0	with gr0 = -gr0;
	fpu 1 rep 32 vreg0 = [ar0];
	ar0+=gr0	with gr0 = gr7<<27;	

	
	gr7>>=5;
	if =0 delayed goto After32;
		gr0 >>=27;
		gr7--;
<Next32>
	fpu 0 rep 32 vreg1 = [ar1++gr1];
	fpu 1 rep 32 vreg1 = [ar2++gr2];
	fpu 0 .float vreg7 = vreg0 - vreg1;
	fpu 1 .float vreg7 = vreg0 - vreg1;
	if > delayed goto Next32 with gr7--;
		fpu 0 rep 32 [ar6++gr6] = vreg7;
		fpu 1 rep 32 [ar5++gr5] = vreg7;
<After32>	
	gr0--;
	vlen = gr0;
	if < delayed goto end_program 	with gr0 = false;
		gr0++;
		gr0++;
	fpu 0 rep vlen vreg0 = [ar0];
	ar0+=gr0;
	fpu 1 rep vlen vreg0 = [ar0];
	fpu 0 rep vlen vreg1 = [ar1++gr1];
	fpu 1 rep vlen vreg1 = [ar2++gr2];
	fpu 0 .float vreg7 = vreg0 - vreg1;
	fpu 1 .float vreg7 = vreg0 - vreg1;
	fpu 0 rep vlen [ar6++gr6] = vreg7;
	fpu 1 rep vlen [ar5++gr5] = vreg7;	
<end_program>
	pop ar6,gr6;
	pop ar5,gr5;
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	return;
end ".text_demo3d";
