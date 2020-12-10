#include "nmtype.h"
#include "demo3d_common.h"
#include "demo3d_nm1.h"
#include "nmgltex_nm1.h"
#include "nmgltex_common.h"
#include <stdio.h>
#include <math.h>
#include <float.h> //TODO: only FLT_EPSILON is used from float.h

#define TEXTURE_TRIANGLE_SECTION ".text_demo3dExt"

#define TEXTURE_ENABLED
#ifdef TEXTURE_ENABLED
//#define USE_BARYCENTRIC
#define PERSPECTIVE_CORRECT

namespace tex_nm1 {
// typedef enum { NEAREST, LINEAR, NEAREST_MIPMAP_NEAREST, NEAREST_MIPMAP_LINEAR, LINEAR_MIPMAP_NEAREST, LINEAR_MIPMAP_LINEAR } filter_mode_t;
// typedef enum { REPEAT, CLAMP_TO_EDGE } wrap_mode_t;
typedef enum { MINIFICATION, MAGNIFICATION } lod_t;
// typedef enum { MODULATE, REPLACE, DECAL, BLEND, ADD} texEnv_mode_t;

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

// filter_mode_t textureMinFilter = NEAREST; //default NEAREST_MIPMAP_LINEAR
// filter_mode_t textureMagFilter = NEAREST; //default LINEAR
// wrap_mode_t textureWrapS = REPEAT; // default REPEAT
// wrap_mode_t textureWrapT = REPEAT; // default REPEAT
float c = 0.0; // minification vs. magnification switchover point, look glspec 1.3, chapter 3.8.8
lod_t minMagFlag = MINIFICATION;
unsigned int borderWidth = 0; //TEXTURE BORDER - texture image's specified border width
int textureBaseLevel = 0;
int textureMaxLevel = 1000;
color borderColor;

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
	int alignment = 1; //texImage2D loads texture so that unpackAlignment is 1

#ifdef DEBUG
		// if (alignment)
		// {
			// printf("%s: wrong alignment value (%d)", __func__, alignment);
		// }
#endif

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

	int rowPaddingBytes = (width * bytesInPixel) % alignment ? alignment - (width * bytesInPixel) % alignment : 0;	
	unsigned int imageRowWidthBytes = width * bytesInPixel + rowPaddingBytes; 

	unsigned int pixelPos = y * imageRowWidthBytes + x * bytesInPixel;

    if ((format == NMGL_RGB) && (type == NMGL_UNSIGNED_BYTE))
    {
        //Чтение производится из массива данных изображения bmp с учетом наличия в нём 
        //дополнительных байтов для выравнивания по границе 4 байтов
        pixelValue->r = ((NMGLubyte*)pixels)[pixelPos];
        pixelValue->g = ((NMGLubyte*)pixels)[pixelPos + 1];
        pixelValue->b = ((NMGLubyte*)pixels)[pixelPos + 2];
		pixelValue->a = 255;
    }
    else if (((format == NMGL_RGBA) && (type == NMGL_UNSIGNED_BYTE)))
    {
        //Чтение производится из массива данных изображения bmp с учетом наличия в нём 
        //дополнительных байтов для выравнивания по границе 4 байтов
        pixelValue->r = ((NMGLubyte*)pixels)[pixelPos];
        pixelValue->g = ((NMGLubyte*)pixels)[pixelPos + 1];
        pixelValue->b = ((NMGLubyte*)pixels)[pixelPos + 2];
        pixelValue->a = ((NMGLubyte*)pixels)[pixelPos + 3];
    }
    else if ((format == NMGL_ALPHA) && (type == NMGL_UNSIGNED_BYTE))
    {
        //Чтение производится из массива данных изображения bmp с учетом наличия в нём 
        //дополнительных байтов для выравнивания по границе 4 байтов
        pixelValue->r = 0;
        pixelValue->g = 0;
        pixelValue->b = 0;
        pixelValue->a = ((NMGLubyte*)pixels)[pixelPos];
    }
    else if ((format == NMGL_LUMINANCE) && (type == NMGL_UNSIGNED_BYTE))
    {
        //Чтение производится из массива данных изображения bmp с учетом наличия в нём 
        //дополнительных байтов для выравнивания по границе 4 байтов
        pixelValue->r = ((NMGLubyte*)pixels)[pixelPos];
        pixelValue->g = ((NMGLubyte*)pixels)[pixelPos];
        pixelValue->b = ((NMGLubyte*)pixels)[pixelPos];
        pixelValue->a = 255;
    }
    else if ((format == NMGL_LUMINANCE_ALPHA) && (type == NMGL_UNSIGNED_BYTE))
    {
        //Чтение производится из массива данных изображения bmp с учетом наличия в нём 
        //дополнительных байтов для выравнивания по границе 4 байтов
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
float wrapCoord (NMGLint textureWrapMode, int texAxisSize, float texCoord)
{
	float min_coord_val = 1 / (float)texAxisSize*0.5; //CLAMP_TO_EDGE
	//float min_s = 0.0f; //CLAMP
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
		getchar();
		return -1.0;
	}

	return resTexCoord;
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
#endif //TEXTURE_ENABLED

SECTION(TEXTURE_TRIANGLE_SECTION)
void textureTriangle(Pattern* patterns, 
                 TrianglesInfo* triangles,
                 nm32s** pROI,
				 Vector2* ptrnPoints,
		         Size* ptrnSizes,
                 nm32s* pSrcTriangle, 
                 nm32s* pDstTriangle, 
                 int count)
{
#ifdef TEXTURE_ENABLED

#ifdef DEBUG
    // printf ("Start textureTriangle\n"); 
#endif //DEBUG
    
    NMGL_Context_NM1 *cntxt = NMGL_Context_NM1::getContext();
    
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
    nm32s* src = pSrcTriangle;
    int winX0 = 0;
    int winY0 = 0;
    
    for(int cnt=0;cnt<count;cnt++){
        nm64s* pattern = (nm64s*) (patterns + cnt);
        pattern += ptrnPoints[cnt].y;
        
        int width = ptrnSizes[cnt].width;

        if (ptrnPoints[cnt].x < 0) {
            width += ptrnPoints[cnt].x;
        }
        
#ifdef __NM__
		int winOffset = ((int)(pROI[cnt]) - (int)cntxt->smallColorBuff.mData) /*>> 2*/;//>> 2 = divide by sizeof int = 4 bytes
#else //__NM__
		int winOffset = ((int)(pROI[cnt]) - (int)cntxt->smallColorBuff.mData) >> 2;//>> 2 = divide by sizeof int = 4 bytes
#endif //__NM__
																			   //TODO:remove magic number 2
		winY0 = cntxt->texState.segY0 + (winOffset >> 7); //>> 7 = divide by segment width 
		//winY0 = winY0 - 1; //-1 to get the same barycentric values as for OpenGL
														 //TODO: remove magic number 7
		winX0 = cntxt->texState.segX0 + (winOffset & (cntxt->texState.segWidth-1));//get reminder  of division by segment width
        
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

#ifdef PERSPECTIVE_CORRECT
		//Precompute reciprocal of vertex z-coordinate.
		//Part of calculation perspective correct attribute values using barycentric coordinates.
		z0 = 1 / z0;
		z1 = 1 / z1;
		z2 = 1 / z2;

		// Prepare vertex attributes. Divde them biy their vertex z-coordinate
		// (though we use a multiplication here because v.z = 1 / v.z)
		//Part of calculation of perspective correct attribute values using barycentric coordinates.
        s0 *= z0;
        s1 *= z1;
        s2 *= z2;
        t0 *= z0;
        t1 *= z1;
        t2 *= z2;
#endif //PERSPECTIVE_CORRECT

#ifdef USE_BARYCENTRIC
        // Area of triangle.
        // Part of calculation attribute values using barycentric coordinates.
        edgeFunction(x0, y0, x1, y1, x2, y2, &area);
        float oneOverArea = 1.0/area;
        int pixelCnt = 0;
#endif //USE_BARYCENTRIC
        
        for(int y = 0; y < ptrnSizes[cnt].height; y++){
            temp = pattern[y];
            nm32s* pDst = (nm32s*)(dst + y * ptrnSizes[cnt].width);
            nm32s* pSrc = (nm32s*)(src + y * ptrnSizes[cnt].width);
            if (ptrnPoints[cnt].x < 0) {
                for(int i = 0; i > ptrnPoints[cnt].x; i--){
                    pDst[0] = 0x00000000;
                    pDst+=1;
                    pSrc+=1;
                }
            }
            else {
                temp >>= (ptrnPoints[cnt].x * 2);
            }
            
            for(int x = 0; x < width; x++){
                int mul = temp & 0x3;

                if (mul > 0)//pixel belongs to triangle
                {
                    //Calculate x and y of current pixel as float values
                    //relative to triangle vertex coordinates inside segment
                    float xf = winX0 + x + 0.5f; 
                    float yf = winY0 + y + 0.5f;//TODO: Барицентрические координаты не соответствуют растеризованной картинке,
                                               //то есть растеризованные по шаблону пиксели не должны быть растеризованы, 
											   //если использовать барицентрические координаты.
                                               //Но так как они растеризованы, то для них вычисляются неверные барицентрические
											   //координаты и неправильные значения текстурных координат
                                               //Нужно как-то соотнести алгоритм растеризации и вычисление текстурных координат
                    float s = 0.0;
                    float t = 0.0;
#ifdef USE_BARYCENTRIC
                    float w0 = 0;
                    float w1 = 0;
                    float w2 = 0;
                    //TODO: it works only for triangle in CW order
                    edgeFunction(x1, y1, x2, y2, xf, yf, &w0);
                    edgeFunction(x2, y2, x0, y0, xf, yf, &w1);
                    edgeFunction(x0, y0, x1, y1, xf, yf, &w2);
                    
                    w0 = w0*oneOverArea;
                    w1 = w1*oneOverArea;
                    w2 = w2*oneOverArea;
                    
                    // printf ("\n\n pixelCnt = %d xf = %f yf = %f\n", pixelCnt++, xf, yf);
                    // if (w0 < 0)
                    // {
                        // printf ("w0 < 0 w0 = %f\n", w0);
                    // }
                    
                    // if (w1 < 0)
                    // {
                        // printf ("w1 < 0 w1 = %f\n", w1);
                    // }
                    
                    // if (w2 < 0)
                    // {
                        // printf ("w2 < 0 w2 = %f\n", w2);
                    // }
#ifdef PERSPECTIVE_CORRECT
                    //Part of calculation of perspective correct attribute values using barycentric coordinates.
                    float oneOverZ = z0 * w0 + z1 * w1 + z2 * w2;
                    float z = 1 / oneOverZ;
#endif //PERSPECTIVE_CORRECT
                    s = s0 * w0 + s1 * w1 + s2 * w2;
                    t = t0 * w0 + t1 * w1 + t2 * w2;
#ifdef PERSPECTIVE_CORRECT
                    s *= z;
                    t *= z;
#endif //PERSPECTIVE_CORRECT
#else //USE_BARYCENTRIC
#ifdef PERSPECTIVE_CORRECT
                    float oneOverDenominator = 1 / (A2*xf + B2*yf + D2);
                    float rl_s = (A1_s*xf + B1_s*yf + D1_s) * oneOverDenominator;
                    float rl_t = (A1_t*xf + B1_t*yf + D1_t) * oneOverDenominator;
                    s = rl_s;
                    t = rl_t;
#else //PERSPECTIVE_CORRECT                    
                    s = A_s*xf + B_s*yf + D_s;
                    t = A_t*xf + B_t*yf + D_t;
#endif //PERSPECTIVE_CORRECT
#endif //USE_BARYCENTRIC
                    Vec2f st;
                    st.x = s;
                    st.y = t;

/* Calculate minification vs. magnification switchover point */

						if ((textureMagFilter == NMGL_LINEAR) && ((textureMinFilter == NMGL_NEAREST_MIPMAP_NEAREST) || (textureMinFilter == NMGL_NEAREST_MIPMAP_LINEAR)))
							c = 0.5f;
						else
							c = 0.0f;

						/*************************************************************/

/*Calculate partial derivative for u(x,y) and v(x,y). level 0 texture are using to calculate scale factor*/

#ifdef PERSPECTIVE_CORRECT
						float derivOneOverDenom = 1.0 / ((A2*x + B2*y + D2)*(A2*x + B2*y + D2));//TODO: may be xf, yf should be used
						float dudx = (float)boundTexObject->texImages2D[0].width*((A1_s*B2 - A2*B1_s)*y + A1_s*D2 - A2*D1_s)*derivOneOverDenom;
						float dudy = (float)boundTexObject->texImages2D[0].width*((B1_s*A2 - B2*A1_s)*x + B1_s*D2 - B2*D1_s)*derivOneOverDenom;
						float dvdx = (float)boundTexObject->texImages2D[0].height*((A1_t*B2 - A2*B1_t)*y + A1_t*D2 - A2*D1_t)*derivOneOverDenom;
						float dvdy = (float)boundTexObject->texImages2D[0].height*((B1_t*A2 - B2*A1_t)*x + B1_t*D2 - B2*D1_t)*derivOneOverDenom;
#else //PERSPECTIVE_CORRECT  
						float dudx = (float)boundTexObject->texImages2D[0].width*A_s;
						float dudy = (float)boundTexObject->texImages2D[0].width*B_s;
						float dvdx = (float)boundTexObject->texImages2D[0].height*A_t;
						float dvdy = (float)boundTexObject->texImages2D[0].height*B_t;
#endif //PERSPECTIVE_CORRECT

/*Calculate scale factor*/
#ifdef SCALE_FACTOR_IDEAL
						scaleFactor = fmax(sqrtf(dudx*dudx + dvdx*dvdx), sqrtf(dudy*dudy + dvdy*dvdy));
#else
						float mu = fmax(fabs(dudx), fabs(dudy));
						float mv = fmax(fabs(dvdx), fabs(dvdy));
						scaleFactor = fmax(mu, mv);
#endif
						//printf("Scale factor = %f\n", scaleFactor);

/*Calculate level of detail*/

						float lod = 0.0;
						float lod_ = log2f(scaleFactor);

						if (NMGL_TEX_MIN_LOD > NMGL_TEX_MAX_LOD)
						{
							printf("Error. NMGL_TEX_MIN_LOD > NMGL_TEX_MAX_LOD. LOD is undefined. Exit\n");
							return;
						}
						else if (lod_ > NMGL_TEX_MAX_LOD)
							lod = NMGL_TEX_MAX_LOD;
						else if (lod_ < NMGL_TEX_MIN_LOD)
							lod = NMGL_TEX_MIN_LOD;
						else
							lod = lod_;

						//if ((lod < c) || (equalf(lod,c) == 1))
						if (lod <= c) 
							minMagFlag = MAGNIFICATION;
						else 
							minMagFlag = MINIFICATION;

						//printf("Level of detail = %f\n", lod);
						//printf("c = %f\n", c);

						//if(minMagFlag == MAGNIFICATION)
						//	printf("minMagFlag = MAGNIFICATION\n");
						//else
						//	printf("minMagFlag = MINIFICATION\n");

//						getchar();

						unsigned int d = 0;
						unsigned int d1 = 0;
						unsigned int d2 = 0;


						if (((minMagFlag == MINIFICATION) && (textureMinFilter == NMGL_NEAREST)) ||
							((minMagFlag == MAGNIFICATION) && (textureMagFilter == NMGL_NEAREST)))
						{

							//Not mipmapping. So wrap texture coordinates for texture of level 0 
							st.x = wrapCoord(textureWrapS, boundTexObject->texImages2D[0].width, st.x);
							st.y = wrapCoord(textureWrapT, boundTexObject->texImages2D[0].height, st.y);

							pixelValue = getPixelNearest(st, boundTexObject->texImages2D[0]);
						}
						else if (((minMagFlag == MINIFICATION) && (textureMinFilter == NMGL_LINEAR)) ||
							((minMagFlag == MAGNIFICATION) && (textureMagFilter == NMGL_LINEAR)))
						{

							//Not mipmapping. So wrap texture coordinates for texture of level 0 
							if (textureWrapS == NMGL_CLAMP_TO_EDGE)
								st.x = wrapCoord(textureWrapS, boundTexObject->texImages2D[0].width, st.x);
							
							if (textureWrapT == NMGL_CLAMP_TO_EDGE)
								st.y = wrapCoord(textureWrapT, boundTexObject->texImages2D[0].height, st.y);

							pixelValue = getPixelLinear(st, textureWrapS, textureWrapT, boundTexObject->texImages2D[0]);
						}
						else if ((minMagFlag == MINIFICATION) && ((textureMinFilter == NMGL_NEAREST_MIPMAP_NEAREST) || (textureMinFilter == NMGL_LINEAR_MIPMAP_NEAREST)))
						{
							d = 0;
							if (lod < 0.5f || (equalf(lod,0.5f) == 1))
								d = NMGL_TEX_BASE_LEVEL;
							else if ((NMGL_TEX_BASE_LEVEL + lod) <= ((float)q + 0.5f))
								d = ceil(NMGL_TEX_BASE_LEVEL + lod + 0.5f) - 1.0f;
							else if ((NMGL_TEX_BASE_LEVEL + lod) > ((float)q + 0.5f))
								d = q;
							else
							{
								printf("mipmap:NMGL_NEAREST_MIPMAP_NEAREST: d is undefined\n");
								getchar();
							}

							if (textureMinFilter == NMGL_NEAREST_MIPMAP_NEAREST)
							{
								//Mipmapping. So wrap texture coordinates for texture of selected level d 
								st.x = wrapCoord(textureWrapS,boundTexObject->texImages2D[d].width, st.x);
								st.y = wrapCoord(textureWrapT,boundTexObject->texImages2D[d].height, st.y);
								pixelValue = getPixelNearest(st, boundTexObject->texImages2D[d]);
							}
							else if (textureMinFilter == NMGL_LINEAR_MIPMAP_NEAREST)
							{
								//Mipmapping. So wrap texture coordinates for texture of selected level d 
								if (textureWrapS == NMGL_CLAMP_TO_EDGE)
									st.x = wrapCoord(textureWrapS,boundTexObject->texImages2D[d].width, st.x);
								
								if (textureWrapT == NMGL_CLAMP_TO_EDGE)
									st.y = wrapCoord(textureWrapT,boundTexObject->texImages2D[d].height, st.y);
								
								pixelValue = getPixelLinear(st, textureWrapS, textureWrapT, boundTexObject->texImages2D[d]);
							}
							else
							{
								printf("textureMinFilter has unsupported value for mipmapping\n");
								getchar();
							}

						}
						else if ((minMagFlag == MINIFICATION) && ((textureMinFilter == NMGL_NEAREST_MIPMAP_LINEAR) || (textureMinFilter == NMGL_LINEAR_MIPMAP_LINEAR)))
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

							//Mipmapping. So wrap texture coordinates for texture of selected level d1 and d2
							Vec2f st1 = st;
							Vec2f st2 = st;

							if (textureMinFilter == NMGL_NEAREST_MIPMAP_LINEAR)
							{
								//Mipmapping. So wrap texture coordinates for texture of selected level d1 and d2
								st1.x = wrapCoord(textureWrapS,boundTexObject->texImages2D[d1].width,st.x);
								st1.y = wrapCoord(textureWrapT,boundTexObject->texImages2D[d1].height,st.y);
								st2.x = wrapCoord(textureWrapS,boundTexObject->texImages2D[d2].width,st.x);
								st2.y = wrapCoord(textureWrapT,boundTexObject->texImages2D[d2].height,st.y);
								
								t1 = getPixelNearest(st1, boundTexObject->texImages2D[d1]);
								t2 = getPixelNearest(st2, boundTexObject->texImages2D[d2]);
							}
							else if (textureMinFilter == NMGL_LINEAR_MIPMAP_LINEAR)
							{
								//Mipmapping. So wrap texture coordinates for texture of selected level d1 and d2
								if (textureWrapS == NMGL_CLAMP_TO_EDGE)
								{
									st1.x = wrapCoord(textureWrapS,boundTexObject->texImages2D[d1].width, st.x);
									st2.x = wrapCoord(textureWrapS,boundTexObject->texImages2D[d2].width, st.x);
								}
								
								if (textureWrapT == NMGL_CLAMP_TO_EDGE)
								{
									st1.y = wrapCoord(textureWrapT,boundTexObject->texImages2D[d1].height, st.y);
									st2.y = wrapCoord(textureWrapT,boundTexObject->texImages2D[d2].height, st.y);
								}
								
								t1 = getPixelLinear(st1, textureWrapS, textureWrapT, boundTexObject->texImages2D[d1]);
								t2 = getPixelLinear(st2, textureWrapS, textureWrapT, boundTexObject->texImages2D[d2]);
							}
							else
							{
								printf("textureMinFilter has unsupported value for mipmapping\n");
								getchar();
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
							getchar();
                            return;
						}
                        
						//Apply texture finction
						//RGB value
						Vec3f cf; //primary color components of the incoming fragment (primary color of PRIMITIVE pixel OR fragment color from previous texture unit)
								  //Not framebuffer color.Framebuffer color can be used at another stage called Blending (glBlendFunc...)
						Vec3f cs; //texture source color (color from texture array, one tex unit - one texture)
						Vec3f cc; //texture environment color (unique for each texture unit)
						Vec3f cv; //primary color components computed by the texture function (to another OpenGL stages or to next texture unit)

						//Alpha value
						float af; 
						float as;
						float ac; 
						float av;
                        
						//(nm32s)pSrc[0] = 0xARGB
                        vertexRGB.x = ((pSrc[0] >> 16 ) & 0xff)/255.0;//r
                        vertexRGB.y = ((pSrc[0] >> 8) & 0xff)/255.0;//g
                        vertexRGB.z = (pSrc[0] & 0xff)/255.0;//b
                        vertexAlpha = (((pSrc[0]) >> 24) & 0xff)/255.0;//a

						//TODO: extra assignment. name vertexRGB is umbiguous and unnecessary.
						cf.x = vertexRGB.x;
						cf.y = vertexRGB.y;
						cf.z = vertexRGB.z;

						cs.x = (float)pixelValue.r/255.0;
						cs.y = (float)pixelValue.g/255.0;
						cs.z = (float)pixelValue.b/255.0;

						cc.x = texEnvColor.x;
						cc.y = texEnvColor.y;
						cc.z = texEnvColor.z;

						cv.x = 0.0;
						cv.y = 0.0;
						cv.z = 0.0;

						af = vertexAlpha;
						as = (float)pixelValue.a/255.0;  
						ac = texEnvColorAlpha;
						av = 0.0;

						switch (texBaseInternalFormat)
						{
						case NMGL_RGB:
							switch (texEnvMode)
							{
								case NMGL_REPLACE:
									cv.x = cs.x;
									cv.y = cs.y;
									cv.z = cs.z;
									av = af;
									break;

								case NMGL_MODULATE:
									cv.x = cf.x * cs.x;
									cv.y = cf.y * cs.y;
									cv.z = cf.z * cs.z;
									av = af;
									break;

								case NMGL_DECAL:
									cv.x = cs.x;
									cv.y = cs.y;
									cv.z = cs.z;
									av = af;
									break;

								case NMGL_BLEND:
									cv.x = cf.x * (1.0 - cs.x) + cc.x * cs.x;
									cv.y = cf.y * (1.0 - cs.y) + cc.y * cs.y;
									cv.z = cf.z * (1.0 - cs.z) + cc.z * cs.z;
									av = af;
									break;

								case NMGL_ADD:
									cv.x = cf.x + cs.x;
									cv.y = cf.y + cs.y;
									cv.z = cf.z + cs.z;
									av = af;
									break;
							}
							break;
						case NMGL_RGBA:
							switch (texEnvMode)
							{
								case NMGL_REPLACE:
									cv.x = cs.x;
									cv.y = cs.y;
									cv.z = cs.z;
									av = as;
									break;

								case NMGL_MODULATE:
									cv.x = cf.x * cs.x;
									cv.y = cf.y * cs.y;
									cv.z = cf.z * cs.z;
									av = af * as;
									break;

								case NMGL_DECAL:
								  	cv.x = cf.x * (1.0 - as) + cs.x * as;
									cv.y = cf.y * (1.0 - as) + cs.y * as;
									cv.z = cf.z * (1.0 - as) + cs.z * as;
									av = af;
									break;

								case NMGL_BLEND:
									cv.x = cf.x * (1.0 - cs.x) + cc.x * cs.x;
									cv.y = cf.y * (1.0 - cs.y) + cc.y * cs.y;
									cv.z = cf.z * (1.0 - cs.z) + cc.z * cs.z;
									av = af * as;
									break;

								case NMGL_ADD:
									cv.x = cf.x + cs.x;
									cv.y = cf.y + cs.y;
									cv.z = cf.z + cs.z;
									av = af * as;
									break;
							}
							break;

						case NMGL_ALPHA:
							switch (texEnvMode)
							{
								case NMGL_REPLACE:
									cv.x = cf.x;
									cv.y = cf.y;
									cv.z = cf.z;
									av = as;
									break;

								case NMGL_MODULATE:
									cv.x = cf.x;
									cv.y = cf.y;
									cv.z = cf.z;
									av = af * as;
									break;

								case NMGL_DECAL://undefined
								  	cv.x = 1.0;
									cv.y = 1.0;
									cv.z = 1.0;
									av = 1.0;
									break;

								case NMGL_BLEND:
									cv.x = cf.x;
									cv.y = cf.y;
									cv.z = cf.z;
									av = af * as;
									break;

								case NMGL_ADD:
	 							    	cv.x = cf.x;
									cv.y = cf.y;
									cv.z = cf.z;
									av = af * as;
									break;
							}
							break;
						case NMGL_LUMINANCE:
							switch (texEnvMode)
							{
								case NMGL_REPLACE:
									cv.x = cs.x;
									cv.y = cs.y;
									cv.z = cs.z;
									av = af;
									break;

								case NMGL_MODULATE:
									cv.x = cf.x * cs.x;
									cv.y = cf.y * cs.y;
									cv.z = cf.z * cs.z;
									av = af;
									break;

								case NMGL_DECAL://undefined
									cv.x = 1.0;
									cv.y = 1.0;
									cv.z = 1.0;
									av = 1.0;
									break;

								case NMGL_BLEND:
									cv.x = cf.x * (1.0 - cs.x) + cc.x * cs.x;
									cv.y = cf.y * (1.0 - cs.y) + cc.y * cs.y;
									cv.z = cf.z * (1.0 - cs.z) + cc.z * cs.z;
									av = af;
									break;

								case NMGL_ADD:
									cv.x = cf.x + cs.x;
									cv.y = cf.y + cs.y;
									cv.z = cf.z + cs.z;
									av = af;
									break;
							}
							break;
						case NMGL_LUMINANCE_ALPHA:
							switch (texEnvMode)
							{
								case NMGL_REPLACE:
									cv.x = cs.x;
									cv.y = cs.y;
									cv.z = cs.z;
									av = as;
									break;

								case NMGL_MODULATE:
									cv.x = cf.x * cs.x;
									cv.y = cf.y * cs.y;
									cv.z = cf.z * cs.z;
									av = af * as;
									break;

								case NMGL_DECAL://undefined
									cv.x = 1.0;
									cv.y = 1.0;
									cv.z = 1.0;
									av = 1.0;
									break;

								case NMGL_BLEND:
									cv.x = cf.x * (1.0 - cs.x) + cc.x * cs.x;
									cv.y = cf.y * (1.0 - cs.y) + cc.y * cs.y;
									cv.z = cf.z * (1.0 - cs.z) + cc.z * cs.z;
									av = af * as;
									break;

								case NMGL_ADD:
									cv.x = cf.x + cs.x;
									cv.y = cf.y + cs.y;
									cv.z = cf.z + cs.z;
									av = af * as;
									break;
							}
							break;
					        default:
							printf ("Unsupported internal format\n");
							break;
						}
						  
					//Clamp  to 1.0f before color sum (3.9, gl 1.3)
					if (cv.x > 1.0f) cv.x = 1.0f;
					if (cv.y > 1.0f) cv.y = 1.0f;
					if (cv.z > 1.0f) cv.z = 1.0f;
					if (av > 1.0f) av = 1.0f;
					
                    nm32s color = 0;
					//(nm32s)pDst[0] = 0xARGB
					color = color | (((nm32s)(av * 255) & 0xff) << 24);//a
					color = color | (((nm32s)(cv.x * 255) & 0xff) << 16); //r
                    color = color | (((nm32s)(cv.y * 255) & 0xff) << 8);//g
                    color = color | (((nm32s)(cv.z * 255) & 0xff));//b
                    // pDst[0] = mul * pSrc[0];
                    pDst[0] = mul * color;
                }

                pDst += 1;
                pSrc += 1;
                temp >>= 2;
            }
        }
        src += ptrnSizes[cnt].height * ptrnSizes[cnt].width;
        dst += ptrnSizes[cnt].height * ptrnSizes[cnt].width;
    }
#ifdef DEBUG
    // printf ("End textureTriangle\n");     
#endif //DEBUG

#endif //TEXTURE_ENABLED
    return;
}

#ifdef TEXTURE_ENABLED
SECTION(TEXTURE_TRIANGLE_SECTION)
void edgeFunction(float x0, float y0, float x1, float y1, float x2, float y2, float* res)
{
    *res = (x2 - x0) * (y1 - y0) - (y2 - y0) * (x1 - x0);
}
#endif //TEXTURE_ENABLED

} //end of namespace tex_nm1 
