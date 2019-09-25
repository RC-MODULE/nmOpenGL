
//void dotMulC_AddC_v4nm32f(v2nm32f* n_dot_VP, v4nm32f* srcDiffuse, v4nm32f* srcAmbient, v4nm32f* dst, int size);
global _dotMulC_AddC_v4nm32f: label;

begin ".text_demo3d"
<_dotMulC_AddC_v4nm32f>
	ar5 = ar7-2	;
	push ar0,gr0	with gr7 = false;
	push ar1,gr1	with gr7++;
	push ar2,gr2;
	push ar3,gr3;
	push ar4,gr4;
	push ar5,gr5;
	push ar6,gr6	with gr5 = gr7<<2;
	ar0 = [--ar5]	with gr6 = gr5;
	ar1 = [--ar5];
	ar3 = [--ar5];
	ar6 = [--ar5];
	gr7 = [--ar5];

	ar2 = ar1+2	with gr0 = gr7<<27;
	ar4 = ar3+2	with gr0 >>=27;
	ar5 = ar6+2	with gr0--;
	
	fpu 0 rep 32 vreg1 = [ar1];
	fpu 1 rep 32 vreg1 = [ar2];	
	vlen = gr0	with gr7 >>=5;
	fpu 0 rep 32 vreg2 = [ar3];
	if =0 delayed goto AfterRep32;	
		fpu 1 rep 32 vreg2 = [ar4];
		gr7--;
<NextRep32>
	fpu 0 rep 32 vreg0 = [ar0++];
	fpu 0 .float vreg6 = vreg0 * vreg1 + vreg2;
	fpu 1 vreg0 = fpu 0 vreg0;
	fpu 1 .float vreg6 = vreg0 * vreg1 + vreg2;
	if > delayed goto NextRep32	with gr7--;
		fpu 0 rep 32 [ar6++gr6] = vreg6;
		fpu 1 rep 32 [ar5++gr5] = vreg6;
<AfterRep32>	
	gr0;
	if < delayed goto EndProgram;
		nul;
		nul;
	fpu 0 rep vlen vreg1 = [ar1];
	fpu 1 rep vlen vreg1 = [ar2];
	fpu 0 rep vlen vreg2 = [ar3];
	fpu 1 rep vlen vreg2 = [ar4];
	
	fpu 0 rep vlen vreg0 = [ar0++];
	fpu 0 .float vreg6 = vreg0*vreg1 + vreg2;
	fpu 1 vreg0 = fpu 0 vreg0;
	fpu 1 .float vreg6 = vreg0*vreg1 + vreg2;
	fpu 0 rep vlen [ar6++gr6] = vreg6;
	fpu 1 rep vlen [ar5++gr5] = vreg6;
<EndProgram>	
	pop ar6,gr6;
	pop ar5,gr5;
	pop ar4,gr4;
	pop ar3,gr3;
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	return;
end ".text_demo3d";