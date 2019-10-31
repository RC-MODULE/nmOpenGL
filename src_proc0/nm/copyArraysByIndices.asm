//void copyArraysByIndices(void** srcPointers, int* indices, void** dstPointers, int nArrays, int size){

global _copyArraysByIndices: label;
begin ".text_demo3d"
<_copyArraysByIndices>
	ar5 = ar7 -2;
	push ar0, gr0;
	push ar1, gr1;
	push ar2, gr2;
	push ar4, gr4;
	push ar5, gr5;
	push ar6, gr6;
	gr0 = [--ar5]	with gr4 = false;
	ar2 = [--ar5];
	gr6 = [--ar5];
	gr2 = [--ar5];
	gr5 = [--ar5];	
	ar6 = gr6;
<NextIndex>
	gr1 = [ar2++];
	ar0 = gr0		with gr7 = gr2;
<NextArray>
	ar1 = [ar0++];
	ar4 = [ar6++]	with gr7--;
	if > delayed goto NextArray;
		ar5 = [ar1 += gr1];
		[ar4 += gr4] = ar5;
	
	with gr5--;
	if > delayed goto NextIndex;
		ar6 = gr6;
		with gr4++;
	
	pop ar6, gr6;
	pop ar5, gr5;
	pop ar4, gr4;
	pop ar2, gr2;
	pop ar1, gr1;
	pop ar0, gr0;
	return;
end ".text_demo3d";