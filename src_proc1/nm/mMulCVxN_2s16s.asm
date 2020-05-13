//void mMulCVxN_2s16s(Pattern* ppSrcTreangle_2s, Rectangle* window, int* valueC, nm16s* pDstTreangle, int count)

data ".data_demo3d"
	matr: long [14]=(0hl dup 5,
					  11hl,
					  220000hl,
					  3300000000hl,
					  44000000000000hl,				  
					  0hl dup 5);
	addr_matr: word[36] = (matr + 16, matr + 14, matr + 12, matr + 10, matr + 8, matr + 8, matr + 8, matr + 8, 
						 matr + 8, matr + 8, matr + 8, matr + 8, matr + 8, matr + 8, matr + 8, matr + 8, 
						 matr + 8, matr + 8, matr + 8, matr + 8, matr + 8, matr + 8, matr + 8, matr + 8, 
						 matr + 8, matr + 8, matr + 8, matr + 8, matr + 8, matr + 8, matr + 8, matr + 6, 
						 matr + 4, matr + 2, matr, matr);		  
	sb_array: long[36] = ( 0aa8hl,
						   0aa8hl,
						   0aa8hl,
						   0aa8hl,
						   0aa8hl,
						   02aa0hl,
						   0aa80hl,
						   02aa00hl,
						   0aa800hl,
						   02aa000hl,
						   0aa8000hl,
						   02aa0000hl,
						   0aa80000hl,						   
						   02aa00000hl,	
						   0aa800000hl,
						   02aa000000hl,	
						   0aa8000000hl,
						   02aa0000000hl,
						   0aa80000000hl,
						   02aa_00000000hl,
						   0aa8_00000000hl,
						   02aa0_00000000hl,
						   0aa80_00000000hl,
						   02aa00_00000000hl,
						   0aa800_00000000hl,
						   02aa000_00000000hl,
						   0aa8000_00000000hl,
						   02aa0000_00000000hl,
						   0aa80000_00000000hl,
						   02aa00000_00000000hl,
						   0aa800000_00000000hl,
						   0aa800000_00000000hl,
						   0aa800000_00000000hl,
						   0aa800000_00000000hl,
						   0aa800000_00000000hl,
						   0aa800000_00000000hl);
		global _addC4DepthTest_15s: word = 07FFF7FFFh;
end ".data_demo3d";
global _mMulCVxN_2s16s: label;
global _mMulCVxN_2s_v4nm8s: label;

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
	ar4 = [ar3+=4]	with gr2--;
	sb = [ar5+=8]; 
	rep 6 wfifo =[ar4++],ftw,wtw;
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
<_mMulCVxN_2s_RGB565>
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
	nb1 = 84108410h;	//5-6-5
	gr5 = false;
	vr = gr5;
	delayed goto NextTreangle;
		ar6 = [--ar5];		//nm32s* pDstTreangle_32s	
		gr7 = [--ar5];		//int count
<_mMulCVxN_2s16s>
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
	gr5 = [_addC4DepthTest_15s];
	vr = gr5;
	nb1 = 0C000C000h;
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
	gr5 = 3;		//величина gr5 зависит от от возможных отрицательных значений x
	ar3 = addr_matr	with gr3 += gr5;	
	ar3+=gr3		with gr5 = gr3 << 1;
	ar5 = sb_array	with gr3 = false;
	ar5+=gr5;
		
	
	ar4 = [ar3]	with gr3++;
	sb = [ar5] 	with gr3++;		//gr3 = 2;
	rep 6 wfifo = [ar4++], ftw,wtw;
	
	gr2 = [ar1++];		//width
	gr4 = [ar1++];		//height
	ar4 = repNHeightStart	with gr4 <<= 5;
	delayed goto ar4+gr4;
		gr2 >>= 2;			//ширина в 64-битных словах
		gr6 = gr2 << 1;
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