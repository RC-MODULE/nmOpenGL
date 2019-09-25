#include "demo3d_common.h"
#include "nmgl_data0.h"


#pragma code_section ".text_demo3d"
int nmglvsNm0Run()
{
	if(nmglSynchro->exit_nm != SYNCHRO_EXIT){
		return 1;
	}
	else{
		return 0;
	}
} 

