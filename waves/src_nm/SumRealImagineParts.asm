global _SumReImParts: label;

data ".data"
	ZeroOne: word[4] = (float(1), float(0), float(0), float(1));
end ".data";

begin "text"
<_SumReImParts>
	ar5 = ar7 - 2;
	push ar3, gr3;
	push ar2, gr2;
	push ar1, gr1;
	push ar0, gr0;

	ar1 = [--ar5]; 					// input x
	ar2 = [--ar5]; 					// output X
	gr0 = [--ar5];					// size N

	ar3 = ZeroOne;

	gr1 = 4;

	fpu 0 rep 1 vreg1 = [ar3++];	// ZeroOne
	fpu 1 rep 1 vreg1 = [ar3++];	// ZeroOne
	gr3 = gr1;
	ar3 = ar1 + 2;
	gr7 = gr0 >> 7; 				// count of the iterations
	//if =0 goto less64_RealAddImg_32f;

<Loop_RealAddImg_32f>
	fpu 0 rep 32 vreg0 = [ar1++gr1];
	fpu 0 .packer = vreg0 with .float .in_low <= .float .in_high;
	fpu 0 rep 32 vreg3 = .packer;
	fpu 0 .float vreg2 = vreg0 * .retrive(vreg1) + vreg3;
	fpu 2 vreg0 = fpu 0 vreg2;

	fpu 1 rep 32 vreg0 = [ar3++gr3];
	fpu 1 .packer = vreg0 with .float .in_high <= .float .in_low;
	fpu 1 rep 32 vreg3 = .packer;
	fpu 1 .float vreg2 = vreg0 * .retrive(vreg1) + vreg3;
	fpu 2 vreg1 = fpu 1 vreg2;
	fpu 2 .float vreg2 = vreg0 + vreg1;
	//fpu 2 .float vreg2 = /vreg2/;
	gr7--;
	if > delayed goto Loop_RealAddImg_32f;
		fpu 2 .float vreg2 = /vreg2/;
		fpu 2 rep 32 [ar2++] = vreg2;
		//fpu 2 .packer = vreg2 with .fixed_32 <= .float;
		//fpu rep 32 [ar2++] = .packer;
// 	gr2 = gr0 << 26;
// 	gr2 = gr0 >> 26;
// 	if =0 goto exit_RealAddImg_32f;

// <less64_RealAddImg_32f>
// 	gr2 = gr0 >> 1;
// 	gr2--;
// 	vlen = gr2;
// 	fpu 0 rep vlen vreg0 = [ar1++gr1];
// 	fpu 0 .packer = vreg0 with .float .in_low <= .float .in_high;
// 	fpu 0 rep vlen vreg3 = .packer;
// 	fpu 0 .float vreg2 = vreg0 * .retrive(vreg1) + vreg3;
// 	fpu 2 vreg0 = fpu 0 vreg2;

// 	fpu 1 rep vlen vreg0 = [ar3++gr3];
// 	fpu 1 .packer = vreg0 with .float .in_high <= .float .in_low;
// 	fpu 1 rep vlen vreg3 = .packer;
// 	fpu 1 .float vreg2 = vreg0 * .retrive(vreg1) + vreg3;
// 	fpu 2 vreg1 = fpu 1 vreg2;
// 	fpu 2 .float vreg2 = vreg0 + vreg1;
// 	fpu 2 rep vlen [ar2++] = \vreg2\;

// 	gr0 = gr0 >> 31;
// 	if =0 goto exit_RealAddImg_32f; // if N is even then go to exit

// <RealAddImg_32f_1elem>
// 	fpu 0 rep 1 vreg0 = [ar1++gr1];
// 	fpu 0 .packer = vreg0 with .float .in_low <= .float .in_high;
// 	fpu 0 rep 1 vreg3 = .packer;
// 	fpu 0 .float vreg2 = vreg0 * .retrive(vreg1) + vreg3;
// 	fpu 2 vreg0 = fpu 0 vreg2;

// 	fpu 1 rep 1 vreg0 = [ar3++gr3];
// 	fpu 1 .packer = vreg0 with .float .in_high <= .float .in_low;
// 	fpu 1 rep 1 vreg3 = .packer;
// 	fpu 1 .float vreg2 = vreg0 * .retrive(vreg1) + vreg3;
// 	fpu 2 vreg1 = fpu 1 vreg2;
// 	fpu 2 .float vreg2 = vreg0 + vreg1;
// 	fpu 2 rep 1 [ar2++] = \vreg2\;

<exit_RealAddImg_32f>
	pop ar0, gr0;
	pop ar1, gr1;
	pop ar2, gr2;
	pop ar3, gr3;
	return;
end "text";
