//****************************************************************************************
// This is automaticly generated asm-file by profiler-gcc.py
//****************************************************************************************
import from "nmprofiler.mlb";
begin ".text_nmprofiler"
PROFILE_BEGIN( 16 ,0);

	//FUNCTION( hw_init_hook ,      "hw_init_hook    ");//  0000000022000016  [ .text ]
	//FUNCTION( __Z18hostCreatePatternsP13PatternsArray ,          "hostCreatePatter");//  00000000220001d4  [ .text ]
	//FUNCTION( __Z13fillPtrnsInitPjPij ,                          "fillPtrnsInitPjP");//  0000000022000896  [ .text ]
	//FUNCTION( __Z13linePtrnsInitPjPij ,                          "linePtrnsInitPjP");//  0000000022001208  [ .text ]
	//FUNCTION( __Z14pointPtrnsInitPjj ,                           "pointPtrnsInitPj");//  00000000220016b0  [ .text ]
	//FUNCTION( __ZN11DepthBuffer6updateEv ,                     "N11DepthBuffer6u");//  00000000220019f2  [ .text ]
	//FUNCTION( _halCopyRISC ,       "halCopyRISC     ");//  0000000022001ace  [ .text ]
	//NONCFUNC( ARShift32 ,         "ARShift32       ");//  00000000220023b2  [ .text ]
	//NONCFUNC( DFrExp ,            "DFrExp          ");//  000000002200261a  [ .text ]
	//NONCFUNC( UDiv32 ,            "UDiv32          ");//  0000000022002882  [ .text ]
	//NONCFUNC( IDiv32 ,            "IDiv32          ");//  0000000022002894  [ .text ]
	//NONCFUNC( FAdd ,              "FAdd            ");//  0000000022002a60  [ .text ]
	//NONCFUNC( FFrExp ,            "FFrExp          ");//  0000000022002b0a  [ .text ]
	//FUNCTION( FResult ,           "FResult         ");//  0000000022002b4a  [ .text ]
	//NONCFUNC( FDiv ,              "FDiv            ");//  0000000022002c62  [ .text ]
	//NONCFUNC( FMul ,              "FMul            ");//  0000000022002cf0  [ .text ]
	//NONCFUNC( LShift32 ,          "LShift32        ");//  0000000022002e68  [ .text ]
	//NONCFUNC( UMod32 ,            "UMod32          ");//  0000000022003010  [ .text ]
	//NONCFUNC( IMod32 ,            "IMod32          ");//  000000002200303c  [ .text ]
	//NONCFUNC( Mul32 ,             "Mul32           ");//  0000000022003072  [ .text ]
	//NONCFUNC( Mul32Ex ,           "Mul32Ex         ");//  000000002200308c  [ .text ]
	//NONCFUNC( RShift32 ,          "RShift32        ");//  00000000220030cc  [ .text ]
	FUNCTION( _duplicate_16s ,     "duplicate_16s   ");//  0000000022003982  [ .text_dmo3d ]
	FUNCTION( __Z14drawPrimitivesP16NMGL_Context_NM1i ,          "drawPrimitivesP1");//  000000002201687e  [ .text_demo3d ]
	//FUNCTION( __Z8cbUpdatev ,         "cbUpdatev       ");//  0000000022016ac2  [ .text_demo3d ]
	//FUNCTION( __Z7msdInitv ,          "msdInitv        ");//  0000000022016c20  [ .text_demo3d ]
	FUNCTION( __Z6msdAddPKvPvii ,            "msdAdd          ");//  0000000022016e2a  [ .text_demo3d ]
	FUNCTION( __Z8msdAdd2DPKvPvjjjji ,          "msdAdd2D        ");//  0000000022017004  [ .text_demo3d ]
	FUNCTION( __Z6msdAddR9MyDmaTaski ,                          "msdAddR9MyDmaTas");//  00000000220171f8  [ .text_demo3d ]
	FUNCTION( __Z16msdGetStatusCopyii ,                          "msdGetStatusCopy");//  00000000220174e0  [ .text_demo3d ]
	FUNCTION( __Z10msdWaitDmav ,       "msdWaitDmav     ");//  0000000022017504  [ .text_demo3d ]
	FUNCTION( __Z10msdWaitDmai ,       "msdWaitDmai     ");//  00000000220175d4  [ .text_demo3d ]
	//FUNCTION( __Z28colorBufferClearPartCallbackv ,               "colorBufferClear");//  0000000022017684  [ .text_demo3d ]
	//FUNCTION( __Z28depthBufferClearPartCallbackv ,               "depthBufferClear");//  0000000022017698  [ .text_demo3d ]
	FUNCTION( __Z10NMGL_ClearP16NMGL_Context_NM1P10CommandNm1 ,  "NMGL_ClearP16NMG");//  00000000220176aa  [ .text_demo3d ]
	FUNCTION( __Z19NMGL_ReadCommandNm1v ,                        "NMGL_ReadCommand");//  0000000022017808  [ .text_demo3d ]
	FUNCTION( __Z25NMGL_CopySegmentFromImageP16NMGL_Context_NM1P10CommandNm1 ,  "NMGL_CopySegment");//  0000000022017978  [ .text_demo3d ]
	FUNCTION( __Z23NMGL_CopySegmentToImageP16NMGL_Context_NM1P10CommandNm1 ,  "NMGL_CopySegment");//  0000000022017a76  [ .text_demo3d ]
	FUNCTION( __Z14NMGL_DepthMaskP16NMGL_Context_NM1P10CommandNm1 ,  "NMGL_DepthMaskP1");//  0000000022017b68  [ .text_demo3d ]
	FUNCTION( __Z14NMGL_DepthFuncP16NMGL_Context_NM1P10CommandNm1 ,  "NMGL_DepthFuncP1");//  0000000022017b8a  [ .text_demo3d ]
	FUNCTION( __Z10NMGL_DepthP16NMGL_Context_NM1P10CommandNm1 ,  "NMGL_DepthP16NMG");//  0000000022017bac  [ .text_demo3d ]
	FUNCTION( __Z14NMGL_DrawLinesP16NMGL_Context_NM1P10CommandNm1 ,  "NMGL_DrawLinesP1");//  0000000022017bc0  [ .text_demo3d ]
	FUNCTION( __Z15NMGL_DrawPointsP16NMGL_Context_NM1P10CommandNm1 ,  "NMGL_DrawPointsP");//  0000000022017c1a  [ .text_demo3d ]
	FUNCTION( __Z18NMGL_DrawTrianglesP16NMGL_Context_NM1P10CommandNm1 ,  "NMGL_DrawTriangl");//  0000000022017c74  [ .text_demo3d ]
	FUNCTION( __Z13NMGL_SetColorP16NMGL_Context_NM1P10CommandNm1 ,  "NMGL_SetColorP16");//  000000002201801a  [ .text_demo3d ]
	FUNCTION( __Z13NMGL_SetDepthP16NMGL_Context_NM1P10CommandNm1 ,  "NMGL_SetDepthP16");//  0000000022018058  [ .text_demo3d ]
	FUNCTION( __Z14NMGL_PointSizeP16NMGL_Context_NM1P10CommandNm1 ,  "NMGL_PointSizeP1");//  000000002201808c  [ .text_demo3d ]
	FUNCTION( __Z15NMGL_SwapBufferP16NMGL_Context_NM1P10CommandNm1 ,  "NMGL_SwapBufferP");//  00000000220180a0  [ .text_demo3d ]
	FUNCTION( __Z9NMGL_ExitP16NMGL_Context_NM1P10CommandNm1 ,   "NMGL_ExitP16NMGL");//  000000002201818c  [ .text_demo3d ]
	FUNCTION( __Z21NMGL_SetActiveTextureP16NMGL_Context_NM1P10CommandNm1 ,  "NMGL_SetActiveTe");//  0000000022018192  [ .text_demo3d ]
	FUNCTION( __Z24NMGL_SetMipmapLvlPointerP16NMGL_Context_NM1P10CommandNm1 ,  "NMGL_SetMipmapLv");//  00000000220181a6  [ .text_demo3d ]
	FUNCTION( __Z24NMGL_BindActiveTexObjectP16NMGL_Context_NM1P10CommandNm1 ,  "NMGL_BindActiveT");//  00000000220181cc  [ .text_demo3d ]
	FUNCTION( __Z11NMGL_SetWhfP16NMGL_Context_NM1P10CommandNm1 ,  "NMGL_SetWhfP16NM");//  00000000220181f4  [ .text_demo3d ]
	FUNCTION( __Z20NMGL_SetColorPaletteP16NMGL_Context_NM1P10CommandNm1 ,  "NMGL_SetColorPal");//  0000000022018260  [ .text_demo3d ]
	FUNCTION( __Z19NMGL_SetTexEnvColorP16NMGL_Context_NM1P10CommandNm1 ,  "NMGL_SetTexEnvCo");//  0000000022018280  [ .text_demo3d ]
	FUNCTION( __Z18NMGL_SetTexEnvModeP16NMGL_Context_NM1P10CommandNm1 ,  "NMGL_SetTexEnvMo");//  00000000220182b0  [ .text_demo3d ]
	FUNCTION( __Z17NMGL_SetTexParamiP16NMGL_Context_NM1P10CommandNm1 ,  "NMGL_SetTexParam");//  00000000220182cc  [ .text_demo3d ]
	FUNCTION( __Z14NMGL_Texture2dP16NMGL_Context_NM1P10CommandNm1 ,  "NMGL_Texture2dP1");//  000000002201834e  [ .text_demo3d ]
	FUNCTION( __Z13getAddrPtrnsLP12DataForNmpu1 ,                "getAddrPtrnsLP12");//  0000000022018396  [ .text_demo3d ]
	FUNCTION( __Z13getAddrPtrnsPP12DataForNmpu1 ,                "getAddrPtrnsPP12");//  00000000220185c8  [ .text_demo3d ]
	FUNCTION( __Z13getAddrPtrnsTP12DataForNmpu1 ,                "getAddrPtrnsTP12");//  0000000022018978  [ .text_demo3d ]
	//FUNCTION( __Z14selectPatternsPiS_S_S_S_PS_iS_ ,              "selectPatternsPi");//  0000000022018e4e  [ .text_demo3d ]
	FUNCTION( _absIfNegElse0 ,     "absIfNegElse0   ");//  0000000022018eca  [ .text_demo3d ]
	FUNCTION( _baseAddrOffs_16u ,  "baseAddrOffs_16u");//  0000000022018f68  [ .text_demo3d ]
	//FUNCTION( _baseAddrOffs_16s ,  "baseAddrOffs_16s");//  0000000022018f68  [ .text_demo3d ]
	FUNCTION( _baseAddrOffs_32u ,  "baseAddrOffs_32u");//  0000000022019018  [ .text_demo3d ]
	//FUNCTION( _baseAddrOffs_32s ,  "baseAddrOffs_32s");//  0000000022019018  [ .text_demo3d ]
	FUNCTION( _convertABGR32_RGB565 ,                        "convertABGR32_RG");//  0000000022019034  [ .text_demo3d ]
	FUNCTION( _convertRGB565_RGB8888 ,                       "convertRGB565_RG");//  00000000220192a8  [ .text_demo3d ]
	FUNCTION( _copyPacket_32u ,    "copyPacket_32u  ");//  000000002201a2d0  [ .text_demo3d ]
	//FUNCTION( _copyPacket_32s ,    "copyPacket_32s  ");//  000000002201a2d0  [ .text_demo3d ]
	FUNCTION( _depthTest16 ,       "depthTest16     ");//  000000002201a3f2  [ .text_demo3d ]
	FUNCTION( _depthTest32 ,       "depthTest32     ");//  000000002201a424  [ .text_demo3d ]
	FUNCTION( _totalSum ,          "totalSum        ");//  000000002201a456  [ .text_demo3d ]
	FUNCTION( _inverse_8u ,        "inverse_8u      ");//  000000002201a468  [ .text_demo3d ]
	FUNCTION( _inverse_v4nm8u ,    "inverse_v4nm8u  ");//  000000002201a512  [ .text_demo3d ]
	FUNCTION( _mAndVxN_32u ,       "mAndVxN_32u     ");//  000000002201a530  [ .text_demo3d ]
	FUNCTION( _merge_v4nm32s ,     "merge_v4nm32s   ");//  000000002201a5da  [ .text_demo3d ]
	FUNCTION( _mergePtrnsAddr3 ,   "mergePtrnsAddr3 ");//  000000002201a628  [ .text_demo3d ]
	FUNCTION( _mMaskVxN_32s ,      "mMaskVxN_32s    ");//  000000002201a75c  [ .text_demo3d ]
	FUNCTION( _mMulCVxN_2s_RGB8888 ,                         "mMulCVxN_2s_RGB8");//  000000002201abaa  [ .text_demo3d ]
	FUNCTION( _mMulCVxN_2s32s ,    "mMulCVxN_2s32s  ");//  000000002201abc2  [ .text_demo3d ]
	FUNCTION( _mMulCVxN_2s_RGB8888_new ,                     "mMulCVxN_2s_RGB8");//  000000002201b024  [ .text_demo3d ]
	FUNCTION( _mMulCVxN_2s32s_new ,                          "mMulCVxN_2s32s_n");//  000000002201b040  [ .text_demo3d ]
	//FUNCTION( _msdStartCopy3 ,     "msdStartCopy3   ");//  000000002201b4c0  [ .text_demo3d ]
	FUNCTION( _selectPaintSide ,   "selectPaintSide ");//  000000002201b4f8  [ .text_demo3d ]
	FUNCTION( __Z13nmglvsNm1Initv ,    "nmglvsNm1Initv  ");//  000000002201b62c  [ .text_nmglvs ]
	FUNCTION( __Z12nmglvsNm1Runv ,     "nmglvsNm1Runv   ");//  000000002201bc12  [ .text_nmglvs ]
	FUNCTION( __Z13nmglvsNm1Stepv ,    "nmglvsNm1Stepv  ");//  000000002201bc24  [ .text_nmglvs ]
	//FUNCTION( _halSetProcessorNo ,                           "halSetProcessorN");//  000000002201beb2  [ .text_hal ]
	//FUNCTION( _halHandshake ,      "halHandshake    ");//  000000002201bebe  [ .text_hal ]
	//FUNCTION( _halSync ,           "halSync         ");//  000000002201bf3a  [ .text_hal ]
	//FUNCTION( _halSyncAddr ,       "halSyncAddr     ");//  000000002201c01c  [ .text_hal ]
	//FUNCTION( _halSyncArray ,      "halSyncArray    ");//  000000002201c044  [ .text_hal ]
	//FUNCTION( _halInstrCacheEnable ,                         "halInstrCacheEna");//  000000002201c0ae  [ .text_hal ]
	//FUNCTION( _halInstrCacheDisable ,                        "halInstrCacheDis");//  000000002201c0b4  [ .text_hal ]
	//FUNCTION( _halDma2D_IsCompleted ,                        "halDma2D_IsCompl");//  000000002201c102  [ .text_hal ]
	//FUNCTION( _halDma2D_Start ,    "halDma2D_Start  ");//  000000002201c10e  [ .text_hal ]
	//FUNCTION( _halDma2D_StartA ,   "halDma2D_StartA ");//  000000002201c18c  [ .text_hal ]
	//FUNCTION( _halDma2D_StartM ,   "halDma2D_StartM ");//  000000002201c1de  [ .text_hal ]
	//FUNCTION( _halDma2D_StartMA ,  "halDma2D_StartMA");//  000000002201c250  [ .text_hal ]
	//FUNCTION( _halDma2D_StartC ,   "halDma2D_StartC ");//  000000002201c296  [ .text_hal ]
	//FUNCTION( _halDma2D_StartCA ,  "halDma2D_StartCA");//  000000002201c348  [ .text_hal ]
	//FUNCTION( _halDma2D_StartCM ,  "halDma2D_StartCM");//  000000002201c3ce  [ .text_hal ]
	//FUNCTION( _halDma2D_StartCMA ,                           "halDma2D_StartCM");//  000000002201c474  [ .text_hal ]
	//FUNCTION( _TEST ,              "TEST            ");//  000000002201c4f2  [ .text_hal ]
	//FUNCTION( _halSetCallbackDMA ,                           "halSetCallbackDM");//  000000002201c586  [ .text_hal ]
	//FUNCTION( _halDmaSetCallback ,                           "halDmaSetCallbac");//  000000002201c586  [ .text_hal ]
	//FUNCTION( _halGetCallbackDMA ,                           "halGetCallbackDM");//  000000002201c58c  [ .text_hal ]
	//FUNCTION( _halDmaGetCallback ,                           "halDmaGetCallbac");//  000000002201c58c  [ .text_hal ]
	//FUNCTION( _halEnbExtInt ,      "halEnbExtInt    ");//  000000002201c590  [ .text_hal ]
	//FUNCTION( _halDisExtInt ,      "halDisExtInt    ");//  000000002201c596  [ .text_hal ]
	//FUNCTION( _halOpenDMA ,        "halOpenDMA      ");//  000000002201c59c  [ .text_hal ]
	////FUNCTION( _halDmaInit ,        "halDmaInit      ");//  000000002201c59c  [ .text_hal ]
	//FUNCTION( _halDmaInitC ,       "halDmaInitC     ");//  000000002201c5da  [ .text_hal ]
	//FUNCTION( _halStatusDMA ,      "halStatusDMA    ");//  000000002201c5e4  [ .text_hal ]
	//FUNCTION( _halDmaIsCompleted ,                           "halDmaIsComplete");//  000000002201c5f6  [ .text_hal ]
	//FUNCTION( _halMaskIntContMdma_mc12101 ,                  "halMaskIntContMd");//  000000002201c5fc  [ .text_hal ]
	//FUNCTION( _GetPswr ,           "GetPswr         ");//  000000002201c604  [ .text_hal ]
	//FUNCTION( _SetFlagDMA ,        "SetFlagDMA      ");//  000000002201c608  [ .text_hal ]
	//FUNCTION( _halLockDMA ,        "halLockDMA      ");//  000000002201c610  [ .text_hal ]
	//FUNCTION( _halUnlockDMA ,      "halUnlockDMA    ");//  000000002201c618  [ .text_hal ]
	//FUNCTION( _halIsBusyDMA ,      "halIsBusyDMA    ");//  000000002201c61e  [ .text_hal ]
	//FUNCTION( _halWhereMirror ,    "halWhereMirror  ");//  000000002201c624  [ .text_hal ]
	//FUNCTION( _halGetCoreId ,      "halGetCoreId    ");//  000000002201c630  [ .text_hal ]
	//FUNCTION( _halReadCoreID ,     "halReadCoreID   ");//  000000002201c636  [ .text_hal ]
	//FUNCTION( _halSetMirror ,      "halSetMirror    ");//  000000002201c63c  [ .text_hal ]
	//FUNCTION( _halOpenStatusDMA ,  "halOpenStatusDMA");//  000000002201c64c  [ .text_hal ]
	//FUNCTION( SOS ,               "SOS             ");//  000000002201c65e  [ .text_hal ]
	//FUNCTION( _halInitSingleDMA ,  "halInitSingleDMA");//  000000002201c6b2  [ .text_hal ]
	//FUNCTION( _halDmaStart ,       "halDmaStart     ");//  000000002201c73e  [ .text_hal ]
	//FUNCTION( _halDmaStartA ,      "halDmaStartA    ");//  000000002201c796  [ .text_hal ]
	//FUNCTION( _halDmaStartM ,      "halDmaStartM    ");//  000000002201c7d8  [ .text_hal ]
	//FUNCTION( _halDmaStartMA ,     "halDmaStartMA   ");//  000000002201c824  [ .text_hal ]
	//FUNCTION( _halDmaStartC ,      "halDmaStartC    ");//  000000002201c85a  [ .text_hal ]
	//FUNCTION( _halDmaStartCA ,     "halDmaStartCA   ");//  000000002201c8e6  [ .text_hal ]
	//FUNCTION( _halDmaStartCM ,     "halDmaStartCM   ");//  000000002201c95c  [ .text_hal ]
	//FUNCTION( _halDmaStartCMA ,    "halDmaStartCMA  ");//  000000002201c9dc  [ .text_hal ]
	//FUNCTION( _halMapAddrFrom ,    "halMapAddrFrom  ");//  000000002201ca46  [ .text_hal ]
	//FUNCTION( _halMapAddrTo ,      "halMapAddrTo    ");//  000000002201ca5a  [ .text_hal ]
	//FUNCTION( halSleep ,          "halSleep        ");//  000000002201ca6a  [ .text_hal ]
	//FUNCTION( _halSleep ,          "halSleep        ");//  000000002201ca82  [ .text_hal ]
	//FUNCTION( _halLed ,            "halLed          ");//  000000002201ca8e  [ .text_hal ]
	//FUNCTION( _halLedOn ,          "halLedOn        ");//  000000002201ca9e  [ .text_hal ]
	//FUNCTION( _halLedOff ,         "halLedOff       ");//  000000002201cab8  [ .text_hal ]
	//FUNCTION( _halLedSwitch ,      "halLedSwitch    ");//  000000002201cad2  [ .text_hal ]
	//FUNCTION( _halLed4Hi ,         "halLed4Hi       ");//  000000002201caec  [ .text_hal ]
	//FUNCTION( _halLed4Lo ,         "halLed4Lo       ");//  000000002201cb04  [ .text_hal ]
	//FUNCTION( _halEnterCriticalSectionCore ,                 "halEnterCritical");//  000000002201cb1c  [ .text_hal ]
	//FUNCTION( _halExitCriticalSectionCore ,                  "halExitCriticalS");//  000000002201cb58  [ .text_hal ]
	//FUNCTION( _halEnterCriticalSection ,                     "halEnterCritical");//  000000002201cb68  [ .text_hal ]
	//FUNCTION( _halExitCriticalSection ,                      "halExitCriticalS");//  000000002201cba4  [ .text_hal ]
	//FUNCTION( __ZN7tex_nm13maxEii ,  "N7tex_nm13maxEii");//  000000002201cbb6  [ .text_demo3dExt ]
	//FUNCTION( __ZN7tex_nm13minEii ,  "N7tex_nm13minEii");//  000000002201cbca  [ .text_demo3dExt ]
	//FUNCTION( __ZN7tex_nm16equalfEff ,                         "N7tex_nm16equalf");//  000000002201cbde  [ .text_demo3dExt ]
	//FUNCTION( __ZN7tex_nm113getPixelValueEjj10TexImage2DPNS_5colorE ,  "N7tex_nm113getPi");//  000000002201ccb0  [ .text_demo3dExt ]
	//FUNCTION( __ZN7tex_nm19wrapCoordEiif ,                     "N7tex_nm19wrapCo");//  000000002201cdd2  [ .text_demo3dExt ]
	//FUNCTION( __ZN7tex_nm114getPixelLinearENS_5Vec2fEii10TexImage2D ,  "N7tex_nm114getPi");//  000000002201ced6  [ .text_demo3dExt ]
	//FUNCTION( __ZN7tex_nm115getPixelNearestENS_5Vec2fE10TexImage2D ,  "N7tex_nm115getPi");//  000000002201d760  [ .text_demo3dExt ]
	//FUNCTION( __ZN7tex_nm115textureTriangleEPA64_iPNS_13TrianglesInfoEPPiP7Vector2P4SizeS4_S4_i ,  "N7tex_nm115textu");//  000000002201d894  [ .text_demo3dExt ]
	//FUNCTION( __ZN7tex_nm112edgeFunctionEffffffPf ,            "N7tex_nm112edgeF");//  000000002201fb92  [ .text_demo3dExt ]
	//FUNCTION( _depthTestCore32_A_r ,                         "depthTestCore32_");//  000000002201fbf2  [ .text_demo3dExt ]
	////FUNCTION( _depthTestCore16_A_r ,                         "depthTestCore16_");//  000000002201fbf2  [ .text_demo3dExt ]
	//FUNCTION( _depthTestCore32_A_rw ,                        "depthTestCore32_");//  0000000022020020  [ .text_demo3dExt ]
	////FUNCTION( _depthTestCore16_A_rw ,                        "depthTestCore16_");//  0000000022020020  [ .text_demo3dExt ]
	//FUNCTION( _depthTestCore16_Gt_r ,                        "depthTestCore16_");//  000000002202044e  [ .text_demo3dExt ]
	////FUNCTION( _depthTestCore32_Gt_r ,                        "depthTestCore32_");//  000000002202044e  [ .text_demo3dExt ]
	//FUNCTION( _depthTestCore16_Gt_rw ,                       "depthTestCore16_");//  000000002202087c  [ .text_demo3dExt ]
	////FUNCTION( _depthTestCore32_Gt_rw ,                       "depthTestCore32_");//  000000002202087c  [ .text_demo3dExt ]
	//FUNCTION( _depthTestCore16_Lt_r ,                        "depthTestCore16_");//  0000000022020caa  [ .text_demo3dExt ]
	////FUNCTION( _depthTestCore32_Lt_r ,                        "depthTestCore32_");//  0000000022020caa  [ .text_demo3dExt ]
	//FUNCTION( _depthTestCore16_Lt_rw ,                       "depthTestCore16_");//  00000000220210d8  [ .text_demo3dExt ]
	////FUNCTION( _depthTestCore32_Lt_rw ,                       "depthTestCore32_");//  00000000220210d8  [ .text_demo3dExt ]
	//FUNCTION( _depthTestCore16_N_r ,                         "depthTestCore16_");//  0000000022021506  [ .text_demo3dExt ]
	////FUNCTION( _depthTestCore32_N_r ,                         "depthTestCore32_");//  0000000022021506  [ .text_demo3dExt ]
	//FUNCTION( _depthTestCore32_N_rw ,                        "depthTestCore32_");//  0000000022021934  [ .text_demo3dExt ]
	////FUNCTION( _depthTestCore16_N_rw ,                        "depthTestCore16_");//  0000000022021934  [ .text_demo3dExt ]
	FUNCTION( _nmppsAddC_32s ,     "nmppsAddC_32s   ");//  00000000000023c4  [ .text_nmplv ]
	FUNCTION( _nmppsAdd_32s ,      "nmppsAdd_32s    ");//  00000000000023e6  [ .text_nmplv ]
	FUNCTION( _nmppsMulC_32s ,     "nmppsMulC_32s   ");//  0000000000002400  [ .text_nmplv ]
	FUNCTION( _nmppsMulC_AddV_AddC_32s ,                     "nmppsMulC_AddV_A");//  0000000000002424  [ .text_nmplv ]
	//FUNCTION( _nmppsMulC_AddV_AddC32s ,                      "nmppsMulC_AddV_A");//  0000000000002424  [ .text_nmplv ]
	FUNCTION( _nmppsSubC_32s ,     "nmppsSubC_32s   ");//  0000000000002452  [ .text_nmplv ]
	FUNCTION( _nmppsSub_32s ,      "nmppsSub_32s    ");//  000000000000246e  [ .text_nmplv ]
	FUNCTION( _nmppsAndC_32u ,     "nmppsAndC_32u   ");//  0000000000002488  [ .text_nmplv ]
	FUNCTION( _nmppsSet_32s ,      "nmppsSet_32s    ");//  00000000000024a0  [ .text_nmplv ]
	FUNCTION( _nmppsMerge_32s ,    "nmppsMerge_32s  ");//  00000000000024b4  [ .text_nmplv ]
	FUNCTION( _nmppsConvert_4s2s ,                           "nmppsConvert_4s2");//  00000000000024ee  [ .text_nmplv ]
	FUNCTION( _nmppsConvert_8s4s ,                           "nmppsConvert_8s4");//  00000000000025b0  [ .text_nmplv ]
	FUNCTION( _nmppsConvert_32s8s ,                          "nmppsConvert_32s");//  00000000000025d8  [ .text_nmplv ]
	FUNCTION( _nmppsCopy_32s ,     "nmppsCopy_32s   ");//  0000000000002608  [ .text_nmplv ]
	FUNCTION( _nmppsMinMaxEvery_32s ,                        "nmppsMinMaxEvery");//  0000000000002618  [ .text_nmplv ]
	FUNCTION( _nmppsClipCC_32s ,   "nmppsClipCC_32s ");//  000000000000263c  [ .text_nmplv ]
	FUNCTION( _nmppsMaxEvery_32s ,                           "nmppsMaxEvery_32");//  000000000000266c  [ .text_nmplv ]
	FUNCTION( _nmppsMinEvery_32s ,                           "nmppsMinEvery_32");//  000000000000268c  [ .text_nmplv ]
	FUNCTION( _nmppsRemap_32u ,    "nmppsRemap_32u  ");//  00000000000026ac  [ .text_nmplv ]

PROFILE_END();
end ".text_nmprofiler";
