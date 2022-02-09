

global _totalSum:label;
//int totalSum(nm32s* pVec, int size);

begin ".text_demo3d"
<_totalSum>
	ar5 = sp - 2;
	push ar0,gr0	with gr7 = false;
	push ar1,gr1;
	ar0 = [--ar5];
	gr0 = [--ar5];
	gr0--;
<NextValue>
	if >= delayed goto NextValue	with gr0--;
		gr1 = [ar0++];
		gr7 += gr1	noflags;
	pop ar1,gr1;
	pop ar0,gr0;
	return;
end ".text_demo3d";
