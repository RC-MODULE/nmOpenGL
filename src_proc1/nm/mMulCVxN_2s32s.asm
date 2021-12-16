//void mMulCVxN_2s32s(Pattern* ppSrcTreangle_2s, Rectangle* window, int* valueC, nm32s* pDstTreangle_32s, int count)

data ".data_demo3d"
	matr1: long [8]=(0hl dup 3,
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

//extern _addC4DepthTest: word;

global _mMulCVxN_2s32s: label;
global _mMulCVxN_2s_RGB8888: label;

macro mainLoopRepN(N)
	own Next:label;
	own CountWidthLess32:label;
	own endTreangle:label;
	own LastIter: label;
//first column	
	gr4 = ar6	with gr2--;
	rep N ram,data = [ar0++] with vsum, data, vr;
	if =0 delayed goto endTreangle	with gr4+=gr3 noflags;
		rep N [ar6++gr6] = afifo;
		ar0 = ar6;
 <Next>
	ar4 = [ar3+=2]	with gr2--;
	sb = [ar5+=4]; 
	rep 4 wfifo =[ar4++],ftw,wtw;
	ar6 = gr4	with gr4+=gr3 noflags;
	if > delayed goto Next;
		rep N with vsum, ram, vr;
		rep N [ar6++gr6] = afifo; 
<endTreangle>
	ar6 = ar0	with gr7--;
	nul;
	if > delayed goto NextTreangle;
		nul;
		nul;
	delayed goto EndProgram;
		nul;
		nul;
	nul;nul;
	nul;nul;
	nul;nul;
end mainLoopRepN;

begin ".text_demo3d"
<_mMulCVxN_2s_RGB8888>
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
	gr5 = false;
	vr = gr5;
	delayed goto NextTreangle;
		ar6 = [--ar5];		//nm32s* pDstTreangle_32s	
		gr7 = [--ar5];		//int count
<_mMulCVxN_2s32s>
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
<NextTreangle>	
	//write mulC to matrix
	repNHeightStart: label;
	gr5 = [ar2++]		with gr3 = false;
	[matr1+6] = gr5		with gr3++;
	[matr1+9] = gr5		with gr3 <<= 6; //!ВАЖНО - величина gr3 равна размеру паттерна в int
	
	ar5 = gr0	with gr0+=gr3;	//Pattern
	
	gr3 = [gr1]	with gr1++;		//x
	gr5 = [gr1]	with gr1++;		//y
	with gr5 <<= 1;			//!ВАЖНО - умножение на ширину паттерна в int
	ar0 = ar5 + gr5;
	
	//computing addr first matrix and sb
	gr5 = 1;		//величина gr5 зависит от от возможных отрицательных значений x
	ar3 = addr_matr	with gr3 += gr5;	
	ar3+=gr3		with gr5 = gr3 << 1;
	ar5 = sb_array	with gr3 = false;
	ar5+=gr5;
		
	
	ar4 = [ar3]	with gr3++;
	sb = [ar5] 	with gr3++;		//gr3 = 2;
	rep 4 wfifo = [ar4++], ftw,wtw;
	
	gr2 = [ar1++];		//width
	gr4 = [ar1++];		//height
	ar4 = repNHeightStart	with gr4 <<= 5;
	delayed goto ar4+gr4;
	//delayed goto ar4;
		gr2 >>= 1;			//ширина в 64-битных словах
		gr6 = gr2 << 1;
		nul;
<repNHeightStart>
	//mainLoopRepN(1 );
	with gr7--;
	nul;
	if > delayed goto NextTreangle;
		nul;
		nul;
	.repeat 26;
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