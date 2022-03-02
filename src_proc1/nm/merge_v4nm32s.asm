//  i=Merge_8s (int*pdst, int* psrc0, int* psrc1, int size);
//  psrc0 и psrc1 байтовые массивы размером size/2 байтов.
//  pdst массив байтов размером size байтов. Байты из psrc0 пакуются в чётные
//  номера байтов pdst,а байты из psrc1 пакуются в нечётные номера байтов pdst
// Пример: psrc0=8060402h, psrc1=9070503h, pdst=0908070605040302hl.
// Байтами считаем восьмибитные числа. size должно быть кратно ?.
data ".data_demo3d"
matr1: word [10]=(0,0,
 00001h,0000h,    // 0
 0000h,0000h,   // 1
 0000h,00001h,    // 2
 0000h,0000h    // 3
);
end ".data_demo3d";
begin ".text_demo3d"
extern vec_Mul2D2W2_AddVr: label;
global _merge_v4nm32s: label;
<_merge_v4nm32s>
	nb1=80000000h;  // 2 по 32 на выходе
	sb=2;           // 2 по 32 на входе X
	vr=0;
	f1cr=0;
	ar5=ar7-2;
	push ar6,gr6;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;
	push ar3,gr3 with gr1=false;
	push ar4,gr4 with gr1++; // gr1=1
	//===========================================
	ar0= [--ar5] with gr6=gr1<<3;// pdst gr6=8
	ar1= [--ar5] with gr0=gr1<<1;// psrc0 gr0=2
	ar2= [--ar5];
	gr2= [--ar5];
	ar6= [--ar5] with gr1=gr0;   // psrc1 gr1=2
	gr5= [--ar5];// src size (bytes)
	ar3=ar0; gr3=ar1;
	//=========================================== even dbl
	ar4=matr1+2 with gr5>>=1;
	ar5=ar6+4 with gr4=gr5;
	delayed call vec_Mul2D2W2_AddVr;
		ar4=matr1+2;
	//=========================================== odd dbl
	ar0=ar3;ar1=gr3;ar6=ar5 with gr5=gr4;
	delayed call vec_Mul2D2W2_AddVr;
		ar4=matr1;
	
	ar6 = ar5 - 2;
	ar5 = ar6 + 4;
	ar0 = ar2;ar1 = gr2;gr5 = gr4;
	//=========================================== even dbl
	delayed call vec_Mul2D2W2_AddVr;
		ar4=matr1+2;
	//=========================================== odd dbl
	ar0=ar2;ar1=gr2;ar6=ar5 with gr5=gr4;
	delayed call vec_Mul2D2W2_AddVr;
		ar4=matr1;
	<fin>// 1799
	pop ar4,gr4;
	pop ar3,gr3;
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	pop ar6,gr6;
	return;
	
end ".text_demo3d";
/*size  time  ticks per int для pdst=4000h, psrc0,1=80000000h,80001000h
*/
