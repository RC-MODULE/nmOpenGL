#include "asdasd"
//void copyPacket_32s(nm32s** pSrc, nm32s** pDst, int* size, int count);

.macro copy_repN N, asdadssdasdsdas
	if > delayed goto Next;
		rep \N data=[ar0++]	with data;
		rep \N [ar6++] = afifo;
	delayed goto endProgram;
		nul;
		nul;
.endm // copy_repN;

.global _copyPacket_32s
.global _copyPacket_32u
.section .text_demo3d,"ax",@progbits
_copyPacket_32s:
_copyPacket_32u:
	ar5 = ar7 - 2;
	push ar0,gr0	with gr7 = gr5;
	push ar1,gr1;
	push ar2,gr2;
	push ar6,gr6;
	
	ar1 = [--ar5];
	ar2 = [--ar5];
	gr0 = [--ar5];
	gr6 = [--ar5];
	gr5 = [gr0]		with gr0++;	
	ar5 = startCopy with gr5 <<= 2;	//>>1 and <<3
Next:
	delayed goto ar5+gr5;
		gr5 = [gr0]		with gr0++;	
		ar0 = [ar1++]	with gr5 <<= 2; //>>1 and <<3
		ar6 = [ar2++]	with gr6--;
startCopy:
	if > delayed goto Next;
		nul;
		nul;
	delayed goto endProgram;
		nul;
		nul;
	copy_repN 1
	copy_repN 2 
	copy_repN 3 
	copy_repN 4 
	copy_repN 5 
	copy_repN 6 
	copy_repN 7 
	copy_repN 8 
	copy_repN 9 
	copy_repN 10
	copy_repN 11
	copy_repN 12
	copy_repN 13
	copy_repN 14
	copy_repN 15
	copy_repN 16
	copy_repN 17
	copy_repN 18
	copy_repN 19
	copy_repN 20
	copy_repN 21
	copy_repN 22
	copy_repN 23
	copy_repN 24
	copy_repN 25
	copy_repN 26
	copy_repN 27
	copy_repN 28
	copy_repN 29
	copy_repN 30
	copy_repN 31
	copy_repN 32
endProgram:
	pop ar6,gr6;
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0		with gr5 = gr7;
	return;
