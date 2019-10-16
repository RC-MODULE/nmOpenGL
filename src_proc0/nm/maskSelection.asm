//int maskSelection(void** src, nm1** mask, void** dst, int* size, int maxSize){

global _maskSelection: label;
begin ".text_demo3d"
<_maskSelection>
	ar5 = ar7 -2;
	push ar0, gr0;
	push ar1, gr1;
	push ar2, gr2;
	push ar3, gr3	with gr3 = false;
	push ar4, gr4	with gr3++;				//gr3 = 1;
	push ar5, gr5;
	push ar6, gr6;	//gr2 = 32;
	ar0 = [--ar5]	with gr6 = false;
	ar4 = [ar0]		with gr4 = false;
	ar2 = [--ar5];
	ar3 = [ar2];
	gr2 = ar3;
	push ar2, gr2;
	ar6 = [--ar5];
	gr0 = [--ar5];
	gr5 = [gr0];
	gr1 = [--ar5];
	push ar0, gr0;
	
<Next32>
	gr2 = [ar3++]	with gr7 = gr3 << 5;
	ar1 = gr7		with gr2;
	if =0 delayed goto End32;
		gr2 and gr3;
		nul;
<NextBit>	
	if =0 delayed goto EndBit	with gr5--;
		ar1--;
	ar2 = ar6 + gr6;
	ar5 = ar4 + gr4	with gr6++;
	gr0 = [ar5];
	[ar2] = gr0	with gr5;
<EndBit>	
	//check srcSize = 0 or dstSize == 0
	if =0 goto EndProgram	with gr0 = gr6 - gr1;	
	if =0 goto EndProgram	with gr2 >>= 1;
	if <>0 delayed goto NextBit	with gr4++;
		gr2 and gr3;
		nul;
<End32>
	gr7 = ar1;
	gr5 -= gr7;
	if > delayed goto Next32;
		gr4 += gr7;
		nul;
	gr5 = false;
	gr2 = gr6 << 31;
	if =0 delayed goto EndProgram;
		nul;
		nul;
	ar5 = ar6 + gr6	with gr6++;
	ar2 = ar6 + gr6;
	gr0 = [ar5];
	[ar2] = gr0;
<EndProgram>
	pop ar0, gr0	with gr4++;	
	pop ar2, gr2 	with gr4 >>= 5;
	gr3 = gr4 + gr2;
	[gr0] = gr5		with gr4 <<= 5;
	ar4 += gr4;
	[ar0] = ar4;
	[ar2] = gr3		with gr7 = gr6;
	
	pop ar6, gr6;
	pop ar5, gr5;
	pop ar4, gr4;
	pop ar3, gr3;
	pop ar2, gr2;
	pop ar1, gr1;
	pop ar0, gr0;
	return;
end ".text_demo3d";