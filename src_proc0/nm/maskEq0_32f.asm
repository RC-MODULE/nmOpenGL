
//void maskEq0(v4nm32f* srcVec, v2nm32f* srcMask, v4nm32f* dstVec, int size);
global _maskEq0: label;

begin ".text_demo3d"
<_maskEq0>
	ar5 = ar7-2	;
	push ar0,gr0	with gr7 = gr5;
	push ar1,gr1	with gr0 = false;
	push ar2,gr2	with gr0++;
	push ar4,gr4	with gr0<<=2;
	push ar6,gr6	with gr1 = gr0;
	ar0 = [--ar5]	with gr4 = gr0;
	ar2 = [--ar5]	with gr6 = gr0;
	ar6 = [--ar5];
	gr5 = [--ar5];
	ar4 = ar6 + 2;
	ar1 = ar0 + 2;
	gr2 = gr5<<27;
	gr5>>=5;
	if =0 delayed goto AfterRep32;
		gr2>>=27;
		gr5--;
<NextRep32>
	fpu 0 rep 32 vreg0 = [ar0++gr0];
	fpu 1 rep 32 vreg1 = [ar1++gr1];
	fpu 0 rep 32 vreg4 = [ar2++];
	fpu 1 vreg4 = fpu 0 vreg4;
	fpu 0 .float vreg4 + vreg4, set mask if =0;
	fpu 1 .float vreg4 + vreg4, set mask if =0;
	fpu 0 .float vreg0 = mask ? vreg4 : vreg0;
	fpu 1 .float vreg1 = mask ? vreg4 : vreg1;
	if > delayed goto NextRep32	with gr5--;
		fpu 0 rep 32 [ar6++gr6] = vreg0;
		fpu 1 rep 32 [ar4++gr4] = vreg1;
<AfterRep32>
	gr2--;
	if < delayed goto EndProgram;
		nul;
		nul;
	vlen = gr2;
	fpu 0 rep vlen vreg0 = [ar0++gr0];
	fpu 1 rep vlen vreg1 = [ar1++gr1];
	fpu 0 rep vlen vreg4 = [ar2++];
	fpu 1 vreg4 = fpu 0 vreg4;
	fpu 0 .float vreg4 + vreg4, set mask if =0;
	fpu 1 .float vreg4 + vreg4, set mask if =0;
	fpu 0 .float vreg0 = mask ? vreg4 : vreg0;
	fpu 1 .float vreg1 = mask ? vreg4 : vreg1;
	fpu 0 rep vlen [ar6++gr6] = vreg0;
	fpu 1 rep vlen [ar4++gr4] = vreg1;
<EndProgram>
	
	
	pop ar6,gr6	with gr5 = gr7;
	pop ar4,gr4;
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	return;
end ".text_demo3d";
