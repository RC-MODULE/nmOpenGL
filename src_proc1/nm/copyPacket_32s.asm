
//void copyPacket_32s(nm32s** pSrc, nm32s** pDst, int* size, int count);

macro copy_repN(N)
	delayed goto endCopy;
		rep N data=[ar0++]	with data;
		rep N [ar6++] = afifo;
end copy_repN;

begin ".text_demo3d"
global _copyPacket_32s: label;
global _copyPacket_32u: label;
<_copyPacket_32s>
<_copyPacket_32u>
.branch;
	ar5 = ar7-2;
	push ar0,gr0	with gr7 = gr5;
	push ar1,gr1;
	push ar6,gr6;
	startCopy: label;
	
	gr1 = [--ar5];
	ar1 = [--ar5];
	gr0 = [--ar5];
	gr6 = [--ar5];
	gr5 = [gr0]		with gr0++;	
	ar5 = startCopy with gr5 <<= 1;
<Next>
	delayed goto ar5+gr5;
		ar0 = [gr1]		with gr1++;
		ar6 = [ar1++];
		gr6--;
<startCopy>
	delayed goto endCopy;
		nul;
		nul;
	copy_repN(1 );
	copy_repN(2 );
	copy_repN(3 );
	copy_repN(4 );
	copy_repN(5 );
	copy_repN(6 );
	copy_repN(7 );
	copy_repN(8 );
	copy_repN(9 );
	copy_repN(10);
	copy_repN(11);
	copy_repN(12);
	copy_repN(13);
	copy_repN(14);
	copy_repN(15);
	copy_repN(16);
	copy_repN(17);
	copy_repN(18);
	copy_repN(19);
	copy_repN(20);
	copy_repN(21);
	copy_repN(22);
	copy_repN(23);
	copy_repN(24);
	copy_repN(25);
	copy_repN(26);
	copy_repN(27);
	copy_repN(28);
	copy_repN(29);
	copy_repN(30);
	copy_repN(31);
	copy_repN(32);
<endCopy>
	if > delayed goto Next;
		gr5 = [gr0]		with gr0++;	
		gr5<<=1;
	pop ar6,gr6;
	pop ar1,gr1;
	pop ar0,gr0		with gr5 = gr7;
	return;
.wait;
end ".text_demo3d";
