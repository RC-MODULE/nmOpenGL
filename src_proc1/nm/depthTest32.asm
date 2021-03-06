
//void depthFunc_rw(nm16s** pROI, int imageStride, nm16s* pTriangles, nm16s* pDstSub  int* pTriangsHeight, int* pTriangsWidth, int count);

extern _depthTestCore32_Lt_rw: label;

data ".data_demo3d"
	global _currentDepthTest32: word = _depthTestCore32_Lt_rw;
end ".data_demo3d";

begin ".text_demo3d"
global _depthTest32: label;
<_depthTest32>
.branch;
	ar5 = ar7-2;
	push ar0,gr0;
	push ar1,gr1;
	push ar2,gr2;	
	push ar3,gr3;
	push ar4,gr4;
	push ar5,gr5;
	push ar6,gr6;	
	ar2 = [--ar5];		//nm16s** pROI
	gr0 = [--ar5];		//int ImageStride
	ar1 = [--ar5];		//nm16s* pTriangles
	ar6 = [--ar5]	with gr2 = false;		//nm16s* dstMask
	ar3 = [--ar5]	with gr2++;		//Size* sizes
	gr4 = [--ar5]	with gr2++;		//int count
	gr7 = 80000000h;
	nb1 = gr7;
	f1cr = gr7;
	wtw;
<NextTreangle>		
	startProg: label;
	gr7 = [ar3++];		//gr7 = width
	gr5 = [ar3++];		//height
	ar0 = [ar2++]	with	gr7 >>= 1;	//addr in Img
	ar5 = [_currentDepthTest32];
	delayed call ar5;
		with gr6 = gr7<<1;
		with gr1 = gr6;
		nul;
	
	gr4--;
	if > delayed goto NextTreangle;
		nul;
		nul;

	pop ar6,gr6;
	pop ar5,gr5;
	pop ar4,gr4;
	pop ar3,gr3;
	pop ar2,gr2;
	pop ar1,gr1;
	pop ar0,gr0;
	return;
.wait;
end ".text_demo3d";
