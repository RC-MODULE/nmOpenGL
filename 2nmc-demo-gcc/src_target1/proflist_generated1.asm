//****************************************************************************************
// This is automaticly generated asm-file by profiler-gcc.py
//****************************************************************************************
import from "nmprofiler.mlb";
begin ".text_nmprofiler"
PROFILE_BEGIN( 16 );
	FUNCTION( __Z13getAddrPtrnsTP12DataForNmpu1 ,                "getAddrPtrnsTP12");//  0000000000001f18  [ .text_demo3d ]

	//FUNCTION( hw_init_hook ,      "hw_init_hook    ");//  0000000022000/* 016  [ .text ]
	//FUNCTION( __Z13getAddrPtrnsPP11PolygonsOld ,                 "getAddrPtrnsPP11");//  00000000220003c6  [ .text ]
	//FUNCTION( __ZN11DepthBuffer6updateEv ,                     "N11DepthBuffer6u");//  00000000220003d6  [ .text ]
	//FUNCTION( _halCopyRISC ,       "halCopyRISC     ");//  00000000220004ba  [ .text ]
	//NONCFUNC( LShift32 ,          "LShift32        ");//  0000000022000d8a  [ .text ]
	//FUNCTION( LShift64 ,          "LShift64        ");//  0000000022000e16  [ .text ]
	//NONCFUNC( Mul32 ,             "Mul32           ");//  0000000022000f32  [ .text ]
	//NONCFUNC( Mul32Ex ,           "Mul32Ex         ");//  0000000022000f4c  [ .text ]
	//FUNCTION( Mul64 ,             "Mul64           ");//  0000000022000f68  [ .text ]
	//NONCFUNC( ARShift32 ,         "ARShift32       ");//  0000000022001014  [ .text ]
	//NONCFUNC( DAdd ,              "DAdd            ");//  00000000220010a0  [ .text ]
	//NONCFUNC( DFrExp ,            "DFrExp          ");//  0000000022001160  [ .text ]
	//NONCFUNC( DCmp ,              "DCmp            ");//  00000000220012b2  [ .text ]
	//NONCFUNC( DDiv ,              "DDiv            ");//  0000000022001304  [ .text ]
	//NONCFUNC( UDiv32 ,            "UDiv32          ");//  00000000220013c8  [ .text ]
	//NONCFUNC( IDiv32 ,            "IDiv32          ");//  00000000220013da  [ .text ]
	//FUNCTION( IDiv64 ,            "IDiv64          ");//  0000000022001448  [ .text ]
	//FUNCTION( UDiv64 ,            "UDiv64          ");//  00000000220014ae  [ .text ]
	//NONCFUNC( DMul ,              "DMul            ");//  00000000220014cc  [ .text ]
	//NONCFUNC( ConvDtoI32 ,        "ConvDtoI32      ");//  000000002200157e  [ .text ]
	//NONCFUNC( ConvI32toD ,        "ConvI32toD      ");//  00000000220015de  [ .text ]
	//NONCFUNC( UMod32 ,            "UMod32          ");//  00000000220015fa  [ .text ]
	//NONCFUNC( IMod32 ,            "IMod32          ");//  0000000022001626  [ .text ]
	//FUNCTION( UMod64 ,            "UMod64          ");//  000000002200165c  [ .text ]
	//FUNCTION( IMod64 ,            "IMod64          ");//  0000000022001698  [ .text ]
	//NONCFUNC( RShift32 ,          "RShift32        ");//  00000000220016e4  [ .text ]
	//FUNCTION( RShift64 ,          "RShift64        ");//  0000000022001770  [ .text ]
	//NONCFUNC( ConvU32toD ,        "ConvU32toD      ");//  000000002200188c  [ .text ]
	//FUNCTION( _duplicate_16s ,     "duplicate_16s   ");//  0000000022001d3e  [ .text_dmo3d ]
	//FUNCTION( __Z16copyCounterColorv ,                           "copyCounterColor");//  0000000000000200  [ .text_demo3d ]
	//FUNCTION( __Z16copyCounterDepthv ,                           "copyCounterDepth");//  0000000000000212  [ .text_demo3d ]
	//FUNCTION( __Z9swapImagev ,        "swapImagev      ");//  0000000000000224  [ .text_demo3d ]
	//FUNCTION( __Z9drawLinesv ,        "drawLinesv      ");//  0000000000000270  [ .text_demo3d ]
	//FUNCTION( __Z10drawPointsv ,       "drawPointsv     ");//  000000000000043c  [ .text_demo3d ]
	//FUNCTION( __Z13drawTrianglesv ,    "drawTrianglesv  ");//  0000000000000608  [ .text_demo3d ]
	//FUNCTION( __Z13getAddrPtrnsLP11PolygonsOld ,                 "getAddrPtrnsLP11");//  000000000000083a  [ .text_demo3d ]
	//FUNCTION( __Z13getAddrPtrnsTP11PolygonsOld ,                 "getAddrPtrnsTP11");//  000000000000084a  [ .text_demo3d ]
	//FUNCTION( __Z8cbUpdatev ,         "cbUpdatev       ");//  0000000000000ccc  [ .text_demo3d ]
	//FUNCTION( __Z7msdInitv ,          "msdInitv        ");//  0000000000000e12  [ .text_demo3d ]
	//FUNCTION( __Z6msdAddPKvPvii ,            "msdAdd          ");//  000000000000101c  [ .text_demo3d ]
	//FUNCTION( __Z8msdAdd2DPKvPvjjjji ,          "msdAdd2D        ");//  00000000000011fe  [ .text_demo3d ]
	//FUNCTION( __Z6msdAddR9MyDmaTaski ,                          "msdAddR9MyDmaTas");//  00000000000013fa  [ .text_demo3d ]
	//FUNCTION( __Z16msdGetStatusCopyii ,                          "msdGetStatusCopy");//  00000000000016ea  [ .text_demo3d ]
	//FUNCTION( __Z10msdWaitDmav ,       "msdWaitDmav     ");//  000000000000170e  [ .text_demo3d ]
	//FUNCTION( __Z10msdWaitDmai ,       "msdWaitDmai     ");//  00000000000017de  [ .text_demo3d ]
	//FUNCTION( __Z13getAddrPtrnsLP12DataForNmpu1 ,                "getAddrPtrnsLP12");//  000000000000188e  [ .text_demo3d ]
	//FUNCTION( __Z13getAddrPtrnsPP12DataForNmpu1 ,                "getAddrPtrnsPP12");//  0000000000001b66  [ .text_demo3d ]
	//FUNCTION( __Z14selectPatternsPiS_S_S_S_PS_iS_ ,              "selectPatternsPi");//  000000000000259a  [ .text_demo3d ]
	//FUNCTION( _absIfNegElse0 ,     "absIfNegElse0   ");//  0000000000002616  [ .text_demo3d ]
	//FUNCTION( _baseAddrOffs_16u ,  "baseAddrOffs_16u");//  00000000000026b4  [ .text_demo3d ]
	////FUNCTION( _baseAddrOffs_16s ,  "baseAddrOffs_16s");//  00000000000026b4  [ .text_demo3d ]
	//FUNCTION( _baseAddrOffs_32u ,  "baseAddrOffs_32u");//  0000000000002764  [ .text_demo3d ]
	////FUNCTION( _baseAddrOffs_32s ,  "baseAddrOffs_32s");//  0000000000002764  [ .text_demo3d ]
	//FUNCTION( _convertABGR32_RGB565 ,                        "convertABGR32_RG");//  0000000000002780  [ .text_demo3d ]
	//FUNCTION( _convertRGB565_RGB8888 ,                       "convertRGB565_RG");//  000000000000281c  [ .text_demo3d ]
	//FUNCTION( _copyPacket_32u ,    "copyPacket_32u  ");//  00000000000028ca  [ .text_demo3d ]
	////FUNCTION( _copyPacket_32s ,    "copyPacket_32s  ");//  00000000000028ca  [ .text_demo3d ]
	//FUNCTION( _depthTest16 ,       "depthTest16     ");//  0000000000002968  [ .text_demo3d ]
	//FUNCTION( _depthTest32 ,       "depthTest32     ");//  000000000000299a  [ .text_demo3d ]
	//FUNCTION( _totalSum ,          "totalSum        ");//  00000000000029cc  [ .text_demo3d ]
	//FUNCTION( _inverse_8u ,        "inverse_8u      ");//  00000000000029de  [ .text_demo3d ]
	//FUNCTION( _inverse_v4nm8u ,    "inverse_v4nm8u  ");//  0000000000002a88  [ .text_demo3d ]
	//FUNCTION( _mAndVxN_32u ,       "mAndVxN_32u     ");//  0000000000002aa6  [ .text_demo3d ]
	//FUNCTION( _merge_v4nm32s ,     "merge_v4nm32s   ");//  0000000000002b50  [ .text_demo3d ]
	//FUNCTION( _mergePtrnsAddr3 ,   "mergePtrnsAddr3 ");//  0000000000002b9e  [ .text_demo3d ]
	//FUNCTION( _mMaskVxN_32s ,      "mMaskVxN_32s    ");//  0000000000002cd2  [ .text_demo3d ]
	//FUNCTION( _mMulCVxN_2s_RGB8888 ,                         "mMulCVxN_2s_RGB8");//  0000000000003120  [ .text_demo3d ]
	//FUNCTION( _mMulCVxN_2s32s ,    "mMulCVxN_2s32s  ");//  0000000000003138  [ .text_demo3d ]
	//FUNCTION( _selectPaintSide ,   "selectPaintSide ");//  000000000000359a  [ .text_demo3d ]
	//FUNCTION( __Z13nmglvsNm1Initv ,    "nmglvsNm1Initv  ");//  000000002200fc5a  [ .text_nmglvs ]
	//FUNCTION( __Z12nmglvsNm1Runv ,     "nmglvsNm1Runv   ");//  000000002201022a  [ .text_nmglvs ]
	//FUNCTION( __Z13nmglvsNm1Stepv ,    "nmglvsNm1Stepv  ");//  000000002201023c  [ .text_nmglvs ]
	//FUNCTION( _halSetProcessorNo ,                           "halSetProcessorN");//  0000000022010df2  [ .text_hal ]
	//FUNCTION( _halHandshake ,      "halHandshake    ");//  0000000022010dfe  [ .text_hal ]
	//FUNCTION( _halSync ,           "halSync         ");//  0000000022010e7a  [ .text_hal ]
	//FUNCTION( _halSyncAddr ,       "halSyncAddr     ");//  0000000022010f5c  [ .text_hal ]
	//FUNCTION( _halSyncArray ,      "halSyncArray    ");//  0000000022010f84  [ .text_hal ]
	//FUNCTION( _halInstrCacheEnable ,                         "halInstrCacheEna");//  0000000022010fee  [ .text_hal ]
	//FUNCTION( _halInstrCacheDisable ,                        "halInstrCacheDis");//  0000000022010ff4  [ .text_hal ]
	//FUNCTION( _halDma2D_IsCompleted ,                        "halDma2D_IsCompl");//  0000000022011042  [ .text_hal ]
	//FUNCTION( _halDma2D_Start ,    "halDma2D_Start  ");//  000000002201104e  [ .text_hal ]
	//FUNCTION( _halDma2D_StartA ,   "halDma2D_StartA ");//  00000000220110cc  [ .text_hal ]
	//FUNCTION( _halDma2D_StartM ,   "halDma2D_StartM ");//  000000002201111e  [ .text_hal ]
	//FUNCTION( _halDma2D_StartMA ,  "halDma2D_StartMA");//  0000000022011190  [ .text_hal ]
	//FUNCTION( _halDma2D_StartC ,   "halDma2D_StartC ");//  00000000220111d6  [ .text_hal ]
	//FUNCTION( _halDma2D_StartCA ,  "halDma2D_StartCA");//  0000000022011288  [ .text_hal ]
	//FUNCTION( _halDma2D_StartCM ,  "halDma2D_StartCM");//  000000002201130e  [ .text_hal ]
	//FUNCTION( _halDma2D_StartCMA ,                           "halDma2D_StartCM");//  00000000220113b4  [ .text_hal ]
	//FUNCTION( _TEST ,              "TEST            ");//  0000000022011432  [ .text_hal ]
	//FUNCTION( _halSetCallbackDMA ,                           "halSetCallbackDM");//  00000000220114c6  [ .text_hal ]
	////FUNCTION( _halDmaSetCallback ,                           "halDmaSetCallbac");//  00000000220114c6  [ .text_hal ]
	//FUNCTION( _halGetCallbackDMA ,                           "halGetCallbackDM");//  00000000220114cc  [ .text_hal ]
	////FUNCTION( _halDmaGetCallback ,                           "halDmaGetCallbac");//  00000000220114cc  [ .text_hal ]
	//FUNCTION( _halEnbExtInt ,      "halEnbExtInt    ");//  00000000220114d0  [ .text_hal ]
	//FUNCTION( _halDisExtInt ,      "halDisExtInt    ");//  00000000220114d6  [ .text_hal ]
	//FUNCTION( _halOpenDMA ,        "halOpenDMA      ");//  00000000220114dc  [ .text_hal ]
	////FUNCTION( _halDmaInit ,        "halDmaInit      ");//  00000000220114dc  [ .text_hal ]
	//FUNCTION( _halDmaInitC ,       "halDmaInitC     ");//  000000002201151a  [ .text_hal ]
	//FUNCTION( _halStatusDMA ,      "halStatusDMA    ");//  0000000022011524  [ .text_hal ]
	//FUNCTION( _halDmaIsCompleted ,                           "halDmaIsComplete");//  0000000022011536  [ .text_hal ]
	//FUNCTION( _halMaskIntContMdma_mc12101 ,                  "halMaskIntContMd");//  000000002201153c  [ .text_hal ]
	//FUNCTION( _GetPswr ,           "GetPswr         ");//  0000000022011544  [ .text_hal ]
	//FUNCTION( _SetFlagDMA ,        "SetFlagDMA      ");//  0000000022011548  [ .text_hal ]
	//FUNCTION( _halLockDMA ,        "halLockDMA      ");//  0000000022011550  [ .text_hal ]
	//FUNCTION( _halUnlockDMA ,      "halUnlockDMA    ");//  0000000022011558  [ .text_hal ]
	//FUNCTION( _halIsBusyDMA ,      "halIsBusyDMA    ");//  000000002201155e  [ .text_hal ]
	//FUNCTION( _halWhereMirror ,    "halWhereMirror  ");//  0000000022011564  [ .text_hal ]
	//FUNCTION( _halGetCoreId ,      "halGetCoreId    ");//  0000000022011570  [ .text_hal ]
	//FUNCTION( _halReadCoreID ,     "halReadCoreID   ");//  0000000022011576  [ .text_hal ]
	//FUNCTION( _halSetMirror ,      "halSetMirror    ");//  000000002201157c  [ .text_hal ]
	//FUNCTION( _halOpenStatusDMA ,  "halOpenStatusDMA");//  000000002201158c  [ .text_hal ]
	//FUNCTION( SOS ,               "SOS             ");//  000000002201159e  [ .text_hal ]
	//FUNCTION( _halInitSingleDMA ,  "halInitSingleDMA");//  00000000220115f2  [ .text_hal ]
	//FUNCTION( _halDmaStart ,       "halDmaStart     ");//  000000002201167e  [ .text_hal ]
	//FUNCTION( _halDmaStartA ,      "halDmaStartA    ");//  00000000220116d6  [ .text_hal ]
	//FUNCTION( _halDmaStartM ,      "halDmaStartM    ");//  0000000022011718  [ .text_hal ]
	//FUNCTION( _halDmaStartMA ,     "halDmaStartMA   ");//  0000000022011764  [ .text_hal ]
	//FUNCTION( _halDmaStartC ,      "halDmaStartC    ");//  000000002201179a  [ .text_hal ]
	//FUNCTION( _halDmaStartCA ,     "halDmaStartCA   ");//  0000000022011826  [ .text_hal ]
	//FUNCTION( _halDmaStartCM ,     "halDmaStartCM   ");//  000000002201189c  [ .text_hal ]
	//FUNCTION( _halDmaStartCMA ,    "halDmaStartCMA  ");//  000000002201191c  [ .text_hal ]
	//FUNCTION( _halMapAddrFrom ,    "halMapAddrFrom  ");//  0000000022011986  [ .text_hal ]
	//FUNCTION( _halMapAddrTo ,      "halMapAddrTo    ");//  000000002201199a  [ .text_hal ]
	//FUNCTION( halSleep ,          "halSleep        ");//  00000000220119aa  [ .text_hal ]
	//FUNCTION( _halSleep ,          "halSleep        ");//  00000000220119c2  [ .text_hal ]
	//FUNCTION( _halEnterCriticalSectionCore ,                 "halEnterCritical");//  00000000220119ce  [ .text_hal ]
	//FUNCTION( _halExitCriticalSectionCore ,                  "halExitCriticalS");//  0000000022011a0a  [ .text_hal ]
	//FUNCTION( _halEnterCriticalSection ,                     "halEnterCritical");//  0000000022011a1a  [ .text_hal ]
	//FUNCTION( _halExitCriticalSection ,                      "halExitCriticalS");//  0000000022011a56  [ .text_hal ]
	//FUNCTION( _depthTestCore32_A_r ,                         "depthTestCore32_");//  0000000022011a68  [ .text_demo3dExt ]
	////FUNCTION( _depthTestCore16_A_r ,                         "depthTestCore16_");//  0000000022011a68  [ .text_demo3dExt ]
	//FUNCTION( _depthTestCore32_A_rw ,                        "depthTestCore32_");//  0000000022011e96  [ .text_demo3dExt ]
	////FUNCTION( _depthTestCore16_A_rw ,                        "depthTestCore16_");//  0000000022011e96  [ .text_demo3dExt ]
	//FUNCTION( _depthTestCore16_Gt_r ,                        "depthTestCore16_");//  00000000220122c4  [ .text_demo3dExt ]
	////FUNCTION( _depthTestCore32_Gt_r ,                        "depthTestCore32_");//  00000000220122c4  [ .text_demo3dExt ]
	//FUNCTION( _depthTestCore16_Gt_rw ,                       "depthTestCore16_");//  00000000220126f2  [ .text_demo3dExt ]
	////FUNCTION( _depthTestCore32_Gt_rw ,                       "depthTestCore32_");//  00000000220126f2  [ .text_demo3dExt ]
	//FUNCTION( _depthTestCore16_Lt_r ,                        "depthTestCore16_");//  0000000022012b20  [ .text_demo3dExt ]
	////FUNCTION( _depthTestCore32_Lt_r ,                        "depthTestCore32_");//  0000000022012b20  [ .text_demo3dExt ]
	//FUNCTION( _depthTestCore16_Lt_rw ,                       "depthTestCore16_");//  0000000022012f4e  [ .text_demo3dExt ]
	////FUNCTION( _depthTestCore32_Lt_rw ,                       "depthTestCore32_");//  0000000022012f4e  [ .text_demo3dExt ]
	//FUNCTION( _depthTestCore16_N_r ,                         "depthTestCore16_");//  000000002201337c  [ .text_demo3dExt ]
	////FUNCTION( _depthTestCore32_N_r ,                         "depthTestCore32_");//  000000002201337c  [ .text_demo3dExt ]
	//FUNCTION( _depthTestCore32_N_rw ,                        "depthTestCore32_");//  00000000220137aa  [ .text_demo3dExt ]
	////FUNCTION( _depthTestCore16_N_rw ,                        "depthTestCore16_");//  00000000220137aa  [ .text_demo3dExt ]
	//FUNCTION( _nmppsAddC_32s ,     "nmppsAddC_32s   ");//  00000000000052c6  [ .text_nmplv ]
	//FUNCTION( _nmppsAdd_32s ,      "nmppsAdd_32s    ");//  00000000000052e8  [ .text_nmplv ]
	//FUNCTION( _nmppsMulC_32s ,     "nmppsMulC_32s   ");//  0000000000005302  [ .text_nmplv ]
	//FUNCTION( _nmppsMulC_AddC32s ,                           "nmppsMulC_AddC32");//  0000000000005326  [ .text_nmplv ]
	////FUNCTION( _nmppsMulC_AddC_32s ,                          "nmppsMulC_AddC_3");//  0000000000005326  [ .text_nmplv ]
	//FUNCTION( _nmppsMulC_AddV_AddC_32s ,                     "nmppsMulC_AddV_A");//  000000000000534e  [ .text_nmplv ]
	////FUNCTION( _nmppsMulC_AddV_AddC32s ,                      "nmppsMulC_AddV_A");//  000000000000534e  [ .text_nmplv ]
	//FUNCTION( _nmppsSubC_32s ,     "nmppsSubC_32s   ");//  000000000000537c  [ .text_nmplv ]
	//FUNCTION( _nmppsSub_32s ,      "nmppsSub_32s    ");//  0000000000005398  [ .text_nmplv ]
	//FUNCTION( _nmppsAnd_32u ,      "nmppsAnd_32u    ");//  00000000000053b2  [ .text_nmplv ]
	//FUNCTION( _nmppsAndC_32u ,     "nmppsAndC_32u   ");//  00000000000053c8  [ .text_nmplv ]
	//FUNCTION( _nmppsOr_32u ,       "nmppsOr_32u     ");//  00000000000053e0  [ .text_nmplv ]
	//FUNCTION( _nmppsAnd4V_64u ,    "nmppsAnd4V_64u  ");//  00000000000053f6  [ .text_nmplv ]
	//FUNCTION( _nmppsSet_32s ,      "nmppsSet_32s    ");//  0000000000005412  [ .text_nmplv ]
	//FUNCTION( _nmppsMerge_32s ,    "nmppsMerge_32s  ");//  0000000000005426  [ .text_nmplv ]
	//FUNCTION( _nmppsConvert_32s8s ,                          "nmppsConvert_32s");//  0000000000005460  [ .text_nmplv ]
	//FUNCTION( _nmppsCopy_32s ,     "nmppsCopy_32s   ");//  0000000000005490  [ .text_nmplv ]
	//FUNCTION( _nmppsMinMaxEvery_32s ,                        "nmppsMinMaxEvery");//  00000000000054a0  [ .text_nmplv ]
	//FUNCTION( _nmppsClipCC_32s ,   "nmppsClipCC_32s ");//  00000000000054c4  [ .text_nmplv ]
	//FUNCTION( _nmppsCmpEq0_32u31b ,                          "nmppsCmpEq0_32u3");//  00000000000054f4  [ .text_nmplv ]
	//FUNCTION( _nmppsMaxEvery_32s ,                           "nmppsMaxEvery_32");//  0000000000005654  [ .text_nmplv ]
	//FUNCTION( _nmppsMinEvery_32s ,                           "nmppsMinEvery_32");//  0000000000005674  [ .text_nmplv ]
	//FUNCTION( _nmppsRemap_32u ,    "nmppsRemap_32u  ");//  0000000000005694  [ .text_nmplv ]
	//FUNCTION( _nmppsCmpEq0_8u7b ,  "nmppsCmpEq0_8u7b");//  00000000000056b8  [ .text_nmplv ]
	//FUNCTION( _nmppmCopy_32u ,     "nmppmCopy_32u   ");//  0000000000005756  [ .text_nmplm ]
	////FUNCTION( _nmppmCopy_32s ,     "nmppmCopy_32s   ");//  0000000000005756  [ .text_nmplm ]
	//FUNCTION( mtr_CpyCore_nm64 ,  "mtr_CpyCore_nm64");//  0000000000005774  [ .text_nmplm ]
PROFILE_END();
end ".text_nmprofiler";
