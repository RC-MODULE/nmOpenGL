

global _selectPaintSide:label;
//void selectPaintSide(nm32s* crossProd,int X, int Y, nm32s* pDst, int nSize);

macro selectSide (N)
	rep N data=[ar0++] with activate data;
	rep N ram =[ar1];
	delayed goto EndTail;
		rep N data=[ar2] with mask afifo,ram,data;
		rep N [ar6++]=afifo;
	nul;nul;
end selectSide;

begin ".text_demo3d"
<_selectPaintSide>
	ar5 = sp - 2;
	push ar0,gr0	with gr7=gr5;
	push ar1,gr1;
	push ar2,gr2;
	push ar3,gr3;
	push ar6,gr6;
	ar0 = [--ar5];
	ar1 = [--ar5];
	ar2 = [--ar5];
	ar6 = [--ar5];
	gr3 = [--ar5];
	
	gr1 = ar1;
	ar5 = ar7;
	push ar1,gr1;
	ar1 = ar5;
	
	gr2 = ar2;
	ar5 = ar7;
	push ar2,gr2;
	ar2 = ar5;
	
	sir = 80000000h;
	f1cr = sir;
	gr3>>=1;
	gr5=gr3>>5;
	BeforeTail:label;
	Tail:label;
	EndTail: label;
	if =0 delayed goto BeforeTail with gr3<<=27;
		gr3>>=27;
		gr5--;	
<Next32>
	rep 32 data=[ar0++] with activate data;
	rep 32 ram =[ar1];
	if > delayed goto Next32 with gr5--;
		rep 32 data=[ar2] with mask afifo,ram,data;
		rep 32 [ar6++]=afifo;
<BeforeTail>
	ar3 = Tail	with gr3 <<=3;
	delayed goto ar3+gr3;
		nul;
		nul;
		nul;
<Tail>
	delayed goto EndTail;
		nul;
		nul;
	nul;nul;nul;nul;
	selectSide (1 );
	selectSide (2 );
	selectSide (3 );
	selectSide (4 );
	selectSide (5 );
	selectSide (6 );
	selectSide (7 );
	selectSide (8 );
	selectSide (9 );
	selectSide (10);
	selectSide (11);
	selectSide (12);
	selectSide (13);
	selectSide (14);
	selectSide (15);
	selectSide (16);
	selectSide (17);
	selectSide (18);
	selectSide (19);
	selectSide (20);
	selectSide (21);
	selectSide (22);
	selectSide (23);
	selectSide (24);
	selectSide (25);
	selectSide (26);
	selectSide (27);
	selectSide (28);
	selectSide (29);
	selectSide (30);
	selectSide (31);
<EndTail>
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar6,gr6;
	pop ar3,gr3;
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0		with gr5=gr7;
	return;
end ".text_demo3d";
