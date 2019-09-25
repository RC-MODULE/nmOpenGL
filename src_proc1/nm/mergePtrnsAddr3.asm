
//void mergePtrnsAddr3(nm32s** ppSrcCA, nm32s** ppSrcCB, nm32s** ppSrcBA, int step, nm32s** ppDst, int count){
global _mergePtrnsAddr3: label;

macro mainLoopRepN(N)
	rep N ram = [ar1++];
	rep N data = [ar0++]	with data;
	rep N [ar6++] = afifo 	with ram;
	rep N ram = [ar2++];
	delayed goto repNEnd;
		rep N [ar6++] = afifo 	with ram;
		rep N [ar6++] = afifo;
end mainLoopRepN;

begin ".text_demo3d"
<_mergePtrnsAddr3>
	ar5 = ar7-2;
	push ar0,gr0	with gr7 = gr5;
	push ar1,gr1;
	push ar2,gr2;
	push ar6,gr6;
	repNStart:label;
	ar0 = [--ar5];
	ar1 = [--ar5];
	ar2 = [--ar5];
	gr1 = [--ar5];
	ar6 = [--ar5]	with gr1>>=1;
	gr0 = [--ar5];
	gr0>>=1;
	gr5 = gr0 - gr1;
	gr5 A>>= 31;
	gr2 = gr0 and gr5;
	gr5 = gr1 and not gr5;
	gr5 = gr2 or gr5;
	ar5 = repNStart	with gr5 <<= 3;
	delayed goto ar5 + gr5;
		nul;
		nul;
		nul;
<repNStart>
	delayed goto repNEnd;
		nul;
		nul;
	nul;nul;nul;nul;
	mainLoopRepN(1 );
	mainLoopRepN(2 );
	mainLoopRepN(3 );
	mainLoopRepN(4 );
	mainLoopRepN(5 );
	mainLoopRepN(6 );
	mainLoopRepN(7 );
	mainLoopRepN(8 );
	mainLoopRepN(9 );
	mainLoopRepN(10);
	mainLoopRepN(11);
	mainLoopRepN(12);
	mainLoopRepN(13);
	mainLoopRepN(14);
	mainLoopRepN(15);
	mainLoopRepN(16);
	mainLoopRepN(17);
	mainLoopRepN(18);
	mainLoopRepN(19);
	mainLoopRepN(20);
	mainLoopRepN(21);
	mainLoopRepN(22);
	mainLoopRepN(23);
	mainLoopRepN(24);
	mainLoopRepN(25);
	mainLoopRepN(26);
	mainLoopRepN(27);
	mainLoopRepN(28);
	mainLoopRepN(29);
	mainLoopRepN(30);
	mainLoopRepN(31);
	mainLoopRepN(32);
<repNEnd>
	//size-=step
	with gr0 -= gr1;
	
	//gr5 = MIN(SMALL_SIZE, size);
	gr5 = gr0 - gr1;
	gr5 A>>= 31;
	gr2 = gr0 and gr5;
	gr5 = gr1 and not gr5;
	gr5 = gr2 or gr5;
	
	gr5<<=3;
	gr0;
	if > delayed goto ar5 + gr5;
		nul;
		nul;
		nul;
		
	pop ar6,gr6	with gr5 = gr7;
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	return;
end ".text_demo3d";