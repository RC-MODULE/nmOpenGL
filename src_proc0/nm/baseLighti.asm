
//	void baseLighti(v4nm32f* ambient, v2nm32f* n_dot_vp, v4nm32f* diffuse, v2nm32f* n_dot_h_in_srm, v4nm32f* specular, v4nm32f* dst, int count){

data ".data_demo3d"
	one_one: word[2] = (float(1), float(1));
end ".data_demo3d";


begin ".text_demo3d"
global _baseLighti: label;
<_baseLighti>
	ar5 = ar7 - 2;
	push ar0, gr0;
	push ar1, gr1;
	push ar2, gr2;
	push ar3, gr3;
	push ar4, gr4;
	push ar5, gr5;
	push ar6, gr6;
	ar0 = [--ar5];
	fpu 0 rep 1 vreg0 = [ar0++];
	fpu 1 rep 1 vreg0 = [ar0++];
	ar0 = [--ar5];
	ar1 = [--ar5];
	fpu 0 rep 1 vreg2 = [ar1++];
	fpu 1 rep 1 vreg2 = [ar1++];
	ar1 = [--ar5];
	ar2 = [--ar5];
	fpu 2 rep 1 vreg4 = [ar2++];
	fpu 3 rep 1 vreg4 = [ar2++];
	ar6 = [--ar5];
	gr4 = [--ar5];
	gr5 = 4;
	gr6 = gr5;
	ar5 = ar6 + 2;
	
	ar4 = one_one;
	fpu 0 rep 32 vreg5 = [ar4];
	fpu 1 rep 32 vreg5 = [ar4];
	
	gr7 = gr4 << 27;
	gr4 >>= 5;
	if =0 delayed goto AfterRep32;
		gr7 >>= 27;
		gr4--;
<NextRep32>
//fpu 0 - x,y
//fpu 1 - z,w
	fpu 0 rep 32 vreg1 = [ar0++];	//nvp
	fpu 1 vreg1 = fpu 0 vreg1;
	fpu 2 rep 32 vreg3 = [ar1++];	//nh
	fpu 3 vreg3 = fpu 2 vreg3;
	fpu 1 .float vreg1 + vreg1, set mask if <>0;
	fpu 0 .float vreg6 = vreg1 * .retrive(vreg2) + .retrive(vreg0);		//a + nvp * d
	fpu 1 .float vreg6 = vreg1 * .retrive(vreg2) + .retrive(vreg0);
	fpu 2 .float vreg7 = vreg3 * .retrive(vreg4);						//nh * s
	fpu 3 .float vreg7 = vreg3 * .retrive(vreg4);
	fpu 0 vreg7 = fpu 2 vreg7;
	fpu 1 vreg7 = fpu 3 vreg7;
	fp0_lmask = fp1_lmask;
	fp0_hmask = fp1_hmask;
	fpu 0 .float vreg1 = mask ? vreg5: vreg1;		//vreg1 = (vreg1 != 0) ? 1: 0
	fpu 1 .float vreg1 = mask ? vreg5: vreg1;
	fpu 0 .float vreg7 = vreg7 * vreg1 + vreg6;		//a + nvp * d + nh * s
	fpu 1 .float vreg7 = vreg7 * vreg1 + vreg6;
	if > delayed goto NextRep32 with gr4--;
		fpu 0 rep 32 [ar6++gr6] = vreg7;
		fpu 1 rep 32 [ar5++gr5] = vreg7;
<AfterRep32>
	gr7--;
	if < delayed goto EndProgram;
		nul;
		vlen = gr7;
	fpu 0 rep vlen vreg5 = [ar4];
	fpu 1 rep vlen vreg5 = [ar4];
	
	fpu 0 rep vlen vreg1 = [ar0++];	//nvp
	fpu 1 vreg1 = fpu 0 vreg1;
	fpu 2 rep vlen vreg3 = [ar1++];	//nh
	fpu 3 vreg3 = fpu 2 vreg3;
	fpu 1 .float vreg1 + vreg1, set mask if <>0;
	fpu 0 .float vreg6 = vreg1 * .retrive(vreg2) + .retrive(vreg0);		//a + nvp * d
	fpu 1 .float vreg6 = vreg1 * .retrive(vreg2) + .retrive(vreg0);
	fpu 2 .float vreg7 = vreg3 * .retrive(vreg4);						//nh * s
	fpu 3 .float vreg7 = vreg3 * .retrive(vreg4);
	fpu 0 vreg7 = fpu 2 vreg7;
	fpu 1 vreg7 = fpu 3 vreg7;
	fp0_lmask = fp1_lmask;
	fp0_hmask = fp1_hmask;
	fpu 0 .float vreg1 = mask ? vreg5: vreg1;		//vreg1 = (vreg1 != 0) ? 1: 0
	fpu 1 .float vreg1 = mask ? vreg5: vreg1;
	fpu 0 .float vreg7 = vreg7 * vreg1 + vreg6;		//a + nvp * d + nh * s
	fpu 1 .float vreg7 = vreg7 * vreg1 + vreg6;
	fpu 0 rep vlen [ar6++gr6] = vreg7;
	fpu 1 rep vlen [ar5++gr5] = vreg7;
	
<EndProgram>
	pop ar6, gr6;
	pop ar5, gr5;
	pop ar4, gr4;
	pop ar3, gr3;
	pop ar2, gr2;
	pop ar1, gr1;
	pop ar0, gr0;
	return;
end ".text_demo3d";