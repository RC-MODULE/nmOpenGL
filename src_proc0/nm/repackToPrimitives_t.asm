// int repackToPrimitives_t(const v4nm32f *srcVertex,
// 		const v4nm32f *srcColor,
// 		const v2nm32f *srcTex,
// 		TrianglePointers *dstVertex,
// 		int vertexAmount);

global _repackToPrimitives_t: label;// объявление глобальной метки
extern ___errno: label;
//import from rtmacro.mlb;
extern IDiv32: label;

data ".data_demo3d"        //секция инициализированных данных
	M10: word[2] = (float(1), float(0)); // extract X0,X1 from (X0,Y0;X1,Y1)T vec
	M01: word[2] = (float(0), float(1)); // extract Y0,Y1 from (X0,Y0;X1,Y1)T vec
	buf: word[64];					// buf to copy odd words from vreg to mem
	vertCount: word;				// number of input vertexes (used only in GL_TRIANGLES)
	retVal: word;					// return value
	dstVertex: word;
	dstColorEnd: word;
	parity: word;	// 1 if number of output triangles is odd
end ".data_demo3d";  

// Copies n rgba (64-bit) words from src to dst
// get n words from src with srcOffset
// put n words to dst with dstOffset
macro copyCol(src, dst, srcOffset, dstOffset, n)
	own Loop: label;
	own Extract: label;
	gr5 = n;	
	ar0 = src;			//rg input
	gr0 = srcOffset;
	ar2 = ar0 + 2;		//ba input
	gr2 = srcOffset;
	ar4 = dst;			//rg output
	gr4 = dstOffset;
	ar6 = ar4 + 2;		//ba output
	gr6 = dstOffset;
<Loop>
	gr7 = 32;	// gr7 - max number of pairs to load into vreg 
	gr5 - gr7;
	if >= goto Extract;
	gr7 = gr5;
<Extract>
	gr3 = gr7;	// gr7 - subtract from total number of triangles
				// at the end of the loop
	gr3--;		 
	vlen = gr3;
	fpu 1 rep vlen vreg0 = [ar0++gr0];	// copy rg components into vreg0
	fpu 1 rep vlen vreg1 = [ar2++gr2];	// copy ba components into vreg1
	fpu 1 rep vlen [ar4++gr4] = vreg0;	// copy rg components from vreg0 to dst
	fpu 1 rep vlen [ar6++gr6] = vreg1;	// copy ba components from vreg1 to dst	
	gr5  = gr5 - gr7;
	if > goto Loop;
end copyCol;

// Extracts gr0 pairs of coordinates (xy or zw) from coordAddr1 and coordAddr2,
// splits it and save to the one-dimensional array starting at ar4.
// Predefined:
// ar2 - output address of x or z coordinate
// ar4 - output address of y or w coordinate
// gr0 - number of output triangles (it is a number of A, B or C vertexes)
// Modifies: ar5, gr5, ar6, gr6, gr3, gr7, ar2, ar4
// Example for GL_TRIANGLES:
// vreg0 <- coordAddr1:xy0-delta-xy6-delta-xy12
// vreg1 <- coordAddr2:xy3-delta-xy9-delta-xy15
// vreg4 = (x0,x3,x6,x9,x12,x15) 
// vreg4 -> ar4:x0,x3,x6,x9,x12,x15
// vreg4 = (y0,y3,y6,y9,y12,y15) 
// vreg4 -> ar4:x0,x3,x6,x9,x12,x15,y0,y3,y6,y9,y12,y15 
macro extractPair(coordAddr1, coordAddr2, delta)
	own getLoop: label;
	own endLoop: label;
	own Extract: label;
	own indirectCopy: label;
	ar5 = coordAddr1;
	gr5 = ar5;
	ar6 = coordAddr2;
	gr6 = ar6;
<getLoop>
	// gr2 - max number of coordinates that can be extracted in one iteration
	// vreg0 contains 32 pairs of xy, vreg1 contains 32 pairs of xy
	// vreg2 = (1;0) * (vreg0, vreg1) contains 64 x coordinates 
	// vreg2 = (0;1) * (vreg0, vreg1) contains 64 y coordinates 
	// If number of triangles is odd then vreg0 and vreg1 contain the same
	// number of pairs, otherwise vreg1 contains one extra pair
	// Читаем в vreg0 и vreg1 одинаковое число пар 
	// (если 5 треугольников, читаем 3 пары вершин в каждый vreg).
	// Если количество треугольников нечётное, то последняя пара в vreg1 будет
	// лишней (на последней итерации).
	gr2 = 64;	
	gr0 - gr2;
	if >= goto Extract;
	gr2 = gr0;
<Extract>
	gr3 = gr2;	// gr2 - subtract from total number of triangles
				// at the end of the loop
	gr3--;		 
	gr3 >>= 1;	// Divide by 2 to get the number of pairs
	vlen = gr3;
	// Extract pair of coordinates (xy or zw) to vreg0 and vreg1
	ar5 = gr5;
	gr5 = delta;
	fpu 0 rep vlen vreg0 = [ar5++gr5];
	gr5 = ar5;

	ar6 = gr6;
	gr6 = delta;
	fpu 0 rep vlen vreg1 = [ar6++gr6];
	gr6 = ar6;
	// Extract first coordinate of the pair (x or z)
	ar5 = M10;
	ar4 = [ar3++];
	fpu 0 rep vlen vreg4 = [ar5];
	fpu 0 .matrix vreg2 = vreg4 * .trans (vreg0, vreg1);
	fpu 0 rep vlen [ar4++] = vreg2;	
	// Extract second coordinate of the pair (y or w)
	ar5 = M01;
	ar4 = [ar3++];
	fpu 0 rep vlen vreg4 = [ar5];
	fpu 0 .matrix vreg2 = vreg4 * .trans (vreg0, vreg1);
	fpu 0 rep vlen [ar4++] = vreg2;	
<endLoop>	
	gr0  = gr0 - gr2;
	if > goto getLoop;
end extractPair; 

begin ".text_demo3d"			// начало секции кода
<_repackToPrimitives_t>	// определение глобальной метки
    ar5 = ar7 - 2;
    
    push ar0,gr0;
    push ar1,gr1;
    push ar2,gr2;
    push ar3,gr3;
    push ar4,gr4;
    push ar5,gr5;
    push ar6,gr6;   // don't forget about pop
    

    ar0 = [--ar5];  // input vertexes
    ar1 = [--ar5];  // input colors
    ar2 = [--ar5];  // input texture coords
    ar3 = [--ar5];  // output vertices
    gr6 = [--ar5];  // input: count of input vertexes 
    [vertCount] = gr6;	// Used in GL_TRIANGLES
	[dstVertex] = ar3;



	gr0 = 0;
	[retVal] = gr0;
	gr6;
	if =0 goto Exit;

	// Выгрузка координат x точек A всех треугольников для mode=GL_TRIANGLES
	// Get number of triangles 
	// GL_TRIANGLES: (vertCount / 3)
	// Use IDIV32 macro or the exact copy of the macro:
	//
	gr0 = 3;				
	delayed call IDiv32;
	push gr6;
	push gr0;
	gr1 = gr7;	// gr1 = IDiv32(gr6/3)
	
	// Set parity flag
	gr3 = 1;
	gr7 = gr7 and gr3;
	[parity] = gr7;
	// Round gr1 to nearest even (+1 in case of odd, the same in case of even)
	gr1 = gr1 + 1;
	gr1 >>= 1;
	gr1 <<= 1;
	//gr0 = 3;				
	//IDIV32(gr1, gr6, gr0);	// gr1 - res, gr6 - division, gr0 - divider
							// This macro modifies ar5 and gr7
	[retVal] = gr1;
	// gr1 is a number of output triangles and a total number of pairs 
	// (xy or zw) that should be extracted for A or B or C points.

	// Get x, y, z, w, s, t of A points
	// Get xy coordinates of A points
	gr0 = gr1;	// gr0 - loop counter	
	extractPair(ar0, ar0 + 12, 24);
	// Get zw coordinates of A points
	gr0 = gr1;	// gr0 - loop counter	
	extractPair(ar0 + 2, ar0 + 14, 24);
	// Get st coordinates of A points
	gr0 = gr1;	// gr0 - loop counter	
	extractPair(ar2, ar2 + 6, 12);
		
	ar3++;
	ar3++;	// Skip dummy int

	// Get x, y, z, w, s, t of B points
	// Get xy coordinates of B points
	gr0 = gr1;	// gr0 - loop counter	
	extractPair(ar0 + 4, ar0 + 16, 24);
	// Get zw coordinates of B points
	gr0 = gr1;	// gr0 - loop counter	
	extractPair(ar0 + 6, ar0 + 18, 24);
	// Get st coordinates of B points
	gr0 = gr1;	// gr0 - loop counter	
	extractPair(ar2 + 2, ar2 + 8, 12);

	ar3++;	// Skip output color
	ar3++;	// Skip dummy int


	// Get x, y, z, w, s, t of C points
	// Get xy coordinates of C points
	gr0 = gr1;	// gr0 - loop counter	
	extractPair(ar0 + 8, ar0 + 20, 24);
	// Get zw coordinates of C points
	gr0 = gr1;	// gr0 - loop counter	
	extractPair(ar0 + 10, ar0 + 22, 24);
	// Get st coordinates of C points
	gr0 = gr1;	// gr0 - loop counter	
	extractPair(ar2 + 4, ar2 + 10, 12);
	
	ar3 = [dstVertex];
	ar3 += 6;	// Address of A_color pointer
	gr3 = [ar3];
	copyCol(ar1, gr3, 12, 4, gr1);
	ar3 += 8;	// Address of A_color pointer
	gr3 = [ar3];
	copyCol(ar1 + 4, gr3, 12, 4, gr1);
	ar3 += 8;	// Address of A_color pointer
	gr3 = [ar3];
	copyCol(ar1 + 8, gr3, 12, 4, gr1);
	
		//ar3 = [dstVertex];
		//ar3 += 6;
		//gr3 = [ar3];
		//ar3 = gr3;
		//gr3 = float(9);
		//[ar3] = gr3;
		//ar3 = [ar7 - 6];
		//[retVal] = ar3;
		//goto Exit;
	
	// Check the parity
	// !!! In case of odd output triangles extend the output with the last triangle
	// Check if number of triangles is odd
	gr4 = [parity];
	gr4;
	if =0 goto Exit; 

	ar3 = [dstVertex] with gr4 = gr1 - 1;

	.repeat 6;
	ar4 = [ar3++] with gr4 = gr4 - 1;
	gr2 = [ar4 + gr4] with gr4++; 
	[ar4 + gr4] = gr2 with gr4 = gr1 - 1;
	.endrepeat;
	ar4 = [ar3++] with gr4 = gr1 - 1;
	ar0 = ar4 with gr0 = gr4 - 1;
	gr0 <<= 2;
	gr4 <<= 2;	
	ar0 += gr0;
	ar4 += gr4;
	fpu 0 rep 2 vreg0 = [ar0++];
	fpu 0 rep 2 [ar4++] = vreg0;

	ar3 = ar3 + 1;	// Skip dummy

	.repeat 6;
	ar4 = [ar3++] with gr4 = gr4 - 1;
	gr2 = [ar4 + gr4] with gr4++;
	[ar4 + gr4] = gr2 with gr4 = gr1 - 1;
	.endrepeat;
	ar4 = [ar3++] with gr4 = gr1 - 1;
	ar0 = ar4 with gr0 = gr4 - 1;
	gr0 <<= 2;
	gr4 <<= 2;	
	ar0 += gr0;
	ar4 += gr4;
	fpu 0 rep 2 vreg0 = [ar0++];
	fpu 0 rep 2 [ar4++] = vreg0;

	ar3 = ar3 + 1;	// Skip dummy

	.repeat 6;
	ar4 = [ar3++] with gr4 = gr4 - 1;
	gr2 = [ar4 + gr4] with gr4++;
	[ar4 + gr4] = gr2 with gr4 = gr1 - 1;
	.endrepeat;
	ar4 = [ar3++] with gr4 = gr1 - 1;
	ar0 = ar4 with gr0 = gr4 - 1;
	gr0 <<= 2;
	gr4 <<= 2;	
	ar0 += gr0;
	ar4 += gr4;
	fpu 0 rep 2 vreg0 = [ar0++];
	fpu 0 rep 2 [ar4++] = vreg0;

<Exit>
	gr7 = [retVal];
    pop ar6,gr6;
    pop ar5,gr5;
    pop ar4,gr4;
    pop ar3,gr3;
    pop ar2,gr2;
    pop ar1,gr1;
    pop ar0,gr0;

    return;          // возврат из функции, возвращаемое значение хранится в gr7
end ".text_demo3d";      // признак окончания секции кода
