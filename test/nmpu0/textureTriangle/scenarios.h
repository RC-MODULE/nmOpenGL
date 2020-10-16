#ifndef __SCENARIOS_H__
#define __SCENARIOS_H__

#if defined(__NMGLDEF_H__) || defined(__NM__) || defined(_MSC_VER)
	#include "nmgldef.h"
	#include "nmgltex_common.h"
	#include "nmgladapter.h"
	#define TEXTURE(x) teximage_ ## x
	extern "C" TexImage2D teximage_mytexture_256_256;
	extern "C" TexImage2D teximage_256_256;
	extern "C" TexImage2D teximage_mytexture;
	extern "C" TexImage2D teximage_128_128;
	extern "C" TexImage2D teximage_64_64;
	extern "C" TexImage2D teximage_32_32;
	extern "C" TexImage2D teximage_16_16;
	extern "C" TexImage2D teximage_8_8;
	extern "C" TexImage2D teximage_4_4;
	extern "C" TexImage2D teximage_2_2;
	extern "C" TexImage2D teximage_1_1;
#else
	#include <GL/glew.h>
	#define TEXTURE(x) # x ".bmp"
#endif

#ifdef __NM__
#define SECTION(sec) __attribute__((section(sec)))
#else
#define SECTION(sec)
#endif // __GNUC__

struct Scenario {
	const char *name;
	GLint wrapS;
	GLint wrapT;
	GLint minFilter;
	GLint magFilter;
	int texture_count;
#if defined(__NMGLDEF_H__) || defined(__NM__) || defined(_MSC_VER)
	TexImage2D texname[9];
#else
	const char *texname[9];
#endif
};

extern Scenario scenarios[48];

#endif //__SCENARIOS_H__
