/*!
 *  \file remap_32u.asm
 *  \author Zhilenkov Ivan
 */


/////////////////////////////////////////////////////////////////////////////////
//! \fn void _remap_32u(nm32u* pSrcVec, nm32u* pDstVec, nm32s* pRemapTable, int nSize);
//!
//! \perfinclude remap_32u.html

begin ".text_nmplv"
global _remap_32u:label;
<_remap_32u>
    ar5 = ar7 - 2;
	push ar0, gr0;
    push ar1, gr1;
	push ar2, gr2;
	push ar3, gr3;
	push ar4, gr4;
	push ar5, gr5;
	push ar6, gr6;

	gr4 = [--ar5];	//	nm32u*	pSrcVec,		
	ar6 = [--ar5];	//	nm32u*	pDstVec,
	ar1 = [--ar5];	//	nm32s*			RemapTable
	gr7 = [--ar5];	//	int				nSize,

	gr7 >>=1;
	with gr3 = gr4;	//	src_base
	with gr7--;
	<next_Permutation>
		ar4 = [ar1++];		// src_disp1  =RemapTable[i]	
		ar3 = [ar1++];		// src_disp2  =RemapTable[i+1]
		gr1 = [ar4+=gr4];	// src_val1   =pSrcVec[src_base+src_disp1]
		gr5 = [ar3+=gr3];	// src_val2   =pSrcVec[src_base+src_disp2]
    if > delayed goto next_Permutation with gr7--;
		[ar6++] = gr1;		// pDstVec[i]  =src_val1
		[ar6++] = gr5;		// pDstVec[i+1]=src_val2


	pop ar6, gr6;
	pop ar5, gr5;
	pop ar4, gr4;
	pop ar3, gr3;
	pop ar2, gr2;
    pop ar1, gr1;
	pop ar0, gr0;
    return;
end ".text_nmplv";
