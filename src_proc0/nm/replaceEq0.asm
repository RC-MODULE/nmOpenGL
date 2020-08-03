
//void replaceEq0_32f(float* srcVec, float* dstVec, int size, float nReplaceC);
global _replaceEq0_32f: label;

nobits ".data_demo3d"
	replNum : long;
end ".data_demo3d";

begin ".text_demo3d"
<_replaceEq0_32f>
	ar5 = ar7-2	;
	push ar0,gr0	with gr7 = gr5;
	push ar2,gr2;
	push ar6,gr6;
	ar0 = [--ar5];
	ar6 = [--ar5];
	gr5 = [--ar5];
	gr2 = [--ar5];
	ar2 = gr2;
	ar5 = replNum;
	[ar5] = ar2,gr2;
	fpu 0 rep 32 vreg4 = [ar5];
	gr2 = gr5<<26;
	gr5>>=6;
	if =0 delayed goto AfterRep32;
		gr2>>=27;
		gr5--;
<NextRep32>
	fpu 0 rep 32 vreg0 = [ar0++];
	fpu 0 .float vreg0 + vreg0, set mask if =0;
	fp0_lmask = fp0_lmask;
	if > delayed goto NextRep32	with gr5--;
		fpu 0 .float vreg0 = mask ? vreg4 : vreg0;
		fpu 0 rep 32 [ar6++] = vreg0;
<AfterRep32>
	gr2--;
	if < delayed goto EndProgram;
		nul;
		nul;
	vlen = gr2;
	fpu 0 rep vlen vreg4 = [ar5];
	fpu 0 rep vlen vreg0 = [ar0++];
	fpu 0 .float vreg0 + vreg0, set mask if =0;
	fp0_lmask = fp0_lmask;
	fpu 0 .float vreg0 = mask ? vreg4 : vreg0;
	fpu 0 rep vlen [ar6++] = vreg0;
<EndProgram>
	
	
	pop ar6,gr6	with gr5 = gr7;
	pop ar2,gr2;
	pop ar0,gr0;
	return;
end ".text_demo3d";
