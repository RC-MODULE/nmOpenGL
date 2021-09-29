#include "nmglvs_nmc1.h"
#include "demo3d_common.h"
#include "link.h"

#include "nmprofiler.h"


SECTION(".data_imu0") int linkWord[2];

int main()
{ 
	halLedOn(0);
#if defined(__GNUC__) && defined(DEBUG)
	//nmprofiler_init();
	//nmprofiler_disable();
#endif // __GNUC__
	nmglvsNm1Init();
	halLedOn(1);

	while (nmglvsNm1Run()) {
		halLedOn(2);
		nmglvsNm1Step();
		halLedOff(2);
	}
	return 0x611D611D;
} 

