	
//void readDividedMask(nm1* maskEven, nm1* maskOdd, int* dstIndices, int size){

begin ".text_demo3d"
global _readDividedMask: label;
<_readDividedMask>
	ar5 = sp - 2;
	push ar0, gr0	with gr7 = false;
	push ar1, gr1	with gr1 = false;
	push ar2, gr2	with gr1++;
	push ar3, gr3;
	push ar4, gr4;
	push ar5, gr5;
	push ar6, gr6	with gr0 = gr7;
	ar0 = [--ar5];
	ar1 = [--ar5];
	ar6 = [--ar5];
	gr5 = [--ar5]	 with gr2 = gr1 << 5;
	gr5 >>=1 ;
	if =0 delayed goto End;	
<NextWord>
	gr3 = [ar0++];
	gr4 = [ar1++];
	gr3 or gr4;
<EvenMask>
	if =0 delayed goto EndWord;
		gr3 and gr1;	
		nul;
	if =0 delayed goto OddMask;
		gr3 >>= 1;
		gr4 and gr1;
	gr0 <<= 1;
	[ar6++] = gr0	with gr0 >>= 1;	//запись 2*i
	gr7++;
	gr4 and gr1;
<OddMask>
	if =0 delayed goto EndBit;
		gr2--;
		gr4 >>= 1;
	gr0 <<= 1;
	gr0++;
	[ar6++] = gr0	with gr0>>=1;	//запись 2*i+1
	gr7++;	
<EndBit>
	delayed goto EvenMask;
		gr0++;
		gr3 or gr4;
<EndWord>
	gr0 += gr2;
	gr2 = gr1 << 5;
	gr0 - gr5;
	if < delayed goto NextWord;
		nul;
		nul;
<End>
	gr7 and gr1;
	if =0 delayed goto EndProgram;
		ar6--;
		nul;	
	gr0 = [ar6++];
	[ar6++] = gr0	with gr7++;	
<EndProgram>
	pop ar6, gr6;
	pop ar5, gr5;
	pop ar4, gr4;
	pop ar3, gr3;
	pop ar2, gr2;
	pop ar1, gr1;
	pop ar0, gr0;
	return;
end ".text_demo3d";
