//void inverse_8u(const nm8u *srcArray,  nm8u *dstArray, int size)

data ".data_demo3d"
	Weights:	long[64] = (
		00000000000000080hl,00000000000000020hl,
		00000000000000008hl,00000000000000002hl,
		00000000000008000hl,00000000000002000hl,
		00000000000000800hl,00000000000000200hl,
		00000000000800000hl,00000000000200000hl,
		00000000000080000hl,00000000000020000hl,
		00000000080000000hl,00000000020000000hl,
		00000000008000000hl,00000000002000000hl,
		00000008000000000hl,00000002000000000hl,
		00000000800000000hl,00000000200000000hl,
		00000800000000000hl,00000200000000000hl,
		00000080000000000hl,00000020000000000hl,
		00080000000000000hl,00020000000000000hl,
		00008000000000000hl,00002000000000000hl,
		08000000000000000hl,02000000000000000hl,
		00800000000000000hl,00200000000000000hl,
		
		00000000000000040hl,00000000000000010hl,
		00000000000000004hl,00000000000000001hl,
		00000000000004000hl,00000000000001000hl,
		00000000000000400hl,00000000000000100hl,
		00000000000400000hl,00000000000100000hl,
		00000000000040000hl,00000000000010000hl,
		00000000040000000hl,00000000010000000hl,
		00000000004000000hl,00000000001000000hl,
		00000004000000000hl,00000001000000000hl,
		00000000400000000hl,00000000100000000hl,
		00000400000000000hl,00000100000000000hl,
		00000040000000000hl,00000010000000000hl,
		00040000000000000hl,00010000000000000hl,
		00004000000000000hl,00001000000000000hl,
		04000000000000000hl,01000000000000000hl,
		00400000000000000hl,00100000000000000hl);
end ".data_demo3d";

macro inverseRepN(N)
	delayed goto ar5;
		rep N ram = [ar0++], wtw	with vsum, ram, 0;
		rep N with vsum, shift ram, afifo;
		rep N [ar6++] = afifo;
end inverseRepN;

begin ".text_demo3d"
global _inverse_8u: label;
<_inverse_8u>
	StartTail: label;
	EndTail: label;
	ar5 = ar7 - 2;
	push ar0, gr0	with gr7 = gr5;
	push ar4, gr4;
	push ar6, gr6;
	ar0 = [--ar5];
	ar6 = [--ar5];
	gr5 = [--ar5];
	nb1 = 0FFFFFFFFh;
	sb = 0AAAAAAAAh;
	
	gr0 = gr5 << 24;	
	ar4 = Weights	with gr5 >>= 8;
	if =0 delayed goto AfterRep32	with gr0 >>= 27;
		rep 32 wfifo = [ar4++], ftw, wtw;
		gr5--;

<NextRep32>
	rep 32 wfifo = [ar4++], ftw;
	rep 32 ram = [ar0++], wtw	with vsum, ram, 0;
	ar4 = Weights;	
	if > delayed goto NextRep32	with gr5--;
		rep 32 wfifo = [ar4++], ftw, wtw with vsum, shift ram, afifo;	
		rep 32 [ar6++] = afifo;
<AfterRep32>
	ar5 = StartTail	with gr5 = gr0 << 2;
	delayed goto ar5 + gr5;
		rep 32 wfifo = [ar4++], ftw;
		ar5 = EndTail;
<StartTail>
	delayed goto EndTail;
		wtw; nul;
	inverseRepN(1 );
	inverseRepN(2 );
	inverseRepN(3 );
	inverseRepN(4 );
	inverseRepN(5 );
	inverseRepN(6 );
	inverseRepN(7 );
	inverseRepN(8 );
	inverseRepN(9 );
	inverseRepN(10);
	inverseRepN(11);
	inverseRepN(12);
	inverseRepN(13);
	inverseRepN(14);
	inverseRepN(15);
	inverseRepN(16);
	inverseRepN(17);
	inverseRepN(18);
	inverseRepN(19);
	inverseRepN(20);
	inverseRepN(21);
	inverseRepN(22);
	inverseRepN(23);
	inverseRepN(24);
	inverseRepN(25);
	inverseRepN(26);
	inverseRepN(27);
	inverseRepN(28);
	inverseRepN(29);
	inverseRepN(30);
	inverseRepN(31);
<EndTail>
	
	pop ar6, gr6;
	pop ar4, gr4;
	pop ar0, gr0	with gr5 = gr7;
	return;
end ".text_dmo3d";