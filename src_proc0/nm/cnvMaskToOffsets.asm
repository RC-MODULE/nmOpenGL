//int cnvMaskToOffsets(nm1* mask, int* offsets, int size)
//Based on readMask 

begin ".text_demo3d"
global _cnvMaskToOffsets: label;
<_cnvMaskToOffsets>
	ar5 = sp - 2;
	push ar0, gr0;
	push ar1, gr1;
	push ar2, gr2	with gr1 = false;
	push ar3, gr3 with gr1++;
	push ar5, gr5	with gr0 = false;
	push ar6, gr6 with gr3 = false;
	ar0 = [--ar5]	with gr6 = gr0;
	ar6 = [--ar5];
	gr5 = [--ar5];
	
	gr7 = [ar0++] with gr2 = gr1 << 5;
	gr7;
<NextBit>
	if =0 delayed goto EndWord;
		gr0 - gr5;	
		nul;
	if >= delayed goto EndWord;
		gr7 and gr1;	
		nul;
		gr3++ noflags;
	if =0 delayed goto NextBit	with gr0++;
		gr2--;
		gr7 >>= 1;
	delayed goto NextBit with gr3-- noflags;
		[ar6++] = gr3	with gr6++ noflags;
		gr3 = gr1;
<EndWord>
	gr0 += gr2;
	gr3 += gr2;
	gr0 - gr5;
	if < delayed goto NextBit;
		gr7 = [ar0++] with gr2 = gr1 << 5;
		gr7;
	gr1 = 1;
<End>
	with gr7 = gr6;
	pop ar6, gr6;
	pop ar5, gr5;
	pop ar3, gr3;
	pop ar2, gr2;
	pop ar1, gr1;
	pop ar0, gr0;
	return;
end ".text_demo3d";
