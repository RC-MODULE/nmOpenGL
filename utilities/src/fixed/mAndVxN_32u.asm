
//void mAndVxN_32u(nm32u** pSrc1, nm32u** pSrc2, nm32u** pDst, int* size, int count);

macro and_repN(N)
	delayed goto ar3;
		rep N ram=[ar0++];
		rep N data=[ar1++]	with data and ram;
		rep N [ar6++] = afifo;
end and_repN;

extern vec_vsum_data_0: label;
global _mAndVxN_32u: label;
begin ".text_demo3d"
<_mAndVxN_32u>
	ar5 = ar7-2;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
	push ar3,gr3;
	push ar5,gr5;
	push ar6,gr6;
	startCopy: label;
	endCopy: label;
	
	gr0 = [--ar5];
	gr1 = [--ar5];
	gr6 = [--ar5];
	ar2 = [--ar5];
	gr7 = [--ar5];
	gr5 = [ar2++];	
	ar3 = endCopy;
	ar5 = startCopy with gr5 <<= 1;
<Next>
	delayed goto ar5+gr5;
		ar0 = [gr0]	with gr0++;
		ar1 = [gr1] with gr1++;
		ar6 = [gr6]	with gr6++;
<startCopy>
	delayed goto endCopy;
		nul;
		nul;
	and_repN(1 );
	and_repN(2 );
	and_repN(3 );
	and_repN(4 );
	and_repN(5 );
	and_repN(6 );
	and_repN(7 );
	and_repN(8 );
	and_repN(9 );
	and_repN(10);
	and_repN(11);
	and_repN(12);
	and_repN(13);
	and_repN(14);
	and_repN(15);
	and_repN(16);
	and_repN(17);
	and_repN(18);
	and_repN(19);
	and_repN(20);
	and_repN(21);
	and_repN(22);
	and_repN(23);
	and_repN(24);
	and_repN(25);
	and_repN(26);
	and_repN(27);
	and_repN(28);
	and_repN(29);
	and_repN(30);
	and_repN(31);
	and_repN(32);
<endCopy>
	gr5 = [ar2++]	with gr7--;	
	if > delayed goto Next;
		gr5<<=1;
		nul;
	pop ar6,gr6;
	pop ar5,gr5;
	pop ar3,gr3;
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	return;
end ".text_demo3d";
