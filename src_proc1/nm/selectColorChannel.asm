

global _selectColorChannel:label;
//void selectColorChannel(v4nm8s* srcImage, int channel, nm8s* dst, int size);

data ".data_demo3d"
	matr: long[128] = (00000000000000001hl,
		0hl dup 3,
		00000000000000100hl,
		0hl dup 3,
		
		00000000000010000hl,
		0hl dup 3,
		00000000001000000hl,
		0hl dup 3,
		
		00000000100000000hl,
		0hl dup 3,
		00000010000000000hl,
		0hl dup 3,
		
		00001000000000000hl,
		0hl dup 3,
		00100000000000000hl,
		0hl dup 3,
		
		
		0hl dup 1,
		00000000000000001hl,
		0hl dup 3,
		00000000000000100hl,
		0hl dup 2,
		
		0hl dup 1,
		00000000000010000hl,
		0hl dup 3,
		00000000001000000hl,
		0hl dup 2,
		
		0hl dup 1,
		00000000100000000hl,
		0hl dup 3,
		00000010000000000hl,
		0hl dup 2,
		
		0hl dup 1,
		00001000000000000hl,
		0hl dup 3,
		00100000000000000hl,
		0hl dup 2,
		
		
		0hl dup 2,
		00000000000000001hl,
		0hl dup 3,
		00000000000000100hl,
		0hl dup 1,
		
		0hl dup 2,
		00000000000010000hl,
		0hl dup 3,
		00000000001000000hl,
		0hl dup 1,
		
		0hl dup 2,
		00000000100000000hl,
		0hl dup 3,
		00000010000000000hl,
		0hl dup 1,
		
		0hl dup 2,
		00001000000000000hl,
		0hl dup 3,
		00100000000000000hl,
		0hl dup 1,
		
		
		0hl dup 3,
		00000000000000001hl,
		0hl dup 3,
		00000000000000100hl,
		
		0hl dup 3,
		00000000000010000hl,
		0hl dup 3,
		00000000001000000hl,
		
		0hl dup 3,
		00000000100000000hl,
		0hl dup 3,
		00000010000000000hl,
		
		0hl dup 3,
		00001000000000000hl,
		0hl dup 3,
		00100000000000000hl
		);	
		
end ".data_demo3d";

macro selectChannelRepN (N)
	rep N data = [ar0++gr0],ftw,wtw	with vsum, data, 0;
	rep N data = [ar1++gr1],ftw,wtw	with vsum, data, afifo;	
	rep N data = [ar2++gr2],ftw,wtw	with vsum, data, afifo;	
	rep N data = [ar3++gr3]		with vsum, data, afifo;	
	delayed goto EndTail;
		rep N [ar6++gr6] = afifo;
		nul;
end selectChannelRepN;

begin ".text_demo3d"
<_selectColorChannel>
	ar5 = sp - 2	with gr7 = false;
	push ar0,gr0	with gr7++;
	push ar1,gr1	with gr0 = gr7 << 3;
	push ar2,gr2	with gr1 = gr0;
	push ar3,gr3	with gr2 = gr0;
	push ar4,gr4	with gr3 = gr0;
	push ar5,gr5;
	push ar6,gr6;
	ar0 = [--ar5]	with gr6 = gr7 << 1;
	gr4 = [--ar5];
	ar6 = [--ar5];
	gr5 = [--ar5];
	ar1 = ar0 + 2;
	ar2 = ar0 + 4;
	ar3 = ar0 + 6;
	nb1 = 80808080h;
	sb = 2020202h;
	ar4 = matr	with gr4 <<= 6;
	ar4 += gr4	with gr5 >>= 3;
	ar5 = ar4;
	rep 32 wfifo = [ar4++], ftw, wtw;
	gr7 = gr5 << 27;
	gr5>>=5;
	if =0 delayed goto AfterRep32;
		gr5--;
		nul;
<NextRep32>
	ar4 = ar5;
	rep 32 data = [ar0++gr0],ftw,wtw	with vsum, data, 0;
	rep 32 data = [ar1++gr1],ftw,wtw	with vsum, data, afifo;	
	rep 32 data = [ar2++gr2],ftw,wtw	with vsum, data, afifo;	
	rep 32 wfifo = [ar4++];
	if > delayed goto NextRep32 with gr5--;
		rep 32 data = [ar3++gr3],ftw,wtw	with vsum, data, afifo;	
		rep 32 [ar6++gr6] = afifo;
<AfterRep32>
	StartTail: label;
	ar4 = StartTail	with gr4 = gr7 >> 24;
	delayed goto ar4 + gr4;
		nul;
		nul;
		nul;
<StartTail>
	ftw, wtw;
	ftw, wtw;
	delayed goto EndTail;
		ftw, wtw;
		nul;
	nul;nul;
	selectChannelRepN(1 );
	selectChannelRepN(2 );
	selectChannelRepN(3 );
	selectChannelRepN(4 );
	selectChannelRepN(5 );
	selectChannelRepN(6 );
	selectChannelRepN(7 );
	selectChannelRepN(8 );
	selectChannelRepN(9 );
	selectChannelRepN(10);
	selectChannelRepN(11);
	selectChannelRepN(12);
	selectChannelRepN(13);
	selectChannelRepN(14);
	selectChannelRepN(15);
	selectChannelRepN(16);
	selectChannelRepN(17);
	selectChannelRepN(18);
	selectChannelRepN(19);
	selectChannelRepN(20);
	selectChannelRepN(21);
	selectChannelRepN(22);
	selectChannelRepN(23);
	selectChannelRepN(24);
	selectChannelRepN(25);
	selectChannelRepN(26);
	selectChannelRepN(27);
	selectChannelRepN(28);
	selectChannelRepN(29);
	selectChannelRepN(30);
	selectChannelRepN(31);
<EndTail>
	
	
	pop ar6,gr6;
	pop ar5,gr5;
	pop ar4,gr4;
	pop ar3,gr3;
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	return;
end ".text_demo3d";
