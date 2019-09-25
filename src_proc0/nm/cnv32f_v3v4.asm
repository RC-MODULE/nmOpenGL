//void cnv32f_v3v4(nm32f* src_v3nm32f, nm32f* dst_v4nm32f, float value4, int size) 

global _cnv32f_v3v4: label;


//a0,a1,a2,b0,b1,b2 -> c0,c1,c2,v, d0,d1,d2, v
begin ".text_demo3d"
<_cnv32f_v3v4>
	ar5 = sp - 2;
	push ar0, gr0;
	push ar1, gr1;
	push ar2, gr2;
	push ar4, gr4;
	push ar5, gr5;
	push ar6, gr6;
	ar0 = [--ar5]	with gr6 = false;
	ar6 = [--ar5]	with gr6++;
	gr1 = [--ar5]	with gr6 <<= 3;			//gr6 = 8
	gr7 = [--ar5];
	
	ar1 = gr1	with gr7>>=1;
	ar5 = ar7;
	
	push ar1, gr1;
	fpu rep 32 .packer = [ar5] with .double <= .float;
	fpu 2 rep 32 (vreg2, vreg3) = .packer;			//fpu 2 vreg3 = value
	gr0 = 6;
	
	gr1 = gr7 << 27;
	gr7>>=5;
	if =0 delayed goto AfterRep32	with gr1 >>= 27;
		ar2 = ar0;
		gr2 = ar6;
<NextRep32>
	//a0,a1 -> c1,c2
	ar1 = ar2;
	ar4 = gr2;
	ar0 = ar1;
	ar6 = ar4;
	fpu 0 rep 32 vreg0 = [ar0++gr0];
	fpu 0 rep 32 [ar6++gr6] = vreg0;	
	ar2 = ar0;
	gr2 = ar6;
	//a2, b0 -> double
	ar0 = ar1 + 2;
	ar6 = ar4 + 2;
	fpu rep 32 .packer = [ar0++gr0] with .double <= .float;
	fpu 1 rep 32 (vreg0,vreg1) = .packer;					//fpu 1 vreg0 = a2, vreg1 = b0
	//a2, b0 -> c2,v
	fpu 2 vreg2 = fpu 1 vreg0;
	fpu 2 .packer = (vreg2,vreg3) with .float <= .double;
	fpu rep 32 [ar6++gr6] = .packer;
	//b0, b1 -> double
	ar0 = ar1 + 4;
	ar6 = ar4 + 4;
	fpu rep 32 .packer = [ar0++gr0] with .double <= .float;
	fpu 2 rep 32 (vreg0,vreg1) = .packer;					//fpu 2 vreg0 = b1, vreg1 = b2
	//b0,b1 -> d0,d1
	fpu 3 vreg0 = fpu 1 vreg1;
	fpu 3 vreg1 = fpu 2 vreg0;
	fpu 3 .packer = (vreg0,vreg1) with .float <= .double;
	fpu rep 32 [ar6++gr6] = .packer;
	//b2,v -> d2,v
	ar6 = ar4 + 6	with gr7--;
	fpu 2 vreg2 = fpu 2 vreg1;
	if > delayed goto NextRep32;
		fpu 2 .packer = (vreg2,vreg3) with .float <= .double;
		fpu rep 32 [ar6++gr6] = .packer;
<AfterRep32>	
	gr1--;
	if < delayed goto EndProgram;
		vlen = gr1;
		nul;
	fpu rep vlen .packer = [ar5] with .double <= .float;
	fpu 2 rep vlen (vreg2, vreg3) = .packer;			//fpu 2 vreg3 = value
	ar1 = ar2;
	ar4 = gr2;
	ar0 = ar1;
	ar6 = ar4;
	fpu 0 rep vlen vreg0 = [ar0++gr0];
	fpu 0 rep vlen [ar6++gr6] = vreg0;	
	//a2, b0 -> double
	ar0 = ar1 + 2;
	ar6 = ar4 + 2;
	fpu rep vlen .packer = [ar0++gr0] with .double <= .float;
	fpu 1 rep vlen (vreg0,vreg1) = .packer;					//fpu 1 vreg0 = a2, vreg1 = b0
	//a2, b0 -> c2,v
	fpu 2 vreg2 = fpu 1 vreg0;
	fpu 2 .packer = (vreg2,vreg3) with .float <= .double;
	fpu rep vlen [ar6++gr6] = .packer;
	//b0, b1 -> double
	ar0 = ar1 + 4;
	ar6 = ar4 + 4;
	fpu rep vlen .packer = [ar0++gr0] with .double <= .float;
	fpu 2 rep vlen (vreg0,vreg1) = .packer;					//fpu 2 vreg0 = b1, vreg1 = b2
	//b0,b1 -> d0,d1
	fpu 3 vreg0 = fpu 1 vreg1;
	fpu 3 vreg1 = fpu 2 vreg0;
	fpu 3 .packer = (vreg0,vreg1) with .float <= .double;
	fpu rep vlen [ar6++gr6] = .packer;
	//b2,v -> d2,v
	ar6 = ar4 + 6	with gr7--;
	fpu 2 vreg2 = fpu 2 vreg1;
	fpu 2 .packer = (vreg2,vreg3) with .float <= .double;
	fpu rep vlen [ar6++gr6] = .packer;
<EndProgram>	

	pop ar1, gr1;
	pop ar6, gr6;
	pop ar5, gr5;
	pop ar4, gr4;
	pop ar2, gr2;
	pop ar1, gr1;
	pop ar0, gr0;
	return;
end ".text_demo3d";