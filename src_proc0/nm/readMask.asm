/*!
 *  \file readMask.asm
 *  \author Zhilenkov Ivan
 */	
//void readMask(nm1* srcVec, int* dstIndices, int size){

begin ".text_demo3d"
global _readMask: label;
<_readMask>
	ar5 = sp - 2;
	push ar0, gr0;
	push ar1, gr1;
	push ar2, gr2	with gr1 = false;
	push ar5, gr5	with gr1++;
	push ar6, gr6	with gr0 = false;
	ar0 = [--ar5]	with gr6 = gr0;
	ar6 = [--ar5];
	gr5 = [--ar5];
	
	gr7 = [ar0++] with gr2 = gr1 << 5;
	gr7;
<NextBit>
	if =0 delayed goto EndWord;
		gr7 and gr1;	
		nul;
	if =0 delayed goto NextBit	with gr0++;
		gr2--;
		gr7 >>= 1;
	delayed goto NextBit with gr0-- noflags;
		[ar6++] = gr0	with gr6++ noflags;
		gr0++ noflags;
<EndWord>
	gr0 += gr2;
	gr0 - gr5;
	if < delayed goto NextBit;
		gr7 = [ar0++] with gr2 = gr1 << 5;
		gr7;
	gr1 = 1; 	//для RGB8888
	//gr1 = 3;	//для RGB565
<End>
	gr6 and gr1;
//дублирование последнего элемента до тех пор, пока число элементов не будет кратно числу пикселей в long
	if =0 delayed goto EndProgram;
		ar6--;
		nul;	
	delayed goto End;
		gr0 = [ar6++];
		[ar6++] = gr0	with gr6++;	
<EndProgram>
	with gr7 = gr6;
	pop ar6, gr6;
	pop ar5, gr5;
	pop ar2, gr2;
	pop ar1, gr1;
	pop ar0, gr0;
	return;
end ".text_demo3d";
