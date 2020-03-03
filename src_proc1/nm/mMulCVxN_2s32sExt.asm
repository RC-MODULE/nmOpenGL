//void mMulCVxN_2s32sExt(nm2s** ppSrcTreangle_2s, int* offsets, int* widths, int* heights, nm32s* pDstTreangle_32s, int* valueC,  int count){

data ".data_demo3d"
	matr1: long [8]=(0hl, 0hl, 0hl,
					  11hl,
					  2200000000hl,				  
					  0hl dup 3);
	addr_matr: word[34] = (matr1 + 8, matr1 + 6, matr1 + 4, matr1 + 4, matr1 + 4, matr1 + 4, matr1 + 4, matr1 + 4, 
						 matr1 + 4, matr1 + 4, matr1 + 4, matr1 + 4, matr1 + 4, matr1 + 4, matr1 + 4, matr1 + 4, 
						 matr1 + 4, matr1 + 4, matr1 + 4, matr1 + 4, matr1 + 4, matr1 + 4, matr1 + 4, matr1 + 4, 
						 matr1 + 4, matr1 + 4, matr1 + 4, matr1 + 4, matr1 + 4, matr1 + 4, matr1 + 4, matr1 + 2, matr1, matr1);		  
	sb_array: long[34] = ( 0a8hl,
						   0a8hl,
						   0a8hl,
						   02a0hl,
						   0a80hl,
						   02a00hl,
						   0a800hl,
						   02a000hl,
						   0a8000hl,
						   02a0000hl,
						   0a80000hl,						   
						   02a00000hl,	
						   0a800000hl,
						   02a000000hl,	
						   0a8000000hl,
						   02a0000000hl,
						   0a80000000hl,
						   02a_00000000hl,
						   0a8_00000000hl,
						   02a0_00000000hl,
						   0a80_00000000hl,
						   02a00_00000000hl,
						   0a800_00000000hl,
						   02a000_00000000hl,
						   0a8000_00000000hl,
						   02a0000_00000000hl,
						   0a80000_00000000hl,
						   02a00000_00000000hl,
						   0a800000_00000000hl,
						   02a000000_00000000hl,
						   0a8000000_00000000hl,
						   0a8000000_00000000hl,
						   0a8000000_00000000hl,
						   0a8000000_00000000hl);
		global _addC4DepthTest: word = 07FFFFFFFh;
end ".data_demo3d";
global _mMulCVxN_2s32sExt: label;
global _mMulCVxN_2s_v4nm8sExt: label;

macro mainLoopRepN(N)
	own Next:label;
	own CountWidthLess32:label;
	own endTreangle:label;
	own LastIter: label;
//first column	
	gr3 = ar6	with gr4--;
	rep N ram,data = [ar0++] with vsum, data, vr;
	if =0 delayed goto endTreangle	with gr3+=gr1 noflags;
		rep N [ar6++gr6] = afifo;
		ar0 = ar6;
 <Next>
	ar4 = [ar1++gr1]	with gr4--;
	sb = [ar5++gr5]; 
	rep 4 wfifo =[ar4++],ftw,wtw;
	ar6 = gr3	with gr3+=gr1 noflags;
	if > delayed goto Next;
		rep N with vsum, ram, vr;
		rep N [ar6++gr6] = afifo; 
<endTreangle>
	pop ar4, gr4	with gr7--;
	ar6 = ar0;
	if > delayed goto NextTreangle;
		nul;
		nul;
	delayed goto EndProgram;
		nul;
		nul;
	nul;nul;nul;nul;
	nul;nul;nul;nul;
end mainLoopRepN;

begin ".text_demo3d"
<_mMulCVxN_2s_v4nm8sExt>
	ar5 = ar7-2;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
	push ar3,gr3;
	push ar4,gr4;
	push ar5,gr5;
	push ar6,gr6;
	gr0 = [--ar5];		//nm2s** ppSrcTreangle_2s
	gr4 = [--ar5];		//int* offsets  -1..32
	ar2 = [--ar5];		//int* widths
	ar3 = [--ar5];		//int* heights
	ar6 = [--ar5];		//nm32s* pDstTreangle_32s
	nb1 = 80808080h;
	gr5 = false;
	vr = gr5;
	delayed goto NextTreangle;
		ar4 = [--ar5];		//int* valueC
		gr7 = [--ar5];		//int count
<_mMulCVxN_2s32sExt>
	ar5 = ar7-2;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
	push ar3,gr3;
	push ar4,gr4;
	push ar5,gr5;
	push ar6,gr6;
	gr0 = [--ar5];		//nm2s** ppSrcTreangle_2s
	gr4 = [--ar5];		//int* offsets  -1..32
	ar2 = [--ar5];		//int* widths
	ar3 = [--ar5];		//int* heights
	ar6 = [--ar5];		//nm32s* pDstTreangle_32s
	ar4 = [--ar5];		//int* valueC
	gr7 = [--ar5];		//int count
	gr5 = [_addC4DepthTest];
	vr = gr5;
	nb1 = 0C0000000h;
<NextTreangle>	
	repNHeightStart: label;
	gr5 = [ar4++];
	[matr1+6] = gr5;
	[matr1+9] = gr5;
	
	ar0 = [gr0] with gr0++;
	gr1 = [gr4]	with gr4++;	//offsetX
	
	//computing addr first matrix and sb
	push ar4, gr4	with gr1++;
	ar1 = addr_matr;
	ar1+=gr1	with gr5 = gr1 << 1;
	ar5 = sb_array;
	ar5+=gr5;
		
	gr1 = 2;
	ar4 = [ar1++gr1]	with gr5 = gr1 << 1;
	sb = [ar5++gr5];
	rep 4 wfifo = [ar4++], ftw,wtw;
	
	gr4 = [ar3++];		//height
	ar4 = repNHeightStart	with gr4 <<= 5;
	delayed goto ar4+gr4;
		gr4 = [ar2++];		//width
		gr6 = gr4		with gr4>>=1;
		nul;
<repNHeightStart>
	mainLoopRepN(1 );
	
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