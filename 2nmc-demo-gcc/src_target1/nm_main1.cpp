#include "demo3d_nm1.h"
#include "hal_target.h"

#include "nmgl.h"
#include "nmglvs_nmc1.h"

#include "nmprofiler.h"


#define TRACE_SIZE	0x10000

extern long trace_buffer[TRACE_SIZE];

int main()
{ 
	halSleep(500);
#if defined(__GNUC__) && defined(DEBUG)
	nmprofiler_init();
	nmprofiler_disable();
#endif // __GNUC__

	nmglvsNm1Init();

	halHostSync((int)trace_buffer);
	halHostSync((int)TRACE_SIZE);

	while (nmglvsNm1Run()) {
		nmglvsNm1Step();
	}
	return 0x611D611D;
} 

