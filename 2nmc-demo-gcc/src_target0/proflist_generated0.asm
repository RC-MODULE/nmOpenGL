//****************************************************************************************
// This is automaticly generated asm-file by profiler-gcc.py
//****************************************************************************************
import from "nmprofiler.mlb";
begin ".text_nmprofiler"
PROFILE_BEGIN( 16 );

	FUNCTION( hw_init_hook ,      "hw_init_hook    ");//  0000000020000016  [ .text ]
	FUNCTION( __Z18nmglvsExit_mc12101v ,                         "nmglvsExit_mc121");//  000000002000020c  [ .text ]
	FUNCTION( __Z12nmglvsNm0Runv ,     "nmglvsNm0Runv   ");//  0000000020000330  [ .text ]
	FUNCTION( __Z16nmglvsSwapBufferv ,                           "nmglvsSwapBuffer");//  0000000020000370  [ .text ]
	FUNCTION( __Z15updatePolygonsPP11PolygonsOldP6Pointsiii ,    "updatePolygonsPP");//  0000000020000504  [ .text ]
	FUNCTION( __ZNK8Triangle8edgeSizeEi ,                      "NK8Triangle8edge");//  0000000020000570  [ .text ]
	FUNCTION( __ZNK8Triangle17edgeGetProjectionEi ,            "NK8Triangle17edg");//  000000002000062a  [ .text ]
	FUNCTION( __Z24checkAndSplitLargestEdgeRK8TriangleffRS_S2_ ,                    "checkAndSplitLar");//  0000000020000672  [ .text ]
	FUNCTION( __Z22triangulateOneTriangleRK8TriangleffP6BufferS3_ ,                      "triangulateOneTr");//  00000000200008de  [ .text ]
	FUNCTION( __Z11triangulatePKfPK9s_v4nm32fiiiiPfPS1_Pi ,    "triangulatePKf  ");//  0000000020000dc8  [ .text ]
	FUNCTION( __Z15nmglBindTexturejj ,                           "nmglBindTexturej");//  00000000200010ca  [ .text ]
	FUNCTION( __Z18getTexelSizeUbytesi ,                         "getTexelSizeUbyt");//  0000000020001436  [ .text ]
	FUNCTION( __Z8initLvlsjP16NMGL_Context_NM0 ,                "initLvlsjP16NMGL");//  000000002000148e  [ .text ]
	FUNCTION( __Z10isPowerOf2i ,       "isPowerOf2i     ");//  00000000200015d8  [ .text ]
	FUNCTION( __Z10copyPixelsPKviiiPPv ,        "copyPixels      ");//  00000000200015fc  [ .text ]
	FUNCTION( __Z14nmglTexImage2DjiiiiijjPKv ,                      "nmglTexImage2Dji");//  0000000020001664  [ .text ]
	FUNCTION( __Z17nmglTexSubImage2DjiiiiijjPKv ,                   "nmglTexSubImage2");//  000000002000186e  [ .text ]
	FUNCTION( _nmppsMerge_32f ,    "nmppsMerge_32f  ");//  00000000200019a6  [ .text ]
	FUNCTION( _halCopyRISC ,       "halCopyRISC     ");//  00000000200019e8  [ .text ]
	NONCFUNC( ARShift32 ,         "ARShift32       ");//  0000000020002fd0  [ .text ]
	NONCFUNC( DAdd ,              "DAdd            ");//  000000002000305c  [ .text ]
	NONCFUNC( DFrExp ,            "DFrExp          ");//  000000002000311c  [ .text ]
	NONCFUNC( DCmp ,              "DCmp            ");//  000000002000326e  [ .text ]
	NONCFUNC( DDiv ,              "DDiv            ");//  00000000200032c0  [ .text ]
	NONCFUNC( UDiv32 ,            "UDiv32          ");//  0000000020003384  [ .text ]
	NONCFUNC( IDiv32 ,            "IDiv32          ");//  0000000020003396  [ .text ]
	FUNCTION( IDiv64 ,            "IDiv64          ");//  0000000020003404  [ .text ]
	FUNCTION( UDiv64 ,            "UDiv64          ");//  000000002000346a  [ .text ]
	NONCFUNC( DMul ,              "DMul            ");//  0000000020003488  [ .text ]
	NONCFUNC( ConvDtoF ,          "ConvDtoF        ");//  000000002000353a  [ .text ]
	NONCFUNC( FAdd ,              "FAdd            ");//  0000000020003570  [ .text ]
	NONCFUNC( FFrExp ,            "FFrExp          ");//  000000002000361a  [ .text ]
	FUNCTION( FResult ,           "FResult         ");//  000000002000365a  [ .text ]
	NONCFUNC( FMul ,              "FMul            ");//  0000000020003800  [ .text ]
	FUNCTION( ConvFtoD ,          "ConvFtoD        ");//  0000000020003882  [ .text ]
	FUNCTION( ConvFtoI32 ,        "ConvFtoI32      ");//  00000000200038b6  [ .text ]
	FUNCTION( ConvFtoU32 ,        "ConvFtoU32      ");//  000000002000390c  [ .text ]
	NONCFUNC( LShift32 ,          "LShift32        ");//  000000002000395c  [ .text ]
	FUNCTION( LShift64 ,          "LShift64        ");//  00000000200039e8  [ .text ]
	FUNCTION( UMod64 ,            "UMod64          ");//  0000000020003b04  [ .text ]
	FUNCTION( IMod64 ,            "IMod64          ");//  0000000020003b40  [ .text ]
	NONCFUNC( Mul32 ,             "Mul32           ");//  0000000020003b8c  [ .text ]
	NONCFUNC( Mul32Ex ,           "Mul32Ex         ");//  0000000020003ba6  [ .text ]
	FUNCTION( Mul64 ,             "Mul64           ");//  0000000020003bc2  [ .text ]
	NONCFUNC( RShift32 ,          "RShift32        ");//  0000000020003be6  [ .text ]
	FUNCTION( RShift64 ,          "RShift64        ");//  0000000020003c72  [ .text ]
	NONCFUNC( ConvDtoI32 ,        "ConvDtoI32      ");//  0000000020004968  [ .text ]
	FUNCTION( ConvDtoU32 ,        "ConvDtoU32      ");//  00000000200049c0  [ .text ]
	NONCFUNC( ConvI32toD ,        "ConvI32toD      ");//  0000000020004a06  [ .text ]
	NONCFUNC( UMod32 ,            "UMod32          ");//  0000000020004a22  [ .text ]
	NONCFUNC( IMod32 ,            "IMod32          ");//  0000000020004a4e  [ .text ]
	NONCFUNC( ConvU32toD ,        "ConvU32toD      ");//  0000000020004a84  [ .text ]
	FUNCTION( __Z13nmglvsNm0Initv ,    "nmglvsNm0Initv  ");//  0000000020004ebc  [ .text_nmglvs ]
	FUNCTION( _nmppsDiv_32f ,      "nmppsDiv_32f    ");//  0000000000000200  [ .text_int ]
	FUNCTION( _nmppsExp_32f ,      "nmppsExp_32f    ");//  00000000000005d2  [ .text_int ]
	FUNCTION( _nmppsLn_32f ,       "nmppsLn_32f     ");//  000000000000095c  [ .text_int ]
	FUNCTION( __Z17nmglActiveTexturej ,                          "nmglActiveTextur");//  000000002005ebf8  [ .text_nmgl ]
	FUNCTION( __Z9nmglBeginj ,        "nmglBeginj      ");//  000000002005ece6  [ .text_nmgl ]
	FUNCTION( __Z12texObjCreatejjP16NMGL_Context_NM0 ,           "texObjCreatejjP1");//  000000002005ed14  [ .text_nmgl ]
	FUNCTION( __Z9nmglClearj ,        "nmglClearj      ");//  000000002005ed40  [ .text_nmgl ]
	FUNCTION( __Z14nmglClearColorffff ,                          "nmglClearColorff");//  000000002005ee0e  [ .text_nmgl ]
	FUNCTION( __Z15nmglClearDepthff ,  "nmglClearDepthff");//  000000002005ef1a  [ .text_nmgl ]
	FUNCTION( __Z11nmglColor4fffff ,   "nmglColor4fffff ");//  000000002005f014  [ .text_nmgl ]
	FUNCTION( __Z12nmglColor4ubiiii ,  "nmglColor4ubiiii");//  000000002005f05c  [ .text_nmgl ]
	FUNCTION( __Z12nmglColor4fvPf ,    "nmglColor4fvPf  ");//  000000002005f0e2  [ .text_nmgl ]
	FUNCTION( __Z13nmglColor4ubvPi ,   "nmglColor4ubvPi ");//  000000002005f128  [ .text_nmgl ]
	FUNCTION( __Z16nmglColorPointerijiPKv ,                         "nmglColorPointer");//  000000002005f170  [ .text_nmgl ]
	FUNCTION( __Z12nmglCullFacej ,     "nmglCullFacej   ");//  000000002005f1c2  [ .text_nmgl ]
	FUNCTION( __Z13nmglDepthFuncj ,    "nmglDepthFuncj  ");//  000000002005f1ea  [ .text_nmgl ]
	FUNCTION( __Z13nmglDepthMaski ,    "nmglDepthMaski  ");//  000000002005f2d4  [ .text_nmgl ]
	FUNCTION( __Z15nmglDepthRangefff ,                           "nmglDepthRangeff");//  000000002005f3a2  [ .text_nmgl ]
	FUNCTION( __Z11nmglDisablej ,      "nmglDisablej    ");//  000000002005f470  [ .text_nmgl ]
	FUNCTION( __Z22nmglDisableClientStatej ,                     "nmglDisableClien");//  000000002005f662  [ .text_nmgl ]
	FUNCTION( __Z14nmglDrawArraysjii ,                           "nmglDrawArraysji");//  000000002005f6c8  [ .text_nmgl ]
	FUNCTION( __Z10nmglEnablej ,       "nmglEnablej     ");//  00000000200600d2  [ .text_nmgl ]
	FUNCTION( __Z21nmglEnableClientStatej ,                      "nmglEnableClient");//  00000000200602b0  [ .text_nmgl ]
	FUNCTION( __Z7nmglEndv ,          "nmglEndv        ");//  0000000020060312  [ .text_nmgl ]
	FUNCTION( __Z10nmglFinishv ,       "nmglFinishv     ");//  000000002006043a  [ .text_nmgl ]
	FUNCTION( __Z9nmglFlushv ,        "nmglFlushv      ");//  00000000200604d2  [ .text_nmgl ]
	FUNCTION( __Z13nmglFrontFacej ,    "nmglFrontFacej  ");//  00000000200604d8  [ .text_nmgl ]
	FUNCTION( __Z12nmglFrustumfffffff ,                          "nmglFrustumfffff");//  0000000020060500  [ .text_nmgl ]
	FUNCTION( __Z15nmglGenTexturesiPj ,                          "nmglGenTexturesi");//  00000000200605dc  [ .text_nmgl ]
	FUNCTION( __Z12nmglGetErrorv ,     "nmglGetErrorv   ");//  0000000020060606  [ .text_nmgl ]
	FUNCTION( __Z13nmglGetFloatvjPf ,  "nmglGetFloatvjPf");//  0000000020060610  [ .text_nmgl ]
	FUNCTION( __Z15nmglGetIntegervjPi ,                          "nmglGetIntegervj");//  00000000200606a4  [ .text_nmgl ]
	FUNCTION( __Z10nmglLightfjjf ,     "nmglLightfjjf   ");//  00000000200606b0  [ .text_nmgl ]
	FUNCTION( __Z11nmglLightfvjjPKf ,     "nmglLightfvjj   ");//  000000002006079e  [ .text_nmgl ]
	FUNCTION( __Z16nmglLoadIdentityv ,                           "nmglLoadIdentity");//  000000002006099e  [ .text_nmgl ]
	FUNCTION( __Z15nmglLoadMatrixfPKf ,   "nmglLoadMatrixf ");//  00000000200609ba  [ .text_nmgl ]
	FUNCTION( __Z13nmglMaterialfjjf ,  "nmglMaterialfjjf");//  00000000200609ea  [ .text_nmgl ]
	FUNCTION( __Z14nmglMaterialfvjjPKf ,  "nmglMaterialfvjj");//  0000000020060a40  [ .text_nmgl ]
	FUNCTION( __Z14nmglMatrixModej ,   "nmglMatrixModej ");//  0000000020060b4e  [ .text_nmgl ]
	FUNCTION( __Z15nmglMultMatrixfPKf ,   "nmglMultMatrixf ");//  0000000020060b8e  [ .text_nmgl ]
	FUNCTION( __Z12nmglNormal3ffff ,   "nmglNormal3ffff ");//  0000000020060bb8  [ .text_nmgl ]
	FUNCTION( __Z13nmglNormal3fvPKf ,     "nmglNormal3fv   ");//  0000000020060c00  [ .text_nmgl ]
	FUNCTION( __Z17nmglNormalPointerjiPKv ,                         "nmglNormalPointe");//  0000000020060c46  [ .text_nmgl ]
	FUNCTION( __Z19nmglNormalPointerNMjiPKv ,                       "nmglNormalPointe");//  0000000020060c86  [ .text_nmgl ]
	FUNCTION( __Z10nmglOrthofffffff ,  "nmglOrthofffffff");//  0000000020060cc6  [ .text_nmgl ]
	FUNCTION( __Z13nmglPointSizef ,    "nmglPointSizef  ");//  0000000020060d98  [ .text_nmgl ]
	FUNCTION( __Z13nmglPopMatrixv ,    "nmglPopMatrixv  ");//  0000000020060eb6  [ .text_nmgl ]
	FUNCTION( __Z14nmglPushMatrixv ,   "nmglPushMatrixv ");//  0000000020060edc  [ .text_nmgl ]
	FUNCTION( __Z11nmglRotatefffff ,   "nmglRotatefffff ");//  0000000020060f3a  [ .text_nmgl ]
	FUNCTION( __Z10nmglScaleffff ,     "nmglScaleffff   ");//  00000000200610d4  [ .text_nmgl ]
	FUNCTION( __Z19nmglTexCoordPointerijiPKv ,                      "nmglTexCoordPoin");//  0000000020061126  [ .text_nmgl ]
	FUNCTION( __Z14nmglTranslateffff ,                           "nmglTranslatefff");//  000000002006118c  [ .text_nmgl ]
	FUNCTION( __Z12nmglVertex2fff ,    "nmglVertex2fff  ");//  00000000200611f6  [ .text_nmgl ]
	FUNCTION( __Z12nmglVertex3ffff ,   "nmglVertex3ffff ");//  000000002006123c  [ .text_nmgl ]
	FUNCTION( __Z13nmglVertex2fvPKf ,     "nmglVertex2fv   ");//  0000000020061284  [ .text_nmgl ]
	FUNCTION( __Z13nmglVertex3fvPKf ,     "nmglVertex3fv   ");//  00000000200612ca  [ .text_nmgl ]
	FUNCTION( __Z17nmglVertexPointerijiPKv ,                        "nmglVertexPointe");//  0000000020061310  [ .text_nmgl ]
	FUNCTION( __Z12nmglViewportiiii ,  "nmglViewportiiii");//  0000000020061364  [ .text_nmgl ]
	FUNCTION( _firstNonZeroIndx_32s ,                        "firstNonZeroIndx");//  0000000000000d24  [ .text_nmplv ]
	FUNCTION( _remap_32u ,         "remap_32u       ");//  0000000000000d3e  [ .text_nmplv ]
	FUNCTION( _nmppsCmpGtC_v2nm32f ,                         "nmppsCmpGtC_v2nm");//  0000000000000d60  [ .text_nmplv ]
	FUNCTION( _nmppsCmpLtC_v2nm32f ,                         "nmppsCmpLtC_v2nm");//  0000000000000d90  [ .text_nmplv ]
	FUNCTION( _nmppsAdd_32f ,      "nmppsAdd_32f    ");//  0000000000000dc0  [ .text_nmplv ]
	FUNCTION( _nmppsMul_Mul_Sub_32f ,                        "nmppsMul_Mul_Sub");//  0000000000000de8  [ .text_nmplv ]
	FUNCTION( _nmppsMulC_32f ,     "nmppsMulC_32f   ");//  0000000000000e22  [ .text_nmplv ]
	FUNCTION( _nmppsMulC_AddC_32f ,                          "nmppsMulC_AddC_3");//  0000000000000e54  [ .text_nmplv ]
	FUNCTION( _nmppsMulC_AddV_32f ,                          "nmppsMulC_AddV_3");//  0000000000000e8c  [ .text_nmplv ]
	FUNCTION( _nmppsMulC_AddV_AddC_32f ,                     "nmppsMulC_AddV_A");//  0000000000000eba  [ .text_nmplv ]
	FUNCTION( _nmppsSub_32f ,      "nmppsSub_32f    ");//  0000000000000ef2  [ .text_nmplv ]
	FUNCTION( _nmppsSubC_32f ,     "nmppsSubC_32f   ");//  0000000000000f1a  [ .text_nmplv ]
	FUNCTION( _nmppsSubCRev_32f ,  "nmppsSubCRev_32f");//  0000000000000f4c  [ .text_nmplv ]
	FUNCTION( _nmppsAddC_32f ,     "nmppsAddC_32f   ");//  0000000000000f84  [ .text_nmplv ]
	FUNCTION( __Z28copyColorByIndices_BGRA_RGBAP9s_v4nm32sPiS0_i ,  "copyColorByIndic");//  000000002006149c  [ .text_demo3d ]
	FUNCTION( __Z13copyTrianglesRK9TrianglesiRS_ii ,     "copyTriangles   ");//  00000000200614e2  [ .text_demo3d ]
	FUNCTION( __Z21cullFaceSortTrianglesR9Triangles ,            "cullFaceSortTria");//  00000000200615b6  [ .text_demo3d ]
	FUNCTION( __Z5lightP9s_v4nm32fS0_i ,                        "lightP9s_v4nm32f");//  0000000020061936  [ .text_demo3d ]
	FUNCTION( __Z7pow_32fPfS_fiS_ ,   "pow_32fPfS_fiS_ ");//  0000000020061c36  [ .text_demo3d ]
	FUNCTION( __Z13pushToLines_lPKfS0_S0_PK9s_v4nm32fR5Linesi ,                      "pushToLines_lPKf");//  0000000020061c78  [ .text_demo3d ]
	FUNCTION( __Z14pushToLines_llPKfS0_S0_PK9s_v4nm32fR5Linesi ,                     "pushToLines_llPK");//  0000000020061d4c  [ .text_demo3d ]
	FUNCTION( __Z14pushToLines_lsPKfS0_S0_PK9s_v4nm32fR5Linesi ,                     "pushToLines_lsPK");//  0000000020061e28  [ .text_demo3d ]
	FUNCTION( __Z17pushToTriangles_tPKfS0_S0_PK9s_v4nm32fR9Trianglesi ,                  "pushToTriangles_");//  0000000020062020  [ .text_demo3d ]
	FUNCTION( __Z18pushToTriangles_tfPKfS0_S0_PK9s_v4nm32fR9Trianglesi ,                 "pushToTriangles_");//  0000000020062178  [ .text_demo3d ]
	FUNCTION( __Z18pushToTriangles_tsPKfS0_S0_PK9s_v4nm32fR9Trianglesi ,                 "pushToTriangles_");//  00000000200622d0  [ .text_demo3d ]
	FUNCTION( __Z10lineOffsetR5LinesS0_i ,                       "lineOffsetR5Line");//  0000000020062428  [ .text_demo3d ]
	FUNCTION( __Z10rasterizeLPK5LinesPK7BitMask ,                          "rasterizeLPK5Lin");//  000000002006245e  [ .text_demo3d ]
	FUNCTION( __Z11pointOffsetR6PointsS0_i ,                     "pointOffsetR6Poi");//  0000000020062984  [ .text_demo3d ]
	FUNCTION( __Z10rasterizePPK6PointsPK7BitMask ,                         "rasterizePPK6Poi");//  00000000200629ae  [ .text_demo3d ]
	FUNCTION( __Z14triangleOffsetR9TrianglesS0_i ,               "triangleOffsetR9");//  0000000020062ece  [ .text_demo3d ]
	FUNCTION( __Z10rasterizeTPK9TrianglesPK7BitMask ,                      "rasterizeTPK9Tri");//  0000000020062f10  [ .text_demo3d ]
	FUNCTION( _readMaskToLimitDst ,                          "readMaskToLimitD");//  0000000020063416  [ .text_demo3d ]
	FUNCTION( __Z21reverseMatrix3x3in4x4P11s_mat4nm32fS0_ ,      "reverseMatrix3x3");//  00000000200634f2  [ .text_demo3d ]
	FUNCTION( __Z14setSegmentMaskP9s_v2nm32fS0_P7BitMaski ,      "setSegmentMaskP9");//  0000000020063718  [ .text_demo3d ]
	FUNCTION( __Z15updatePolygonsLP12DataForNmpu1P5Linesiii ,    "updatePolygonsLP");//  0000000020063928  [ .text_demo3d ]
	FUNCTION( __Z15updatePolygonsPP12DataForNmpu1P6Pointsiii ,   "updatePolygonsPP");//  0000000020063a88  [ .text_demo3d ]
	FUNCTION( __Z15updatePolygonsTP12DataForNmpu1P9Trianglesiii ,  "updatePolygonsTP");//  0000000020063b3a  [ .text_demo3d ]
	FUNCTION( __Z15updatePolygonsLP11PolygonsOldP5Linesiii ,     "updatePolygonsLP");//  0000000020063da6  [ .text_demo3d ]
	FUNCTION( __Z15updatePolygonsTP11PolygonsOldP9Trianglesiii ,  "updatePolygonsTP");//  0000000020063dac  [ .text_demo3d ]
	FUNCTION( _absIfNegElse0_32f ,                           "absIfNegElse0_32");//  0000000020064366  [ .text_demo3d ]
	FUNCTION( _addC_v4nm32f ,      "addC_v4nm32f    ");//  0000000020064390  [ .text_demo3d ]
	FUNCTION( _baseLighti ,        "baseLighti      ");//  00000000200643cc  [ .text_demo3d ]
	FUNCTION( _clamp_32f ,         "clamp_32f       ");//  000000002006442e  [ .text_demo3d ]
	FUNCTION( _cnv32f_v2v4 ,       "cnv32f_v2v4     ");//  0000000020064476  [ .text_demo3d ]
	FUNCTION( _cnv32f_v3v4 ,       "cnv32f_v3v4     ");//  00000000200644a8  [ .text_demo3d ]
	FUNCTION( _copyArraysByIndices ,                         "copyArraysByIndi");//  0000000020064518  [ .text_demo3d ]
	FUNCTION( _copyColorByIndices ,                          "copyColorByIndic");//  0000000020064540  [ .text_demo3d ]
	FUNCTION( _copyOddOffset ,     "copyOddOffset   ");//  0000000020064560  [ .text_demo3d ]
	FUNCTION( _dotC_gt0_v4nm32f ,  "dotC_gt0_v4nm32f");//  00000000200645a0  [ .text_demo3d ]
	FUNCTION( _dotMulC_Add_v4nm32f ,                         "dotMulC_Add_v4nm");//  00000000200645f8  [ .text_demo3d ]
	FUNCTION( _dotMulC_AddC_v4nm32f ,                        "dotMulC_AddC_v4n");//  0000000020064638  [ .text_demo3d ]
	FUNCTION( _dotMulV_v4nm32f ,   "dotMulV_v4nm32f ");//  0000000020064676  [ .text_demo3d ]
	FUNCTION( _dotV_gt0_v4nm32f ,  "dotV_gt0_v4nm32f");//  00000000200646ac  [ .text_demo3d ]
	FUNCTION( _dotV_v4nm32f ,      "dotV_v4nm32f    ");//  0000000020064700  [ .text_demo3d ]
	FUNCTION( _doubleAbsIfNegElse0_32f ,                     "doubleAbsIfNegEl");//  0000000020064752  [ .text_demo3d ]
	FUNCTION( _doubleAdd_32f ,     "doubleAdd_32f   ");//  000000002006478c  [ .text_demo3d ]
	FUNCTION( _doubleClamp_32f ,   "doubleClamp_32f ");//  00000000200647c4  [ .text_demo3d ]
	FUNCTION( _doubleMulC_32f ,    "doubleMulC_32f  ");//  000000002006481e  [ .text_demo3d ]
	FUNCTION( _doubleSub_32f ,     "doubleSub_32f   ");//  0000000020064856  [ .text_demo3d ]
	FUNCTION( _doubleSubC_32f ,    "doubleSubC_32f  ");//  000000002006488e  [ .text_demo3d ]
	FUNCTION( _fastInvSqrt ,       "fastInvSqrt     ");//  00000000200648c6  [ .text_demo3d ]
	FUNCTION( _findMinMax2 ,       "findMinMax2     ");//  0000000020064916  [ .text_demo3d ]
	FUNCTION( _findMinMax3 ,       "findMinMax3     ");//  0000000020064952  [ .text_demo3d ]
	FUNCTION( _loadIdentify ,      "loadIdentify    ");//  00000000200649a2  [ .text_demo3d ]
	FUNCTION( _maskEq0 ,           "maskEq0         ");//  00000000200649b0  [ .text_demo3d ]
	FUNCTION( _maskSelectionLight_RGBA_BGRA ,                "maskSelectionLig");//  00000000200649ec  [ .text_demo3d ]
	FUNCTION( _meanToInt2 ,        "meanToInt2      ");//  0000000020064a46  [ .text_demo3d ]
	FUNCTION( _meanToInt3 ,        "meanToInt3      ");//  0000000020064a7a  [ .text_demo3d ]
	FUNCTION( _mul_mat4nm32f_v4nm32f ,                       "mul_mat4nm32f_v4");//  0000000020064ab6  [ .text_demo3d ]
	FUNCTION( _mul_v4nm32f_mat4nm32f ,                       "mul_v4nm32f_mat4");//  0000000020064af8  [ .text_demo3d ]
	FUNCTION( _mulC_v4nm32f ,      "mulC_v4nm32f    ");//  0000000020064b3a  [ .text_demo3d ]
	FUNCTION( _nmppsCmpGteLteMirrorV_32f ,                   "nmppsCmpGteLteMi");//  0000000020064b6e  [ .text_demo3d ]
	FUNCTION( _readMask ,          "readMask        ");//  0000000020064bb2  [ .text_demo3d ]
	FUNCTION( _replaceEq0_32f ,    "replaceEq0_32f  ");//  0000000020064be8  [ .text_demo3d ]
	FUNCTION( _set_v4nm32f ,       "set_v4nm32f     ");//  0000000020064c18  [ .text_demo3d ]
	FUNCTION( _sortByY2 ,          "sortByY2        ");//  0000000020064c48  [ .text_demo3d ]
	FUNCTION( _sortByY3 ,          "sortByY3        ");//  0000000020064c7e  [ .text_demo3d ]
	FUNCTION( _split_v2nm32f ,     "split_v2nm32f   ");//  0000000020064cdc  [ .text_demo3d ]
	FUNCTION( _split_v4nm32f ,     "split_v4nm32f   ");//  0000000020064d1c  [ .text_demo3d ]
	FUNCTION( _subCRev_v4nm32f ,   "subCRev_v4nm32f ");//  0000000020064d88  [ .text_demo3d ]
	FUNCTION( _ternaryLt0_AddC_AddC_32f ,                    "ternaryLt0_AddC_");//  0000000020064dbe  [ .text_demo3d ]
	FUNCTION( _tripleMulC_32f ,    "tripleMulC_32f  ");//  0000000020064e0c  [ .text_demo3d ]
	FUNCTION( _vertexPrimitiveRepack ,                       "vertexPrimitiveR");//  0000000020064e4e  [ .text_demo3d ]
	FUNCTION( _halSetProcessorNo ,                           "halSetProcessorN");//  00000000200656ae  [ .text_hal ]
	FUNCTION( _halHandshake ,      "halHandshake    ");//  00000000200656ba  [ .text_hal ]
	FUNCTION( _halSync ,           "halSync         ");//  0000000020065736  [ .text_hal ]
	FUNCTION( _halSyncAddr ,       "halSyncAddr     ");//  0000000020065818  [ .text_hal ]
	FUNCTION( _halSyncArray ,      "halSyncArray    ");//  0000000020065840  [ .text_hal ]
	FUNCTION( _halInstrCacheEnable ,                         "halInstrCacheEna");//  00000000200658aa  [ .text_hal ]
	FUNCTION( _halInstrCacheDisable ,                        "halInstrCacheDis");//  00000000200658b0  [ .text_hal ]
	FUNCTION( _TEST ,              "TEST            ");//  00000000200658ba  [ .text_hal ]
	FUNCTION( _halDmaSetCallback ,                           "halDmaSetCallbac");//  000000002006594e  [ .text_hal ]
	//FUNCTION( _halSetCallbackDMA ,                           "halSetCallbackDM");//  000000002006594e  [ .text_hal ]
	FUNCTION( _halGetCallbackDMA ,                           "halGetCallbackDM");//  0000000020065954  [ .text_hal ]
	//FUNCTION( _halDmaGetCallback ,                           "halDmaGetCallbac");//  0000000020065954  [ .text_hal ]
	FUNCTION( _halEnbExtInt ,      "halEnbExtInt    ");//  0000000020065958  [ .text_hal ]
	FUNCTION( _halDisExtInt ,      "halDisExtInt    ");//  000000002006595e  [ .text_hal ]
	FUNCTION( _halOpenDMA ,        "halOpenDMA      ");//  0000000020065964  [ .text_hal ]
	//FUNCTION( _halDmaInit ,        "halDmaInit      ");//  0000000020065964  [ .text_hal ]
	FUNCTION( _halDmaInitC ,       "halDmaInitC     ");//  00000000200659a2  [ .text_hal ]
	FUNCTION( _halStatusDMA ,      "halStatusDMA    ");//  00000000200659ac  [ .text_hal ]
	FUNCTION( _halDmaIsCompleted ,                           "halDmaIsComplete");//  00000000200659be  [ .text_hal ]
	FUNCTION( _halMaskIntContMdma_mc12101 ,                  "halMaskIntContMd");//  00000000200659c4  [ .text_hal ]
	FUNCTION( _GetPswr ,           "GetPswr         ");//  00000000200659cc  [ .text_hal ]
	FUNCTION( _SetFlagDMA ,        "SetFlagDMA      ");//  00000000200659d0  [ .text_hal ]
	FUNCTION( _halLockDMA ,        "halLockDMA      ");//  00000000200659d8  [ .text_hal ]
	FUNCTION( _halUnlockDMA ,      "halUnlockDMA    ");//  00000000200659e0  [ .text_hal ]
	FUNCTION( _halIsBusyDMA ,      "halIsBusyDMA    ");//  00000000200659e6  [ .text_hal ]
	FUNCTION( _halWhereMirror ,    "halWhereMirror  ");//  00000000200659ec  [ .text_hal ]
	FUNCTION( _halGetCoreId ,      "halGetCoreId    ");//  00000000200659f8  [ .text_hal ]
	FUNCTION( _halReadCoreID ,     "halReadCoreID   ");//  00000000200659fe  [ .text_hal ]
	FUNCTION( _halSetMirror ,      "halSetMirror    ");//  0000000020065a04  [ .text_hal ]
	FUNCTION( _halOpenStatusDMA ,  "halOpenStatusDMA");//  0000000020065a14  [ .text_hal ]
	FUNCTION( SOS ,               "SOS             ");//  0000000020065a26  [ .text_hal ]
	FUNCTION( _halInitSingleDMA ,  "halInitSingleDMA");//  0000000020065a7a  [ .text_hal ]
	FUNCTION( _halDmaStart ,       "halDmaStart     ");//  0000000020065b06  [ .text_hal ]
	FUNCTION( _halDmaStartA ,      "halDmaStartA    ");//  0000000020065b5e  [ .text_hal ]
	FUNCTION( _halDmaStartM ,      "halDmaStartM    ");//  0000000020065ba0  [ .text_hal ]
	FUNCTION( _halDmaStartMA ,     "halDmaStartMA   ");//  0000000020065bec  [ .text_hal ]
	FUNCTION( _halDmaStartC ,      "halDmaStartC    ");//  0000000020065c22  [ .text_hal ]
	FUNCTION( _halDmaStartCA ,     "halDmaStartCA   ");//  0000000020065cae  [ .text_hal ]
	FUNCTION( _halDmaStartCM ,     "halDmaStartCM   ");//  0000000020065d24  [ .text_hal ]
	FUNCTION( _halDmaStartCMA ,    "halDmaStartCMA  ");//  0000000020065da4  [ .text_hal ]
	FUNCTION( _halMapAddrFrom ,    "halMapAddrFrom  ");//  0000000020065e0e  [ .text_hal ]
	FUNCTION( _halMapAddrTo ,      "halMapAddrTo    ");//  0000000020065e22  [ .text_hal ]
	FUNCTION( halSleep ,          "halSleep        ");//  0000000020065e32  [ .text_hal ]
	FUNCTION( _halSleep ,          "halSleep        ");//  0000000020065e4a  [ .text_hal ]
	FUNCTION( _halEnterCriticalSectionCore ,                 "halEnterCritical");//  0000000020065e56  [ .text_hal ]
	FUNCTION( _halExitCriticalSectionCore ,                  "halExitCriticalS");//  0000000020065e92  [ .text_hal ]
	FUNCTION( _halEnterCriticalSection ,                     "halEnterCritical");//  0000000020065ea2  [ .text_hal ]
	FUNCTION( _halExitCriticalSection ,                      "halExitCriticalS");//  0000000020065ede  [ .text_hal ]
	FUNCTION( _mipmap ,            "mipmap          ");//  0000000020066010  [ .textures_mipmap_mem ]

PROFILE_END();
end ".text_nmprofiler";
