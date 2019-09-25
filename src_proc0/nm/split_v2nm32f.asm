
//void split_v2nm32f(float* srcVec4, int step, float* dstX, float* dstY, int countVec);
global _split_v2nm32f: label;

data ".data_demo3d"
.align;
	one_zero : word[2] = (float(1),float(0));
	zero_one : word[2] = (float(0),float(1));
end ".data_demo3d";

begin ".text_demo3d"
<_split_v2nm32f>
	ar5 = ar7-2	with gr7 = gr5;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
	push ar3,gr3;
	ar0 = [--ar5];
	gr0 = [--ar5];
	gr0<<=1;		
	ar2 = [--ar5];	//X
	ar3 = [--ar5];	//Y
	gr5 = [--ar5];
	ar1 = ar0+gr0	with gr0<<=1;			
	gr5>>=1;
	gr1 = gr0 		with gr3 = gr5<<27;
	ar5 = one_zero		with gr3>>=27;
	
	//fpu 0 - X
	//fpu 1 - Y
	fpu 0 rep 32 vreg4 = [ar5];
	ar5 = zero_one	with	gr5>>=5;
	if =0 delayed goto Tail;
		fpu 1 rep 32 vreg4 = [ar5];
		gr5--;
<Next32>
	fpu 0 rep 32 vreg0 = [ar0++gr0];		//X
	fpu 1 vreg0 = fpu 0 vreg0;				//Y
	fpu 0 rep 32 vreg1 = [ar1++gr1];		//X
	fpu 1 vreg1 = fpu 0 vreg1;				//Y
	fpu 0 .matrix vreg6 = (vreg0,vreg1) * vreg4;		//получение X
	fpu 1 .matrix vreg6 = (vreg0,vreg1) * vreg4;		//получение Y
	if > delayed goto Next32	with gr5--;
		fpu 0 rep 32 [ar2++] = vreg6;						//запись X
		fpu 1 rep 32 [ar3++] = vreg6;						//запись Y
<Tail>		
	gr3--;
	if < delayed goto end_program;
		ar5 = one_zero;
	vlen = gr3;
	fpu 0 rep vlen vreg4 = [ar5];
	ar5 = zero_one;
	fpu 1 rep vlen vreg4 = [ar5];
	
	fpu 0 rep vlen vreg0 = [ar0++gr0];		//X
	fpu 1 vreg0 = fpu 0 vreg0;				//Y
	fpu 0 rep vlen vreg1 = [ar1++gr1];		//X
	fpu 1 vreg1 = fpu 0 vreg1;				//Y
	fpu 0 .matrix vreg6 = (vreg0,vreg1) * vreg4;		//получение X
	fpu 1 .matrix vreg6 = (vreg0,vreg1) * vreg4;		//получение Y
	fpu 0 rep vlen [ar2++] = vreg6;						//запись X
	fpu 1 rep vlen [ar3++] = vreg6;						//запись Y
		
<end_program>
	pop ar3,gr3	with gr5 = gr7;
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	return;
end ".text_demo3d";