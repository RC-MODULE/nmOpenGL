#include "demo3d_nm1.h"

#include "nmgl.h"
#include "nmglvs_nmc1.h"

int main()
{ 
	nmglvsNm1Init();

#if defined(__GNUC__) && defined(PROFILER1)
	nmprofiler_init();
	nmprofiler_enable();
#endif // __GNUC__

	while (nmglvsNm1Run()) {
		nmglvsNm1Step();
	}

#if defined(__GNUC__) && defined(PROFILER1)
	nmprofiler_disable();
#endif // __GNUC__
	return 0x611D611D;
} 

