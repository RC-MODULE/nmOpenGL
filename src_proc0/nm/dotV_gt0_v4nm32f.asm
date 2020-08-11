
//void dotv_gt0_v4nm32f(v4nm32f* srcVec1, v4nm32f* srcVec2, nm32f* dstValues, int size);
global _dotV_gt0_v4nm32f: label;

data ".data_demo3d"
.align;
	one_one: word [2] = (float(1), float(1));
end ".data_demo3d";

begin ".text_demo3d"
<_dotV_gt0_v4nm32f>
	ar5 = ar7-2	;
	push ar0,gr0;
	push ar1,gr1	with gr0 = false;
	push ar2,gr2	with gr0++;
	push ar3,gr3	with gr0 <<= 2;
	push ar5,gr5 	with gr1 = gr0;
	push ar6,gr6	with gr2 = gr0;
	ar0 = [--ar5]	with gr3 = gr0;
	ar1 = [--ar5];
	ar6 = [--ar5];
	gr5 = [--ar5];
	
	//ar0 - x0,y0
	//ar1 - x1,y1
	//ar2 - z0,w0
	//ar3 - z1,w1
	ar2 = ar0 + 2	with gr7 = gr5 << 27;
	ar3 = ar1 + 2	with gr7 >>= 27;
	with gr5 >>= 5;
	if =0 delayed goto AfterDot32;
		gr5--;
		nul;
		
	ar5 = one_one;
	fpu 2 rep 32 vreg4 = [ar5];
	fpu 3 vreg4 = fpu 2 vreg4;
	fpu 3 .float vreg5 = vreg4 - vreg4;
<NextDot32>
	fpu 0 rep 32 vreg0 = [ar0++gr0];		//(x0,y0)
	fpu 0 rep 32 vreg1 = [ar1++gr1];		//(x1,y1)
	fpu 0 .float vreg7 = vreg0 * vreg1;		//(x0*x1,y0*y1)
	fpu 1 vreg2 = fpu 0 vreg7;
	fpu 1 rep 32 vreg0 = [ar2++gr2];		//(z0,w0)
	fpu 1 rep 32 vreg1 = [ar3++gr3];		//(z1,w1)
	fpu 1 .float vreg7 = vreg0 * vreg1 + vreg2;		//(x0*x1 + z0*z1, y0*y1 + w0*w1)
	
	fpu 2 vreg7 = fpu 1 vreg7;
	fpu 2 vreg6 = fpu 1 vreg7;	
	
	fpu 2 .matrix vreg3 = (vreg6,vreg7) * vreg4;
	fpu 3 vreg3 = fpu 2 vreg3;
	//зануление если <0
	fpu 3 .float vreg0 = vreg3 - vreg5, set mask if <;
	fp3_lmask = fp3_lmask;
	fp3_hmask = fp3_hmask;
	if > delayed goto NextDot32		with gr5--;
		fpu 3 .float vreg0 = mask ? vreg5: vreg0;
		fpu 3 rep 32 [ar6++] = vreg0;
	
<AfterDot32>	
	gr7--;
	if < delayed goto EndProgram;
		vlen = gr7;
		nul;
	ar5 = one_one;
	fpu 2 rep vlen vreg4 = [ar5];
	fpu 3 vreg4 = fpu 2 vreg4;
	fpu 3 .float vreg5 = vreg4 - vreg4;

	fpu 0 rep vlen vreg0 = [ar0++gr0];		//(x0,y0)
	fpu 0 rep vlen vreg1 = [ar1++gr1];		//(x1,y1)
	fpu 0 .float vreg7 = vreg0 * vreg1;		//(x0*x1,y0*y1)
	fpu 1 vreg2 = fpu 0 vreg7;
	fpu 1 rep vlen vreg0 = [ar2++gr2];		//(z0,w0)
	fpu 1 rep vlen vreg1 = [ar3++gr3];		//(z1,w1)
	fpu 1 .float vreg7 = vreg0 * vreg1 + vreg2;		//(x0*x1 + z0*z1, y0*y1 + w0*w1)
	
	fpu 2 vreg7 = fpu 1 vreg7;
	fpu 2 vreg6 = fpu 1 vreg7;	
	
	fpu 2 .matrix vreg3 = (vreg6,vreg7) * vreg4;
	fpu 3 vreg3 = fpu 2 vreg3;
	//зануление если <0
	fpu 3 .float vreg0 = vreg3-vreg5, set mask if <;
	fp3_lmask = fp3_lmask;
	fp3_hmask = fp3_hmask;
	fpu 3 .float vreg0 = mask ? vreg5: vreg0;
	fpu 3 rep vlen [ar6++] = vreg0;
<EndProgram>
	
	pop ar6,gr6;
	pop ar5,gr5;
	pop ar3,gr3;
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	return;
end ".text_demo3d";
