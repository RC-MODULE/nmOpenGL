
//void dotV_32f(v4nm32f* srcVec1, v4nm32f* srcVec2, v2nm32f* dstValues, int size);
global _dotV_v4nm32f: label;

data ".data_demo3d"
.align;
	zero_double: word [2] = (0,0);
	one_one: word [2] = (float(1), float(1));
end ".data_demo3d";

begin ".text_demo3d"
<_dotV_v4nm32f>
	ar5 = ar7-2	;
	push ar0,gr0	with gr0 = false;
	push ar1,gr1	with gr0++;
	push ar2,gr2	with gr0++;
	push ar3,gr3;
	push ar4,gr4	with gr4 = gr0;
	push ar5,gr5;
	push ar6,gr6;
	ar4 = one_one;
	fpu 2 rep 32 vreg4 = [ar4];
	ar4 = zero_double;
	fpu 3 rep 32 vreg5 = [ar4];
	ar0 = [--ar5];
	ar1 = [--ar5];
	ar6 = [--ar5];
	gr5 = [--ar5];
	gr7 = gr5<<27;
	gr7 >>=27;
	gr7--;
	vlen = gr7		with	gr0 = gr4<<1;		
	ar2 = ar0 + 2	with 	gr2 = gr0;
	ar3 = ar1 + 2	with	gr3 = gr0;
	gr1 = gr0	with gr5>>=5;
	if =0 delayed goto AfterDot32;
		nul;
		gr5--;
<NextDot32>
	fpu 0 rep 32 vreg0 = [ar0++gr0];
	fpu 0 rep 32 vreg1 = [ar1++gr1];
	fpu 0 .float vreg7 = vreg0*vreg1;
	fpu 1 vreg2 = fpu 0 vreg7;
	fpu 1 rep 32 vreg0 = [ar2++gr2];
	fpu 1 rep 32 vreg1 = [ar3++gr3];
	fpu 1 .float vreg7 = vreg0*vreg1 + vreg2;
	fpu 2 vreg7 = fpu 1 vreg7;
	fpu 2 vreg6 = fpu 1 vreg7;	
	if > delayed goto NextDot32		with gr5--;
		fpu 2 .matrix vreg3 = (vreg6,vreg7) * vreg4;
		fpu 2 rep 32 [ar6++] = vreg3;
	
<AfterDot32>	
	gr7;
	if < delayed goto EndProgram;
		ar4 = one_one;
	fpu 2 rep vlen vreg4 = [ar4];
	ar4 = zero_double;
	fpu 3 rep vlen vreg5 = [ar4];

	fpu 0 rep vlen vreg0 = [ar0++gr0];
	fpu 0 rep vlen vreg1 = [ar1++gr1];
	fpu 0 .float vreg7 = vreg0*vreg1;
	fpu 1 vreg2 = fpu 0 vreg7;
	fpu 1 rep vlen vreg0 = [ar2++gr2];
	fpu 1 rep vlen vreg1 = [ar3++gr3];
	fpu 1 .float vreg7 = vreg0*vreg1 + vreg2;
	fpu 2 vreg7 = fpu 1 vreg7;
	fpu 2 vreg6 = fpu 1 vreg7;	
	fpu 2 .matrix vreg3 = (vreg6,vreg7) * vreg4;
	fpu 2 rep vlen [ar6++] = vreg3;
<EndProgram>
	
	
	pop ar6,gr6;
	pop ar5,gr5;
	pop ar4,gr4;
	pop ar3,gr3;
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	return;
end ".text_demo3d";
