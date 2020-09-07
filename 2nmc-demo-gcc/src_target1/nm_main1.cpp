#include "demo3d_nm1.h"

#include "nmgl.h"
#include "nmglvs_nmc1.h"
#include "nmprofiler.h"

int main()
{ 
	nmglvsNm1Init();

	nmprofiler_init();
	nmprofiler_disable();

	while (nmglvsNm1Run()) {
		nmglvsNm1Step();
	}
	return 0x611D611D;
} 

