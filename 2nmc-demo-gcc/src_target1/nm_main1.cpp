#include "nmglvs_nmc1.h"

#include "nmprofiler.h"

int main()
{
	printf("nm1 start\n");
#if defined(__GNUC__) && defined(DEBUG)
	//nmprofiler_init();
	//nmprofiler_disable();
#endif // __GNUC__

	nmglvsNm1Init();

	while (nmglvsNm1Run()) {
		nmglvsNm1Step();
	}
	return 0x611D611D;
} 

