global _meanToInt2: label;
//void meanToInt3(float* src1, float* src2, int* result, int size)

begin ".data_demo3d"
	half : word[2] = (float(0.5),float(0.5));
end ".data_demo3d";

begin ".text_demo3d"
<_meanToInt2>
	ar5 = ar7 - 2;
	push ar0,gr0;//Az
	push ar1,gr1;//Bz
	push ar3,gr3;//result
	push ar5,gr5;

	ar0 = [--ar5];//src1
	ar1 = [--ar5];//src2
	ar3 = [--ar5];//result
	gr5 = [--ar5];//amm

	gr7 = gr5 << 26;
	
	ar5 = half with gr5 = gr5 >> 6;
	if =0 delayed goto REMINDER	with gr7 = gr7 >> 27;
		fpu 1 rep 1 vreg0 = [ar5]; 
		gr5--;
<LOOP>	
	fpu 0 rep 32 vreg0 = [ar0++];//load src1
	fpu 0 rep 32 vreg1 = [ar1++];//load src2
	fpu 0 .float vreg2 = vreg1 + vreg0;
	fpu 1 vreg1 = fpu 0 vreg2;
	fpu 1 .float vreg2 = vreg1*.retrive(vreg0);
	if <>0 delayed goto LOOP with gr5--;
		fpu 1 .packer = vreg2 with .fixed_32 <= .float;    
		fpu rep 32 [ar3++] = .packer;
<REMINDER>
	gr7--;
	if < delayed goto END;
		vlen = gr7;
		nul;
	fpu 0 rep vlen vreg0 = [ar0++];//load src1
	fpu 0 rep vlen vreg1 = [ar1++];//load src2
	fpu 0 .float vreg2 = vreg1 + vreg0;
	fpu 1 vreg1 = fpu 0 vreg2;
	fpu 1 .float vreg2 = vreg1*.retrive(vreg0);
	fpu 1 .packer = vreg2 with .fixed_32 <= .float;    
	fpu rep vlen [ar3++] = .packer;
<END>
	pop ar5,gr5;
	pop ar3,gr3;
	pop ar1,gr1;
	pop ar0,gr0;
	return;	
end ".text_demo3d";