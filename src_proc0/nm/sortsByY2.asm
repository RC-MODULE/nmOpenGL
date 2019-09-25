//void sortByY2(v2nm32f* pAxy,
//						 v2nm32f* pBxy,
//						 int size)

global _sortByY2: label;

begin ".text_demo3d"
<_sortByY2>
	ar5 = ar7-2;
	push ar0, gr0;
	push ar1, gr1;
	push ar5, gr5;
	ar0 = [--ar5];
	ar1 = [--ar5];
	gr5 = [--ar5];
	gr7 = gr5 << 27;
	gr5 >>=5;
	if =0 delayed goto AfterRep32;
		nul;
		gr7 >>= 27;
<NextRep32>	
	gr0 = ar0;
	gr1 = ar1;	
	fpu 0 rep 32 vreg0 = [ar0++];
	fpu 0 rep 32 vreg1 = [ar1++];
	fpu 0 .float vreg2 = vreg0;
	fpu 0 .float vreg0 - vreg1, set mask if >;		//y0>y1?
	fp0_lmask = fp0_hmask; 
	fpu 0 .float vreg0 = mask ? vreg1;				//vreg0 = minY
	fpu 0 .float vreg1 = mask ? vreg2;				//vreg1 = maxY
	ar0 = gr0;
	ar1 = gr1	with gr5--;
	if > delayed goto NextRep32;
		fpu 0 rep 32 [ar0++] = vreg0;
		fpu 0 rep 32 [ar1++] = vreg1;
<AfterRep32>
	gr7--;
	if < delayed goto EndProgram;
		gr0 = ar0;
		gr1 = ar1;
	vlen = gr7;
	fpu 0 rep vlen vreg0 = [ar0++];
	fpu 0 rep vlen vreg1 = [ar1++];
	fpu 0 .float vreg2 = vreg0;
	fpu 0 .float vreg0 - vreg1, set mask if >;		//y0>y1?
	fp0_lmask = fp0_hmask; 
	fpu 0 .float vreg0 = mask ? vreg1;				//vreg0 = minY
	fpu 0 .float vreg1 = mask ? vreg2;				//vreg1 = maxY
	ar0 = gr0;
	ar1 = gr1;
	fpu 0 rep vlen [ar0++] = vreg0;
	fpu 0 rep vlen [ar1++] = vreg1;
	
<EndProgram>	

	pop ar5, gr5;
	pop ar1, gr1;
	pop ar0, gr0;
	return;
end ".text_demo3d";