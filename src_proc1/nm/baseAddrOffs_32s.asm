
//void baseAddrOffs_32s(nm32s* baseAddr, int* offsets, nm32s** ppDst, int size);
//void baseAddrOffs_32s(nm32u* baseAddr, int* offsets, nm32u** ppDst, int size);

extern vec_data_add_ram: label;

begin ".text_demo3d"
global _baseAddrOffs_32s: label;
global _baseAddrOffs_32u: label;
<_baseAddrOffs_32s>
<_baseAddrOffs_32u>
.branch;
	ar5 = ar7-2;
	push ar0,gr0	with gr7 = gr5;
	push ar1,gr1	with gr0 = false;
	push ar6,gr6	with gr0++;
	ar6 = [--ar5]	with gr0++;
	gr6 = ar6;
	ar1 = ar7;
	push ar6, gr6;	
	ar0 = [--ar5];	
	ar6 = [--ar5]	with gr6 = gr0;
	gr5 = [--ar5];
	
	nb1 = 80000000hl;
	
	delayed call vec_data_add_ram;
		gr5 >>= 1;
		nul;
		
	pop ar6,gr6;
	pop ar6,gr6;
	pop ar1,gr1;
	pop ar0,gr0		with gr5 = gr7;
	return;
.wait;
end ".text_demo3d";
