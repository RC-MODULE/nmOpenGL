#include "utility.h"
#include "context.h"


typedef struct{

	PoolPointer *pools;
	int poolsCount;
	int poolSize32;


} NMGL_CoreContextFloat;

#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

	NMGL_CoreContextFloat *getCoreContextFloat(void);

#ifdef __cplusplus
}
#endif //__cplusplus

