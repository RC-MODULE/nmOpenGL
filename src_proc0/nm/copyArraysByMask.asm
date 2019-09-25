//int copyArraysByMask(float** srcPointers, nm1* mask, float** dstPointers, int nArrays, int size){

global _copyArraysByMask: label;
begin ".text_demo3d"
<_copyArraysByMask>
	ar5 = ar7 -2;
	push ar0, gr0;
	push ar1, gr1;
	push ar2, gr2	with gr1 = false;
	push ar3, gr3	with gr3 = false;
	push ar4, gr4	with gr3++;				//gr3 = 1;
	push ar5, gr5	with gr4 = false;
	push ar6, gr6	with gr6 = gr3 << 5;	//gr6 = 32;
	ar0 = [--ar5];
	ar2 = [--ar5];
	ar6 = [--ar5];
	gr0 = [--ar5];
	gr5 = [--ar5];		
<Next32>
	gr7 = gr5 - gr6;
	gr7 A>>=  31;
	gr2 = gr5 and gr7;
	gr7 = gr6 and not gr7;
	gr6 = gr2 or gr7;
	
	gr2 = [ar2++];
	gr2;
	if =0 delayed goto End32;
		gr1+=gr6;
		gr2 and gr3;
	gr1-=gr6 noflags;
<NextBit>
	if =0 delayed goto EndBit;
		nul;
		nul;
	push ar0, gr0;
	push ar6, gr6;
<NextArray>
	ar1 = [ar0++];
	ar4 = [ar6++]	with gr0--;
	if > delayed goto NextArray;
		gr7 = [ar1 += gr1];
		[ar4 += gr4] = gr7;
	
	pop ar6, gr6	with gr4++;	
	pop ar0, gr0;
<EndBit>
	gr6--;
	if > delayed goto NextBit	with gr1++;
		gr2>>=1;
		gr2 and gr3;
<End32>
	gr6 = gr3 << 5;
	gr5 -= gr6;			//size -= 32;
	if > delayed goto Next32;
		nul;
		nul;
		
	
	gr4 and gr3;
	if =0 delayed goto EndProgram;
		gr1 = gr4;
		with gr1--;		
<Tail>
//дублирование последнего элемента
	ar1 = [ar6];
	ar4 = [ar6++]	with gr0--;
	if > delayed goto Tail;
		gr7 = [ar1 += gr1];
		[ar4 += gr4] = gr7;
	gr4++;
<EndProgram>
	gr7 = gr4;
	
	pop ar6, gr6;
	pop ar5, gr5;
	pop ar4, gr4;
	pop ar3, gr3;
	pop ar2, gr2;
	pop ar1, gr1;
	pop ar0, gr0;
	return;
end ".text_demo3d";