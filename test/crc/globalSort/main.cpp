#include "nmpp.h"
#include "demo3d_common.h"
#include "stdio.h"
#include "minrep.h"
#include "time.h"


SECTION(".data_shared") v4nm32f src[4000];
SECTION(".data_shared") v4nm32f dst[4000];

void globalSegSort_v4nm32f(v4nm32f* src, v4nm32f* dst, int size);

int main()
{
	
	globalSegSort_v4nm32f(src, dst, 4000);
	return 0;
}
