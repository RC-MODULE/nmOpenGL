//void mMulCVxN_2s32s(Pattern* ppSrcTreangle_2s, Rectangle* window, int* valueC, nm32s* pDstTreangle_32s, int count)

extern _addC4DepthTest: word;
data ".data_demo3d"
	matr: long[34] = (0hl dup 34);

	//global _addC4DepthTest: word = 07FFFFFFFh;

	full_masks: long[33] = ( 0_00000000_00000003hl,
						  	 0_00000000_0000000fhl,
							 0_00000000_0000003chl,
							 0_00000000_000000f0hl,
							 0_00000000_000003c0hl,
							 0_00000000_00000f00hl,
							 0_00000000_00003c00hl,
							 0_00000000_0000f000hl,
							 0_00000000_0003c000hl,
							 0_00000000_000f0000hl,
							 0_00000000_003c0000hl,
							 0_00000000_00f00000hl,
							 0_00000000_03c00000hl,
							 0_00000000_0f000000hl,
							 0_00000000_3c000000hl,
							 0_00000000_f0000000hl,
							 0_00000003_c0000000hl,
							 0_0000000f_00000000hl,
							 0_0000003c_00000000hl,
							 0_000000f0_00000000hl,
							 0_000003c0_00000000hl,
							 0_00000f00_00000000hl,
							 0_00003c00_00000000hl,
							 0_0000f000_00000000hl,
							 0_0003c000_00000000hl,
							 0_000f0000_00000000hl,
							 0_003c0000_00000000hl,
							 0_00f00000_00000000hl,
							 0_03c00000_00000000hl,
							 0_0f000000_00000000hl,
							 0_3c000000_00000000hl,
							 0_f0000000_00000000hl, 
							 0_30000000_00000000hl );

end ".data_demo3d";
global _mMulCVxN_2s32s_new: label;
global _mMulCVxN_2s_RGB8888_new: label;

macro mainLoopRepN(N)
	own NextColumn:label;
	own EndRep:label;
//first column	
	rep N ram = [ar0++];
	gr4 = ar6;	
	rep N data = [ar3]	with vsum data, ram, vr;
	rep N [ar6++gr6] = afifo;
	ar0 = ar6	with gr2--;
	nul;
	if <= delayed goto EndRep;
		ar3 += gr3	with 	gr4 += gr5;	
		ar6 = gr4	with 	gr2--;
<NextColumn>
	rep N data = [ar3]	with vsum data, ram, vr;
	rep N [ar6++gr6] = afifo;
	if > delayed goto NextColumn;
		ar3 += gr3	with	gr4 += gr5;
		ar6 = gr4	with 	gr2--;
<EndRep>
	ar6 = ar0 with gr7--;
	nul;
	if > delayed goto NextTreangle;
		nul;
		nul;
	delayed goto EndProgram;
		nul;
		nul;
	.repeat 6;
	nul;
	.endrepeat;
end mainLoopRepN;

begin ".text_demo3d"
<_mMulCVxN_2s_RGB8888_new>
	ar5 = ar7-2;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
	push ar3,gr3;
	push ar4,gr4;
	push ar5,gr5;
	push ar6,gr6;
	gr0 = [--ar5];		//Patterns* triangles
	gr1 = [--ar5];		//Point* point
	ar1 = [--ar5];		//Size size
	ar2 = [--ar5];		//int* valueC
	nb1 = 80808080h;
	sb = 0AAAAAAAAh;
	gr5 = false;
	vr = gr5;
	delayed goto NextTreangle;
		ar6 = [--ar5];		//nm32s* pDstTreangle_32s	
		gr7 = [--ar5];		//int count
<_mMulCVxN_2s32s_new>
	ar5 = ar7-2;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
	push ar3,gr3;
	push ar4,gr4;
	push ar5,gr5;
	push ar6,gr6;
	gr0 = [--ar5];		//Patterns* triangles
	gr1 = [--ar5];		//Point* point
	ar1 = [--ar5];		//Size size
	ar2 = [--ar5];		//int* valueC
	ar6 = [--ar5];		//nm32s* pDstTreangle_32s	
	gr7 = [--ar5];		//int count
	gr5 = [_addC4DepthTest];
	vr = gr5;
	nb1 = 0C0000000h;
	sb = 0AAAAAAAAh;
<NextTreangle>	
	//write mulC to matrix
	repNHeightStart: label;
	gr5 = [ar2++];
	[matr] = gr5;		[matr + 3] = gr5;
	gr5 = 4;
	ar5 = matr;
	rep 16 data = [ar5] with data;
	ar5 += gr5;
	rep 16 [ar5++gr5] = afifo;
	ar5 = matr + 2;
	rep 16 data = [ar5] with data;
	rep 16 [ar5++gr5] = afifo;
	//------------------------

	//pattern
	gr3 = 64;
	ar5 = gr0 with gr0 += gr3;
	gr3 = [gr1]	with gr1++;		//x
	gr5 = [gr1]	with gr1++;		//y
	gr5 <<= 1;
	ar0 = ar5 + gr5;
	//loading wfifo
	gr4 = gr3 << 31;
	gr4 >>= 31;
	gr4 <<= 1;
	ar4 = matr;
	ar4 += gr4;
	rep 32 wfifo = [ar4++], ftw, wtw;	
	//------------------------

	//select first mask (gr3 = x)
	gr5 = 1;
	gr3 += gr5;				
	ar3 = full_masks	with gr3 <<= 1;
	ar3 += gr3;
	gr5 = 2;
	gr3 = gr5 << 1;

	//select macro
	gr2 = [ar1++];		//width
	gr4 = [ar1++];		//height
	ar4 = repNHeightStart	with gr4 <<= 5;
	delayed goto ar4+gr4;
	//delayed goto ar4;
		gr2 >>= 1;			//ширина в 64-битных словах
		gr6 = gr2 << 1;
		nul;

<repNHeightStart>
	delayed goto repNHeightEnd;
		vnul;
		vnul;
	.repeat 28;
	nul;
	.endrepeat;
	
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
<repNHeightEnd>
	gr7--;
	if > delayed goto NextTreangle;
		vnul;
		vnul;
<EndProgram>
		
	pop ar6,gr6;
	pop ar5,gr5;
	pop ar4,gr4;
	pop ar3,gr3;
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	return;
end ".text_demo3d";