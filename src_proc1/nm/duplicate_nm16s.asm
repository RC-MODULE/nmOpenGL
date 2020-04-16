//void duplicate_16s(const nm16s *srcArray,  nm32s *dstArray, int count)

data ".data_demo3d"
	matr: long[8] = (00000000000010001hl,
					 00001000100000000hl,
					 00000000000000000hl,
					 00000000000000000hl,
					 
					 00000000000000000hl,
					 00000000000000000hl,
					 00000000000010001hl,
					 00001000100000000hl );
end ".data_demo3d";

extern vec_vsum_data_0: label;

begin ".text_dmo3d"
global _duplicate_16s: label;
<_duplicate_16s>
	ar5 = ar7 - 2;
	push ar0, gr0	with gr7 = gr5;
	push ar4, gr4	with gr0 = false;
	push ar6, gr6	with gr0++;
	ar0 = [--ar5]	with gr0++;
	ar6 = [--ar5]	with gr6 = gr0 << 1;
	gr5 = [--ar5];
	nb1 = 80008000h;
	sb = 20002h;
	
	ar4 = matr	with gr5 >>= 2;
	rep 8 wfifo = [ar4++], ftw, wtw;
	
	ar5 = ar6 + 2;
	ar4 = ar0;
	
	delayed call  vec_vsum_data_0;
		nul;
		nul;
		
	ftw, wtw;
	delayed call  vec_vsum_data_0;
		ar0 = ar4;
		ar6 = ar5;
	

	
	pop ar6, gr6;
	pop ar4, gr4;
	pop ar0, gr0	with gr5 = gr7;
	return;
end ".text_dmo3d";