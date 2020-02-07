
//void clamp_32f(nm32f* pSrcVec1, nm32f* pSrcVec2, float min, float max, nm32f* pDstVec1, nm32f* pDstVec2, int size);
global _doubleClamp_32f: label;

nobits ".data_demo3d"
	buffer_long_min: long;
	buffer_long_max: long;
end ".data_demo3d";

begin ".text_demo3d"
<_doubleClamp_32f>
	ar5 = ar7 - 2	;
	push ar0,gr0;
	push ar1,gr1;
	push ar4,gr4;
	push ar5,gr5;
	push ar6,gr6;
	
	ar0 = [--ar5];
	ar1 = [--ar5];

	ar4 = [--ar5];
	gr4 = ar4;
	gr0 = buffer_long_min;
	[gr0] = ar4,gr4;
	fpu 0 rep 32 vreg1 = [gr0];
	fpu 2 vreg1 = fpu 0 vreg1;
	
	ar4 = [--ar5];
	gr4 = ar4;
	gr0 = buffer_long_max;
	[gr0] = ar4,gr4;
	fpu 1 rep 32 vreg1 = [gr0];
	fpu 3 vreg1 = fpu 1 vreg1;
	
	ar6 = [--ar5];
	ar4 = [--ar5];
	gr5 = [--ar5];
	gr7 = gr5<<26;
	gr5>>=6;
	if =0 delayed goto After32;
		gr7 >>= 27;
		gr5--;
<Next32>
	fpu 0 rep 32 vreg0 = [ar0++];
	fpu 2 rep 32 vreg0 = [ar1++];
	fpu 0 .float vreg0 - vreg1, set mask if <;		//mask if src < min
	fpu 2 .float vreg0 - vreg1, set mask if <;		//mask if src < min
	fp0_lmask = fp0_lmask;
	fp2_lmask = fp2_lmask;
	fpu 0 .float vreg0 = mask ? vreg1 : vreg0;
	fpu 2 .float vreg0 = mask ? vreg1 : vreg0;
	fpu 1 vreg0 = fpu 0 vreg0; 
	fpu 3 vreg0 = fpu 2 vreg0; 
	fpu 1 .float vreg0 - vreg1, set mask if >;		//mask if src > max
	fpu 3 .float vreg0 - vreg1, set mask if >;		//mask if src > max
	fp1_lmask = fp1_lmask;
	fp3_lmask = fp3_lmask;
	fpu 1 .float vreg0 = mask ? vreg1 : vreg0;
	fpu 3 .float vreg0 = mask ? vreg1 : vreg0;
	if > delayed goto Next32 with gr5--;
		fpu 1 rep 32 [ar6++] = vreg0;
		fpu 3 rep 32 [ar4++] = vreg0;
<After32>
	gr0 = buffer_long_min	with gr7--;
	if < delayed goto end_program;
		gr1 = buffer_long_max;
	fpu 0 rep vlen vreg1 = [gr0];
	fpu 2 vreg1 = fpu 0 vreg1;
	fpu 1 rep vlen vreg1 = [gr1];
	fpu 3 vreg1 = fpu 1 vreg1;
	
	fpu 0 rep vlen vreg0 = [ar0++];
	fpu 2 rep vlen vreg0 = [ar1++];
	fpu 0 .float vreg0 - vreg1, set mask if <;		//mask if src < min
	fpu 2 .float vreg0 - vreg1, set mask if <;		//mask if src < min
	fp0_lmask = fp0_lmask;
	fp2_lmask = fp2_lmask;
	fpu 0 .float vreg0 = mask ? vreg1 : vreg0;
	fpu 2 .float vreg0 = mask ? vreg1 : vreg0;
	fpu 1 vreg0 = fpu 0 vreg0; 
	fpu 3 vreg0 = fpu 2 vreg0; 
	fpu 1 .float vreg0 - vreg1, set mask if >;		//mask if src > max
	fpu 3 .float vreg0 - vreg1, set mask if >;		//mask if src > max
	fp1_lmask = fp1_lmask;
	fp3_lmask = fp3_lmask;
	fpu 1 .float vreg0 = mask ? vreg1 : vreg0;
	fpu 3 .float vreg0 = mask ? vreg1 : vreg0;
	fpu 1 rep vlen [ar6++] = vreg0;
	fpu 3 rep vlen [ar4++] = vreg0;
<end_program>
	pop ar6,gr6;
	pop ar5,gr5;
	pop ar4,gr4;
	pop ar1,gr1;
	pop ar0,gr0;
	return;
end ".text_demo3d";
