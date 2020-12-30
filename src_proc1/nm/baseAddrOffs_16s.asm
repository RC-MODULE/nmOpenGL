
//void baseAddrOffs_16s(nm16s* baseAddr, int* offsets, nm16s** ppDst, int size);
//void baseAddrOffs_16s(nm16u* baseAddr, int* offsets, nm16u** ppDst, int size);
data ".data_demo3d"
	matr: long [2] = (00000000000000001hl,
					  00000000100000000hl);
end ".data_demo3d";

macro shifAddRamRepN(N)
	delayed goto ar5;	
		rep N ram = [ar1];
		rep N data = [ar0++]	with vsum, shift data, ram;
		rep N [ar6++] = afifo;
end shifAddRamRepN;

extern vec_data_add_ram: label;

begin ".text_demo3d"
global _baseAddrOffs_16s: label;
global _baseAddrOffs_16u: label;
<_baseAddrOffs_16s>
<_baseAddrOffs_16u>
	StartTail: label;
	EndTail: label;
	ar5 = ar7-2;
	push ar0,gr0	with gr7 = gr5;
	push ar1,gr1;
	push ar6,gr6;
	ar1 = matr;
	nb1 = 0C000C000hl;
	sb = 2hl;
	rep 2 wfifo = [ar1++], ftw, wtw;
	ar6 = [--ar5];
	gr6 = ar6;
	ar1 = ar7;
	push ar6, gr6;	
	ar0 = [--ar5];	
	ar6 = [--ar5];
	gr5 = [--ar5];
	
	gr1 = gr5 << 26;
	gr5 >>= 6;	
	if =0 delayed goto AfterRep32	with gr1 >>= 27;	
		rep 32 ram = [ar1];
		gr5--;
<NextRep32>
	if > delayed goto NextRep32 with gr5--;
		rep 32 data = [ar0++]	with vsum, shift data, ram;
		rep 32 [ar6++] = afifo;
<AfterRep32>
	ar5 = StartTail;
	gr5 = gr1 << 2; 
	delayed goto ar5 + gr5;
		ar5 = EndTail;
<StartTail>
	delayed goto EndTail;
		nul;
		nul;
	shifAddRamRepN(1 );
	shifAddRamRepN(2 );
	shifAddRamRepN(3 );
	shifAddRamRepN(4 );
	shifAddRamRepN(5 );
	shifAddRamRepN(6 );
	shifAddRamRepN(7 );
	shifAddRamRepN(8 );
	shifAddRamRepN(9 );
	shifAddRamRepN(10);
	shifAddRamRepN(11);
	shifAddRamRepN(12);
	shifAddRamRepN(13);
	shifAddRamRepN(14);
	shifAddRamRepN(15);
	shifAddRamRepN(16);
	shifAddRamRepN(17);
	shifAddRamRepN(18);
	shifAddRamRepN(19);
	shifAddRamRepN(20);
	shifAddRamRepN(21);
	shifAddRamRepN(22);
	shifAddRamRepN(23);
	shifAddRamRepN(24);
	shifAddRamRepN(25);
	shifAddRamRepN(26);
	shifAddRamRepN(27);
	shifAddRamRepN(28);
	shifAddRamRepN(29);
	shifAddRamRepN(30);
	shifAddRamRepN(31);
<EndTail>
	pop ar6,gr6;
	pop ar6,gr6;
	pop ar1,gr1;
	pop ar0,gr0		with gr5 = gr7;
	return;
end ".text_demo3d";
