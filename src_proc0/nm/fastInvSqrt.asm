	
//void invSqrt(float* srcVec, float* dstVec, int size){

data ".data_demo3d"
	half: word[2] = (float(-0.5), float(-0.5));
	threehalf: word[2] = (float(1.5), float(1.5));
end ".data_demo3d";


begin ".text_demo3d"
global _fastInvSqrt: label;
<_fastInvSqrt>
	ar5 = sp-2;
	push ar0, gr0;
	push ar1, gr1;
	push ar2, gr2;
	push ar5, gr5;
	push ar6, gr6;
	
	ar0 = [--ar5];
	ar6 = [--ar5];
	gr5 = [--ar5];
	
	gr0 = ar0	with gr5;
	if =0 delayed goto EndTail;
		gr6 = ar6;
		ar5 = gr5;

	gr7 = 05F3759DFh;
	
	NextSub: label;
	gr1 = [ar0++];
	with gr1 >>= 1;
	ar1 = NextSub	with gr5--;
<NextSub>		
	if > delayed goto ar1	with gr2 = gr7 - gr1;		
		gr1 = [ar0++];
		[ar6++] = gr2	with gr1 >>= 1;
		with gr5--;
		

	ar0 = gr0;
	ar6 = gr6;
	gr5 = ar5;	
	
	ar1 = half	with gr7 = gr5 << 26;
	fpu 0 rep 1 vreg2 = [ar1];
	ar1 = threehalf	with gr5 >>= 6;
	fpu 2 rep 1 vreg2 = [ar1];
	if =0 delayed goto Tail;
		gr7 >>= 27;
		ar5 = ar6;
	
//first iteration of Newton's method		
//y1 = y0 * (1.5 - 0.5 * x * y0 * y0)
<Next32>

	fpu 0 rep 32 vreg0 = [ar0++];						//x0
	fpu 1 rep 32 vreg1 = [ar6++];						//y0
	fpu 0 .float vreg4 = vreg0 * .retrive(vreg2);		//-0.5 * x
	fpu 1 .float vreg4 = vreg1 * vreg1;					//y0 * y0
	fpu 2 vreg0 = fpu 0 vreg4;
	fpu 2 vreg1 = fpu 1 vreg4;
	fpu 2 .float vreg4 = vreg0 * vreg1 + .retrive(vreg2);					//-0.5 * x * y0 * y0 + 1.5
	
	fpu 3 vreg0 = fpu 2 vreg4;
	fpu 3 vreg1 = fpu 1 vreg1;
	fpu 3 .float vreg4 = vreg0 * vreg1;
	fpu 3 rep 32 [ar5++] = vreg4;
	
<Tail>
	gr7--;
	if < delayed goto EndTail;
		vlen = gr7;
		nul;
		
	fpu 0 rep vlen vreg0 = [ar0++];						//x0
	fpu 1 rep vlen vreg1 = [ar6++];						//y0
	fpu 0 .float vreg4 = vreg0 * .retrive(vreg2);		//-0.5 * x
	fpu 1 .float vreg4 = vreg1 * vreg1;					//y0 * y0
	fpu 2 vreg0 = fpu 0 vreg4;
	fpu 2 vreg1 = fpu 1 vreg4;
	fpu 2 .float vreg4 = vreg0 * vreg1 + .retrive(vreg2);					//-0.5 * x * y0 * y0 + 1.5
	
	fpu 3 vreg0 = fpu 2 vreg4;
	fpu 3 vreg1 = fpu 1 vreg1;
	fpu 3 .float vreg4 = vreg0 * vreg1;
	fpu 3 rep vlen [ar5++] = vreg4;
	
<EndTail>
	pop ar6, gr6;
	pop ar5, gr5;
	pop ar2, gr2;
	pop ar1, gr1;
	pop ar0, gr0;
	return;
end ".text_demo3d";
