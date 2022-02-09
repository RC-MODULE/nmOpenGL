#ifndef __DEBUG_PRINT_H__
#define __DEBUG_PRINT_H__

#include "stdio.h"

/*#ifdef __QEMU_TEST
#include "stdio.h"
#else

#ifdef __cplusplus
extern "C"{
#include "nm_printf.h"
}
#else
#include "nm_printf.h"
#endif

#endif //*/


#define DEBUG_PLOG_ERROR(...)    { printf("%s:%d: error: ", __FILE__, __LINE__); printf(__VA_ARGS__); }

#ifdef DEBUG_LEVEL

//#define DEBUG_PLOG_LEVEL_0(...)  if(DEBUG_LEVEL > 0) { printf("%s:%d: ", __FILE__, __LINE__); printf(__VA_ARGS__); }
//#define DEBUG_PLOG_LEVEL_1(...)  if(DEBUG_LEVEL > 1) { printf("%s:%d: ", __FILE__, __LINE__); printf(__VA_ARGS__); }
//#define DEBUG_PLOG_LEVEL_2(...)  if(DEBUG_LEVEL > 2) { printf("%s:%d: ", __FILE__, __LINE__); printf(__VA_ARGS__); }
//#define DEBUG_PLOG_LEVEL_3(...)  if(DEBUG_LEVEL > 3) { printf("%s:%d: ", __FILE__, __LINE__); printf(__VA_ARGS__); }
//#define DEBUG_PLOG_LEVEL_4(...)  if(DEBUG_LEVEL > 4) { printf("%s:%d: ", __FILE__, __LINE__); printf(__VA_ARGS__); }
//#define DEBUG_PLOG_LEVEL_5(...)  if(DEBUG_LEVEL > 5) { printf("%s:%d: ", __FILE__, __LINE__); printf(__VA_ARGS__); }

#define DEBUG_PLOG_FILE()        if(DEBUG_LEVEL > 0) { printf("%s: \n", __FILE__); }
#define DEBUG_PLOG_LEVEL_0(...)  if(DEBUG_LEVEL > 0) { printf(__VA_ARGS__); }
#define DEBUG_PLOG_LEVEL_1(...)  if(DEBUG_LEVEL > 1) { printf(__VA_ARGS__); }
#define DEBUG_PLOG_LEVEL_2(...)  if(DEBUG_LEVEL > 2) { printf(__VA_ARGS__); }
#define DEBUG_PLOG_LEVEL_3(...)  if(DEBUG_LEVEL > 3) { printf(__VA_ARGS__); }
#define DEBUG_PLOG_LEVEL_4(...)  if(DEBUG_LEVEL > 4) { printf(__VA_ARGS__); }
#define DEBUG_PLOG_LEVEL_5(...)  if(DEBUG_LEVEL > 5) { printf(__VA_ARGS__); }

#else
#define DEBUG_PLOG_FILE()     
#define DEBUG_PLOG_LEVEL_0(...)  
#define DEBUG_PLOG_LEVEL_1(...)  
#define DEBUG_PLOG_LEVEL_2(...)  
#define DEBUG_PLOG_LEVEL_3(...)  
#define DEBUG_PLOG_LEVEL_4(...)  
#define DEBUG_PLOG_LEVEL_5(...)  

#endif //DEBUG_LEVEL



#endif //__DEBUG_PRINT_H__