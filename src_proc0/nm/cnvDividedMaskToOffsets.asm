//int cnvDividedMaskToOffsets(nm1* maskEven, nm1* maskOdd, int* offsets, int size);
//Based on readDividedMask. size must be odd for correct result.

begin ".text_demo3d"
global _cnvDividedMaskToOffsets: label;
<_cnvDividedMaskToOffsets>
	ar5 = sp - 2;
	push ar0, gr0	with gr7 = false;
	push ar1, gr1	with gr1 = false;
	push ar2, gr2	with gr1++;
	push ar3, gr3;
	push ar4, gr4;
	push ar5, gr5 with gr0 = gr7;
	push ar6, gr6	with gr6 = false;
	ar0 = [--ar5];
	ar1 = [--ar5];
	ar6 = [--ar5];
	gr5 = [--ar5]	 with gr2 = gr1 << 5;
	gr5 >>=1 ;
	if =0 delayed goto End;	
	gr3 = [ar0++];
	gr4 = [ar1++];
<NextWord>
	gr3 or gr4;
<EvenMask>
	if =0 delayed goto EndWord;
		gr0 - gr5;	
		nul;
	if >= delayed goto EndWord;
		gr3 and gr1;	
		nul;
	if =0 delayed goto EvenMaskElse;
		gr3 >>= 1;
		gr4 and gr1;
	[ar6++] = gr6	with gr6 = gr1;	
	gr7++;
	gr4 and gr1;
<OddMask>
	if =0 delayed goto OddMaskElse;
		gr2--;
		gr4 >>= 1;
	[ar6++] = gr6	with gr6 = gr1;	
	gr7++;	
<EndBit>
	delayed goto EvenMask;
		gr0++;
		gr3 or gr4;
<EvenMaskElse>
  delayed goto OddMask;
    gr6++ noflags;
    nul;
<OddMaskElse>
  delayed goto EndBit;
    gr6++ noflags;
    nul;
<EndWord>
	gr0 += gr2;
  gr2 <<= 1;
  gr6 += gr2; 
	gr2 = gr1 << 5;
	gr0 - gr5;
	if < delayed goto NextWord;
	  gr3 = [ar0++];
	  gr4 = [ar1++];
<End>
	pop ar6, gr6;
	pop ar5, gr5;
	pop ar4, gr4;
	pop ar3, gr3;
	pop ar2, gr2;
	pop ar1, gr1;
	pop ar0, gr0;
	return;
end ".text_demo3d";
