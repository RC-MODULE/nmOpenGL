#include "demo3d_nm1.h"

#include "nmgl.h"
#include "nmglvs_nmc1.h"
#include "nmprofiler.h"

int main()
{ 
	halSleep(3000);
	nmglvsNm1Init();
#if defined(PROFILER1) && defined(__GNUC__)
	nmprofiler_init();
	nmprofiler_disable();
#endif // (PROFILER1) && defined(__GNUC__)
	while (nmglvsNm1Run()) {
		nmglvsNm1Step();
	}
	return 0x611D611D;
} 

