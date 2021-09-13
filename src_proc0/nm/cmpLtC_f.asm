extern shiftedZeroes: word;
data ".data_demo3d"
  dupC: long = 0hl;
end ".data_demo3d";

//void cmpLtC_f(float* src, float C, nm1* dstMaskEven, nm1* dstMaskOdd, int size);
global _cmpLtC_f: label;

begin ".text_demo3d"
<_cmpLtC_f>
	ar5 = ar7 - 2;
	push ar3, gr3;
	push ar2, gr2;
	push ar1, gr1;
	push ar0, gr0;

	ar0 = [--ar5]; // src
	ar3 = [--ar5]; // C
	ar1 = [--ar5]; // dstMaskEven
	ar2 = [--ar5]; // dstMaskOdd
	gr1 = [--ar5]; // size
  
  with gr1;
  if =0 delayed goto exit_CmpLtC_f;
    nul;
    gr3 = ar3;

  [dupC] = ar3,gr3;
  ar3 = dupC with gr3 = false;

	fpu 0 rep 1 vreg1 = [ar3];
	ar3 = gr3;
	sir = ar3, gr3;
	gr2 = gr1 >> 6; 
	if =0 delayed goto less64_CmpLtC_f;
		fp0_lmask = sir;
		fp0_hmask = sir;


<CmpLtC_f>
	fpu 0 rep 32 vreg0 = [ar0++]; 						// читаем 64 элемента массива src
	fpu 0 .float vreg2 = vreg0 - .retrive(vreg1), set mask if <;
	gr2--;
	if > delayed goto CmpLtC_f;
		[ar1++] = fp0_lmask;
		[ar2++] = fp0_hmask;

	gr1 = gr1 << 26;									// вычисляем остаток
	gr1 = gr1 >> 26;									// вычисляем остаток
	if =0 delayed goto exit_CmpLtC_f;					// если остаток 0, значит обработаны все элементы
    	fp0_lmask = sir;
    	fp0_hmask = sir;
<less64_CmpLtC_f>
  gr3 = 1 with gr0 = gr1 - 1; //gr3 = 1 and calc vlen (count = gr1/2 - 1 = (gr1-1)/2, vlen = count - 1)
  gr2 = gr1 with gr0 = gr0 >> 1; //remember residue count and calc vlen (count = gr1/2 - 1 = (gr1-1)/2, vlen = count - 1)
	vlen = gr0;
	fpu 0 rep vlen vreg0 = [ar0++];
	fpu 0 .float vreg2 = vreg0 - .retrive(vreg1), set mask if <;
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
<exit_CmpLtC_f>
	pop ar0, gr0;
	pop ar1, gr1;
	pop ar2, gr2;
	pop ar3, gr3;
	return;
end ".text_demo3d";
