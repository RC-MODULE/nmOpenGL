#ifndef __U_ASSERT_H__
#define __U_ASSERT_H__
#include "debugprint.h"
#include <cstdlib>

#ifdef NDEBUG
#  define uassert(condition) ((void)0)
#else
#  define uassert(expr) if(!(expr)) { DEBUG_PLOG_ERROR("Assertion \'%s\' failed\n", #expr); abort(); }
#endif

#ifdef NDEBUG
#   define m_uassert(condition) ((void)0)
#else
#   define m_uassert(expr, ...) if(!(expr)) { DEBUG_PLOG_ERROR("Assertion \'%s\' failed\t", #expr); DEBUG_PLOG_LEVEL_0(__VA_ARGS__); abort(); }
#endif


#endif //__U_ASSERT_H__