/*!
 *  \file meanToInt3.asm
 *  \author Zhilenkov Ivan
 */
global _meanToInt3: label;
//void meanToInt3(float* src1, float* src2, float* src3, int* result, int size)

begin ".data_demo3d"
	one_third : word[2] = (float(0.333333333),float(0.333333333));
	half_half : word[2] = (float(0.5),float(0.5));
end ".data_demo3d";

begin ".text_demo3d"
<_meanToInt3>
	ar5 = ar7 - 2;
	push ar0,gr0;//Az
	push ar1,gr1;//Bz
	push ar2,gr2;//Cz
	push ar3,gr3;//result
	push ar5,gr5;

	ar0 = [--ar5];//Az
	ar1 = [--ar5];//Bz
	ar2 = [--ar5];//Cz
	ar3 = [--ar5];//result
	gr5 = [--ar5];//amm

	gr7 = gr5 << 26;
	gr5 = gr5 >> 6;
	ar5 = half_half;
	fpu	0 rep 1 vreg3 = [ar5];
	ar5 = one_third; 
	if =0 delayed goto REMINDER; 
		fpu 0 rep 1 vreg0 = [ar5]; 
		gr7 = gr7 >> 27; 
<LOOP>	
	fpu 1 rep 32 vreg0 = [ar0++];//load Az
	fpu 1 rep 32 vreg1 = [ar1++];//load Bz
	fpu 1 .float vreg2 = vreg1 + vreg0;
	fpu 2 rep 32 vreg0 = [ar2++];//load Cz
	fpu 2 vreg1 = fpu 1 vreg2;
	fpu 2 .float vreg2 = vreg1 + vreg0;
	fpu 0 vreg1 = fpu 2 vreg2;
	gr5--;
	fpu 0 .float vreg2 = vreg1*.retrive(vreg0) + .retrive(vreg3);   
	if <>0 delayed goto LOOP with gr7;
		fpu 0 .packer = vreg2 with .fixed_32 <= .float;    
		fpu rep 32 [ar3++] = .packer;
<REMINDER>
	if =0 delayed goto END;
		gr7--;
		vlen = gr7;
	fpu 1 rep vlen vreg0 = [ar0++];//load Az
	fpu 1 rep vlen vreg1 = [ar1++];//load Bz
	fpu 1 .float vreg2 = vreg1 + vreg0;
	fpu 2 rep vlen vreg0 = [ar2++];//load Cz
	fpu 2 vreg1 = fpu 1 vreg2;
	fpu 2 .float vreg2 = vreg1 + vreg0;
	fpu 0 vreg1 = fpu 2 vreg2;
	fpu 0 .float vreg2 = vreg1*.retrive(vreg0) + .retrive(vreg3);   
	fpu 0 .packer = vreg2 with .fixed_32 <= .float;    
	fpu rep vlen [ar3++] = .packer;
<END>
	pop ar5,gr5;
	pop ar3,gr3;
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	return;	
end ".text_demo3d";