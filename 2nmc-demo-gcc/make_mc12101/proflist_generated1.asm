//****************************************************************************************
// This is automaticly generated asm-file by profiler-gcc.lua
//****************************************************************************************
import from "nmprofiler.mlb";
begin ".text_nmprofiler"
PROFILE_BEGIN(16);

/* FUNCTION(__ZN11DepthBuffer6updateEv,"__ZN11DepthBuffer6updateEv");// 0x00000000270001e6 [.text]
FUNCTION(_print_32x,      "_print_32x      ");// 0x00000000270002de [.text]
NONCFUNC(Mul32,           "Mul32           ");// 0x00000000270002fc [.text]
NONCFUNC(ARShift32,       "ARShift32       ");// 0x000000002700039e [.text]
NONCFUNC(DAdd,            "DAdd            ");// 0x000000002700042a [.text]
NONCFUNC(DFrExp,          "DFrExp          ");// 0x00000000270004ea [.text]
NONCFUNC(DResult,         "DResult         ");// 0x000000002700052c [.text]
NONCFUNC(DCmp,            "DCmp            ");// 0x000000002700063c [.text]
NONCFUNC(DDiv,            "DDiv            ");// 0x000000002700068e [.text]
NONCFUNC(UDiv32,          "UDiv32          ");// 0x0000000027000752 [.text]
NONCFUNC(IDiv32,          "IDiv32          ");// 0x0000000027000764 [.text]
FUNCTION(IDiv64,          "IDiv64          ");// 0x00000000270007d2 [.text]
FUNCTION(UDiv64,          "UDiv64          ");// 0x0000000027000838 [.text]
NONCFUNC(DMul,            "DMul            ");// 0x0000000027000856 [.text]
NONCFUNC(ConvDtoI32,      "ConvDtoI32      ");// 0x0000000027000908 [.text]
NONCFUNC(ConvI32toD,      "ConvI32toD      ");// 0x0000000027000968 [.text]
NONCFUNC(LShift32,        "LShift32        ");// 0x0000000027000984 [.text]
FUNCTION(LShift64,        "LShift64        ");// 0x0000000027000a10 [.text]
NONCFUNC(UMod32,          "UMod32          ");// 0x0000000027000b2c [.text]
NONCFUNC(IMod32,          "IMod32          ");// 0x0000000027000b58 [.text]
FUNCTION(UMod64,          "UMod64          ");// 0x0000000027000b8e [.text]
FUNCTION(IMod64,          "IMod64          ");// 0x0000000027000bca [.text]
NONCFUNC(RShift32,        "RShift32        ");// 0x0000000027000c16 [.text]
FUNCTION(RShift64,        "RShift64        ");// 0x0000000027000ca2 [.text] */
FUNCTION(__Z9drawLinesP16NMGL_Context_NM1,"__Z9drawLinesP16NMGL_Context_NM1");// 0x0000000000000200 [.text_demo3d]
FUNCTION(__Z13drawTrianglesP16NMGL_Context_NM1,"__Z13drawTrianglesP16NMGL_Context_NM1");// 0x000000000000045e [.text_demo3d]
FUNCTION(__Z13getAddrPtrnsLP16NMGL_Context_NM1P8PatternsP8Polygons,"__Z13getAddrPtrnsLP16NMGL_Context_NM1P8PatternsP8Polygons");// 0x00000000000006a6 [.text_demo3d]
FUNCTION(__Z13getAddrPtrnsTP16NMGL_Context_NM1P8PatternsP8Polygons,"__Z13getAddrPtrnsTP16NMGL_Context_NM1P8PatternsP8Polygons");// 0x00000000000006ae [.text_demo3d]
FUNCTION(__Z8cbUpdatev,   "__Z8cbUpdatev   ");// 0x0000000000000abe [.text_demo3d]
FUNCTION(__Z11msdAddImageP11ImageBufferP12ImageSegmentii,"__Z11msdAddImageP11ImageBufferP12ImageSegmentii");// 0x0000000000000b78 [.text_demo3d]
FUNCTION(__Z6msdAddPKvPvi,"__Z6msdAddPKvPvi");// 0x0000000000000c08 [.text_demo3d]
FUNCTION(__Z8msdAdd2DPKvPvjjjj,"__Z8msdAdd2DPKvPvjjjj");// 0x0000000000000c42 [.text_demo3d]
FUNCTION(__Z16msdGetStatusCopyi,"__Z16msdGetStatusCopyi");// 0x0000000000000c8c [.text_demo3d]
FUNCTION(__Z12msdStartCopyv,"__Z12msdStartCopyv");// 0x0000000000000ca0 [.text_demo3d]
FUNCTION(__Z13msdSimpleCopyPKvPvi,"__Z13msdSimpleCopyPKvPvi");// 0x0000000000000d1a [.text_demo3d]
FUNCTION(__Z10msdWaitDmav,"__Z10msdWaitDmav");// 0x0000000000000d4e [.text_demo3d]
FUNCTION(__Z14selectPatternsPiS_S_S_S_PS_iS_,"__Z14selectPatternsPiS_S_S_S_PS_iS_");// 0x0000000000000d74 [.text_demo3d]
FUNCTION(_absIfNegElse0,  "_absIfNegElse0  ");// 0x0000000000000df0 [.text_demo3d]
FUNCTION(_copyPacket_32u, "_copyPacket_32u ");// 0x0000000000000e8e [.text_demo3d]
//FUNCTION(_copyPacket_32s, "_copyPacket_32s ");// 0x0000000000000e8e [.text_demo3d]
FUNCTION(_depthTest,      "_depthTest      ");// 0x0000000000000f2c [.text_demo3d]
FUNCTION(_mAndVxN_32u,    "_mAndVxN_32u    ");// 0x0000000000000f5e [.text_demo3d]
FUNCTION(_mergePtrnsAddr2,"_mergePtrnsAddr2");// 0x0000000000001008 [.text_demo3d]
FUNCTION(_mergePtrnsAddr3,"_mergePtrnsAddr3");// 0x00000000000010fa [.text_demo3d]
FUNCTION(_mMaskVxN_32s,   "_mMaskVxN_32s   ");// 0x000000000000122e [.text_demo3d]
FUNCTION(_mMulCVxN_2s_v4nm8sExt,"_mMulCVxN_2s_v4nm8sExt");// 0x000000000000167c [.text_demo3d]
FUNCTION(_mMulCVxN_2s32sExt,"_mMulCVxN_2s32sExt");// 0x0000000000001694 [.text_demo3d]
FUNCTION(_selectColorChannel,"_selectColorChannel");// 0x0000000000001af4 [.text_demo3d]
FUNCTION(_selectPaintSide,"_selectPaintSide");// 0x0000000000001c36 [.text_demo3d]
FUNCTION(__Z13nmglvsNm1InitR16NMGL_Context_NM1,"__Z13nmglvsNm1InitR16NMGL_Context_NM1");// 0x0000000027008fc0 [.text_nmglvs]
FUNCTION(__Z12nmglvsNm1RunR16NMGL_Context_NM1,"__Z12nmglvsNm1RunR16NMGL_Context_NM1");// 0x000000002700914e [.text_nmglvs]
FUNCTION(__Z13nmglvsNm1StepR16NMGL_Context_NM1,"__Z13nmglvsNm1StepR16NMGL_Context_NM1");// 0x0000000027009160 [.text_nmglvs]
/* FUNCTION(_depthTestCore_A_r,"_depthTestCore_A_r");// 0x000000002700a7ec [.text_demo3dExt]
FUNCTION(_depthTestCore_A_rw,"_depthTestCore_A_rw");// 0x000000002700ac1a [.text_demo3dExt]
FUNCTION(_depthTestCore_Gt_r,"_depthTestCore_Gt_r");// 0x000000002700b048 [.text_demo3dExt]
FUNCTION(_depthTestCore_Gt_rw,"_depthTestCore_Gt_rw");// 0x000000002700b476 [.text_demo3dExt]
FUNCTION(_depthTestCore_Lt_r,"_depthTestCore_Lt_r");// 0x000000002700b8a4 [.text_demo3dExt]
FUNCTION(_depthTestCore_Lt_rw,"_depthTestCore_Lt_rw");// 0x000000002700bcd2 [.text_demo3dExt]
FUNCTION(_depthTestCore_N_r,"_depthTestCore_N_r");// 0x000000002700c100 [.text_demo3dExt]
FUNCTION(_depthTestCore_N_rw,"_depthTestCore_N_rw");// 0x000000002700c52e [.text_demo3dExt] */
FUNCTION(_nmppsAddC_32s,  "_nmppsAddC_32s  ");// 0x0000000000004b1e [.text_nmplv]
FUNCTION(_nmppsAdd_32s,   "_nmppsAdd_32s   ");// 0x0000000000004b40 [.text_nmplv]
FUNCTION(_nmppsMulC_AddC32s,"_nmppsMulC_AddC32s");// 0x0000000000004b5a [.text_nmplv]
//FUNCTION(_nmppsMulC_AddC_32s,"_nmppsMulC_AddC_32s");// 0x0000000000004b5a [.text_nmplv]
FUNCTION(_nmppsMulC_AddV_AddC_32s,"_nmppsMulC_AddV_AddC_32s");// 0x0000000000004b82 [.text_nmplv]
//FUNCTION(_nmppsMulC_AddV_AddC32s,"_nmppsMulC_AddV_AddC32s");// 0x0000000000004b82 [.text_nmplv]
FUNCTION(_nmppsSub_32s,   "_nmppsSub_32s   ");// 0x0000000000004bb0 [.text_nmplv]
FUNCTION(_nmppsAnd_32u,   "_nmppsAnd_32u   ");// 0x0000000000004bca [.text_nmplv]
FUNCTION(_nmppsAndC_32u,  "_nmppsAndC_32u  ");// 0x0000000000004be0 [.text_nmplv]
FUNCTION(_nmppsOr_32u,    "_nmppsOr_32u    ");// 0x0000000000004bf8 [.text_nmplv]
FUNCTION(_nmppsAnd4V_64u, "_nmppsAnd4V_64u ");// 0x0000000000004c0e [.text_nmplv]
FUNCTION(_nmppsSet_32s,   "_nmppsSet_32s   ");// 0x0000000000004c2a [.text_nmplv]
FUNCTION(_nmppsConvert_32s8s,"_nmppsConvert_32s8s");// 0x0000000000004c3e [.text_nmplv]
FUNCTION(_nmppsCopy_32s,  "_nmppsCopy_32s  ");// 0x0000000000004c6e [.text_nmplv]
FUNCTION(_nmppsRemap_32u, "_nmppsRemap_32u ");// 0x0000000000004c7e [.text_nmplv]
FUNCTION(_nmppmCopy_32u,  "_nmppmCopy_32u  ");// 0x0000000000004ca2 [.text_nmplm]
//FUNCTION(_nmppmCopy_32s,  "_nmppmCopy_32s  ");// 0x0000000000004ca2 [.text_nmplm]
FUNCTION(mtr_CpyCore_nm64,"mtr_CpyCore_nm64");// 0x0000000000004cc0 [.text_nmplm]
FUNCTION(_totalSum,"_totalSum");// 0x0000000000004cc0 [.text_nmplm]

PROFILE_END();
end ".text_nmprofiler";
