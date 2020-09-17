
//вычисляет модуль если число отрицательное, иначе записывает ноль
global _absIfNegElse0:label;
//void absIfNeg(int* src, int* dst, int size);

macro AbsIfNegRepN(N)
	delayed goto ar5;
		rep N ram, data = [ar0++] with 0-data;
		rep N with activate ram and afifo;
		rep N [ar6++] = afifo;
end AbsIfNegRepN;

begin ".text_demo3d"
<_absIfNegElse0>
	NextRep32: label;
	StartTail: label;
	EndTail: label;
	
	ar5 = sp - 2;
	push ar0, gr0;
	push ar5, gr5;
	push ar6, gr6;
	ar0 = [--ar5];
	ar6 = [--ar5];
	gr5 = [--ar5];
	
	gr0 = gr5 << 26;
	gr5 >>= 6;
	if =0 delayed goto AfterRep32	with gr0 >>= 27;
		ar5 = NextRep32	with gr5--;
<NextRep32>
	if > delayed goto ar5	with gr5--;
		rep 32 ram, data = [ar0++] with 0-data;
		rep 32 with activate ram and afifo;
		rep 32 [ar6++] = afifo;
<AfterRep32>
	ar5 = StartTail	with gr5 = gr0 << 2;
	delayed goto ar5+gr5;
		nul;
		ar5 = EndTail;
<StartTail>
	delayed goto EndTail;
		nul;
		nul;
	AbsIfNegRepN(1 );
	AbsIfNegRepN(2 );
	AbsIfNegRepN(3 );
	AbsIfNegRepN(4 );
	AbsIfNegRepN(5 );
	AbsIfNegRepN(6 );
	AbsIfNegRepN(7 );
	AbsIfNegRepN(8 );
	AbsIfNegRepN(9 );
	AbsIfNegRepN(10);
	AbsIfNegRepN(11);
	AbsIfNegRepN(12);
	AbsIfNegRepN(13);
	AbsIfNegRepN(14);
	AbsIfNegRepN(15);
	AbsIfNegRepN(16);
	AbsIfNegRepN(17);
	AbsIfNegRepN(18);
	AbsIfNegRepN(19);
	AbsIfNegRepN(20);
	AbsIfNegRepN(21);
	AbsIfNegRepN(22);
	AbsIfNegRepN(23);
	AbsIfNegRepN(24);
	AbsIfNegRepN(25);
	AbsIfNegRepN(26);
	AbsIfNegRepN(27);
	AbsIfNegRepN(28);
	AbsIfNegRepN(29);
	AbsIfNegRepN(30);
	AbsIfNegRepN(31);
<EndTail>

	pop ar6, gr6;
	pop ar5, gr5;
	pop ar0, gr0;
	return;
end ".text_demo3d";
