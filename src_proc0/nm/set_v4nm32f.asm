
//void set_v4nm32f(v4nm32f* dstVec, v4nm32f* valueC, int size);
global _set_v4nm32f: label;

begin ".text_demo3d"
<_set_v4nm32f>
	ar5 = ar7-2		with gr7 = false;
	push ar0,gr0	with gr7++;
	push ar1,gr1	with gr7++;
	push ar2,gr2	with gr0 = gr7<<1;
	ar0 = [--ar5]	with gr1 = gr0;
	ar2 = [--ar5]	with gr2 = gr7;
	fpu 0 rep 32 vreg0 = [ar2];
	ar2 += gr2	with gr2 = -gr2;	//gr2=2
	fpu 1 rep 32 vreg0 = [ar2];
	ar2 += gr2;
	gr2 = [--ar5];					//gr2 = size
	ar1 = ar0 + 2	with gr7 = gr2<<27;
	gr2>>=5;
	if =0 delayed goto AfterRep32	with gr7>>=27;
		nul;
		gr2--;
<NextRep32>
	if > delayed goto NextRep32	with gr2--;
		fpu 0 rep 32 [ar0++gr0] = vreg0;
		fpu 1 rep 32 [ar1++gr1] = vreg0;
<AfterRep32>
	gr7--;
	if < delayed goto EndProg	with gr2 = false;
		vlen = gr7				with gr2++;
		gr2++;
	fpu 0 rep vlen vreg0 = [ar2];
	ar2 += gr2;
	fpu 1 rep vlen vreg0 = [ar2];
	fpu 0 rep vlen [ar0++gr0] = vreg0;
	fpu 1 rep vlen [ar1++gr1] = vreg0;
<EndProg>
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	return;
end ".text_demo3d";
