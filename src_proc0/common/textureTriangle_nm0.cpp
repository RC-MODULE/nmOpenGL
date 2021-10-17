#include "demo3d_common.h"
#include "nmblas.h"
#include "demo3d_nm0.h"
#include "nmgltex_nm0.h"
#include "nmgltex_common.h"
#include <stdio.h>
#include <math.h>
#include <float.h> //TODO: only FLT_EPSILON is used from float.h
#include "filter.h"

#define TEXTURE_TRIANGLE_SECTION ".text"

//#define PERSPECTIVE_CORRECT

namespace nm0_version {

typedef enum { MINIFICATION, MAGNIFICATION } lod_t;

typedef struct Vec2f {
    float x;
    float y;
} Vec2f;

typedef struct Vec3f {
    float x;
    float y;
    float z;
} Vec3f;

typedef struct color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} color;

float c = 0.0; // minification vs. magnification switchover point, look glspec 1.3, chapter 3.8.8
lod_t minMagFlag = MINIFICATION;
unsigned int borderWidth = 0; //TEXTURE BORDER - texture image's specified border width
int textureBaseLevel = 0;
int textureMaxLevel = 1000;
color borderColor;
const float scaleCoeff1_255 = 1/255.0;

extern float initVecx [32];
extern float initVecy [32];
extern float vecxf [32];
extern float vecyf [32];
extern float vecx [32];
extern float vecy [32];
extern float ones [32];
extern float oneOverDenominator [32];
extern float derivOneOverDenom [32];
extern float buf0 [32];


int minf(float a, float b);
int maxf(float a, float b);
void edgeFunction(float x0, float y0, float x1, float y1, float x2, float y2, float* res);

SECTION(".data_imu0") float vecs [32];
SECTION(".data_imu0") float vect [32];

SECTION(".data_imu0") float vecdudx [32];
SECTION(".data_imu0") float vecdudy [32];
SECTION(".data_imu0") float vecdvdx [32];
SECTION(".data_imu0") float vecdvdy [32];

//texture source color (color from texture array, one tex unit - one texture)
SECTION(".data_imu0") float veccsx [32];
SECTION(".data_imu0") float veccsy [32];
SECTION(".data_imu0") float veccsz [32];
SECTION(".data_imu0") float vecas [32];

SECTION(".data_imu0") float veccvx [32];
SECTION(".data_imu0") float veccvy [32];
SECTION(".data_imu0") float veccvz [32];
SECTION(".data_imu0") float vecav [32];

SECTION(".data_imu0") float veccfx [32];
SECTION(".data_imu0") float veccfy [32];
SECTION(".data_imu0") float veccfz [32];
SECTION(".data_imu0") float vecaf [32];

SECTION(".data_imu0") float vecmu [32];
SECTION(".data_imu0") float vecmv [32];
SECTION(".data_imu3") float vecScaleFactor [32]; //used in sqrtf so paced in another bank than buf0, buf1 and buf2

SECTION(".data_imu0") float veclod [32];
SECTION(".data_imu0") float vecLodDoubled [64];
SECTION(".data_imu0") float vecLodForMin [32];

SECTION(".data_imu0") int minificationIndices [32];//TODO: maybe one array enough to store indices
SECTION(".data_imu0") int magnificationIndices [32];

SECTION(".data_imu0") float vecsmin [32];//minificationed s
SECTION(".data_imu0") float vectmin [32];//minificationed t
SECTION(".data_imu0") float vecsmag [32];//magnificationed s
SECTION(".data_imu0") float vectmag [32];//magnificationed t

SECTION(".data_imu1") float buf1 [32];
SECTION(".data_imu2") float buf2 [32];
SECTION(".data_imu3") float buf3 [32];
SECTION(".data_imu4") float buf4 [32];
SECTION(".data_imu5") float buf5 [32];
SECTION(".data_imu6") float buf6 [32];
SECTION(".data_imu7") float buf7 [32];

SECTION(".data_imu0") int buf0s [32];
SECTION(".data_imu1") int buf1s [32];
SECTION(".data_imu2") int buf2s [32];
SECTION(".data_imu3") int buf3s [32];
SECTION(".data_imu0") nm32f vecpixelpos [32];

SECTION(".data_imu0") int vecr_i [32];
SECTION(".data_imu1") int vecg_i [32];
SECTION(".data_imu2") int vecb_i [32];
SECTION(".data_imu3") int veca_i [32];

SECTION(".data_imu0") int r00 [32];
SECTION(".data_imu1") int g00 [32];
SECTION(".data_imu2") int b00 [32];
SECTION(".data_imu3") int a00 [32];

SECTION(".data_imu0") int r10 [32];
SECTION(".data_imu1") int g10 [32];
SECTION(".data_imu2") int b10 [32];
SECTION(".data_imu3") int a10 [32];

SECTION(".data_imu0") int r01 [32];
SECTION(".data_imu1") int g01 [32];
SECTION(".data_imu2") int b01 [32];
SECTION(".data_imu3") int a01 [32];

SECTION(".data_imu0") int r11 [32];
SECTION(".data_imu1") int g11 [32];
SECTION(".data_imu2") int b11 [32];
SECTION(".data_imu3") int a11 [32];

SECTION(".data_imu0") int veci0 [32];
SECTION(".data_imu1") int vecj0 [32];
SECTION(".data_imu2") int veci1 [32];
SECTION(".data_imu3") int vecj1 [32];

SECTION(".data_imu1") int maskAll0 = 0x00000000;

SECTION(".data_imu0") float vec1a1b [32];
SECTION(".data_imu1") float veca1b [32];
SECTION(".data_imu2") float vec1ab [32];
SECTION(".data_imu3") float vecab [32];

SECTION(".data_imu0") float uMinus05 [32];
SECTION(".data_imu1") float vMinus05 [32];

SECTION(TEXTURE_TRIANGLE_SECTION)
int max (int a, int b)
{
    return (b > a) ? b : a;
}

SECTION(TEXTURE_TRIANGLE_SECTION)
int min (int a, int b)
{
    return (b < a) ? b : a;
}


/* Compare two floats, 1 if are equal, 0 - otherwise */
SECTION(TEXTURE_TRIANGLE_SECTION)
int equalf(float a, float b)
{
	if (((a < 0) && (b >= 0)) || ((a >= 0) && (b < 0)))
		return 0;

	a = fabs(a);
	b = fabs(b);

	float max = (a > b) ? a : b;
	if (fabs(a - b) <= (FLT_EPSILON * max)) {
		return 1;
	}
	else {
		return 0;
	}
}

SECTION(TEXTURE_TRIANGLE_SECTION)
int getPixelValue(unsigned int x, unsigned int y, TexImage2D image, color * pixelValue)
{

    unsigned int rowDataSize = 0;
    unsigned int rawDataSize = 0;
    unsigned int rowPadding = 0;
    unsigned int bmpFileSize = 0;
    unsigned int bitsInPixel = 24;
    unsigned int bytesInPixel = 0;
    
    unsigned int width = image.width;
    unsigned int height = image.height;
    void * pixels = image.pixels;
    NMGLint format = image.internalformat;
    NMGLenum type = NMGL_UNSIGNED_BYTE;//TODO: if constant internal type then 'type' variable is unnecessary

	switch (format)
	{
		case NMGL_RGB:
		  bytesInPixel = 3;
		  break;
	
		case NMGL_RGBA:
		  bytesInPixel = 4;
		  break;
	
		case NMGL_ALPHA:
		case NMGL_LUMINANCE:
		  bytesInPixel = 1;
		  break;
	
		case NMGL_LUMINANCE_ALPHA:
		  bytesInPixel = 2;
		  break;
		  
		default:
		  bytesInPixel = 3;
	
		  break;
	}

	//It is supposed that unpack alignment is converted to 1 in TexImage2D.
	//So alignment of row in texture images for textureTriangle is 1
	//and there are no padding bytes ( see UNPACK_ALIGNMENT)
	unsigned int imageRowWidthBytes = width * bytesInPixel; 

	unsigned int pixelPos = y * imageRowWidthBytes + x * bytesInPixel;

    if ((format == NMGL_RGB) && (type == NMGL_UNSIGNED_BYTE))
    {
        pixelValue->r = ((NMGLubyte*)pixels)[pixelPos];
        pixelValue->g = ((NMGLubyte*)pixels)[pixelPos + 1];
        pixelValue->b = ((NMGLubyte*)pixels)[pixelPos + 2];
		pixelValue->a = 255;
    }
    else if (((format == NMGL_RGBA) && (type == NMGL_UNSIGNED_BYTE)))
    {
        pixelValue->r = ((NMGLubyte*)pixels)[pixelPos];
        pixelValue->g = ((NMGLubyte*)pixels)[pixelPos + 1];
        pixelValue->b = ((NMGLubyte*)pixels)[pixelPos + 2];
        pixelValue->a = ((NMGLubyte*)pixels)[pixelPos + 3];
    }
    else if ((format == NMGL_ALPHA) && (type == NMGL_UNSIGNED_BYTE))
    {
        pixelValue->r = 0;
        pixelValue->g = 0;
        pixelValue->b = 0;
        pixelValue->a = ((NMGLubyte*)pixels)[pixelPos];
    }
    else if ((format == NMGL_LUMINANCE) && (type == NMGL_UNSIGNED_BYTE))
    {
        pixelValue->r = ((NMGLubyte*)pixels)[pixelPos];
        pixelValue->g = ((NMGLubyte*)pixels)[pixelPos];
        pixelValue->b = ((NMGLubyte*)pixels)[pixelPos];
        pixelValue->a = 255;
    }
    else if ((format == NMGL_LUMINANCE_ALPHA) && (type == NMGL_UNSIGNED_BYTE))
    {
        pixelValue->r = ((NMGLubyte*)pixels)[pixelPos];
        pixelValue->g = ((NMGLubyte*)pixels)[pixelPos];
        pixelValue->b = ((NMGLubyte*)pixels)[pixelPos];
        pixelValue->a = ((NMGLubyte*)pixels)[pixelPos + 1];
    }
    else
    {
        printf ("Error: %s %s %d", __FILE__, __func__, __LINE__);
    }
    return 0;
}

SECTION(TEXTURE_TRIANGLE_SECTION)
int getPixelValueVec(nm32s* x, nm32s* y, nm1* evenMask, nm1* oddMask, TexImage2D image, nm32s* r, nm32s* g, nm32s* b, nm32s* a, int size)
{

    unsigned int rowDataSize = 0;
    unsigned int rawDataSize = 0;
    unsigned int rowPadding = 0;
    unsigned int bmpFileSize = 0;
    unsigned int bitsInPixel = 24;
    unsigned int bytesInPixel = 0;
    
    unsigned int width = image.width;
    unsigned int height = image.height;
    void * pixels = image.pixels;
    NMGLint format = image.internalformat;
    NMGLenum type = NMGL_UNSIGNED_BYTE;//TODO: if constant internal type then 'type' variable is unnecessary

	switch (format)
	{
		case NMGL_RGB:
		  bytesInPixel = 3;
		  break;
	
		case NMGL_RGBA:
		  bytesInPixel = 4;
		  break;
	
		case NMGL_ALPHA:
		case NMGL_LUMINANCE:
		  bytesInPixel = 1;
		  break;
	
		case NMGL_LUMINANCE_ALPHA:
		  bytesInPixel = 2;
		  break;
		  
		default:
		  bytesInPixel = 3;
	
		  break;
	}

	//It is supposed that unpack alignment is converted to 1 in TexImage2D.
	//So alignment of row in texture images for textureTrianel is 1
	//and there are no padding bytes ( see UNPACK_ALIGNMENT)
    unsigned int imageRowWidthBytes = width * bytesInPixel;

	//unsigned int pixelPos = y * imageRowWidthBytes + x * bytesInPixel;
    nmppsConvert_32s32f(x, buf2, size);
    nmppsMulC_32f(buf2, buf3, (float)bytesInPixel, size);
    nmppsConvert_32s32f(y, buf2, size);
    nmppsMulC_AddV_AddC_32f(buf2, imageRowWidthBytes, buf3, 0, vecpixelpos, size); //TODO: size must be even

    for (int i = 0; i < size; i++) {
        
        if ( (((nm32s*)oddMask)[0] == 0x0) && (((nm32s*)evenMask)[0] == 0x0)) //skip using masks if they are all zeroes. 
																			  //TODO:[0] used ('size' must be < 32) but we should process more than one 32-bit mask later
        {

        }
        else if (((i & 1) && (nmppsGet_1(oddMask, i>>1) == 1)) || (((i & 1) == 0) && (nmppsGet_1(evenMask, i>>1) == 1)))
        {
            //used for getPixelLinearVec to set border color. Border color is supposed 0.
            r[i] = 0; 
            g[i] = 0;
            b[i] = 0;
            a[i] = 0;
            continue;
        }

        unsigned int pixelPos = vecpixelpos[i];
        
        if ((format == NMGL_RGB) && (type == NMGL_UNSIGNED_BYTE))
        {
            r[i] = ((NMGLubyte*)pixels)[pixelPos];
            g[i] = ((NMGLubyte*)pixels)[pixelPos + 1];
            b[i] = ((NMGLubyte*)pixels)[pixelPos + 2];
            a[i] = 255;
        }
        else if (((format == NMGL_RGBA) && (type == NMGL_UNSIGNED_BYTE)))
        {
            r[i] = ((NMGLubyte*)pixels)[pixelPos];
            g[i] = ((NMGLubyte*)pixels)[pixelPos + 1];
            b[i] = ((NMGLubyte*)pixels)[pixelPos + 2];
            a[i] = ((NMGLubyte*)pixels)[pixelPos + 3];
        }
        else if ((format == NMGL_ALPHA) && (type == NMGL_UNSIGNED_BYTE))
        {
            r[i] = 0;
            g[i] = 0;
            b[i] = 0;
            a[i] = ((NMGLubyte*)pixels)[pixelPos];
        }
        else if ((format == NMGL_LUMINANCE) && (type == NMGL_UNSIGNED_BYTE))
        {
            r[i] = ((NMGLubyte*)pixels)[pixelPos];
            g[i] = ((NMGLubyte*)pixels)[pixelPos];
            b[i] = ((NMGLubyte*)pixels)[pixelPos];
            a[i] = 255;
        }
        else if ((format == NMGL_LUMINANCE_ALPHA) && (type == NMGL_UNSIGNED_BYTE))
        {
           r[i] = ((NMGLubyte*)pixels)[pixelPos];
           g[i] = ((NMGLubyte*)pixels)[pixelPos];
           b[i] = ((NMGLubyte*)pixels)[pixelPos];
           a[i] = ((NMGLubyte*)pixels)[pixelPos + 1];
        }
        else
        {
            printf ("Error: %s %s %d", __FILE__, __func__, __LINE__);
        }
    }
    return 0;
}


SECTION(TEXTURE_TRIANGLE_SECTION)
float wrapCoord (NMGLint textureWrapMode, int texAxisSize, float texCoord)
{
    //TODO: may be optimized if calculate and save 1/(float)texAxisSize*0.5 when TexImage2D is called
	float min_coord_val = 1 / (float)texAxisSize*0.5; //CLAMP_TO_EDGE
	float max_coord_val = 1.0f - min_coord_val;

	float resTexCoord = 0.0f;

	//Apply texture Wrap modes
	if (textureWrapMode == NMGL_REPEAT)
		resTexCoord = texCoord - floor(texCoord);
	else if (textureWrapMode == NMGL_CLAMP_TO_EDGE)
	{
		if (texCoord > max_coord_val) resTexCoord = max_coord_val;
		else if (texCoord < min_coord_val) resTexCoord = min_coord_val;
		else resTexCoord = texCoord;
	}
	else
	{
		printf("Unsupported textureWrapS or textureWrapT value. Exit.\n");
		return -1.0;
	}

	return resTexCoord;
}

SECTION(TEXTURE_TRIANGLE_SECTION)
int wrapCoordVec (NMGLint textureWrapMode, int texAxisSize, float* texCoord, float* dstTexCoord, int size)
{
    //TODO: may be optimized if calculate and save 1/(float)texAxisSize*0.5 when TexImage2D is called
	float min_coord_val = 1 / (float)texAxisSize*0.5; //CLAMP_TO_EDGE
	float max_coord_val = 1.0f - min_coord_val;

	//Apply texture Wrap modes
	if (textureWrapMode == NMGL_REPEAT)
    {
        //resTexCoord = texCoord - floor(texCoord);
        nmppsConvert_32f32s_floor(texCoord, buf0s, 0, size);
        nmppsConvert_32s32f(buf0s, buf0, size);
        nmppsSub_32f(texCoord, buf0, dstTexCoord, size);
    }
	else if (textureWrapMode == NMGL_CLAMP_TO_EDGE)
	{
		//if (texCoord > max_coord_val) resTexCoord = max_coord_val;
		//else if (texCoord < min_coord_val) resTexCoord = min_coord_val;
		//else resTexCoord = texCoord;
        clamp_32f(texCoord, min_coord_val, max_coord_val, dstTexCoord, size);
	}
	else
	{
		printf("Unsupported textureWrapS or textureWrapT value. Exit.\n");
		return -1.0;
	}

	return 0;
}

SECTION(TEXTURE_TRIANGLE_SECTION)   
color getPixelLinear(Vec2f st, NMGLint textureWrapS, NMGLint textureWrapT, TexImage2D texture)
{

	int i0 = 0;
	int j0 = 0;
	int i1 = 0;
	int j1 = 0;
	color pixelValue;
	
	float u = texture.width*st.x; //2^n = textureWidth
	float v = texture.height*st.y; //2^m = textureHeight
	
	float u_floor = floor(u - 0.5f);
	float v_floor = floor(v - 0.5f);

	if (textureWrapS == NMGL_REPEAT)
		i0 = fmod(u_floor, texture.width);
	else
		i0 = u_floor;

	if (textureWrapT == NMGL_REPEAT)
		j0 = fmod(v_floor, texture.height);
	else
		j0 = v_floor;

	float i0plus1 = i0 + 1;
	float j0plus1 = j0 + 1;

	if (textureWrapS == NMGL_REPEAT)
		i1 = fmod(i0plus1, texture.width);
	else
		i1 = i0plus1;

	if (textureWrapT == NMGL_REPEAT)
		j1 = fmod(j0plus1, texture.height);
	else
		j1 = j0plus1;

	float alpha = (u - 0.5) - floor(u - 0.5); //frac(u - 0.5)
	float beta = (v - 0.5) - floor(v - 0.5); //frac(v - 0.5)

	color pixel_i0j0;
	color pixel_i1j0;
	color pixel_i0j1;
	color pixel_i1j1;

	if ((i0 < 0) || (i0 >= texture.width) || (j0 < 0) || (j0 >= texture.height))
	{
		pixel_i0j0 = borderColor;
	}
	else
	{
		getPixelValue(i0, j0, texture, &pixel_i0j0);
	}


	if ((i1 < 0) || (i1 >= texture.width) || (j0 < 0) || (j0 >= texture.height))
	{
		pixel_i1j0 = borderColor;
	}
	else
	{
		getPixelValue(i1, j0, texture, &pixel_i1j0);
	}

	if ((i0 < 0) || (i0 >= texture.width) || (j1 < 0) || (j1 >= texture.height))
	{
		pixel_i0j1 = borderColor;
	}
	else
	{
		getPixelValue(i0, j1, texture, &pixel_i0j1);
	}

	if ((i1 < 0) || (i1 >= texture.width) || (j1 < 0) || (j1 >= texture.height))
	{
		pixel_i1j1 = borderColor;
	}
	else
	{
		getPixelValue(i1, j1, texture, &pixel_i1j1);
	}

	float one_a_one_b = (1 - alpha)*(1 - beta);
	float a_one_b = alpha*(1 - beta);
	float one_a_b = (1 - alpha)*beta;
	float a_b = alpha*beta;

	pixelValue.r = one_a_one_b*pixel_i0j0.r + a_one_b*pixel_i1j0.r + one_a_b*pixel_i0j1.r + a_b*pixel_i1j1.r;
	pixelValue.g = one_a_one_b*pixel_i0j0.g + a_one_b*pixel_i1j0.g + one_a_b*pixel_i0j1.g + a_b*pixel_i1j1.g;
	pixelValue.b = one_a_one_b*pixel_i0j0.b + a_one_b*pixel_i1j0.b + one_a_b*pixel_i0j1.b + a_b*pixel_i1j1.b;
	pixelValue.a = one_a_one_b*pixel_i0j0.a + a_one_b*pixel_i1j0.a + one_a_b*pixel_i0j1.a + a_b*pixel_i1j1.a;

	return pixelValue;//TODO return by pointer
}

SECTION(TEXTURE_TRIANGLE_SECTION)   
int getPixelLinearVec(float* s, float* t, NMGLint textureWrapS, NMGLint textureWrapT, TexImage2D texture, nm32s* r, nm32s* g, nm32s* b, nm32s* a, int size)
{
	//float u = texture.width*st.x; //2^n = textureWidth
    //TODO: may be use shift in cycle
    nmppsMulC_32f(s, buf0, (float)texture.width, size);

	//float u_floor = floor(u - 0.5f);
    nmppsSubC_32f(buf0, uMinus05, 0.5, size);
    nmppsConvert_32f32s_floor(uMinus05, buf0s, 0, size);//buf0s = u_floor
    nmppsConvert_32f32s_floor(uMinus05, veci0, 0, size);//one more time - need buf0s = u_floor
    //reg state: buf0s = veci0 = u_floor

	//float v = texture.height*st.y; //2^m = textureHeight
    //TODO: may be use shift in cycle
    nmppsMulC_32f(t, buf1, (float)texture.height, size);

	//float v_floor = floor(v - 0.5f);
    nmppsSubC_32f(buf1, vMinus05, 0.5, size);
    nmppsConvert_32f32s_floor(vMinus05, buf1s, 0, size);
    nmppsConvert_32f32s_floor(vMinus05, vecj0, 0, size);//one more time - need buf1s = v_floor
    //reg state: buf1s = vecj0 = v_floor, 

	//if (textureWrapS == NMGL_REPEAT)
	//	i0 = fmod(u_floor, texture.width);
	//else
	//	i0 = u_floor;
    ////NOTE:veci0 = u_floor default, width and height are 2^n. TODO: may be use shift (but sign may be <0)
    if (textureWrapS == NMGL_REPEAT) {
        for (int i = 0; i < size; i++) {
            //veci0[i] = (unsigned int)fmod(veci0[i], texture.width);
            veci0[i] = veci0[i] % texture.width;
        }
    }
    //reg state: buf0s = u_floor

	//if (textureWrapT == NMGL_REPEAT)
	//	j0 = fmod(v_floor, texture.height);
	//else
	//	j0 = v_floor;
    ////NOTE:vecj0= v_floor default, width and height are 2^n. TODO: may be use shift (but sign may be <0)
    if (textureWrapT == NMGL_REPEAT) {
        for (int i = 0; i < size; i++) {
            //vecj0[i] = (unsigned int)fmod(vecj0[i], texture.height);
            vecj0[i] = vecj0[i] % texture.height;
        }
    }
    //reg state: buf1s = v_floor

	//float i0plus1 = i0 + 1;
    nmppsConvert_32s32f(veci0, buf1, size); 
    nmppsAddC_32f(buf1, buf2, 1.0, size);
    nmppsConvert_32f32s_floor(buf2, veci1, 0, size);//TODO: need simply nmppsConvert_32f32s
    ////reg state: buf0s = u_floor

	//float j0plus1 = j0 + 1;
    nmppsConvert_32s32f(vecj0, buf1, size); 
    nmppsAddC_32f(buf1, buf2, 1.0, size);
    nmppsConvert_32f32s_floor(buf2, vecj1, 0, size);//TODO: need simply nmppsConvert_32f32s
    ////reg state: buf1s = v_floor

    //	if (textureWrapS == NMGL_REPEAT)
    //		i1 = fmod(i0plus1, texture.width);
    //	else
    //		i1 = i0plus1;
    ////NOTE:veci1 = i0plus1 default, width and height are 2^n. TODO: may be use shift (but sign may be <0)
    if (textureWrapS == NMGL_REPEAT) {
        for (int i = 0; i < size; i++) {
            //veci1[i] = (unsigned int)fmod(veci1[i], texture.width);
            veci1[i] = veci1[i] % texture.width;
        }
    }

	//if (textureWrapT == NMGL_REPEAT)
	//	j1 = fmod(j0plus1, texture.height);
	//else
	//	j1 = j0plus1;
    ////NOTE:vecj1= j0pus1 default, width and height are 2^n. TODO: may be use shift (but sign may be <0)
    if (textureWrapT == NMGL_REPEAT) {
        for (int i = 0; i < size; i++) {
            //vecj1[i] = (unsigned int)fmod(vecj1[i], texture.height);
            vecj1[i] = vecj1[i] % texture.height;
        }
    }
    ////reg state: buf0s = u_floor
    ////reg state: buf1s = v_floor

	//float alpha = (u - 0.5) - floor(u - 0.5); //frac(u - 0.5)
    nmppsConvert_32s32f(buf0s, buf2, size);
    nmppsSub_32f(uMinus05, buf2, buf0, size);

    ////reg state: buf0 = alpha = frac(u-0.5), buf0s = u_floor,
    ////reg state: buf1s = v_floor

	//float beta = (v - 0.5) - floor(v - 0.5); //frac(v - 0.5)
    nmppsConvert_32s32f(buf1s, buf3, size);
    nmppsSub_32f(vMinus05, buf3, buf1, size);

    ////reg state: buf0 = alpha = frac(u-0.5), buf0s = u_floor,
    ////reg state: buf1 = beta = frac(v - 0.5), buf1s = v_floor


    //prepare to get pixels (works only for primWidth < 32 because one 32bit mask is used)
    nm32s maskEveni0Lt0  = 0;
    nm32s maskEveni0GteW = 0; 
    nm32s maskEvenj0Lt0  = 0;
    nm32s maskEvenj0GteH = 0;
    nm32s maskEveni1Lt0  = 0;
    nm32s maskEveni1GteW = 0;
    nm32s maskEvenj1Lt0  = 0;
    nm32s maskEvenj1GteH = 0;

    nm32s maskOddi0Lt0  = 0;
    nm32s maskOddi0GteW = 0;
    nm32s maskOddj0Lt0  = 0;
    nm32s maskOddj0GteH = 0;
    nm32s maskOddi1Lt0  = 0;
    nm32s maskOddi1GteW = 0;
    nm32s maskOddj1Lt0  = 0;
    nm32s maskOddj1GteH = 0;

    nmppsConvert_32s32f (veci0, buf2, size);
    cmpLtC_f  (buf2, 0, (nm1*)&maskEveni0Lt0 , (nm1*)&maskOddi0Lt0 , size);
    cmpGteC_f (buf2, (float)texture.width, (nm1*)&maskEveni0GteW, (nm1*)&maskOddi0GteW, size);

    nmppsConvert_32s32f (vecj0, buf2, size);
    cmpLtC_f  (buf2, 0, (nm1*)&maskEvenj0Lt0 , (nm1*)&maskOddj0Lt0 , size);
    cmpGteC_f (buf2, (float)texture.height, (nm1*)&maskEvenj0GteH, (nm1*)&maskOddj0GteH, size);

    nmppsConvert_32s32f (veci1, buf2, size);
    cmpLtC_f  (buf2, 0, (nm1*)&maskEveni1Lt0 , (nm1*)&maskOddi1Lt0 , size);
    cmpGteC_f (buf2, (float)texture.width, (nm1*)&maskEveni1GteW, (nm1*)&maskOddi1GteW, size);

    nmppsConvert_32s32f (vecj1, buf2, size);
    cmpLtC_f  (buf2, 0, (nm1*)&maskEvenj1Lt0 , (nm1*)&maskOddj1Lt0 , size);
    cmpGteC_f (buf2, (float)texture.height, (nm1*)&maskEvenj1GteH, (nm1*)&maskOddj1GteH, size);

    int maskEveni0 = maskEveni0Lt0 | maskEveni0GteW;
    int  maskOddi0 =  maskOddi0Lt0 |  maskOddi0GteW;

    int maskEvenj0 = maskEvenj0Lt0 | maskEvenj0GteH;
    int  maskOddj0 =  maskOddj0Lt0 |  maskOddj0GteH;

    int maskEveni1 = maskEveni1Lt0 | maskEveni1GteW;
    int  maskOddi1 =  maskOddi1Lt0 |  maskOddi1GteW;

    int maskEvenj1 = maskEvenj1Lt0 | maskEvenj1GteH;
    int  maskOddj1 =  maskOddj1Lt0 |  maskOddj1GteH;

    int maskEveni0j0 = maskEveni0 | maskEvenj0;
    int  maskOddi0j0 =  maskOddi0 |  maskOddj0;

    int maskEveni1j0 = maskEveni1 | maskEvenj0;
    int  maskOddi1j0 =  maskOddi1 |  maskOddj0;

    int maskEveni0j1 = maskEveni0 | maskEvenj1;
    int  maskOddi0j1 =  maskOddi0 |  maskOddj1;

    int maskEveni1j1 = maskEveni1 | maskEvenj1;
    int  maskOddi1j1 =  maskOddi1 |  maskOddj1;

    //if ((i0 < 0) || (i0 >= texture.width) || (j0 < 0) || (j0 >= texture.height))
    //{
    //    pixel_i0j0 = borderColor;
    //}
    //else
    //{
    //    getPixelValue(i0, j0, texture, &pixel_i0j0);
    //}
    getPixelValueVec(veci0, vecj0, (nm1*)&maskEveni0j0, (nm1*)&maskOddi0j0, texture, r00, g00, b00, a00, size);

    //if ((i1 < 0) || (i1 >= texture.width) || (j0 < 0) || (j0 >= texture.height))
    //{
    //    pixel_i1j0 = borderColor;
    //}
    //else
    //{
    //    getPixelValue(i1, j0, texture, &pixel_i1j0);
    //}
    getPixelValueVec(veci1, vecj0, (nm1*)&maskEveni1j0, (nm1*)&maskOddi1j0, texture, r10, g10, b10, a10, size);

    //if ((i0 < 0) || (i0 >= texture.width) || (j1 < 0) || (j1 >= texture.height))
    //{
    //    pixel_i0j1 = borderColor;
    //}
    //else
    //{
    //    getPixelValue(i0, j1, texture, &pixel_i0j1);
    //}
    getPixelValueVec(veci0, vecj1, (nm1*)&maskEveni0j1, (nm1*)&maskOddi0j1, texture, r01, g01, b01, a01, size);

    //if ((i1 < 0) || (i1 >= texture.width) || (j1 < 0) || (j1 >= texture.height))
    //{
    //    pixel_i1j1 = borderColor;
    //}
    //else
    //{
    //    getPixelValue(i1, j1, texture, &pixel_i1j1);
    //}
    getPixelValueVec(veci1, vecj1, (nm1*)&maskEveni1j1, (nm1*)&maskOddi1j1, texture, r11, g11, b11, a11, size);


    //float one_a_one_b = (1 - alpha)*(1 - beta);
    nmppsSub_32f(ones, buf0, buf2, size);
    nmppsSub_32f(ones, buf1, buf3, size);
    nmppsMul_32f(buf2, buf3, vec1a1b, size);

    //float a_one_b = alpha*(1 - beta);
    nmppsSub_32f(ones, buf1, buf3, size);
    nmppsMul_32f(buf0, buf3, veca1b, size);

    //float one_a_b = (1 - alpha)*beta;
    nmppsSub_32f(ones, buf0, buf3, size);
    nmppsMul_32f(buf1, buf3, vec1ab, size);

    //float a_b = alpha*beta;
    nmppsMul_32f(buf0, buf1, vecab, size);

    //r[i] = vec1a1b[i]*r00[i] + veca1b[i]*r10[i] + vec1ab[i]*r01[i] + vecab[i]*r11[i];
    nmppsConvert_32s32f(r00, buf4, size);
    nmppsConvert_32s32f(r10, buf5, size);
    nmppsConvert_32s32f(r01, buf6, size);
    nmppsConvert_32s32f(r11, buf7, size);
    nmppsMul_32f(vec1a1b, buf4, buf0, size);
    nmppsMul_Add_32f(veca1b,  buf5, buf0, buf1, size);
    nmppsMul_Add_32f(vec1ab,  buf6, buf1, buf2, size);
    nmppsMul_Add_32f( vecab,  buf7, buf2, buf3, size);
    nmppsConvert_32f32s_floor(buf3, r, 0, size);

    //g[i] = vec1a1b[i]*g00[i] + veca1b[i]*g10[i] + vec1ab[i]*g01[i] + vecab[i]*g11[i];
    nmppsConvert_32s32f(g00, buf4, size);
    nmppsConvert_32s32f(g10, buf5, size);
    nmppsConvert_32s32f(g01, buf6, size);
    nmppsConvert_32s32f(g11, buf7, size);
    nmppsMul_32f(vec1a1b, buf4, buf0, size);
    nmppsMul_Add_32f(veca1b,  buf5, buf0, buf1, size);
    nmppsMul_Add_32f(vec1ab,  buf6, buf1, buf2, size);
    nmppsMul_Add_32f( vecab,  buf7, buf2, buf3, size);
    nmppsConvert_32f32s_floor(buf3, g, 0, size);

    //b[i] = vec1a1b[i]*b00[i] + veca1b[i]*b10[i] + vec1ab[i]*b01[i] + vecab[i]*b11[i];
    nmppsConvert_32s32f(b00, buf4, size);
    nmppsConvert_32s32f(b10, buf5, size);
    nmppsConvert_32s32f(b01, buf6, size);
    nmppsConvert_32s32f(b11, buf7, size);
    nmppsMul_32f(vec1a1b, buf4, buf0, size);
    nmppsMul_Add_32f(veca1b,  buf5, buf0, buf1, size);
    nmppsMul_Add_32f(vec1ab,  buf6, buf1, buf2, size);
    nmppsMul_Add_32f( vecab,  buf7, buf2, buf3, size);
    nmppsConvert_32f32s_floor(buf3, b, 0, size);

    //a[i] = vec1a1b[i]*a00[i] + veca1b[i]*a10[i] + vec1ab[i]*a01[i] + vecab[i]*a11[i];
    nmppsConvert_32s32f(a00, buf4, size);
    nmppsConvert_32s32f(a10, buf5, size);
    nmppsConvert_32s32f(a01, buf6, size);
    nmppsConvert_32s32f(a11, buf7, size);
    nmppsMul_32f(vec1a1b, buf4, buf0, size);
    nmppsMul_Add_32f(veca1b,  buf5, buf0, buf1, size);
    nmppsMul_Add_32f(vec1ab,  buf6, buf1, buf2, size);
    nmppsMul_Add_32f( vecab,  buf7, buf2, buf3, size);
    nmppsConvert_32f32s_floor(buf3, a, 0, size);

    return 0;
}


SECTION(TEXTURE_TRIANGLE_SECTION)
color getPixelNearest(Vec2f st, TexImage2D texture)
{

    unsigned int texel_i = 0;
    unsigned int texel_j = 0;
    color pixelValue;


    float u = texture.width*st.x; //2^n = textureWidth
    float v = texture.height*st.y; //2^m = textureHeight

    texel_i = st.x < 1 ? floor(u) : texture.width - 1;
    texel_j = st.y < 1 ? floor(v) : texture.height - 1;
    getPixelValue(texel_i, texel_j, texture, &pixelValue);

    return pixelValue;//TODO return by pointer

}

    SECTION(TEXTURE_TRIANGLE_SECTION)
int getPixelNearestVec(float* s, float* t, TexImage2D texture, nm32s* r, nm32s* g, nm32s* b, nm32s* a, int size)
{
    unsigned int texel_i = 0;
    unsigned int texel_j = 0;
    color pixelValue;

    int texWidth = (float)texture.width;
    int texHeight = (float)texture.height;

    //float u = texture.width*st.x; //2^n = textureWidth
    nmppsMulC_32f(s, buf0, texWidth, size);

    //float v = texture.height*st.y; //2^m = textureHeight
    nmppsMulC_32f(t, buf1, texHeight, size);

    //texel_i = st.x < 1 ? floor(u) : texture.width - 1;
    float texWidthDecr = texWidth - 1;
    //floor(u)
    nmppsConvert_32f32s_floor(buf0, buf0s, 0, size);
    //ternary
    cmpLtCSelVC_f(s, 1.0f, buf0s, texWidthDecr, buf0s, size);


    //texel_j = st.y < 1 ? floor(v) : texture.height - 1;
    float texHeightDecr = texHeight - 1;	
    //floor(v)
    nmppsConvert_32f32s_floor(buf1, buf1s, 0, size);
    //ternary
    cmpLtCSelVC_f(t, 1.0f, buf1s, texHeightDecr, buf1s, size);    

    //getPixelValue(texel_i, texel_j, texture, &pixelValue);
    getPixelValueVec(buf0s, buf1s, (nm1*)&maskAll0, (nm1*)&maskAll0, texture, r, g, b, a, size);

    return 0;

}

    SECTION(TEXTURE_TRIANGLE_SECTION)
void textureTriangle(TrianglesInfo* triangles, nm32s* pDstTriangle, int count)
{


    NMGL_Context_NM0 *cntxt = NMGL_Context_NM0::getContext();

    //Текущий текстурный модуль
    unsigned int activeTexUnitIndex = 0; //only one texture unit is supported at now

    //Текстурный объект, привязанный к активному текстурному модулю
    TexObject* boundTexObject = cntxt->texState.texUnits[activeTexUnitIndex].boundTexObject;

    float scaleFactor = 1.0;
    borderColor.r = 0.0f;
    borderColor.g = 0.0f;
    borderColor.b = 0.0f;
    borderColor.a = 0.0f;

    color pixelValue;
    pixelValue.r = 0;
    pixelValue.g = 0;
    pixelValue.b = 0;

    //texEnvColor consists of rgb + alpha
    Vec3f texEnvColor;
    float texEnvColorAlpha;
    texEnvColor.x = cntxt->texState.texUnits[activeTexUnitIndex].texEnvColor[0];
    texEnvColor.y = cntxt->texState.texUnits[activeTexUnitIndex].texEnvColor[1];
    texEnvColor.z = cntxt->texState.texUnits[activeTexUnitIndex].texEnvColor[2];
    texEnvColorAlpha = cntxt->texState.texUnits[activeTexUnitIndex].texEnvColor[3];

    //primitive color (glColor3f)
    Vec3f vertexRGB;
    vertexRGB.x = 1.0;
    vertexRGB.y = 1.0;
    vertexRGB.z = 1.0;
    float vertexAlpha = 1.0;


    NMGLint textureMinFilter = boundTexObject->texMinFilter; //default NEAREST_MIPMAP_LINEAR
    NMGLint textureMagFilter = boundTexObject->texMagFilter; //default LINEAR
    NMGLint textureWrapS = boundTexObject->texWrapS; // default NMGL_REPEAT
    NMGLint textureWrapT = boundTexObject->texWrapT; // default NMGL_REPEAT

    NMGLint texEnvMode = cntxt->texState.texUnits[activeTexUnitIndex].texFunctionName; //default = NMGL_MODULATE

    NMGLint texBaseInternalFormat = boundTexObject->texImages2D[0].internalformat;//use level 0 texture format to select texture function

    //Calculate some parameters from OpenGL 1.3 spec
    int n = log2(boundTexObject->texImages2D[0].width);
    int m = log2(boundTexObject->texImages2D[0].height);
    int p = max(n,m) + NMGL_TEX_BASE_LEVEL; //p = max{n,m,l} + NMGL_TEX_BASE_LEVEL
    int q = min(p,NMGL_TEX_MAX_LEVEL);//min{p,NMGL_TEX_MAX_LEVEL} page 140, glspec 1.3

    long long int temp;
    nm32s* dst = pDstTriangle;
    int winX0 = 0;
    int winY0 = 0;

    for(int cnt=0;cnt<count;cnt++){
        nm32s* dstTriagle = (nm32s*) (pDstTriangle + WIDTH_PTRN * HEIGHT_PTRN * cnt);

        //start calculate pixel value for texturing

        float area = 0;
        float x0 = triangles->x0[cnt];
        float y0 = triangles->y0[cnt];
        float x1 = triangles->x1[cnt];
        float y1 = triangles->y1[cnt];
        float x2 = triangles->x2[cnt];
        float y2 = triangles->y2[cnt];

        float s0 = triangles->s0[cnt];
        float t0 = triangles->t0[cnt];
        float s1 = triangles->s1[cnt];
        float t1 = triangles->t1[cnt];
        float s2 = triangles->s2[cnt];
        float t2 = triangles->t2[cnt];

        float z0 = triangles->z0[cnt];
        float z1 = triangles->z1[cnt];
        float z2 = triangles->z2[cnt];
		
        v4nm32s colors = triangles->colors[cnt];//TODO: colors should be pointer to colored  fragments of incoming primitive. At now all pixels of triangle has one color, so Cf always has the same value.

        winY0 = minf(y0, minf(y1, y2)); 
        winX0 = minf(x0, minf(x1, x2)); 

        unsigned int maxX = maxf(x0, maxf(x1, x2));
        unsigned int maxY = maxf(y0, maxf(y1, y2));

        unsigned int primHeight = maxY - winY0 + 1;
        unsigned int primWidth = maxX - winX0 + 1;
        primWidth += primWidth & 1;//make primWidth even to use in nmpp functions

#ifdef DEBUG
        if (primWidth > 32) 
            printf ("%s: Warning: primWidth is greater than 32 but some functions (getPixelLinearVec) in that function can work only for primWidth <= 32\n\r");
#endif DEBUG

#ifdef PERSPECTIVE_CORRECT        
        // Compute some coefficients.
        // Used for:
        // * something similar to linear-rational interpolation 
        // * to calculate scale factor
        // Formulas obtained by simlifying formulas for barycentric coordinates.
        float z12 = z1 * z2;
        float z02 = z0 * z2;
        float z10 = z1 * z0;

        float A2_12 = z12*(y2 - y1);
        float A2_02 = z02*(y0 - y2);
        float A2_10 = z10*(y1 - y0);

        float B2_12 = -z12*(x2 - x1);
        float B2_02 = -z02*(x0 - x2);
        float B2_10 = -z10*(x1 - x0);

        float D2_12 = z12*(x2*y1 - x1*y2);
        float D2_02 = z02*(x0*y2 - x2*y0);
        float D2_10 = z10*(x1*y0 - x0*y1);

        float A2 = A2_12 + A2_02 + A2_10;
        float B2 = B2_12 + B2_02 + B2_10;
        float D2 = D2_12 + D2_02 + D2_10;

        float A1_s = A2_12*s0 + A2_02*s1 + A2_10*s2;
        float B1_s = B2_12*s0 + B2_02*s1 + B2_10*s2;
        float D1_s = D2_12*s0 + D2_02*s1 + D2_10*s2;

        float A1_t = A2_12*t0 + A2_02*t1 + A2_10*t2;
        float B1_t = B2_12*t0 + B2_02*t1 + B2_10*t2;
        float D1_t = D2_12*t0 + D2_02*t1 + D2_10*t2;

        /*****************************************************************/
#else //PERSPECTIVE_CORRECT
        float oneOverTriSquare = 1.0/((x0-x1)*(y2-y1)-(y0-y1)*(x2-x1));
        float A0 = y2-y1;
        float A1 = y0-y2;
        float A2 = y1-y0;

        float B0 = x1-x2;
        float B1 = x2-x0;
        float B2 = x0-x1;

        float D0 = x2*y1-y2*x1;
        float D1 = y2*x0-x2*y0;
        float D2 = x1*y0-x0*y1;

        float A_s = oneOverTriSquare*(A0*s0 + A1*s1 + A2*s2);
        float B_s = oneOverTriSquare*(B0*s0 + B1*s1 + B2*s2);
        float D_s = oneOverTriSquare*(D0*s0 + D1*s1 + D2*s2);

        float A_t = oneOverTriSquare*(A0*t0 + A1*t1 + A2*t2);
        float B_t = oneOverTriSquare*(B0*t0 + B1*t1 + B2*t2);
        float D_t = oneOverTriSquare*(D0*t0 + D1*t1 + D2*t2);

#endif //PERSPECTIVE_CORRECT

        nmblas_scopy(primWidth, initVecx, 1, buf0, 1);
        nmblas_scopy(primWidth, initVecy, 1, buf1, 1);

        nmppsAddC_32f(buf0, vecxf, winX0, primWidth);
        nmppsAddC_32f(buf1, vecyf, winY0, primWidth);
        for(int y = 0; y < primHeight; y++){

            nm32s* pDst = (nm32s*)(dstTriagle + y * WIDTH_PTRN);

#ifdef PERSPECTIVE_CORRECT
            //float oneOverDenominator = 1 / (A2*xf + B2*yf + D2);
            nmppsMulC_AddC_32f(vecyf, B2, D2, buf0, primWidth);
            nmppsMulC_AddV_32f(vecxf, buf0, buf1, A2, primWidth);
            nmppsDiv_32f(ones, buf1, oneOverDenominator, primWidth);

            //float s = (A1_s*xf + B1_s*yf + D1_s) * oneOverDenominator;
            nmppsMulC_AddC_32f(vecyf, B1_s, D1_s, buf0, primWidth);
            nmppsMulC_AddV_32f(vecxf, buf0, buf1, A1_s, primWidth);
            nmppsMul_AddC_32f(buf1, oneOverDenominator, 0.0, vecs, primWidth);

            //float t = (A1_t*xf + B1_t*yf + D1_t) * oneOverDenominator;
            nmppsMulC_AddC_32f(vecyf, B1_t, D1_t, buf0, primWidth);
            nmppsMulC_AddV_32f(vecxf, buf0, buf1, A1_t, primWidth);
            nmppsMul_AddC_32f(buf1, oneOverDenominator, 0.0, vect, primWidth);
#else //PERSPECTIVE_CORRECT                    
            //s = A_s*xf + B_s*yf + D_s;
            nmppsMulC_AddC_32f(vecyf, B_s, D_s, buf1, primWidth);
            nmppsMulC_AddV_32f(vecxf, buf1, vecs, A_s, primWidth);

            //t = A_t*xf + B_t*yf + D_t;
            nmppsMulC_AddC_32f(vecyf, B_t, D_t, buf0, primWidth);
            nmppsMulC_AddV_32f(vecxf, buf0, vect, A_t, primWidth);
#endif //PERSPECTIVE_CORRECT

            /* Calculate minification vs. magnification switchover point */

            if ((textureMagFilter == NMGL_LINEAR) && ((textureMinFilter == NMGL_NEAREST_MIPMAP_NEAREST) || (textureMinFilter == NMGL_NEAREST_MIPMAP_LINEAR)))
                c = 0.5f;
            else
                c = 0.0f;

            /*************************************************************/

            /*Calculate partial derivative for u(x,y) and v(x,y). level 0 texture are using to calculate scale factor*/

#ifdef PERSPECTIVE_CORRECT
            //float derivOneOverDenom = 1.0 / ((A2*x + B2*y + D2)*(A2*x + B2*y + D2));//TODO: may be xf, yf should be used
            nmppsSubC_32f(vecyf, vecy, 0.5, primWidth);
            nmppsMulC_AddC_32f(vecy, B2, D2, buf0, primWidth);
            nmppsMulC_AddV_32f(vecx, buf0, buf1, A2, primWidth);
            nmppsMul_AddC_32f(buf1, buf1, 0.0, buf0, primWidth);
            nmppsDiv_32f(ones, buf0, derivOneOverDenom, primWidth);

            float tmp0 = 0.0;
            float tmp1 = 0.0;

            //float dudx = (float)boundTexObject->texImages2D[0].width*((A1_s*B2 - A2*B1_s)*y + A1_s*D2 - A2*D1_s)*derivOneOverDenom;
            tmp0 = A1_s*B2 - A2*B1_s;
            tmp1 = A1_s*D2 - A2*D1_s;
            nmppsMulC_AddC_32f(vecy, tmp0, tmp1, buf0, primWidth);
            nmppsMul_AddC_32f(buf0, derivOneOverDenom, 0.0, buf1, primWidth);
            nmppsMulC_AddC_32f(buf1, (float)boundTexObject->texImages2D[0].width, 0.0, vecdudx, primWidth);

            //float dudy = (float)boundTexObject->texImages2D[0].width*((B1_s*A2 - B2*A1_s)*x + B1_s*D2 - B2*D1_s)*derivOneOverDenom;
            tmp0 = B1_s*A2 - B2*A1_s;
            tmp1 = B1_s*D2 - B2*D1_s;
            nmppsMulC_AddC_32f(vecx, tmp0, tmp1, buf0, primWidth);
            nmppsMul_AddC_32f(buf0, derivOneOverDenom, 0.0, buf1, primWidth);
            nmppsMulC_AddC_32f(buf1, (float)boundTexObject->texImages2D[0].width, 0.0, vecdudy, primWidth);

            //float dvdx = (float)boundTexObject->texImages2D[0].height*((A1_t*B2 - A2*B1_t)*y + A1_t*D2 - A2*D1_t)*derivOneOverDenom;
            tmp0 = A1_t*B2 - A2*B1_t;
            tmp1 = A1_t*D2 - A2*D1_t;
            nmppsMulC_AddC_32f(vecy, tmp0, tmp1, buf0, primWidth);
            nmppsMul_AddC_32f(buf0, derivOneOverDenom, 0.0, buf1, primWidth);
            nmppsMulC_AddC_32f(buf1, (float)boundTexObject->texImages2D[0].height, 0.0, vecdvdx, primWidth);

            //float dvdy = (float)boundTexObject->texImages2D[0].height*((B1_t*A2 - B2*A1_t)*x + B1_t*D2 - B2*D1_t)*derivOneOverDenom;
            tmp0 = B1_t*A2 - B2*A1_t;
            tmp1 = B1_t*D2 - B2*D1_t;
            nmppsMulC_AddC_32f(vecx, tmp0, tmp1, buf0, primWidth);
            nmppsMul_AddC_32f(buf0, derivOneOverDenom, 0.0, buf1, primWidth);
            nmppsMulC_AddC_32f(buf1, (float)boundTexObject->texImages2D[0].height, 0.0, vecdvdy, primWidth);					

#else //PERSPECTIVE_CORRECT  
            //float dudx = (float)boundTexObject->texImages2D[0].width*A_s;
            nmppsMulC_AddC_32f(ones, (float)boundTexObject->texImages2D[0].width*A_s, 0.0, vecdudx, primWidth);

            //float dudy = (float)boundTexObject->texImages2D[0].width*B_s;
            nmppsMulC_AddC_32f(ones, (float)boundTexObject->texImages2D[0].width*B_s, 0.0, vecdudy, primWidth);

            //float dvdx = (float)boundTexObject->texImages2D[0].height*A_t;
            nmppsMulC_AddC_32f(ones, (float)boundTexObject->texImages2D[0].height*A_t, 0.0, vecdvdx, primWidth);

            //float dvdy = (float)boundTexObject->texImages2D[0].height*B_t;
            nmppsMulC_AddC_32f(ones, (float)boundTexObject->texImages2D[0].height*B_t, 0.0, vecdvdy, primWidth);

#endif //PERSPECTIVE_CORRECT

            /*Calculate scale factor*/
#ifndef SCALE_FACTOR_IDEAL
            //float mu = fmax(fabs(dudx), fabs(dudy));
            abs_32f(vecdudx, buf0, primWidth);
            abs_32f(vecdudy, buf1, primWidth);
            findMax2(buf0, buf1, vecmu, primWidth);

            //float mv = fmax(fabs(dvdx), fabs(dvdy));
            abs_32f(vecdvdx, buf0, primWidth);
            abs_32f(vecdvdy, buf1, primWidth);
            findMax2(buf0, buf1, vecmv, primWidth);

            //scaleFactor = fmax(mu, mv);
            findMax2(vecmu, vecmv, vecScaleFactor, primWidth);
#else //SCALE_FACTOR_IDEAL

            //scaleFactor = fmax(sqrtf(dudx*dudx + dvdx*dvdx), sqrtf(dudy*dudy + dvdy*dvdy));
            //1) dudx*dudx + dvdx*dvdx
            nmppsMul_32f(vecdudx, vecdudx, buf0, primWidth);
            nmppsMul_32f(vecdvdx, vecdvdx, buf1, primWidth);
            nmppsAdd_32f(buf0, buf1, buf2, primWidth);

            //2) dudy*dudy + dvdy*dvdy
            nmppsMul_32f(vecdudy, vecdudy, buf0, primWidth);
            nmppsMul_32f(vecdvdy, vecdvdy, buf1, primWidth);
            nmppsAdd_32f(buf0, buf1, buf3, primWidth);

            //3) fmax(dudx*dudx + dvdx*dvdx, dudy*dudy + dvdy*dvdy)
            findMax2(buf2, buf3, buf0, primWidth)

            //4) sqrtf(fmax())
            nmppsSqrt_32f(buf0, vecScaleFactor, buf1, primWidth);
#endif //SCALE_FACTOR_IDEAL				

            /*Calculate level of detail*/

            //float lod_ = log2f(scaleFactor) = 1.442695 * ln(scaleFactor)
            nmppsLn_32f (vecScaleFactor, buf0, primWidth);
            nmppsMulC_32f(buf0, buf1, 1.442695f, primWidth);
            //buf1 = lod_

            if (NMGL_TEX_MIN_LOD > NMGL_TEX_MAX_LOD)
            {
                printf("Error. NMGL_TEX_MIN_LOD > NMGL_TEX_MAX_LOD. LOD is undefined. Exit\n");
                return;
            }
            else {
                ////Clamping lod
                //if (lod_ > NMGL_TEX_MAX_LOD)
                //	lod = NMGL_TEX_MAX_LOD;
                //else if (lod_ < NMGL_TEX_MIN_LOD)
                //	lod = NMGL_TEX_MIN_LOD;
                //else
                //	lod = lod_;
                clamp_32f(buf1, (float)NMGL_TEX_MIN_LOD, (float)NMGL_TEX_MAX_LOD, veclod, primWidth);
            }


            //// Generate minification and magnification masks

            //if (lod <= c) 
            //	minMagFlag = MAGNIFICATION;
            //else 
            //	minMagFlag = MINIFICATION;
            int primWidthX2 = primWidth << 1;
            //primWidth <= 32 so one nm32s is enough to store minification/magnification flags for primWidth elements
            nm32s magFlags = 0;
            nm32s minFlags = 0;
            nm32s dummy = 0;
            //double lod to get flags for all primWidth elements in one even mask
            nmppsMerge_32f(veclod, veclod, vecLodDoubled, primWidth); 
            cmpLteC_f(vecLodDoubled, (float)c, (nm1*)(&magFlags), (nm1*)(&dummy), primWidthX2); //magFlags contains ones for elements for MAGNIFICATION
            minFlags = ~magFlags;//minFlags contains ones for elements for MINIFICATION

            //TODO: may be zeroing minificationIndices and magnificationIndices first
            int minCount = cnvMaskToIndices((nm1*)&minFlags, minificationIndices, primWidth);
            int magCount = cnvMaskToIndices((nm1*)&magFlags, magnificationIndices, primWidth);

            if (minCount > 0) {
                //TODO: check minCount for 32, may be it is not necessary to select
                selectElementsByIndices((float*)vecs, minificationIndices, (float*)vecsmin, sizeof32(float), minCount);
                selectElementsByIndices((float*)vect, minificationIndices, (float*)vectmin, sizeof32(float), minCount);
                selectElementsByIndices((float*)veclod, minificationIndices, (float*)vecLodForMin, sizeof32(float), minCount);

                //MINIFICATION branch
                if (textureMinFilter == NMGL_NEAREST)
                {
                    //Not mipmapping. So wrap texture coordinates for texture of level 0 
                    //st.x = wrapCoord(textureWrapS, boundTexObject->texImages2D[0].width, st.x);
                    wrapCoordVec(textureWrapS, boundTexObject->texImages2D[0].width, vecsmin, vecsmin, minCount);

                    //st.y = wrapCoord(textureWrapT, boundTexObject->texImages2D[0].height, st.y);
                    wrapCoordVec(textureWrapT, boundTexObject->texImages2D[0].height, vectmin, vectmin, minCount);

                    //pixelValue = getPixelNearest(st, boundTexObject->texImages2D[0]);
                    getPixelNearestVec(vecsmin, vectmin, boundTexObject->texImages2D[0], vecr_i, vecg_i, vecb_i, veca_i, minCount);
                }
                else if (textureMinFilter == NMGL_LINEAR)
                {
                    //Not mipmapping. So wrap texture coordinates for texture of level 0 
                    if (textureWrapS == NMGL_CLAMP_TO_EDGE)
                        //st.x = wrapCoord(textureWrapS, boundTexObject->texImages2D[0].width, st.x);
                        wrapCoordVec(textureWrapS, boundTexObject->texImages2D[0].width, vecsmin, vecsmin, minCount);

                    if (textureWrapT == NMGL_CLAMP_TO_EDGE)
                        //st.y = wrapCoord(textureWrapT, boundTexObject->texImages2D[0].height, st.y);
                        wrapCoordVec(textureWrapT, boundTexObject->texImages2D[0].height, vectmin, vectmin, minCount);

                    //pixelValue = getPixelLinear(st, textureWrapS, textureWrapT, boundTexObject->texImages2D[0]);
                    getPixelLinearVec(vecsmin, vectmin, textureWrapS, textureWrapT, boundTexObject->texImages2D[0], vecr_i, vecg_i, vecb_i, veca_i, minCount);
                }
                else
                {
                    for (int x = 0; x < minCount; x++) {

                        Vec2f st;
                        st.x = vecsmin[x];
                        st.y = vectmin[x];
                        float lod = vecLodForMin[x];

                        unsigned int d = 0;
                        unsigned int d1 = 0;
                        unsigned int d2 = 0;
                        if ((textureMinFilter == NMGL_NEAREST_MIPMAP_NEAREST) || (textureMinFilter == NMGL_LINEAR_MIPMAP_NEAREST))
                        {
                            d = 0;
                            if (lod < 0.5f || (equalf(lod, 0.5f) == 1))
                                d = NMGL_TEX_BASE_LEVEL;
                            else if ((NMGL_TEX_BASE_LEVEL + lod) <= ((float)q + 0.5f))
                                d = ceil(NMGL_TEX_BASE_LEVEL + lod + 0.5f) - 1.0f;
                            else if ((NMGL_TEX_BASE_LEVEL + lod) > ((float)q + 0.5f))
                                d = q;
                            else
                            {
                                printf("mipmap:NMGL_NEAREST_MIPMAP_NEAREST: d is undefined\n");
                            }

                            if (textureMinFilter == NMGL_NEAREST_MIPMAP_NEAREST)
                            {
                                //Mipmapping. So wrap texture coordinates for texture of selected level d 
                                st.x = wrapCoord(textureWrapS, boundTexObject->texImages2D[d].width, st.x);
                                st.y = wrapCoord(textureWrapT, boundTexObject->texImages2D[d].height, st.y);
                                pixelValue = getPixelNearest(st, boundTexObject->texImages2D[d]);

                            }
                            else if (textureMinFilter == NMGL_LINEAR_MIPMAP_NEAREST)
                            {
                                //Mipmapping. So wrap texture coordinates for texture of selected level d 
                                if (textureWrapS == NMGL_CLAMP_TO_EDGE)
                                    st.x = wrapCoord(textureWrapS, boundTexObject->texImages2D[d].width, st.x);

                                if (textureWrapT == NMGL_CLAMP_TO_EDGE)
                                    st.y = wrapCoord(textureWrapT, boundTexObject->texImages2D[d].height, st.y);

                                pixelValue = getPixelLinear(st, textureWrapS, textureWrapT, boundTexObject->texImages2D[d]);
                            }
                            else
                            {
                                printf("textureMinFilter has unsupported value for mipmapping\n");
                            }

                        }
                        else if ((textureMinFilter == NMGL_NEAREST_MIPMAP_LINEAR) || (textureMinFilter == NMGL_LINEAR_MIPMAP_LINEAR))
                        {
                            d1 = 0;
                            d2 = 0;
                            color t1;
                            color t2;

                            if (borderWidth + lod >= q)
                                d1 = q;
                            else
                                d1 = floor(borderWidth + lod);

                            if (borderWidth + lod >= q)
                                d2 = q;
                            else
                                d2 = d1 + 1;

                            Vec2f st1 = st;
                            Vec2f st2 = st;

                            if (textureMinFilter == NMGL_NEAREST_MIPMAP_LINEAR)
                            {
                                //Mipmapping. So wrap texture coordinates for texture of selected level d1 and d2
                                st1.x = wrapCoord(textureWrapS, boundTexObject->texImages2D[d1].width, st.x);
                                st1.y = wrapCoord(textureWrapT, boundTexObject->texImages2D[d1].height, st.y);
                                st2.x = wrapCoord(textureWrapS, boundTexObject->texImages2D[d2].width, st.x);
                                st2.y = wrapCoord(textureWrapT, boundTexObject->texImages2D[d2].height, st.y);

                                t1 = getPixelNearest(st1, boundTexObject->texImages2D[d1]);
                                t2 = getPixelNearest(st2, boundTexObject->texImages2D[d2]);
                            }
                            else if (textureMinFilter == NMGL_LINEAR_MIPMAP_LINEAR)
                            {
                                //Mipmapping. So wrap texture coordinates for texture of selected level d1 and d2
                                if (textureWrapS == NMGL_CLAMP_TO_EDGE)
                                {
                                    st1.x = wrapCoord(textureWrapS, boundTexObject->texImages2D[d1].width, st.x);
                                    st2.x = wrapCoord(textureWrapS, boundTexObject->texImages2D[d2].width, st.x);
                                }

                                if (textureWrapT == NMGL_CLAMP_TO_EDGE)
                                {
                                    st1.y = wrapCoord(textureWrapT, boundTexObject->texImages2D[d1].height, st.y);
                                    st2.y = wrapCoord(textureWrapT, boundTexObject->texImages2D[d2].height, st.y);
                                }

                                t1 = getPixelLinear(st1, textureWrapS, textureWrapT, boundTexObject->texImages2D[d1]);
                                t2 = getPixelLinear(st2, textureWrapS, textureWrapT, boundTexObject->texImages2D[d2]);
                            }
                            else
                            {
                                printf("textureMinFilter has unsupported value for mipmapping\n");
                            }

                            float frac_lod = lod - floor(lod);

                            pixelValue.r = (1.0 - frac_lod)*t1.r + frac_lod*t2.r;
                            pixelValue.g = (1.0 - frac_lod)*t1.g + frac_lod*t2.g;
                            pixelValue.b = (1.0 - frac_lod)*t1.b + frac_lod*t2.b;
                            pixelValue.a = (1.0 - frac_lod)*t1.a + frac_lod*t2.a;
                        }
                        else
                        {
                            printf("Unsupported parameter combination. Exit.\n");
                            return;
                        }

                        vecr_i[x] = pixelValue.r;
                        vecg_i[x] = pixelValue.g;
                        vecb_i[x] = pixelValue.b;
                        veca_i[x] = pixelValue.a;
                    }
                }

                //for (int x = 0; x < minCount; x++) {
                //    buf0[x] = vecr_i[x] * scaleCoeff1_255; //cs.x
                //    buf1[x] = vecg_i[x] * scaleCoeff1_255; //cs.y
                //    buf2[x] = vecb_i[x] * scaleCoeff1_255; //cs.z
                //    buf3[x] = veca_i[x] * scaleCoeff1_255; //as
                //}
                nmppsConvert_32s32f (vecr_i, buf0, minCount);
                nmppsConvert_32s32f (vecg_i, buf1, minCount);
                nmppsConvert_32s32f (vecb_i, buf2, minCount);
                nmppsConvert_32s32f (veca_i, buf3, minCount);
                nmppsMulC_32f(buf0, buf4, scaleCoeff1_255, minCount);
                nmppsMulC_32f(buf1, buf5, scaleCoeff1_255, minCount);
                nmppsMulC_32f(buf2, buf6, scaleCoeff1_255, minCount);
                nmppsMulC_32f(buf3, buf7, scaleCoeff1_255, minCount);

                //TODO: check minCount for 32, may be it is not necessary to deselect
                deselectElementsByIndices(buf4, minificationIndices, veccsx, sizeof32(float), minCount);
                deselectElementsByIndices(buf5, minificationIndices, veccsy, sizeof32(float), minCount);
                deselectElementsByIndices(buf6, minificationIndices, veccsz, sizeof32(float), minCount);
                deselectElementsByIndices(buf7, minificationIndices, vecas, sizeof32(float), minCount);
                //minificated pixels are ready and stored to veccs, vecas
            }

            //MAGNIFICATION branch
            if (magCount > 0) {
                //TODO: check magCount for 32, may be it is not necessary to select
                selectElementsByIndices((float*)vecs, magnificationIndices, (float*)vecsmag, sizeof32(float), magCount);
                selectElementsByIndices((float*)vect, magnificationIndices, (float*)vectmag, sizeof32(float), magCount);

                if (textureMagFilter == NMGL_NEAREST)
                {
                    ////Not mipmapping. So wrap texture coordinates for texture of level 0 
                    //st.x = wrapCoord(textureWrapS, boundTexObject->texImages2D[0].width, st.x);
                    wrapCoordVec(textureWrapS, boundTexObject->texImages2D[0].width, vecsmag, vecsmag, magCount);
                    //st.y = wrapCoord(textureWrapT, boundTexObject->texImages2D[0].height, st.y);
                    wrapCoordVec(textureWrapT, boundTexObject->texImages2D[0].height, vectmag, vectmag, magCount);
                    //pixelValue = getPixelNearest(st, boundTexObject->texImages2D[0]);
                    getPixelNearestVec(vecsmag, vectmag, boundTexObject->texImages2D[0], vecr_i, vecg_i, vecb_i, veca_i, magCount);
                }
                else if (textureMagFilter == NMGL_LINEAR)
                {
                    //Not mipmapping. So wrap texture coordinates for texture of level 0 
                    if (textureWrapS == NMGL_CLAMP_TO_EDGE)
                        //st.x = wrapCoord(textureWrapS, boundTexObject->texImages2D[0].width, st.x);
                        wrapCoordVec(textureWrapS, boundTexObject->texImages2D[0].width, vecsmag, vecsmag, magCount);

                    if (textureWrapT == NMGL_CLAMP_TO_EDGE)
                        //st.y = wrapCoord(textureWrapT, boundTexObject->texImages2D[0].height, st.y);
                        wrapCoordVec(textureWrapT, boundTexObject->texImages2D[0].height, vectmag, vectmag, magCount);

                    //pixelValue = getPixelLinear(st, textureWrapS, textureWrapT, boundTexObject->texImages2D[0]);
                    getPixelLinearVec(vecsmag, vectmag, textureWrapS, textureWrapT, boundTexObject->texImages2D[0], vecr_i, vecg_i, vecb_i, veca_i, magCount);
                }
                else
                {
                    printf("Unsupported parameter combination. Exit.\n");
                    return;
                }

				//for (int x = 0; x < magCount; x++) {
				//    buf0[x] = vecr_i[x] * scaleCoeff1_255; //cs.x
				//    buf1[x] = vecg_i[x] * scaleCoeff1_255; //cs.y
				//    buf2[x] = vecb_i[x] * scaleCoeff1_255; //cs.z
				//    buf3[x] = veca_i[x] * scaleCoeff1_255; //as
				//}
                nmppsConvert_32s32f (vecr_i, buf0, magCount);
                nmppsConvert_32s32f (vecg_i, buf1, magCount);
                nmppsConvert_32s32f (vecb_i, buf2, magCount);
                nmppsConvert_32s32f (veca_i, buf3, magCount);
                nmppsMulC_32f(buf0, buf4, scaleCoeff1_255, magCount);
                nmppsMulC_32f(buf1, buf5, scaleCoeff1_255, magCount);
                nmppsMulC_32f(buf2, buf6, scaleCoeff1_255, magCount);
                nmppsMulC_32f(buf3, buf7, scaleCoeff1_255, magCount);

                //TODO: check magCount for 32, may be it is not necessary to select
                deselectElementsByIndices(buf4, magnificationIndices, veccsx, sizeof32(float), magCount);
                deselectElementsByIndices(buf5, magnificationIndices, veccsy, sizeof32(float), magCount);
                deselectElementsByIndices(buf6, magnificationIndices, veccsz, sizeof32(float), magCount);
                deselectElementsByIndices(buf7, magnificationIndices, vecas, sizeof32(float), magCount);
                //magnificated pixels are ready and stored to veccs, vecas
            }
            //minificated and magnificated pixels combined
			
			//Parse 'colors' structure. B,G,R,A order is used in 'colors'. 
			Vec3f cf; //primary color components of the incoming fragment (primary color of PRIMITIVE pixel OR fragment color from previous texture unit)
								//Not framebuffer color.Framebuffer color can be used at another stage called Blending (glBlendFunc...)
			//TODO: colors should be pointer to colored  fragments of incoming primitive. At now all pixels of triangle has one color, so Cf always has the same value.
			float af; 
			cf.x = (float)(colors.vec[1] & 0x00000000ffffffff) * scaleCoeff1_255;//r
			cf.y = (float)((colors.vec[0] >> 32) & 0x00000000ffffffff) * scaleCoeff1_255;//g;
			cf.z = (float)(colors.vec[0] & 0x00000000ffffffff) * scaleCoeff1_255;//b;
			af = (float)((colors.vec[1] >> 32) & 0x00000000ffffffff) * scaleCoeff1_255;//a;
			
			for(int x = 0; x < primWidth; x++){
				veccfx[x] = cf.x;
				veccfy[x] = cf.y;
				veccfz[x] = cf.z;
				vecaf[x] = af;
			}

			Vec3f cc; //texture environment color (unique for each texture unit)
			float ac; 
			cc.x = texEnvColor.x;
			cc.y = texEnvColor.y;
			cc.z = texEnvColor.z;
			ac = texEnvColorAlpha;
						
			//Apply texture function

			switch (texBaseInternalFormat)
			{
				case NMGL_RGB:
					switch (texEnvMode)
					{
						case NMGL_REPLACE:
							//cv.x = cs.x;
							nmblas_scopy(primWidth, veccsx, 1, veccvx, 1);
							//cv.y = cs.y;
							nmblas_scopy(primWidth, veccsy, 1, veccvy, 1);
							//cv.z = cs.z;
							nmblas_scopy(primWidth, veccsz, 1, veccvz, 1);
							//av = af;
							nmblas_scopy(primWidth, vecaf, 1, vecav, 1);
							break;

						case NMGL_MODULATE:
							//cv.x = cf.x * cs.x;
							nmppsMul_32f(veccfx, veccsx, veccvx, primWidth);
							//cv.y = cf.y * cs.y;
							nmppsMul_32f(veccfy, veccsy, veccvy, primWidth);
							//cv.z = cf.z * cs.z;
							nmppsMul_32f(veccfz, veccsz, veccvz, primWidth);
							//av = af;
							nmblas_scopy(primWidth, vecaf, 1, vecav, 1);
							
							break;

						case NMGL_DECAL:
							//cv.x = cs.x;
							nmblas_scopy(primWidth, veccsx, 1, veccvx, 1);
							//cv.y = cs.y;
							nmblas_scopy(primWidth, veccsy, 1, veccvy, 1);
							//cv.z = cs.z;
							nmblas_scopy(primWidth, veccsz, 1, veccvz, 1);						
							//av = af;
							nmblas_scopy(primWidth, vecaf, 1, vecav, 1);
							
							break;

						case NMGL_BLEND:
							//cv.x = cf.x * (1.0 - cs.x) + cc.x * cs.x;
							nmppsSub_32f(ones, veccsx, buf0, primWidth);
							nmppsMul_32f (veccfx, buf0, buf2, primWidth);
							nmppsMulC_32f(veccsx, buf1, cc.x, primWidth);
							nmppsAdd_32f(buf2, buf1, veccvx, primWidth);
							
							//cv.y = cf.y * (1.0 - cs.y) + cc.y * cs.y;
							nmppsSub_32f(ones, veccsy, buf0, primWidth);
							nmppsMul_32f (veccfy, buf0, buf2, primWidth);
							nmppsMulC_32f(veccsy, buf1, cc.y, primWidth);
							nmppsAdd_32f(buf2, buf1, veccvy, primWidth);
							
							//cv.z = cf.z * (1.0 - cs.z) + cc.z * cs.z;
							nmppsSub_32f(ones, veccsz, buf0, primWidth);
							nmppsMul_32f (veccfz, buf0, buf2, primWidth);
							nmppsMulC_32f(veccsz, buf1, cc.z, primWidth);
							nmppsAdd_32f(buf2, buf1, veccvz, primWidth);
							
							//av = af;
							nmblas_scopy(primWidth, vecaf, 1, vecav, 1);
							
							break;

						case NMGL_ADD:
							//cv.x = cf.x + cs.x;
							nmppsAdd_32f(veccfx, veccsx, veccvx, primWidth);
							//cv.y = cf.y + cs.y;
							nmppsAdd_32f(veccfy, veccsy, veccvy, primWidth);
							//cv.z = cf.z + cs.z;
							nmppsAdd_32f(veccfz, veccsz, veccvz, primWidth);
							//av = af;
							nmblas_scopy(primWidth, vecaf, 1, vecav, 1);
							break;
					}
					break;
				case NMGL_RGBA:
					switch (texEnvMode)
					{
						case NMGL_REPLACE:
							//cv.x = cs.x;
							nmblas_scopy(primWidth, veccsx, 1, veccvx, 1);
							//cv.y = cs.y;
							nmblas_scopy(primWidth, veccsy, 1, veccvy, 1);
							//cv.z = cs.z;
							nmblas_scopy(primWidth, veccsz, 1, veccvz, 1);	
							//av = as;
							nmblas_scopy(primWidth, vecas, 1, vecav, 1);	
							break;

						case NMGL_MODULATE:
							//cv.x = cf.x * cs.x;
							nmppsMul_32f(veccfx, veccsx, veccvx, primWidth);
							//cv.y = cf.y * cs.y;
							nmppsMul_32f(veccfy, veccsy, veccvy, primWidth);
							//cv.z = cf.z * cs.z;
							nmppsMul_32f(veccfz, veccsz, veccvz, primWidth);
							//av = af * as;
							nmppsMul_32f(vecaf, vecas, vecav, primWidth);
							break;

						case NMGL_DECAL:
							//cv.x = cf.x * (1.0 - as) + cs.x * as;
							nmppsSub_32f(ones, vecas, buf0, primWidth);
							nmppsMul_32f (veccfx, buf0, buf2, primWidth);
							nmppsMul_32f(veccsx, vecas, buf1, primWidth);
							nmppsAdd_32f(buf2, buf1, veccvx, primWidth);
							
							//cv.y = cf.y * (1.0 - as) + cs.y * as;
							nmppsSub_32f(ones, vecas, buf0, primWidth);
							nmppsMul_32f (veccfy, buf0, buf2, primWidth);
							nmppsMul_32f(veccsy, vecas, buf1, primWidth);
							nmppsAdd_32f(buf2, buf1, veccvy, primWidth);
							
							//cv.z = cf.z * (1.0 - as) + cs.z * as;
							nmppsSub_32f(ones, vecas, buf0, primWidth);
							nmppsMul_32f (veccfz, buf0, buf2, primWidth);
							nmppsMul_32f(veccsz, vecas, buf1, primWidth);
							nmppsAdd_32f(buf2, buf1, veccvz, primWidth);
							
							//av = af;
							nmblas_scopy(primWidth, vecaf, 1, vecav, 1);
							break;

						case NMGL_BLEND:
							//cv.x = cf.x * (1.0 - cs.x) + cc.x * cs.x;
							nmppsSub_32f(ones, veccsx, buf0, primWidth);
							nmppsMul_32f (veccfx, buf0, buf2, primWidth);
							nmppsMulC_32f(veccsx, buf1, cc.x, primWidth);
							nmppsAdd_32f(buf2, buf1, veccvx, primWidth);
							
							//cv.y = cf.y * (1.0 - cs.y) + cc.y * cs.y;
							nmppsSub_32f(ones, veccsy, buf0, primWidth);
							nmppsMul_32f (veccfy, buf0, buf2, primWidth);
							nmppsMulC_32f(veccsy, buf1, cc.y, primWidth);
							nmppsAdd_32f(buf2, buf1, veccvy, primWidth);
							
							//cv.z = cf.z * (1.0 - cs.z) + cc.z * cs.z;
							nmppsSub_32f(ones, veccsz, buf0, primWidth);
							nmppsMul_32f (veccfz, buf0, buf2, primWidth);
							nmppsMulC_32f(veccsz, buf1, cc.z, primWidth);
							nmppsAdd_32f(buf2, buf1, veccvz, primWidth);
							
							//av = af * as;
							nmppsMul_32f(vecaf, vecas, vecav, primWidth);
							break;

						case NMGL_ADD:
							//cv.x = cf.x + cs.x;
							nmppsAdd_32f(veccfx, veccsx, veccvx, primWidth);
							//cv.y = cf.y + cs.y;
							nmppsAdd_32f(veccfy, veccsy, veccvy, primWidth);
							//cv.z = cf.z + cs.z;
							nmppsAdd_32f(veccfz, veccsz, veccvz, primWidth);
							//av = af * as;
							nmppsMul_32f(vecaf, vecas, vecav, primWidth);
							break;
					}
					break;

				case NMGL_ALPHA:
					switch (texEnvMode)
					{
						case NMGL_REPLACE:
							//cv.x = cf.x;
							nmblas_scopy(primWidth, veccfx, 1, veccvx, 1);
							//cv.y = cf.y;
							nmblas_scopy(primWidth, veccfy, 1, veccvy, 1);
							//cv.z = cf.z;
							nmblas_scopy(primWidth, veccfz, 1, veccvz, 1);
							//av = as;
							nmblas_scopy(primWidth, vecas, 1, vecav, 1);
							break;

						case NMGL_MODULATE:
							//cv.x = cf.x;
							nmblas_scopy(primWidth, veccfx, 1, veccvx, 1);
							//cv.y = cf.y;
							nmblas_scopy(primWidth, veccfy, 1, veccvy, 1);
							//cv.z = cf.z;
							nmblas_scopy(primWidth, veccfz, 1, veccvz, 1);
							//av = af * as;
							nmppsMul_32f(vecaf, vecas, vecav, primWidth);
							break;

						case NMGL_DECAL://undefined
							//cv.x = 1.0;
							nmblas_scopy(primWidth, ones, 1, veccvx, 1);
							//cv.y = 1.0;
							nmblas_scopy(primWidth, ones, 1, veccvy, 1);
							//cv.z = 1.0;
							nmblas_scopy(primWidth, ones, 1, veccvz, 1);
							//av = 1.0;
							nmblas_scopy(primWidth, ones, 1, vecav, 1);
							break;

						case NMGL_BLEND:
							//cv.x = cf.x;
							nmblas_scopy(primWidth, veccfx, 1, veccvx, 1);
							//cv.y = cf.y;
							nmblas_scopy(primWidth, veccfy, 1, veccvy, 1);
							//cv.z = cf.z;
							nmblas_scopy(primWidth, veccfz, 1, veccvz, 1);
							//av = af * as;
							nmppsMul_32f(vecaf, vecas, vecav, primWidth);
							break;

						case NMGL_ADD:
							//cv.x = cf.x;
							nmblas_scopy(primWidth, veccfx, 1, veccvx, 1);
							//cv.y = cf.y;
							nmblas_scopy(primWidth, veccfy, 1, veccvy, 1);
							//cv.z = cf.z;
							nmblas_scopy(primWidth, veccfz, 1, veccvz, 1);
							//av = af * as;
							nmppsMul_32f(vecaf, vecas, vecav, primWidth);
							break;
					}
					break;
				case NMGL_LUMINANCE:
					switch (texEnvMode)
					{
						case NMGL_REPLACE:
							//cv.x = cs.x;
							nmblas_scopy(primWidth, veccsx, 1, veccvx, 1);
							//cv.y = cs.y;
							nmblas_scopy(primWidth, veccsy, 1, veccvy, 1);
							//cv.z = cs.z;
							nmblas_scopy(primWidth, veccsz, 1, veccvz, 1);	
							//av = af;
							nmblas_scopy(primWidth, vecaf, 1, vecav, 1);
							break;

						case NMGL_MODULATE:
							//cv.x = cf.x * cs.x;
							nmppsMul_32f(veccfx, veccsx, veccvx, primWidth);
							//cv.y = cf.y * cs.y;
							nmppsMul_32f(veccfy, veccsy, veccvy, primWidth);
							//cv.z = cf.z * cs.z;
							nmppsMul_32f(veccfz, veccsz, veccvz, primWidth);
							//av = af;
							nmblas_scopy(primWidth, vecaf, 1, vecav, 1);
							break;

						case NMGL_DECAL://undefined
							//cv.x = 1.0;
							nmblas_scopy(primWidth, ones, 1, veccvx, 1);
							//cv.y = 1.0;
							nmblas_scopy(primWidth, ones, 1, veccvy, 1);
							//cv.z = 1.0;
							nmblas_scopy(primWidth, ones, 1, veccvz, 1);
							//av = 1.0;
							nmblas_scopy(primWidth, ones, 1, vecav, 1);
							break;

						case NMGL_BLEND:
							//cv.x = cf.x * (1.0 - cs.x) + cc.x * cs.x;
							nmppsSub_32f(ones, veccsx, buf0, primWidth);
							nmppsMul_32f (veccfx, buf0, buf2, primWidth);
							nmppsMulC_32f(veccsx, buf1, cc.x, primWidth);
							nmppsAdd_32f(buf2, buf1, veccvx, primWidth);
							
							//cv.y = cf.y * (1.0 - cs.y) + cc.y * cs.y;
							nmppsSub_32f(ones, veccsy, buf0, primWidth);
							nmppsMul_32f (veccfy, buf0, buf2, primWidth);
							nmppsMulC_32f(veccsy, buf1, cc.y, primWidth);
							nmppsAdd_32f(buf2, buf1, veccvy, primWidth);
							
							//cv.z = cf.z * (1.0 - cs.z) + cc.z * cs.z;
							nmppsSub_32f(ones, veccsz, buf0, primWidth);
							nmppsMul_32f (veccfz, buf0, buf2, primWidth);
							nmppsMulC_32f(veccsz, buf1, cc.z, primWidth);
							nmppsAdd_32f(buf2, buf1, veccvz, primWidth);
							
							//av = af;
							nmblas_scopy(primWidth, vecaf, 1, vecav, 1);
							break;

						case NMGL_ADD:
							//cv.x = cf.x + cs.x;
							nmppsAdd_32f(veccfx, veccsx, veccvx, primWidth);
							//cv.y = cf.y + cs.y;
							nmppsAdd_32f(veccfy, veccsy, veccvy, primWidth);
							//cv.z = cf.z + cs.z;
							nmppsAdd_32f(veccfz, veccsz, veccvz, primWidth);
							//av = af;
							nmblas_scopy(primWidth, vecaf, 1, vecav, 1);
							break;
					}
					break;
				case NMGL_LUMINANCE_ALPHA:
					switch (texEnvMode)
					{
						case NMGL_REPLACE:
							//cv.x = cs.x;
							nmblas_scopy(primWidth, veccsx, 1, veccvx, 1);
							//cv.y = cs.y;
							nmblas_scopy(primWidth, veccsy, 1, veccvy, 1);
							//cv.z = cs.z;
							nmblas_scopy(primWidth, veccsz, 1, veccvz, 1);
							//av = as;
							nmblas_scopy(primWidth, vecas, 1, vecav, 1);	
							break;

						case NMGL_MODULATE:
							//cv.x = cf.x * cs.x;
							nmppsMul_32f(veccfx, veccsx, veccvx, primWidth);
							//cv.y = cf.y * cs.y;
							nmppsMul_32f(veccfy, veccsy, veccvy, primWidth);
							//cv.z = cf.z * cs.z;
							nmppsMul_32f(veccfz, veccsz, veccvz, primWidth);
							//av = af * as;
							nmppsMul_32f(vecaf, vecas, vecav, primWidth);
							break;

						case NMGL_DECAL://undefined
							//cv.x = 1.0;
							nmblas_scopy(primWidth, ones, 1, veccvx, 1);
							//cv.y = 1.0;
							nmblas_scopy(primWidth, ones, 1, veccvy, 1);
							//cv.z = 1.0;
							nmblas_scopy(primWidth, ones, 1, veccvz, 1);
							//av = 1.0;
							nmblas_scopy(primWidth, ones, 1, vecav, 1);
							break;

						case NMGL_BLEND:
							//cv.x = cf.x * (1.0 - cs.x) + cc.x * cs.x;
							nmppsSub_32f(ones, veccsx, buf0, primWidth);
							nmppsMul_32f (veccfx, buf0, buf2, primWidth);
							nmppsMulC_32f(veccsx, buf1, cc.x, primWidth);
							nmppsAdd_32f(buf2, buf1, veccvx, primWidth);
							
							//cv.y = cf.y * (1.0 - cs.y) + cc.y * cs.y;
							nmppsSub_32f(ones, veccsy, buf0, primWidth);
							nmppsMul_32f (veccfy, buf0, buf2, primWidth);
							nmppsMulC_32f(veccsy, buf1, cc.y, primWidth);
							nmppsAdd_32f(buf2, buf1, veccvy, primWidth);
							
							//cv.z = cf.z * (1.0 - cs.z) + cc.z * cs.z;
							nmppsSub_32f(ones, veccsz, buf0, primWidth);
							nmppsMul_32f (veccfz, buf0, buf2, primWidth);
							nmppsMulC_32f(veccsz, buf1, cc.z, primWidth);
							nmppsAdd_32f(buf2, buf1, veccvz, primWidth);
							
							//av = af * as;
							nmppsMul_32f(vecaf, vecas, vecav, primWidth);
							break;

						case NMGL_ADD:
							//cv.x = cf.x + cs.x;
							nmppsAdd_32f(veccfx, veccsx, veccvx, primWidth);
							//cv.y = cf.y + cs.y;
							nmppsAdd_32f(veccfy, veccsy, veccvy, primWidth);
							//cv.z = cf.z + cs.z;
							nmppsAdd_32f(veccfz, veccsz, veccvz, primWidth);
							//av = af * as;
							nmppsMul_32f(vecaf, vecas, vecav, primWidth);
							break;
					}
					break;
					
				default:
					printf ("Unsupported internal format\n");
					break;
			}

            ////Process cv, av
			////Vec3f cv; //primary color components computed by the texture function (to another OpenGL stages or to next texture unit)
			////float av;

            ////Clamp cv, av to 1.0f before color sum (3.9, gl 1.3)
            //if (cv.x > 1.0f) cv.x = 1.0f;
            //if (cv.y > 1.0f) cv.y = 1.0f;
            //if (cv.z > 1.0f) cv.z = 1.0f;
            //if (av > 1.0f) av = 1.0f;
            clamp_32f(veccvx, 0.0, 1.0, buf0, primWidth);
            clamp_32f(veccvy, 0.0, 1.0, buf1, primWidth);
            clamp_32f(veccvz, 0.0, 1.0, buf2, primWidth);
            clamp_32f(vecav , 0.0, 1.0, buf3, primWidth);

            //convert from [0;1] to [0;255]
            nmppsMulC_32f(buf0, buf4, 255, primWidth);
            nmppsMulC_32f(buf1, buf5, 255, primWidth);
            nmppsMulC_32f(buf2, buf6, 255, primWidth);
            nmppsMulC_32f(buf3, buf7, 255, primWidth);

            //convert from float to int
            nmppsConvert_32f32s_floor(buf4, buf0s, 0, primWidth);
            nmppsConvert_32f32s_floor(buf5, buf1s, 0, primWidth);
            nmppsConvert_32f32s_floor(buf6, buf2s, 0, primWidth);
            nmppsConvert_32f32s_floor(buf7, buf3s, 0, primWidth);

            //convert from 32_32_32_32 to ARGB 8_8_8_8
			for(int x = 0; x < primWidth; x++){
                nm32s cvx = buf0s[x];
                nm32s cvy = buf1s[x];
                nm32s cvz = buf2s[x];
                nm32s av = buf3s[x];
				nm32s color = 0;
				//(nm32s)pDst[0] = 0xARGB
				color = color | (av   << 24);   //a
				color = color | (cvx  << 16);   //r
				color = color | (cvy  << 8);    //g
				color = color | cvz;            //b
				pDst[0] = color;
                pDst += 1;
            }

            nmppsAddC_32f(vecyf, buf0, 1.0, primWidth);
            nmblas_scopy(primWidth, buf0, 1, vecyf, 1);
        }
    }

    return;
}

} //end of namespace nm0_version
