#include "demo3d_nm1.h"

#include "nmgl.h"
#include "nmglvs_nmc1.h"

#include "link.h"

int main()
{ 
	nmglvsNm1Init();

	while (nmglvsNm1Run()) {
		nmglvsNm1Step();
	}
	return 0x611D611D;
} 

