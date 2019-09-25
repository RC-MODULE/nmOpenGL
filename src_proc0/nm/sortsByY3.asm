//void sortByYinTriangle(v2nm32f* srcAxy,
//						 v2nm32f* srcBxy,
//						 v2nm32f* srcCxy,
//						 int size)

global _sortByY3: label;

begin ".text_demo3d"
<_sortByY3>
	ar5 = ar7-2;
	push ar0, gr0;
	push ar1, gr1;
	push ar2, gr2;
	push ar5, gr5;
	ar0 = [--ar5];
	ar1 = [--ar5];
	ar2 = [--ar5];
	gr5 = [--ar5];
	gr7 = gr5 << 27;
	gr5 >>=5;
	if =0 delayed goto AfterRep32;
		nul;
		gr7 >>= 27;
<NextRep32>	
	gr0 = ar0;
	gr1 = ar1;
	gr2 = ar2;
	fpu 0 rep 32 vreg0 = [ar0++];	//axy
	fpu 0 rep 32 vreg1 = [ar2++];	//cxy
	fpu 0 .float vreg2 = vreg0;
	fpu 0 .float vreg0 - vreg1, set mask if >;		//y0>y2?
	fp0_lmask = fp0_hmask; 
	fpu 0 .float vreg0 = mask ? vreg1;
	fpu 1 vreg0 = fpu 0 vreg0;
	fpu 0 .float vreg1 = mask ? vreg2;
	fpu 2 vreg1 = fpu 0 vreg1;	

	fpu 1 rep 32 vreg1 = [ar1++];	//bxy
	fpu 1 .float vreg2 = vreg1;
	fpu 1 .float vreg0 - vreg1, set mask if >;		//y0>y1?
	fp1_lmask = fp1_hmask; 
	ar0 = gr0;
	fpu 1 .float vreg1 = mask ? vreg0;
	fpu 2 vreg0 = fpu 1 vreg1;
	fpu 1 .float vreg0 = mask ? vreg2;
	fpu 1 rep 32 [ar0++] = vreg0;
	
	ar1 = gr1;
	ar2 = gr2;
	fpu 2 .float vreg2 = vreg0;
	fpu 2 .float vreg0 - vreg1, set mask if >;		//y1>y2?
	fp2_lmask = fp2_hmask; 
	fpu 2 .float vreg0 = mask ? vreg1;
	fpu 2 .float vreg1 = mask ? vreg2;

	gr5--;
	if > delayed goto NextRep32;
		fpu 2 rep 32 [ar1++] = vreg0;
		fpu 2 rep 32 [ar2++] = vreg1;
<AfterRep32>
	gr7--;
	if < delayed goto EndProgram;
		gr0 = ar0;
		gr1 = ar1;
	gr2 = ar2;
	vlen = gr7;
	
	fpu 0 rep vlen vreg0 = [ar0++];	//axy
	fpu 0 rep vlen vreg1 = [ar2++];	//cxy
	fpu 0 .float vreg2 = vreg0;
	fpu 0 .float vreg0 - vreg1, set mask if >;		//y0>y2?
	fp0_lmask = fp0_hmask; 
	fpu 0 .float vreg0 = mask ? vreg1 : vreg0;
	fpu 1 vreg0 = fpu 0 vreg0;
	fpu 0 .float vreg1 = mask ? vreg2 : vreg1;
	fpu 2 vreg1 = fpu 0 vreg1;	

	fpu 1 rep vlen vreg1 = [ar1++];	//bxy
	fpu 1 .float vreg2 = vreg1;
	fpu 1 .float vreg0 - vreg1, set mask if >;		//y0>y1?
	fp1_lmask = fp1_hmask; 
	ar0 = gr0;
	fpu 1 .float vreg1 = mask ? vreg0 : vreg1;
	fpu 2 vreg0 = fpu 1 vreg1;
	fpu 1 .float vreg0 = mask ? vreg2 : vreg0;
	fpu 1 rep vlen [ar0++] = vreg0;
	
	ar1 = gr1;
	ar2 = gr2;
	fpu 2 .float vreg2 = vreg0;
	fpu 2 .float vreg0 - vreg1, set mask if >;		//y1>y2?
	fp2_lmask = fp2_hmask; 
	fpu 2 .float vreg0 = mask ? vreg1 : vreg0;
	fpu 2 .float vreg1 = mask ? vreg2 : vreg1;

	fpu 2 rep vlen [ar1++] = vreg0;
	fpu 2 rep vlen [ar2++] = vreg1;
<EndProgram>	

	pop ar5, gr5;
	pop ar2, gr2;
	pop ar1, gr1;
	pop ar0, gr0;
	return;
end ".text_demo3d";