
//void depthFunc_rw(nm16s** pROI, int imageStride, nm16s* pTriangles, nm16s* pDstSub  int* pTriangsHeight, int* pTriangsWidth, int count);

macro macroDepthTest(N)
	own NextColumn:label;
	own EndColumn:label;
	
	rep N ram =[ar0++gr0];
	rep N data = [ar1++gr1] with 0 - data;
	rep N [ar6++gr6] = afifo	with not activate afifo;	
	ar0 = gr4	with gr4 += gr2;
	ar1 = gr5	with gr5 += gr2;
	rep N data = [ar1++gr1] with mask afifo, data, ram;
	rep N [ar0++gr0] = afifo;
	push ar1,gr1;	
	push ar6,gr6;
	ar2 += gr2 with gr7--;
	if <= delayed goto EndColumn;
		ar0 = gr4;		
		ar6 = ar2;
<NextColumn>	
	ar1 = gr5;
	rep N ram =[ar0++gr0];
	rep N data = [ar1++gr1] with 0 - data;
	rep N [ar6++gr6] = afifo	with not activate afifo;
	ar0 = gr4	with gr4 += gr2;
	ar1 = gr5	with gr5 += gr2;
	rep N data = [ar1++gr1] with mask afifo, data, ram;
	rep N [ar0++gr0] = afifo;
	nul;
	ar2 += gr2	with gr7--;
	if > delayed goto NextColumn;
		ar0 = gr4;	
		ar6 = ar2;
<EndColumn>
	delayed goto DepthTestMacros_end;
		pop ar6,gr6;
		pop ar1,gr1;
end macroDepthTest;

//[in] ar0,gr0 - image
//[in, out] ar1,gr1 - triangle
//[in, out] ar6,gr6 - sub for ext mask
//[in] gr5 - height
//[in] gr7 - width in 64-word

begin ".text_demo3dExt"
global _depthTestCore_A_rw: label;
<_depthTestCore_A_rw>
	DepthTestMacros_start:label;
	push ar2, gr2;
	push ar4, gr4;
	ar4 = DepthTestMacros_start	with gr4 = gr5 << 5;
	delayed goto ar4+gr4	with gr2 = false;	
		ar2 = ar6	with gr2++;
		gr4 = ar0	with gr2++;
		gr5 = ar1;		
<DepthTestMacros_start>
	macroDepthTest(1 );
	macroDepthTest(1 );
	macroDepthTest(2 );
	macroDepthTest(3 );
	macroDepthTest(4 );
	macroDepthTest(5 );
	macroDepthTest(6 );
	macroDepthTest(7 );
	macroDepthTest(8 );
	macroDepthTest(9 );
	macroDepthTest(10);
	macroDepthTest(11);
	macroDepthTest(12);
	macroDepthTest(13);
	macroDepthTest(14);
	macroDepthTest(15);
	macroDepthTest(16);
	macroDepthTest(17);
	macroDepthTest(18);
	macroDepthTest(19);
	macroDepthTest(20);
	macroDepthTest(21);
	macroDepthTest(22);
	macroDepthTest(23);
	macroDepthTest(24);
	macroDepthTest(25);
	macroDepthTest(26);
	macroDepthTest(27);
	macroDepthTest(28);
	macroDepthTest(29);
	macroDepthTest(30);
	macroDepthTest(31);
	macroDepthTest(32);
<DepthTestMacros_end>	
	pop ar4, gr4;
	pop ar2, gr2;
	delayed return;
		nul;
		nul;
		nul;
end ".text_demo3dExt";
