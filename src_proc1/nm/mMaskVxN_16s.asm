//***************************************************************************/
//*                     RC Module Inc., Moscow, Russia                      */
//*                     NeuroMatrix(r) NM640x Software                      */
//*                                                                         */
//*   Software design:  A.Brodyazhenko                                      */
//*                                                                         */
//*   File:             mMaskVxN_16s.asm 		                            */
//*   Contents:         This is the choice function of values				*/
//*						from 2 vectors by a mask							*/
//***************************************************************************/

// Функция выбора значений из 2-х векторов по маске
// pTriangles массив изображений с треугольниками (его размер равен размеру pMask и pROI)
// pMask массив с масками для каждого изображения треугольника
// pROI массив массив указателей на изображения (перезаписываются маскированными данными), взятые из основного изображения
// imageStride страйд, с которым будут браться изображения pROI
// pTriangsHeight массив высот для изображений с треугольниками
// pTriangsWidth массив ширин для изображений с треугольниками
// count количество изображений с треугольниками

//**************************Perfomance******************************//
// | hieght (rep)         |    1 |    2 |    4 |    8 |   16 |    32 |
//------------------------------------------------------------------//
// | width                |   32 |   32 |   32 |   32 |   32 |    32 |
// | count                |   32 |   32 |   32 |   32 |   32 |    32 |
//------------------------------------------------------------------//
// | ticks (theory)       |  512 | 1024 | 2048 | 4096 | 8192 | 16384 |
// | ticks (practice)     | 4805 | 4805 | 4869 | 5445 | 8965 | 16735 |
// | efficiency factor, % |   10 |   21 |   42 |   75 |   91 |    98 |
//******************************************************************//

global _mMaskVxN_16s:label;
// void mMaskVxN_16s(nm16s* pTriangles, nm16s* pMask, nm16s** pROI, int ImageStride, int* pTrianglesHeight, int* pTrianglesWidth, int count);

ResidualStrings: label;

macro RESIDUAL_STRINGS(N)
	own NextColumnInTrMaskROI:label;
	own EndMacro: label;
	nul;
	ar6 = gr4 with gr7--;
	if < delayed goto CheckCountForExit;
	gr3 = ar0; 			//	gr2 = 2;
	gr4 = ar1;
	gr6 = ar5;

	rep N data = [ar1++gr1] with activate data;
	rep N ram  = [ar0++gr0];
	rep N data = [ar5++gr5] with mask afifo, ram, data;
	ar5 = gr6 with gr4 += gr2 noflags;
	rep N [ar5++gr5] = afifo;

	push ar0,gr0 with gr6 += gr2 noflags ; // сохраняем адрес (это адрес следуещей маски в массиве pMask) последнего элемента 1-ого столбца pMask
	push ar1,gr1 with gr3 += gr2 noflags;  // сохраняем адрес (это адрес следуещей маски в массиве pTriangles) последнего элемента 1-ого столбца pTriangles

	if =0 delayed goto EndMacro;
	<NextColumnInTrMaskROI>
		ar5 = gr6;
		ar1 = gr4;
		ar0 = gr3;

		rep N data = [ar1++gr1] with activate data;					// читаем столбец из pMask и делаем активацию над ним (получаем маску)
		rep N ram  = [ar0++gr0];									// читаем столбец из pROI (изображение)

		rep N data = [ar5++gr5] with mask afifo, ram, data;			// читаем столбец из pTriangles (изображение с треугольником) и накладываем маску на изображение с треугольником (pTriangles) и pROI
		ar5 = gr6 with gr7--;
		rep N [ar5++gr5] = afifo;									// перезаписываем pROI

		if > delayed goto NextColumnInTrMaskROI with gr6 += gr2;
			gr4 += gr2;
			gr3 += gr2;
<EndMacro>
	delayed goto CheckCountForExit;
		pop ar1,gr1;
		pop ar0,gr0;
end RESIDUAL_STRINGS;

begin ".text_demo3d"
<_mMaskVxN_16s>
	ar5 = sp - 2;
	push ar0, gr0;
	push ar1, gr1;
	push ar2, gr2 with gr2 = false;
	push ar3, gr3 with gr2++;
	push ar4, gr4 with gr2++;
	push ar5, gr5;
	push ar6, gr6;

	ar0 = [--ar5];		// *pTriangles
	ar1 = [--ar5];		// *pMask
	ar2 = [--ar5];		// **pROI
	gr5 = [--ar5];		// ImageStride
	ar3 = [--ar5];		// *size
	gr4 = [--ar5];		// count (число изображений с треугольниками,  масками и ROI)
	gr5 >>= 1;

	f1cr = 80000000h;

//----------------Первый итерация цикла (нужна, чтобы сохранить адреса для слудующих изображений с масками и треуголиниками----------------//
<NextTrMaskROI>
	gr7 = [ar3++];					// читаем ширину(маски, изображжения с треугольником и ROI) из массива size	
	gr6 = [ar3++];					// читаем высоту(маски, изображжения с треугольником и ROI) из массива size
	ar5 = [ar2++] with gr6 <<= 5;	// читаем pROI[i] (адрес на область ROI в основном изображении)

									// число столбцов (во всех изображениях одинаково) в каждом изображении треугольников, масок и ROI(счетчик вложенного цикла)

	ar6 = ResidualStrings;

	delayed goto ar6 + gr6 with gr7 >>= 2;						// pTriagsWidth[i] >> 2
		with gr0 = gr7 << 1;
		with gr1 = gr0;

<ResidualStrings>
	RESIDUAL_STRINGS(01); //dummy

	RESIDUAL_STRINGS(01);
	RESIDUAL_STRINGS(02);
	RESIDUAL_STRINGS(03);
	RESIDUAL_STRINGS(04);
	RESIDUAL_STRINGS(05);
	RESIDUAL_STRINGS(06);
	RESIDUAL_STRINGS(07);
	RESIDUAL_STRINGS(08);
	RESIDUAL_STRINGS(09);
	RESIDUAL_STRINGS(10);
	RESIDUAL_STRINGS(11);
	RESIDUAL_STRINGS(12);
	RESIDUAL_STRINGS(13);
	RESIDUAL_STRINGS(14);
	RESIDUAL_STRINGS(15);
	RESIDUAL_STRINGS(16);
	RESIDUAL_STRINGS(17);
	RESIDUAL_STRINGS(18);
	RESIDUAL_STRINGS(19);
	RESIDUAL_STRINGS(20);
	RESIDUAL_STRINGS(21);
	RESIDUAL_STRINGS(22);
	RESIDUAL_STRINGS(23);
	RESIDUAL_STRINGS(24);
	RESIDUAL_STRINGS(25);
	RESIDUAL_STRINGS(26);
	RESIDUAL_STRINGS(27);
	RESIDUAL_STRINGS(28);
	RESIDUAL_STRINGS(29);
	RESIDUAL_STRINGS(30);
	RESIDUAL_STRINGS(31);
	RESIDUAL_STRINGS(32);

<CheckCountForExit>
	gr4 = ar6;
	gr4--;
	if > delayed goto NextTrMaskROI;
		nul;
		nul;
<exit_mMaskVxN_16s>
	pop ar6, gr6;
	pop ar5, gr5;
	pop ar4, gr4;
	pop ar3, gr3;
	pop ar2, gr2;
	pop ar1, gr1;
	pop ar0, gr0;
	return;
end ".text_demo3d";
