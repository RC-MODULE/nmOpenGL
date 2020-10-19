#include "scenarios.h"
SECTION(".text") Scenario scenarios[17] = { 
	// magFilter tests
	{{0.0f, 0.3125f, 0.46875f, 0.9375f, 0.8125f, 0.15625f}, GL_CLAMP_TO_EDGE,	GL_CLAMP_TO_EDGE,	GL_NEAREST,					GL_NEAREST, 	GL_MODULATE, 1, {TEXTURE(4_4)}}, 
	{{0.0f, 0.3125f, 0.46875f, 0.9375f, 0.8125f, 0.15625f}, GL_CLAMP_TO_EDGE,	GL_CLAMP_TO_EDGE,	GL_NEAREST,					GL_LINEAR, 		GL_MODULATE, 1, {TEXTURE(4_4)}}, 
	// minFilter tests
	{{0.0f, 0.3125f, 0.46875f, 0.9375f, 0.8125f, 0.15625f}, GL_CLAMP_TO_EDGE,	GL_CLAMP_TO_EDGE,	GL_LINEAR,					GL_NEAREST, 	GL_MODULATE, 1, {TEXTURE(256_256)}},
	{{0.0f, 0.3125f, 0.46875f, 0.9375f, 0.8125f, 0.15625f}, GL_CLAMP_TO_EDGE,	GL_CLAMP_TO_EDGE,	GL_NEAREST,					GL_NEAREST, 	GL_MODULATE, 1, {TEXTURE(256_256)}},
	{{0.0f, 0.3125f, 0.46875f, 0.9375f, 0.8125f, 0.15625f}, GL_CLAMP_TO_EDGE,	GL_CLAMP_TO_EDGE,	GL_NEAREST_MIPMAP_NEAREST,	GL_NEAREST, 	GL_MODULATE, 9, {TEXTURE(mytexture_256_256), TEXTURE(128_128), TEXTURE(64_64), TEXTURE(32_32), TEXTURE(16_16), TEXTURE(8_8), TEXTURE(4_4), TEXTURE(2_2), TEXTURE(1_1)}},
	{{0.0f, 0.3125f, 0.46875f, 0.9375f, 0.8125f, 0.15625f}, GL_CLAMP_TO_EDGE,	GL_CLAMP_TO_EDGE,	GL_NEAREST_MIPMAP_LINEAR,	GL_NEAREST, 	GL_MODULATE, 9, {TEXTURE(mytexture_256_256), TEXTURE(128_128), TEXTURE(64_64), TEXTURE(32_32), TEXTURE(16_16), TEXTURE(8_8), TEXTURE(4_4), TEXTURE(2_2), TEXTURE(1_1)}},
	{{0.0f, 0.3125f, 0.46875f, 0.9375f, 0.8125f, 0.15625f}, GL_CLAMP_TO_EDGE,	GL_CLAMP_TO_EDGE,	GL_LINEAR_MIPMAP_NEAREST,	GL_NEAREST, 	GL_MODULATE, 9, {TEXTURE(mytexture_256_256), TEXTURE(128_128), TEXTURE(64_64), TEXTURE(32_32), TEXTURE(16_16), TEXTURE(8_8), TEXTURE(4_4), TEXTURE(2_2), TEXTURE(1_1)}},
	{{0.0f, 0.3125f, 0.46875f, 0.9375f, 0.8125f, 0.15625f}, GL_CLAMP_TO_EDGE,	GL_CLAMP_TO_EDGE,	GL_LINEAR_MIPMAP_LINEAR,	GL_NEAREST, 	GL_MODULATE, 9, {TEXTURE(mytexture_256_256), TEXTURE(128_128), TEXTURE(64_64), TEXTURE(32_32), TEXTURE(16_16), TEXTURE(8_8), TEXTURE(4_4), TEXTURE(2_2), TEXTURE(1_1)}},
	// wrapS tests
	{{0.0f, 0.625f,  0.9375f,  1.87f,   1.625f,  0.3125f},  GL_CLAMP_TO_EDGE,	GL_CLAMP_TO_EDGE,	GL_NEAREST,					GL_NEAREST, 	GL_MODULATE, 1, {TEXTURE(4_4)}}, 
	{{0.0f, 0.625f,  0.9375f,  1.87f,   1.625f,  0.3125f},  GL_REPEAT,			GL_CLAMP_TO_EDGE,	GL_LINEAR,					GL_LINEAR, 		GL_MODULATE, 1, {TEXTURE(4_4)}}, 
	// wrapT tests
	{{0.0f, 0.625f,  0.9375f,  1.87f,   1.625f,  0.3125f},  GL_CLAMP_TO_EDGE,	GL_CLAMP_TO_EDGE,	GL_NEAREST,					GL_NEAREST, 	GL_MODULATE, 1, {TEXTURE(4_4)}}, 
	{{0.0f, 0.625f,  0.9375f,  1.87f,   1.625f,  0.3125f},  GL_CLAMP_TO_EDGE,	GL_REPEAT,			GL_LINEAR,					GL_LINEAR, 		GL_MODULATE, 1, {TEXTURE(4_4)}}, 
	// texFunction tests
	{{0.0f, 0.3125f, 0.46875f, 0.9375f, 0.8125f, 0.15625f}, GL_CLAMP_TO_EDGE,	GL_CLAMP_TO_EDGE,	GL_NEAREST,					GL_NEAREST, 	GL_MODULATE, 1, {TEXTURE(mytexture_256_256)}},
	{{0.0f, 0.3125f, 0.46875f, 0.9375f, 0.8125f, 0.15625f}, GL_CLAMP_TO_EDGE,	GL_CLAMP_TO_EDGE,	GL_NEAREST,					GL_NEAREST, 	GL_REPLACE,	 1, {TEXTURE(mytexture_256_256)}},
	{{0.0f, 0.3125f, 0.46875f, 0.9375f, 0.8125f, 0.15625f}, GL_CLAMP_TO_EDGE,	GL_CLAMP_TO_EDGE,	GL_NEAREST,					GL_NEAREST, 	GL_BLEND,	 1, {TEXTURE(mytexture_256_256)}},
	{{0.0f, 0.3125f, 0.46875f, 0.9375f, 0.8125f, 0.15625f}, GL_CLAMP_TO_EDGE,	GL_CLAMP_TO_EDGE,	GL_NEAREST,					GL_NEAREST, 	GL_DECAL,	 1, {TEXTURE(mytexture_256_256)}},
	{{0.0f, 0.3125f, 0.46875f, 0.9375f, 0.8125f, 0.15625f}, GL_CLAMP_TO_EDGE,	GL_CLAMP_TO_EDGE,	GL_NEAREST,					GL_NEAREST, 	GL_ADD,		 1, {TEXTURE(mytexture_256_256)}},
};

