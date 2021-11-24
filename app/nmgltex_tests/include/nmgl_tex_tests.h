#ifndef NMGL_TEX_TESTS_H_
#define NMGL_TEX_TESTS_H_

int run_nmglActiveTexture_test();
int run_nmglClientActiveTexture_test();
int run_nmglMultiTexCoord2f_test();
int run_nmglMultiTexCoord2fv_test();
int run_nmglTexEnvfv_test();
int run_nmglTexImage2D_test();
int run_nmglTexSubImage2D_test();
int run_nmglGetTexEnvfv_test();
int run_nmglGetTexParameteriv_test();
int run_nmglTexEnvi_test();
int run_nmglGetTexEnviv_test();
int run_nmglTexParameteri_test();
int run_nmglGenTextures_test();
int run_nmglBindTexture_test();
int run_nmglTexCoordPointer_test();
int run_nmglPixelStorei_test();
int run_nmglLineWidth_test();
int run_nmglLineStipple_test();
int run_nmglPolygonStipple_test();
int run_nmglGetPolygonStipple_test();
int run_nmglPolygonOffset_test();
int run_nmglBlendFunc_test();
int run_nmglGetString_test();
int run_nmglGetBooleanv_test();
int run_nmglIsEnabled_test();
int run_nmglGetPointerv_test();
int run_nmglGetLightfv_test();
int run_nmglAlphaFunc_test();
int run_nmglColorMask_test();


//EXT_paletted_texture
int run_nmglColorTableEXT_test();
int run_nmglColorSubTableEXT_test();
int run_nmglGetColorTableEXT_test();
int run_nmglGetColorTableParameterivEXT_test();

#endif /* NMGL_TEX_TESTS_ */