/*!
 *  \file copyColorByIndices.asm
 *  \author Zhilenkov Ivan
 */
//void copyColorByIndices(v4nm32s* srcLight, int* indices, v4nm32s* dstLight, int size){

global _copyColorByIndices: label;
begin ".text_demo3d"
<_copyColorByIndices>
	ar5 = ar7 -2;
	push ar0, gr0;
	push ar1, gr1;
	push ar2, gr2;
	push ar6, gr6;
	ar0 = [--ar5];
	gr1 = [--ar5];
	ar6 = [--ar5];
	gr7 = [--ar5];	
	with gr7;
	if =0 delayed goto EndProgram;
<Next>
	gr0 = [gr1]	with gr1++;
	gr0 <<= 2;
	ar1 = ar0 + gr0;
	ar2, gr2 = [ar1++];
	[ar6++] = ar2, gr2	with gr7--;	
	if > delayed goto Next;
		ar2, gr2 = [ar1++];
		[ar6++] = ar2, gr2;
	
<EndProgram>
	
	pop ar6, gr6;
	pop ar2, gr2;
	pop ar1, gr1;
	pop ar0, gr0;
	return;
end ".text_demo3d";