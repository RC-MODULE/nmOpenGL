/*!
 *  \file abs_32f.asm
 *  \author Zhilenkov Ivan
 */
global _abs_32f:label;
//void checkRectanglesOverlaps(v2nm32f* minXY, v2nm32f* maxXY, v2nm32f* upperRightLimit, v2nm32f* lowerLeftLimit, nm1* mask, int size);

begin ".text_demo3d"
global _checkRectanglesOverlaps: label;
<_checkRectanglesOverlaps>
	ar5 = sp - 2;
	push ar0, gr0;
	push ar1, gr1;
	push ar2, gr2;
	push ar6, gr6;
	ar0 = [--ar5];				//minXY
	ar1 = [--ar5];				//maxXY
	ar2 = [--ar5];				//upperRightLimit
	fpu 0 rep 1 vreg1 = [ar2];
	ar2 = [--ar5];				//lowerLeftLimit
	fpu 1 rep 1 vreg1 = [ar2];
	ar6 = [--ar5];				//mask
	gr7 = [--ar5];				//size
	gr2 = gr7 << 27;
	gr7 >>= 5;
	if =0 delayed goto Less32;
		gr2 >>= 27;
		nul;
<Next32>
	fpu 0 rep 32 vreg0 = [ar0++]; 						// читаем 64 элемента массива minXY
	fpu 1 rep 32 vreg0 = [ar1++]; 						// читаем 64 элемента массива maxXY
	fpu 0 .float vreg2 = vreg0 - .retrive(vreg1), set mask if <;	//{Xmin-Xur,Y-Yup}
	fpu 1 .float vreg2 = vreg0 - .retrive(vreg1), set mask if >;

	sir = fp0_lmask;
	gr0 = sir;

	sir = fp0_hmask;
	gr1 = sir;

	sir = fp1_lmask	with gr0 = gr0 and gr1;
	gr1 = sir;
	
	sir = fp1_hmask	with gr0 = gr0 and gr1;
	gr1 = sir	with gr7--;

	if > delayed goto Next32;
		gr0 = gr0 and gr1;
		[ar6++] = gr0;
<Less32>
	gr2--;
	if < delayed goto EndProgram;
		vlen = gr2	with gr2 = false;
		ar2 = gr2;
	sir = ar2,gr2;
	fp0_lmask = sir;
	fp0_hmask = sir;
	fp1_lmask = sir;
	fp1_hmask = sir;

	fpu 0 rep vlen vreg0 = [ar0++]; 						// читаем 64 элемента массива minXY
	fpu 1 rep vlen vreg0 = [ar1++]; 						// читаем 64 элемента массива maxXY
	fpu 0 .float vreg2 = vreg0 - .retrive(vreg1), set mask if <;	//{Xmin-Xur,Y-Yup}
	fpu 1 .float vreg2 = vreg0 - .retrive(vreg1), set mask if >;

	sir = fp0_lmask;
	gr0 = sir;

	sir = fp0_hmask;
	gr1 = sir;

	sir = fp1_lmask	with gr0 = gr0 and gr1;
	gr1 = sir;

	sir = fp1_hmask	with gr0 = gr0 and gr1;
	gr1 = sir;
	gr0 = gr0 and gr1;
	[ar6++] = gr0;
<EndProgram>
	pop ar6, gr6;
	pop ar2, gr2;
	pop ar1, gr1;
	pop ar0, gr0;
	return;
end ".text_demo3d";