/*!
 *  \file maskSelectionLight_RGBA_BGRA.asm
 *  \author Zhilenkov Ivan
 */
 //int maskSelectionLight_RGBA_BGRA(v4nm32s* srcLight, nm1* mask, v4nm32s* dstLight, int size){

global _maskSelectionLight_RGBA_BGRA: label;
begin ".text_demo3d"
<_maskSelectionLight_RGBA_BGRA>
	ar5 = ar7 -2;
	push ar0, gr0;
	push ar1, gr1;
	push ar2, gr2	with gr0 = false;
	push ar3, gr3	with gr3 = false;
	push ar4, gr4	with gr3++;				//gr3 = 1;
	push ar5, gr5	with gr6 = false;
	push ar6, gr6	with gr1 = gr3 << 5;	//gr1 = 32;
	ar0 = [--ar5];
	ar2 = [--ar5];
	ar6 = [--ar5];
	gr5 = [--ar5];		
<Next32>
	gr7 = gr5 - gr1;
	gr7 A>>=  31;
	gr2 = gr5 and gr7;
	gr7 = gr1 and not gr7;
	gr1 = gr2 or gr7;
	
	gr2 = [ar2++];
	gr2;
	if =0 delayed goto End32;
		gr0+=gr1;
		gr2 and gr3;
	gr0-=gr1 noflags;
<NextBit>
	if =0 delayed goto EndBit;
		nul;
		nul;
	push ar0, gr0;
	push ar6, gr6;
	//запись 3-х членов в обратном порядке
	gr0 <<= 2;
	gr6 <<= 2;
	ar0 += gr0;
	ar6 += gr6;
	gr0 = [ar0++];
	gr6 = [ar0++];
	gr7 = [ar0++];
	[ar6++] = gr7;
	[ar6++] = gr6;
	[ar6++] = gr0;
	//запись 4-го члена
	gr7 = [ar0++];
	[ar6++] = gr7;	
	pop ar6, gr6;	
	pop ar0, gr0	with gr6++;
<EndBit>
	gr1--;
	if > delayed goto NextBit	with gr0++;
		gr2>>=1;
		gr2 and gr3;
<End32>
	gr1 = gr3 << 5;
	gr5 -= gr1;			//size -= 32;
	if > delayed goto Next32;
		nul;
		nul;	
	
	gr6 and gr3;
	if =0 delayed goto EndProgram;
		gr0 = gr6;
		with gr0--;		
<Tail>
	//дублирование последнего элемента
	ar0 = ar6	with gr0 <<= 2;
	ar0 += gr0	with gr6 <<= 2;	
	ar6 += gr6	with gr6 >>= 2;
	gr0 = [ar0++];
	ar1 = [ar0++];
	gr7 = [ar0++];
	[ar6++] = gr0;
	[ar6++] = ar1;
	[ar6++] = gr7;
	//запись 4-го члена
	gr7 = [ar0++];
	[ar6++] = gr7;	
	gr6++;
<EndProgram>
	gr7 = gr6;
	
	pop ar6, gr6;
	pop ar5, gr5;
	pop ar4, gr4;
	pop ar3, gr3;
	pop ar2, gr2;
	pop ar1, gr1;
	pop ar0, gr0;
	return;
end ".text_demo3d";