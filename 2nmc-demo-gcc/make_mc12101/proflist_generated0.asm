//****************************************************************************************
// This is automaticly generated asm-file by profiler-gcc.lua
//****************************************************************************************
import from "nmprofiler.mlb";
begin ".text_nmprofiler"
PROFILE_BEGIN(16);

FUNCTION(__Z18nmglvsExit_mc12101v,"__Z18nmglvsExit_mc12101v");// 0x00000000200001ce [.text]
FUNCTION(__Z11synchroInitP7Synchroii,"__Z11synchroInitP7Synchroii");// 0x0000000020000202 [.text]
FUNCTION(__Z13nmglvsNm0Initv,"__Z13nmglvsNm0Initv");// 0x0000000020000276 [.text]
FUNCTION(__Z12nmglvsNm0Runv,"__Z12nmglvsNm0Runv");// 0x000000002000047c [.text]
FUNCTION(__Z16nmglvsSwapBufferv,"__Z16nmglvsSwapBufferv");// 0x0000000020000490 [.text]
FUNCTION(__Z11getDrawInfoP9TrianglesP8DrawInfoiii,"__Z11getDrawInfoP9TrianglesP8DrawInfoiii");// 0x00000000200004ec [.text]
FUNCTION(_readMask,       "_readMask       ");// 0x0000000020000b02 [.text]
FUNCTION(_nmppsMerge_32f, "_nmppsMerge_32f ");// 0x0000000020000b9a [.text]
FUNCTION(_MullMatrix_f,   "_MullMatrix_f   ");// 0x0000000020000bdc [.text]
FUNCTION(_print_32x,      "_print_32x      ");// 0x0000000020000e46 [.text]
NONCFUNC(ARShift32,       "ARShift32       ");// 0x0000000020000e86 [.text]
NONCFUNC(DAdd,            "DAdd            ");// 0x0000000020000f12 [.text]
NONCFUNC(DFrExp,          "DFrExp          ");// 0x0000000020000fd2 [.text]
NONCFUNC(DResult,         "DResult         ");// 0x0000000020001014 [.text]
NONCFUNC(DDiv,            "DDiv            ");// 0x0000000020001124 [.text]
NONCFUNC(UDiv32,          "UDiv32          ");// 0x00000000200011e8 [.text]
NONCFUNC(IDiv32,          "IDiv32          ");// 0x00000000200011fa [.text]
NONCFUNC(DMul,            "DMul            ");// 0x0000000020001240 [.text]
FUNCTION(ConvDtoF,        "ConvDtoF        ");// 0x00000000200012f2 [.text]
NONCFUNC(ConvDtoI32,      "ConvDtoI32      ");// 0x0000000020001328 [.text]
FUNCTION(FAdd,            "FAdd            ");// 0x0000000020001380 [.text]
FUNCTION(FFrExp,          "FFrExp          ");// 0x000000002000142a [.text]
FUNCTION(FResult,         "FResult         ");// 0x000000002000146a [.text]
FUNCTION(FCmp,            "FCmp            ");// 0x000000002000153e [.text]
FUNCTION(FDiv,            "FDiv            ");// 0x0000000020001582 [.text]
FUNCTION(FMul,            "FMul            ");// 0x0000000020001610 [.text]
FUNCTION(ConvFtoD,        "ConvFtoD        ");// 0x0000000020001692 [.text]
FUNCTION(ConvFtoI32,      "ConvFtoI32      ");// 0x00000000200016c6 [.text]
FUNCTION(ConvI32toF,      "ConvI32toF      ");// 0x000000002000171c [.text]
NONCFUNC(LShift32,        "LShift32        ");// 0x0000000020001732 [.text]
FUNCTION(LShift64,        "LShift64        ");// 0x00000000200017be [.text]
NONCFUNC(Mul32,           "Mul32           ");// 0x00000000200018da [.text]
NONCFUNC(RShift32,        "RShift32        ");// 0x00000000200018f4 [.text]
FUNCTION(RShift64,        "RShift64        ");// 0x0000000020001980 [.text]
NONCFUNC(DCmp,            "DCmp            ");// 0x0000000020001b24 [.text]
FUNCTION(IDiv64,          "IDiv64          ");// 0x0000000020001b9e [.text]
FUNCTION(UDiv64,          "UDiv64          ");// 0x0000000020001c04 [.text]
NONCFUNC(ConvI32toD,      "ConvI32toD      ");// 0x0000000020001c22 [.text]
NONCFUNC(UMod32,          "UMod32          ");// 0x0000000020001c3e [.text]
NONCFUNC(IMod32,          "IMod32          ");// 0x0000000020001c6a [.text]
FUNCTION(UMod64,          "UMod64          ");// 0x0000000020001ca0 [.text]
FUNCTION(IMod64,          "IMod64          ");// 0x0000000020001cdc [.text]
NONCFUNC(ConvU32toD,      "ConvU32toD      ");// 0x0000000020001d28 [.text]
FUNCTION(_printf,         "_printf         ");// 0x00000000200021a4 [.text_printf]
FUNCTION(_nmppsDiv_32f,   "_nmppsDiv_32f   ");// 0x0000000000000200 [.text_int]
FUNCTION(_nmppsExp_32f,   "_nmppsExp_32f   ");// 0x00000000000005d2 [.text_int]
FUNCTION(_nmppsLn_32f,    "_nmppsLn_32f    ");// 0x000000000000095c [.text_int]
FUNCTION(__Z7pow_32fPfS_fiS_,"__Z7pow_32fPfS_fiS_");// 0x0000000020009a6e [.text_nmgl]
FUNCTION(__Z9nmglClearj,  "__Z9nmglClearj  ");// 0x0000000020009ab0 [.text_nmgl]
FUNCTION(__Z14nmglClearColorffff,"__Z14nmglClearColorffff");// 0x0000000020009ae8 [.text_nmgl]
FUNCTION(__Z15nmglClearDepthff,"__Z15nmglClearDepthff");// 0x0000000020009b5e [.text_nmgl]
FUNCTION(__Z16nmglColorPointerijiPKv,"__Z16nmglColorPointerijiPKv");// 0x0000000020009ba6 [.text_nmgl]
FUNCTION(__Z12nmglCullFacej,"__Z12nmglCullFacej");// 0x0000000020009bf6 [.text_nmgl]
FUNCTION(__Z13nmglDepthFuncj,"__Z13nmglDepthFuncj");// 0x0000000020009c1c [.text_nmgl]
FUNCTION(__Z13nmglDepthMaskb,"__Z13nmglDepthMaskb");// 0x0000000020009c70 [.text_nmgl]
FUNCTION(__Z15nmglDepthRangefff,"__Z15nmglDepthRangefff");// 0x0000000020009ca8 [.text_nmgl]
FUNCTION(__Z11nmglDisablej,"__Z11nmglDisablej");// 0x0000000020009d62 [.text_nmgl]
FUNCTION(__Z22nmglDisableClientStatej,"__Z22nmglDisableClientStatej");// 0x0000000020009e90 [.text_nmgl]
FUNCTION(__Z14nmglDrawArraysjii,"__Z14nmglDrawArraysjii");// 0x0000000020009ece [.text_nmgl]
FUNCTION(__Z10nmglEnablej,"__Z10nmglEnablej");// 0x000000002000a822 [.text_nmgl]
FUNCTION(__Z21nmglEnableClientStatej,"__Z21nmglEnableClientStatej");// 0x000000002000a950 [.text_nmgl]
FUNCTION(__Z10nmglFinishv,"__Z10nmglFinishv");// 0x000000002000a98e [.text_nmgl]
FUNCTION(__Z13nmglFrontFacej,"__Z13nmglFrontFacej");// 0x000000002000a9ae [.text_nmgl]
FUNCTION(__Z12nmglFrustumfffffff,"__Z12nmglFrustumfffffff");// 0x000000002000a9d4 [.text_nmgl]
FUNCTION(__Z12nmglGetErrorv,"__Z12nmglGetErrorv");// 0x000000002000aaae [.text_nmgl]
FUNCTION(__Z13nmglGetFloatvjPf,"__Z13nmglGetFloatvjPf");// 0x000000002000aab6 [.text_nmgl]
FUNCTION(__Z15nmglGetIntegervjPi,"__Z15nmglGetIntegervjPi");// 0x000000002000ab48 [.text_nmgl]
FUNCTION(__Z10nmglLightfjjf,"__Z10nmglLightfjjf");// 0x000000002000ab52 [.text_nmgl]
FUNCTION(__Z11nmglLightfvjjPKf,"__Z11nmglLightfvjjPKf");// 0x000000002000ac3e [.text_nmgl]
FUNCTION(__Z16nmglLoadIdentityv,"__Z16nmglLoadIdentityv");// 0x000000002000ae1c [.text_nmgl]
FUNCTION(__Z13nmglMaterialfjjf,"__Z13nmglMaterialfjjf");// 0x000000002000ae36 [.text_nmgl]
FUNCTION(__Z14nmglMaterialfvjjPKf,"__Z14nmglMaterialfvjjPKf");// 0x000000002000ae8a [.text_nmgl]
FUNCTION(__Z14nmglMatrixModej,"__Z14nmglMatrixModej");// 0x000000002000af96 [.text_nmgl]
FUNCTION(__Z15nmglMultMatrixfPKf,"__Z15nmglMultMatrixfPKf");// 0x000000002000afd4 [.text_nmgl]
FUNCTION(__Z17nmglNormalPointerjiPKv,"__Z17nmglNormalPointerjiPKv");// 0x000000002000affc [.text_nmgl]
FUNCTION(__Z19nmglNormalPointerNMjiPKv,"__Z19nmglNormalPointerNMjiPKv");// 0x000000002000b03a [.text_nmgl]
FUNCTION(__Z10nmglOrthofffffff,"__Z10nmglOrthofffffff");// 0x000000002000b078 [.text_nmgl]
FUNCTION(__Z13nmglPopMatrixv,"__Z13nmglPopMatrixv");// 0x000000002000b148 [.text_nmgl]
FUNCTION(__Z14nmglPushMatrixv,"__Z14nmglPushMatrixv");// 0x000000002000b16c [.text_nmgl]
FUNCTION(__Z11nmglRotatefffff,"__Z11nmglRotatefffff");// 0x000000002000b1c0 [.text_nmgl]
FUNCTION(__Z10nmglScaleffff,"__Z10nmglScaleffff");// 0x000000002000b358 [.text_nmgl]
FUNCTION(__Z14nmglTranslateffff,"__Z14nmglTranslateffff");// 0x000000002000b3a8 [.text_nmgl]
FUNCTION(__Z17nmglVertexPointerijiPKv,"__Z17nmglVertexPointerijiPKv");// 0x000000002000b410 [.text_nmgl]
FUNCTION(__Z12nmglViewportiiii,"__Z12nmglViewportiiii");// 0x000000002000b45a [.text_nmgl]
FUNCTION(_remap_32u,      "_remap_32u      ");// 0x0000000000000d24 [.text_nmplv]
FUNCTION(_nmppsCmpGtC_v2nm32f,"_nmppsCmpGtC_v2nm32f");// 0x0000000000000d46 [.text_nmplv]
FUNCTION(_nmppsCmpLtC_v2nm32f,"_nmppsCmpLtC_v2nm32f");// 0x0000000000000d76 [.text_nmplv]
FUNCTION(_nmppsAdd_32f,   "_nmppsAdd_32f   ");// 0x0000000000000da6 [.text_nmplv]
FUNCTION(_nmppsMul_Mul_Sub_32f,"_nmppsMul_Mul_Sub_32f");// 0x0000000000000dce [.text_nmplv]
FUNCTION(_nmppsMulC_32f,  "_nmppsMulC_32f  ");// 0x0000000000000e08 [.text_nmplv]
FUNCTION(_nmppsMulC_AddC_32f,"_nmppsMulC_AddC_32f");// 0x0000000000000e3a [.text_nmplv]
FUNCTION(_nmppsSub_32f,   "_nmppsSub_32f   ");// 0x0000000000000e72 [.text_nmplv]
FUNCTION(_nmppsSubC_32f,  "_nmppsSubC_32f  ");// 0x0000000000000e9a [.text_nmplv]
FUNCTION(_nmppsSubCRev_32f,"_nmppsSubCRev_32f");// 0x0000000000000ecc [.text_nmplv]
FUNCTION(_nmppsMulC_AddV_32f,"_nmppsMulC_AddV_32f");// 0x0000000000000f04 [.text_nmplv]
FUNCTION(_nmppsMulC_AddV_AddC_32f,"_nmppsMulC_AddV_AddC_32f");// 0x0000000000000f32 [.text_nmplv]
FUNCTION(__Z12addInstrNMC1P13HalRingBufferiiiiiii,"__Z12addInstrNMC1P13HalRingBufferiiiiiii");// 0x0000000000000f6a [.text_demo3d]
FUNCTION(__Z21cullFaceSortTrianglesP9Trianglesi,"__Z21cullFaceSortTrianglesP9Trianglesi");// 0x0000000000000fdc [.text_demo3d]
FUNCTION(__Z13fillPolygonsLP8PolygonsP5Linesiii,"__Z13fillPolygonsLP8PolygonsP5Linesiii");// 0x00000000000012da [.text_demo3d]
FUNCTION(__Z13fillPolygonsTP8PolygonsP9Trianglesiii,"__Z13fillPolygonsTP8PolygonsP9Trianglesiii");// 0x00000000000012e0 [.text_demo3d]
FUNCTION(__Z5lightP9s_v4nm32fS0_i,"__Z5lightP9s_v4nm32fS0_i");// 0x00000000000018fa [.text_demo3d]
FUNCTION(__Z17pushToTriangles_tPKfS0_S0_PK9s_v4nm32fR9Trianglesi,"__Z17pushToTriangles_tPKfS0_S0_PK9s_v4nm32fR9Trianglesi");// 0x0000000000001c04 [.text_demo3d]
FUNCTION(__Z10rasterizeLP5Linesi,"__Z10rasterizeLP5Linesi");// 0x0000000000001d4e [.text_demo3d]
FUNCTION(__Z12waitPolygonsv,"__Z12waitPolygonsv");// 0x0000000000001d54 [.text_demo3d]
FUNCTION(__Z10rasterizeTPK9TrianglesPK11SegmentMaski,"__Z10rasterizeTPK9TrianglesPK11SegmentMaski");// 0x0000000000001d8a [.text_demo3d]
FUNCTION(__Z21reverseMatrix3x3in4x4P11s_mat4nm32fS0_,"__Z21reverseMatrix3x3in4x4P11s_mat4nm32fS0_");// 0x0000000000002016 [.text_demo3d]
FUNCTION(__Z14setSegmentMaskP9s_v2nm32fS0_P11SegmentMaski,"__Z14setSegmentMaskP9s_v2nm32fS0_P11SegmentMaski");// 0x000000000000223c [.text_demo3d]
FUNCTION(_absIfNegElse0_32f,"_absIfNegElse0_32f");// 0x0000000000002378 [.text_demo3d]
FUNCTION(_addC_v4nm32f,   "_addC_v4nm32f   ");// 0x00000000000023a2 [.text_demo3d]
FUNCTION(_clamp_32f,      "_clamp_32f      ");// 0x00000000000023d8 [.text_demo3d]
FUNCTION(_cnv32f_v2v4,    "_cnv32f_v2v4    ");// 0x0000000000002420 [.text_demo3d]
FUNCTION(_cnv32f_v3v4,    "_cnv32f_v3v4    ");// 0x0000000000002452 [.text_demo3d]
FUNCTION(_copyArraysByIndices,"_copyArraysByIndices");// 0x00000000000024c2 [.text_demo3d]
FUNCTION(_copyOddOffset,  "_copyOddOffset  ");// 0x00000000000024ea [.text_demo3d]
FUNCTION(_dotC_gt0_v4nm32f,"_dotC_gt0_v4nm32f");// 0x000000000000252a [.text_demo3d]
FUNCTION(_dotMulC_Add_v4nm32f,"_dotMulC_Add_v4nm32f");// 0x0000000000002582 [.text_demo3d]
FUNCTION(_dotMulC_AddC_v4nm32f,"_dotMulC_AddC_v4nm32f");// 0x00000000000025c2 [.text_demo3d]
FUNCTION(_dotV_gt0_v4nm32f,"_dotV_gt0_v4nm32f");// 0x0000000000002600 [.text_demo3d]
FUNCTION(_dotV_v4nm32f,   "_dotV_v4nm32f   ");// 0x0000000000002658 [.text_demo3d]
FUNCTION(_findMinMax2,    "_findMinMax2    ");// 0x00000000000026aa [.text_demo3d]
FUNCTION(_findMinMax3,    "_findMinMax3    ");// 0x00000000000026e6 [.text_demo3d]
FUNCTION(_loadIdentify,   "_loadIdentify   ");// 0x0000000000002736 [.text_demo3d]
FUNCTION(_maskEq0,        "_maskEq0        ");// 0x0000000000002744 [.text_demo3d]
FUNCTION(_maskSelectionLight_RGBA_BGRA,"_maskSelectionLight_RGBA_BGRA");// 0x0000000000002780 [.text_demo3d]
FUNCTION(_meanToInt2,     "_meanToInt2     ");// 0x00000000000027da [.text_demo3d]
FUNCTION(_meanToInt3,     "_meanToInt3     ");// 0x000000000000280c [.text_demo3d]
FUNCTION(_mul_mat4nm32f_v4nm32f,"_mul_mat4nm32f_v4nm32f");// 0x0000000000002844 [.text_demo3d]
FUNCTION(_mul_v4nm32f_mat4nm32f,"_mul_v4nm32f_mat4nm32f");// 0x0000000000002886 [.text_demo3d]
FUNCTION(_mulC_v4nm32f,   "_mulC_v4nm32f   ");// 0x00000000000028c8 [.text_demo3d]
FUNCTION(_replaceEq0_32f, "_replaceEq0_32f ");// 0x0000000000002900 [.text_demo3d]
FUNCTION(_set_v4nm32f,    "_set_v4nm32f    ");// 0x000000000000292c [.text_demo3d]
FUNCTION(_sortByY2,       "_sortByY2       ");// 0x0000000000002956 [.text_demo3d]
FUNCTION(_sortByY3,       "_sortByY3       ");// 0x000000000000298c [.text_demo3d]
FUNCTION(_split_v2nm32f,  "_split_v2nm32f  ");// 0x00000000000029ea [.text_demo3d]
FUNCTION(_split_v4nm32f,  "_split_v4nm32f  ");// 0x0000000000002a2a [.text_demo3d]
FUNCTION(_subCRev_v4nm32f,"_subCRev_v4nm32f");// 0x0000000000002a96 [.text_demo3d]
FUNCTION(_ternaryLt0_AddC_AddC_32f,"_ternaryLt0_AddC_AddC_32f");// 0x0000000000002acc [.text_demo3d]

PROFILE_END();
end ".text_nmprofiler";
