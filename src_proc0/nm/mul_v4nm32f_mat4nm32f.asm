/*!
 *  \file mul_v4nm32f_mat4nm32f.asm
 *  \author Zhilenkov Ivan
 */
//void mul_v4nm32f_mat4nm32f( float* srcVector4xN, float* srcMatrix4x4, float* dstVector4xN, int count){

global _mul_v4nm32f_mat4nm32f: label;

begin ".text_demo3d"
<_mul_v4nm32f_mat4nm32f>
	ar5 = ar7-2;
	push ar0,gr0	with gr0 = false;
	push ar1,gr1	with gr0++;
	push ar2,gr2	with gr0<<=2;
	push ar3,gr3	with gr1 = gr0;
	push ar4,gr4	with gr2 = gr0;
	push ar5,gr5	with gr3 = gr0;
	push ar6,gr6	with gr4 = gr0;
	ar2 = [--ar5]	with gr6 = gr0;
	ar0 = [--ar5];
	ar6 = [--ar5];
	gr5 = [--ar5];
	ar3 = ar2+2;
	ar4 = ar6+2;
	
	After32: label;
	fpu 0 rep 2 vreg0 = [ar0++];
	fpu 0 rep 2 vreg1 = [ar0++];
	fpu 1 rep 2 vreg2 = [ar0++];
	fpu 1 rep 2 vreg3 = [ar0++];
	gr7 = gr5<<27;
	gr5>>=5;
	if =0 delayed goto After32;
		nul;
		gr5--;
<Next32>
	fpu 0 rep 32 vreg4 = [ar2++gr2];
	fpu 1 rep 32 vreg5 = [ar3++gr3];
	
	fpu 0 .matrix vreg6 = vreg4 * .retrive(vreg0,vreg1);
	fpu 1 vreg6 = fpu 0 vreg6;
	fpu 1 .matrix vreg7 = vreg5 * .retrive(vreg2,vreg3) + vreg6;
	fpu 1 rep 32 [ar6++gr6] = vreg7;
	
	fpu 0 .matrix vreg6 = vreg4 * .retrive(vreg0,vreg1);
	fpu 1 vreg6 = fpu 0 vreg6;
	if > delayed goto Next32	with gr5--;
		fpu 1 .matrix vreg7 = vreg5 * .retrive(vreg2,vreg3) + vreg6;
		fpu 1 rep 32 [ar4++gr4] = vreg7;
<After32>
	with gr7>>=27;
	if =0 delayed goto end_program;
		gr7--;
		vlen = gr7;
	fpu 0 rep vlen vreg4 = [ar2++gr2];
	fpu 1 rep vlen vreg5 = [ar3++gr3];
	
	fpu 0 .matrix vreg6 = vreg4 * .retrive(vreg0,vreg1);
	fpu 1 vreg6 = fpu 0 vreg6;
	fpu 1 .matrix vreg7 = vreg5 * .retrive(vreg2,vreg3) + vreg6;
	fpu 1 rep vlen [ar6++gr6] = vreg7;
	
	fpu 0 .matrix vreg6 = vreg4 * .retrive(vreg0,vreg1);
	fpu 1 vreg6 = fpu 0 vreg6;
	fpu 1 .matrix vreg7 = vreg5 * .retrive(vreg2,vreg3) + vreg6;
	fpu 1 rep vlen [ar4++gr4] = vreg7;

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