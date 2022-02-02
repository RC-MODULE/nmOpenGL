extern shiftedZeroes: word;

//void cmpLteV_f(float* src, float C, nm1* dstMaskEven, nm1* dstMaskOdd, int size);
global _cmpLteV_f: label;

begin ".text_demo3d"
<_cmpLteV_f>
	ar5 = ar7 - 2;
	push ar4, gr4;
	push ar3, gr3;
	push ar2, gr2;
	push ar1, gr1;
	push ar0, gr0;

	ar0 = [--ar5]; // src1
	ar3 = [--ar5] with gr4 = false; // src2 
	ar1 = [--ar5]; // dstMaskEven
	ar2 = [--ar5]; // dstMaskOdd
	gr1 = [--ar5]; // size
  
  with gr1;
  if =0 delayed goto exit_CmpLteV_f;
    nul;
    nul;

	ar4 = gr4; 
	sir = ar4, gr4;
	gr2 = gr1 >> 6; 
	if =0 delayed goto less64_CmpLteV_f;
		fp0_lmask = sir;
		fp0_hmask = sir;


<CmpLteV_f>
	fpu 0 rep 32 vreg0 = [ar0++]; 						// читаем 64 элемента массива src1
	fpu 0 rep 32 vreg1 = [ar3++]; 						// читаем 64 элемента массива src2
	fpu 0 .float vreg2 = vreg0 - vreg1, set mask if <=;
	gr2--;
	if > delayed goto CmpLteV_f;
		[ar1++] = fp0_lmask;
		[ar2++] = fp0_hmask;

	gr1 = gr1 << 26;									// вычисляем остаток
	gr1 = gr1 >> 26;									// вычисляем остаток
	if =0 delayed goto exit_CmpLteV_f;					// если остаток 0, значит обработаны все элементы
    	fp0_lmask = sir;
    	fp0_hmask = sir;
<less64_CmpLteV_f>
  gr3 = 1 with gr0 = gr1 - 1; //gr3 = 1 and calc vlen (count = gr1/2 - 1 = (gr1-1)/2, vlen = count - 1)
  gr2 = gr1 with gr0 = gr0 >> 1; //remember residue count and calc vlen (count = gr1/2 - 1 = (gr1-1)/2, vlen = count - 1)
	vlen = gr0;
	fpu 0 rep vlen vreg0 = [ar0++];
	fpu 0 rep vlen vreg1 = [ar3++];
	fpu 0 .float vreg2 = vreg0 - vreg1, set mask if <=;
	[ar1++] = fp0_lmask; //write even mask immediately
  
  //odd mask require zeroing of last bit if residue count was odd 
  gr2 and gr3;
  if =0 delayed goto residueCountIsEven; //else - residue count is odd - need to zero last bit in odd mask
    sir = fp0_hmask; 
    gr1 = sir ;
  ar0 = shiftedZeroes;
  gr1 = sir;
  gr2 = [ar0+gr0];
  gr1 = gr1 and gr2;
  sir = gr1;
  fp0_hmask = sir;
<residueCountIsEven>
	[ar2++] = fp0_hmask;
<exit_CmpLteV_f>
	pop ar0, gr0;
	pop ar1, gr1;
	pop ar2, gr2;
	pop ar3, gr3;
	pop ar4, gr4;
	return;
end ".text_demo3d";
