
//void cnv32f_v2v4(v2nm32f* srcVec, float value3, float value4, v4nm32f* dstVec, int countVec);
global _cnv32f_v2v4: label;

nobits ".data_demo3d"
	temp: long;
end ".data_demo3d";

NextRep32:label;

begin ".text_demo3d"
<_cnv32f_v2v4>
	ar5 = ar7-2		with gr7 = false;
	push ar0,gr0	with gr7++;
	push ar1,gr1	with gr7++;
	push ar5,gr5;
	push ar6,gr6;
	
	ar0 = [--ar5];
	ar6 = [--ar5];
	ar1 = temp;
	gr1 = [--ar5];
	[ar1++] = gr1	with gr6 = gr7<<1;
	gr1 = [--ar5];
	[ar1] = gr1		with gr5 = gr7<<1;
	ar1--;
	fpu 0 rep 32 vreg1 = [ar1];	
	gr7 = [--ar5];
	ar5 = ar6;
	ar5+=2			with gr1 = gr7<<27;
	ar1 = NextRep32	with gr7>>=5;
	if =0 delayed goto AfterRep32;
		gr1 >>= 27;
		gr7--;
<NextRep32>
	if > delayed goto ar1	with gr7--;
		fpu 0 rep 32 vreg0 = [ar0++];
		fpu 0 rep 32 [ar6++gr6] = vreg0;
		fpu 0 rep 32 [ar5++gr5] = vreg1;
<AfterRep32>
	ar1 = temp	with gr1--;
	if < delayed goto EndProgram;
		vlen = gr1;
		nul;
	fpu 0 rep vlen vreg1 = [ar1];
	fpu 0 rep vlen vreg0 = [ar0++];
	fpu 0 rep vlen [ar6++gr6] = vreg0;
	fpu 0 rep vlen [ar5++gr5] = vreg1;
<EndProgram>

	pop ar6,gr6;
	pop ar5,gr5;
	pop ar1,gr1;
	pop ar0,gr0;
	return;
end ".text_demo3d";