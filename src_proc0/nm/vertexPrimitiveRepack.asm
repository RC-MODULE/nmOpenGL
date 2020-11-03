// int vertexPrimitiveRepack(const v4nm32f *srcVertex, const v4nm32f *srcColor, nm32f *dstVertex, v4nm32f *dstColor, int mode, int vertCount);

global _vertexPrimitiveRepack: label;// объявление глобальной метки
extern ___errno: label;
//import from rtmacro.mlb;
extern IDiv32: label;

data ".data_demo3d"        //секция инициализированных данных
	M10: word[2] = (float(1), float(0)); // extract X0,X1 from (X0,Y0;X1,Y1)T vec
	M01: word[2] = (float(0), float(1)); // extract Y0,Y1 from (X0,Y0;X1,Y1)T vec
	buf: word[64];					// buf to copy odd words from vreg to mem
	vertCount: word;				// number of input vertexes (used only in GL_TRIANGLES)
	tmpAddress: word;				// used to remember address of y or w row
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
	ar2 = src + 2;		//ba input
	gr2 = srcOffset;
	ar4 = dst;			//rg output
	gr4 = dstOffset;
	ar6 = dst + 2;		//ba output
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
	fpu 0 rep vlen vreg0 = [ar0++gr0];	// copy rg components into vreg0
	fpu 0 rep vlen vreg1 = [ar2++gr2];	// copy ba components into vreg1
	fpu 0 rep vlen [ar4++gr4] = vreg0;	// copy rg components from vreg0 to dst
	fpu 0 rep vlen [ar6++gr6] = vreg1;	// copy ba components from vreg1 to dst	
	gr5  = gr5 - gr7;
	if > goto Loop;
end copyCol;

// Copies n 32-bit words from vreg src to dst (dst - odd address)
// predefined: vlen
// modifies: ar5, gr3, gr7, dst
macro copyVregAtOddAddress(src, dst, n)
	own bufToOutput: label;
	// Copy vreg to buf
	ar5 = buf;
	fpu 0 rep vlen [ar5++] = src;
	// Copy buf to output
	gr3 = n;	// Number of words to copy
	ar5 = buf;
<bufToOutput>	
	gr7 = [ar5++];
	[dst++] = gr7;
	gr3 = gr3 - 1;
	if > goto bufToOutput;
end copyVregAtOddAddress;

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
	[tmpAddress] = ar4;	// used to copy y0 to ar4 at the end of macro
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
	fpu 0 rep vlen vreg4 = [ar5];
	fpu 0 .matrix vreg2 = vreg4 * .trans (vreg0, vreg1);
	fpu 0 rep vlen [ar2++] = vreg2;	
	// Extract second coordinate of the pair (y or w)
	ar5 = M01;
	fpu 0 rep vlen vreg4 = [ar5];
	fpu 0 .matrix vreg2 = vreg4 * .trans (vreg0, vreg1);
	fpu 0 rep vlen [ar4++] = vreg2;	
<endLoop>	
	gr0  = gr0 - gr2;
	if > goto getLoop;
	// Copy Y_0 or W_0 coordinate of A, B or C vertex of the first triangle
	// to the ar4, because it can be overwritten by X or Z coordinate in case
	// of the odd number of triangles:
	// ar4 = [ar0 + 1];
	ar5 = coordAddr1; 
	ar5++;
	gr5 = [ar5];
	ar5 = [tmpAddress];
	[ar5] = gr5;
end extractPair; 

begin ".text_demo3d"			// начало секции кода
<_vertexPrimitiveRepack>	// определение глобальной метки
    ar5 = ar7 - 2;
    
    push ar0,gr0;
    push ar1,gr1;
    push ar2,gr2;
    push ar3,gr3;
    push ar4,gr4;
    push ar6,gr6;   // don't forget about pop
    
    ar0 = [--ar5];  // input vertexes
    ar1 = [--ar5];  // input colors
    ar2 = [--ar5];  // output vertexes
    ar3 = [--ar5];  // output colors
    gr4 = [--ar5];  // intput: mode
    gr6 = [--ar5];  // intput: count of input vertexes 
    [vertCount] = gr6;	// Used in GL_TRIANGLES
	[dstVertex] = ar2;

	gr0 = 0;
	[retVal] = gr0;
	gr6;
	if =0 goto Exit;

	gr0 = 4;	//NMGL_TRIANGLES 4 (nmgldef.h)
	gr4 - gr0;	
	if =0 goto GL_TRIANGLES;

	gr0 = 5;	//NMGL_TRIANGLE_STRIP 5 (nmgldef.h)
	gr4 - gr0;	
	if =0 goto GL_TRIANGLE_STRIP;

	gr0 = 6;	//NMGL_TRIANGLE_FAN 6 (nmgldef.h)
	gr4 - gr0;	
	if =0 goto GL_TRIANGLE_FAN;
	goto Exit;

<GL_TRIANGLES>
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
	// gr1 is a number of output triangles and a total number of pairs (xy or zw) 
	// that should be extracted for A or B or C points.
	// Get xy coordinates of A points
	gr0 = gr1;	// gr0 - loop counter	
	gr2 = gr1;
	ar4 = ar2 + gr2;	// Output address of Y coordinates
	extractPair(ar0, ar0 + 12, 24);
	// Get zw coordinates of A points
	gr0 = gr1;	// gr0 - loop counter	
	ar2 = ar4 with gr2 = gr1;
	ar4 = ar2 + gr2;	// Start address of W coordinates
	extractPair(ar0 + 2, ar0 + 14, 24);
	// Get xy coordinates of B points
	gr0 = gr1;	// gr0 - loop counter	
	ar2 = ar4 with gr2 = gr1;
	ar4 = ar2 + gr2;	// Start address of Y coordinates
	extractPair(ar0 + 4, ar0 + 16, 24);
	// Get zw coordinates of B points
	gr0 = gr1;	// gr0 - loop counter	
	ar2 = ar4 with gr2 = gr1;
	ar4 = ar2 + gr2;	// Start address of W coordinates
	extractPair(ar0 + 6, ar0 + 18, 24);
	// Get xy coordinates of C points
	gr0 = gr1;	// gr0 - loop counter	
	ar2 = ar4 with gr2 = gr1;
	ar4 = ar2 + gr2;	// Start address of Y coordinates
	extractPair(ar0 + 8, ar0 + 20, 24);
	// Get zw coordinates of C points
	gr0 = gr1;	// gr0 - loop counter	
	ar2 = ar4 with gr2 = gr1;
	ar4 = ar2 + gr2;	// Start address of W coordinates
	extractPair(ar0 + 10, ar0 + 22, 24);

	// Extract colors
	// In case of GL_TRIANGLES the number of output colors is equal to the
	// number of input colors (and vertexes)
	gr6 = [vertCount]; 
	gr0 = gr6 << 1; 	// total number of pairs rg and ba for gr1 vertexes
<colLoop>
	gr2 = 32;	
	gr0 - gr2;
	if >= goto CopyColor;
	gr2 = gr0;
<CopyColor>
	gr3 = gr2;
	gr3--;
	vlen = gr3;
	fpu 0 rep vlen vreg0 = [ar1++];	// copy from input colors into vreg
	fpu 0 rep vlen [ar3++] = vreg0; // copy from vreg into output colors	
	[dstColorEnd] = ar3;
	gr0  = gr0 - gr2;
	if > goto colLoop;
	goto checkParity;

<GL_TRIANGLE_STRIP>
	// Выгрузка координат x точек A всех треугольников для mode=GL_TRIANGLE_STRIP
	// Get number of triangles 
	// GL_TRIANGLE_STRIP: (vertCount - 2)
	gr0 = 2;				
	gr1 = gr6 - gr0;	
	
	// Set parity flag
	gr7 = gr1;
	gr3 = 1;
	gr7 = gr7 and gr3;
	[parity] = gr7;
	// Round gr1 to nearest even (+1 in case of odd, the same in case of even)
	gr1 = gr1 + 1;
	gr1 >>= 1;
	gr1 <<= 1;
	[retVal] = gr1;
	// Get xy coordinates of A points
	gr0 = gr1;	// gr0 - loop counter	
	gr2 = gr1;
	ar4 = ar2 + gr2;	// Output address of Y coordinates
	extractPair(ar0, ar0 + 8, 8);
	// Get zw coordinates of A points
	gr0 = gr1;	// gr0 - loop counter	
	ar2 = ar4 with gr2 = gr1;
	ar4 = ar2 + gr2;	// Start address of W coordinates
	extractPair(ar0 + 2, ar0 + 10, 8);
	// Get xy coordinates of B points
	gr0 = gr1;	// gr0 - loop counter	
	ar2 = ar4 with gr2 = gr1;
	ar4 = ar2 + gr2;	// Start address of Y coordinates
	extractPair(ar0 + 4, ar0 + 4, 8);
	// Get zw coordinates of B points
	gr0 = gr1;	// gr0 - loop counter	
	ar2 = ar4 with gr2 = gr1;
	ar4 = ar2 + gr2;	// Start address of W coordinates
	extractPair(ar0 + 6, ar0 + 6, 8);
	// Get xy coordinates of C points
	gr0 = gr1;	// gr0 - loop counter	
	ar2 = ar4 with gr2 = gr1;
	ar4 = ar2 + gr2;	// Start address of Y coordinates
	extractPair(ar0 + 8, ar0 + 12, 8);
	// Get zw coordinates of C points
	gr0 = gr1;	// gr0 - loop counter	
	ar2 = ar4 with gr2 = gr1;
	ar4 = ar2 + gr2;	// Start address of W coordinates
	extractPair(ar0 + 10, ar0 + 14, 8);
	
	// Copy colors of A vertexes
	gr0 = 1;
	gr2 = gr1 and gr0;	//gr2 = gr1 & 1 
	gr0 = gr1 >> 1;
	gr2 += gr0;			//gr2 = gr0/2 + gr1 & 1
	copyCol(ar1, ar3, 8, 24, gr2);
	//////////////////// Save dstColorEnd 
	ar4 -= 12;
	[dstColorEnd] = ar4;
	////////////////////
	gr0 = 1;
	gr2 = gr1 and gr0;
	gr0 = gr1 >> 1;
	gr2 += gr0;
	copyCol(ar1 + 8, ar3 + 12, 8, 24, gr0);
	// Copy colors of B vertexes
	gr0 = 1;
	gr2 = gr1 and gr0;
	gr0 = gr1 >> 1;
	gr2 += gr0;
	copyCol(ar1 + 4, ar3 + 4, 8, 24, gr2);
	gr0 = 1;
	gr2 = gr1 and gr0;
	gr0 = gr1 >> 1;
	gr2 += gr0;
	copyCol(ar1 + 4, ar3 + 16, 8, 24, gr0);
	// Copy colors of C vertexes
	copyCol(ar1 + 8, ar3 + 8, 4, 12, gr1);
	goto checkParity;
	//goto Exit;

<GL_TRIANGLE_FAN>
	// Выгрузка координат x точек A всех треугольников для mode=GL_TRIANGLE_FAN
	// Get number of triangles 
	// GL_TRIANGLE_FAN: (vertCount - 2)
	gr0 = 2;				
	gr1 = gr6 - gr0;	
	// Set parity flag
	gr7 = gr1;
	gr3 = 1;
	gr7 = gr7 and gr3;
	[parity] = gr7;
	// Round gr1 to nearest even (+1 in case of odd, the same in case of even)
	gr1 = gr1 + 1;
	gr1 >>= 1;
	gr1 <<= 1;
	[retVal] = gr1;
	// Get xy coordinates of A points
	gr0 = gr1;	// gr0 - loop counter	
	gr2 = gr1;
	ar4 = ar2 + gr2;	// Output address of Y coordinates
	extractPair(ar0, ar0, 0);
	// Get zw coordinates of A points
	gr0 = gr1;	// gr0 - loop counter	
	ar2 = ar4 with gr2 = gr1;
	ar4 = ar2 + gr2;	// Start address of W coordinates
	extractPair(ar0 + 2, ar0 + 2, 0);
	// Get xy coordinates of B points
	gr0 = gr1;	// gr0 - loop counter	
	ar2 = ar4 with gr2 = gr1;
	ar4 = ar2 + gr2;	// Start address of Y coordinates
	extractPair(ar0 + 4, ar0 + 8, 8);
	// Get zw coordinates of B points
	gr0 = gr1;	// gr0 - loop counter	
	ar2 = ar4 with gr2 = gr1;
	ar4 = ar2 + gr2;	// Start address of W coordinates
	extractPair(ar0 + 6, ar0 + 10, 8);
	// Get xy coordinates of C points
	gr0 = gr1;	// gr0 - loop counter	
	ar2 = ar4 with gr2 = gr1;
	ar4 = ar2 + gr2;	// Start address of Y coordinates
	extractPair(ar0 + 8, ar0 + 12, 8);
	// Get zw coordinates of C points
	gr0 = gr1;	// gr0 - loop counter	
	ar2 = ar4 with gr2 = gr1;
	ar4 = ar2 + gr2;	// Start address of W coordinates
	extractPair(ar0 + 10, ar0 + 14, 8);
	
	// Extract colors
	copyCol(ar1, ar3, 0, 12, gr1);
	//////////////////// Save dstColorEnd 
	ar4 -= 12;
	[dstColorEnd] = ar4;
	////////////////////
	copyCol(ar1 + 4, ar3 + 4, 4, 12, gr1);
	copyCol(ar1 + 8, ar3 + 8, 4, 12, gr1);

<checkParity>
	// !!! In case of odd output triangles extend the output with the last triangle
	// Check if number of triangles is odd
	gr4 = [parity];
	gr4;
	if =0 goto Exit; 
	ar4 = [dstVertex] with gr4 = gr1 - 1;
	ar4 = ar4 + gr4 with gr4 = gr1; 	// last element (n) in the row
	ar2 = ar4 - 1 with gr2 = gr1;	 	// penultimate element (n - 1) in the row 

	.repeat 12;
	gr3 = [ar2++gr2];
	[ar4++gr4] = gr3;
	.endrepeat;

	ar4 = [dstColorEnd];
	ar2 = ar4;
	ar2 -= 12;

	.repeat 12;
	gr3 = [ar2++];
	[ar4++] = gr3;
	.endrepeat;

<Exit>
	gr7 = [retVal];
    pop ar6,gr6;
    pop ar4,gr4;
    pop ar3,gr3;
    pop ar2,gr2;
    pop ar1,gr1;
    pop ar0,gr0;

    return;          // возврат из функции, возвращаемое значение хранится в gr7
end ".text_demo3d";      // признак окончания секции кода
