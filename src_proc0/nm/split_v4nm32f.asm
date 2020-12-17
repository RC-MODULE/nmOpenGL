/*!
 *  \file split_v4nm32f.asm
 *  \author Zhilenkov Ivan
 */
//void split_v4nm32f(float* srcVec4, int step, float* dstX, float* dstY, float* dstZ, float* dstW, int countVec);
global _split_v4nm32f: label;

data ".data_demo3d"
.align;
	one_zero : word[2] = (float(1),float(0));
	zero_one : word[2] = (float(0),float(1));
end ".data_demo3d";

begin ".text_demo3d"
<_split_v4nm32f>
	ar5 = ar7-2	with gr7 = false;
	push ar0,gr0	with gr7++;
	push ar1,gr1	with gr7++;
	push ar2,gr2;
	push ar3,gr3;
	push ar4,gr4;
	push ar5,gr5;
	push ar6,gr6;
	ar0 = [--ar5];
	gr0 = [--ar5];
	gr0<<=2;		
	ar2 = [--ar5];	//X
	ar3 = [--ar5];	//Y
	ar4 = [--ar5];	//Z
	ar6 = [--ar5];	//W
	gr5 = [--ar5];
	ar1 = ar0+gr0	with gr0<<=1;			
	gr5>>=1;
	gr1 = gr0 		with gr4 = gr5<<27;
	ar5 = one_zero		with gr4>>=27;
	
	//fpu 0 - X
	//fpu 1 - Y
	//fpu 2 - Z
	//fpu 3 - W
	fpu 0 rep 32 vreg4 = [ar5];
	fpu 2 vreg4 = fpu 0 vreg4;
	ar5 = zero_one		with gr4--;
	fpu 1 rep 32 vreg4 = [ar5];
	fpu 3 vreg4 = fpu 1 vreg4;

	push ar4,gr4	with	gr5>>=5;
	if =0 delayed goto Tail;
		nul;
		nul;
<Next32>
	gr4 = ar0;
	gr6 = ar1	with gr4 = gr4 + gr7 noflags;
	fpu 0 rep 32 vreg0 = [ar0++gr0];		//X
	fpu 1 vreg0 = fpu 0 vreg0;				//Y
	fpu 0 rep 32 vreg1 = [ar1++gr1];		//X
	fpu 1 vreg1 = fpu 0 vreg1;				//Y
	fpu 0 .matrix vreg6 = (vreg0,vreg1) * vreg4;		//получение X
	fpu 1 .matrix vreg6 = (vreg0,vreg1) * vreg4;		//получение Y
	fpu 0 rep 32 [ar2++] = vreg6;						//запись X
	fpu 1 rep 32 [ar3++] = vreg6;						//запись Y
	
	gr2 = ar0 with gr6 = gr6+ gr7 noflags;
	gr3 = ar1;
	ar0 = gr4;
	ar1 = gr6 with gr5--;
	
	fpu 2 rep 32 vreg0 = [ar0++gr0];
	fpu 3 vreg0 = fpu 2 vreg0;
	fpu 2 rep 32 vreg1 = [ar1++gr1];
	fpu 3 vreg1 = fpu 2 vreg1;
	fpu 2 .matrix vreg6 = (vreg0,vreg1) * vreg4;		//получение Z
	fpu 3 .matrix vreg6 = (vreg0,vreg1) * vreg4;		//получение W
	fpu 2 rep 32 [ar4++] = vreg6;						//запись Z
	fpu 3 rep 32 [ar6++] = vreg6;						//запись W
	if > delayed goto Next32;
		ar0 = gr2;
		ar1 = gr3;
<Tail>		
	ar5 = ar4;
	pop ar4,gr4;
	ar4 = ar5;
	ar5 = one_zero	with gr4;
	if < delayed goto end_program;
		nul;
		nul;
	vlen = gr4;
	fpu 0 rep vlen vreg4 = [ar5];
	fpu 2 vreg4 = fpu 0 vreg4;
	ar5 = zero_one;
	fpu 1 rep vlen vreg4 = [ar5];
	fpu 3 vreg4 = fpu 1 vreg4;
	
	gr4 = ar0;
	gr6 = ar1	with gr4 = gr4 + gr7 noflags;
	fpu 0 rep vlen vreg0 = [ar0++gr0];		//X
	fpu 1 vreg0 = fpu 0 vreg0;				//Y
	fpu 0 rep vlen vreg1 = [ar1++gr1];		//X
	fpu 1 vreg1 = fpu 0 vreg1;				//Y
	fpu 0 .matrix vreg6 = (vreg0,vreg1) * vreg4;		//получение X
	fpu 1 .matrix vreg6 = (vreg0,vreg1) * vreg4;		//получение Y
	fpu 0 rep vlen [ar2++] = vreg6;						//запись X
	fpu 1 rep vlen [ar3++] = vreg6;						//запись Y
	
	ar0 = gr4	with gr6 = gr6+ gr7 noflags;
	ar1 = gr6;
	
	fpu 2 rep vlen vreg0 = [ar0++gr0];
	fpu 3 vreg0 = fpu 2 vreg0;
	fpu 2 rep vlen vreg1 = [ar1++gr1];
	fpu 3 vreg1 = fpu 2 vreg1;
	fpu 2 .matrix vreg6 = (vreg0,vreg1) * vreg4;		//получение Z
	fpu 3 .matrix vreg6 = (vreg0,vreg1) * vreg4;		//получение W
	fpu 2 rep vlen [ar4++] = vreg6;						//запись Z
	fpu 3 rep vlen [ar6++] = vreg6;						//запись W
		
<end_program>
	pop ar6,gr6;
	pop ar5,gr5;
	pop ar4,gr4;
	pop ar3,gr3;
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	return;
end ".text_demo3d";