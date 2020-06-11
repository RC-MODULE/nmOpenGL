#include "nmtype.h"
#include "demo3d_common.h"
#include "demo3d_nm1.h"
#include "nmgltex_nm1.h"
#include "textureTriangle.h"
#include "tex_common.h"
#include <stdio.h>
#include <math.h>
#include <float.h>

// #define USE_BARYCENTRIC

extern NMGL_Context_NM1 cntxt; 

#define  TEXTURE_MIN_LOD   -1000
#define  TEXTURE_MAX_LOD    1000
#define  TEXTURE_BASE_LEVEL 0
#define  TEXTURE_MAX_LEVEL  1000	

// typedef enum { NEAREST, LINEAR, NEAREST_MIPMAP_NEAREST, NEAREST_MIPMAP_LINEAR, LINEAR_MIPMAP_NEAREST, LINEAR_MIPMAP_LINEAR } filter_mode_t;
// typedef enum { REPEAT, CLAMP_TO_EDGE } wrap_mode_t;
typedef enum { MINIFICATION, MAGNIFICATION } lod_t;
typedef enum { MODULATE, REPLACE, DECAL, BLEND, ADD} texEnv_mode_t;

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

//texEnvColor consists of rgb + alpha
Vec3f texEnvColor;
float texEnvColorAlpha;

NMGLint texEnvMode = NMGL_REPLACE; //default = NMGL_MODULATE
NMGLint texBaseInternalFormat = NMGL_RGB; //At now only RGB texture internal format is supported

int max (int a, int b)
{
    return (b > a) ? b : a;
}

int min (int a, int b)
{
    return (b < a) ? b : a;
}

/* Compare two floats, 1 if are equal, 0 - otherwise */
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

    if (format == NMGL_RGB)
        bitsInPixel = 24;
    else if (format == NMGL_RGBA)
        bitsInPixel = 32;

    bytesInPixel = bitsInPixel / 8;
    rowDataSize = ceil((width*bytesInPixel) / 4.0f) * 4; //width in bytes aligned by 4 bytes
    rowPadding = rowDataSize - width*bytesInPixel; //bytes to align
                                                   // printf ("%d\n", rowPadding);
    if ((format == NMGL_RGB) && (type == NMGL_UNSIGNED_BYTE))
    {
        //Чтение производится из массива данных изображения bmp с учетом наличия в нём 
        //дополнительных байтов для выравнивания по границе 4 байтов
        pixelValue->r = ((unsigned char*)pixels)[y * (width * 3 + rowPadding) + x * 3];
        pixelValue->g = ((unsigned char*)pixels)[y * (width * 3 + rowPadding) + x * 3 + 1];
        pixelValue->b = ((unsigned char*)pixels)[y * (width * 3 + rowPadding) + x * 3 + 2];
    }
    else if (((format == NMGL_RGBA) && (type == NMGL_UNSIGNED_BYTE)))
    {
        //Чтение производится из массива данных изображения bmp с учетом наличия в нём 
        //дополнительных байтов для выравнивания по границе 4 байтов
        pixelValue->r = ((unsigned char*)pixels)[(y * width + x) * 4];
        pixelValue->g = ((unsigned char*)pixels)[(y * width + x) * 4 + 1];
        pixelValue->b = ((unsigned char*)pixels)[(y * width + x) * 4 + 2];
        pixelValue->a = ((unsigned char*)pixels)[(y * width + x) * 4 + 3];
    }
    else
    {
        printf ("Error: %s %s %d", __FILE__, __func__, __LINE__);
    }
    return 0;
}


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

color getPixelLinear(Vec2f st, NMGLint textureWrapS, NMGLint textureWrapT, TexImage2D texture)
{

	unsigned int i0 = 0;
	unsigned int j0 = 0;
	unsigned int i1 = 0;
	unsigned int j1 = 0;
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

	double intpart = 0.0;
	float alpha = modf(u - 0.5, &intpart);
	float beta = modf(v - 0.5, &intpart);

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

	return pixelValue;//TODO return by pointer
}

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

SECTION(".text_demo3d")
void textureTriangle(Pattern* patterns, 
                 Triangles* triangles,
                 Rectangle* windows, 
                 nm32s* pSrcTriangle, 
                 nm32s* pDstTriangle, 
                 int count)
{
    printf ("Start textureTriangle\n"); 
    
    //Активный текстурный модуль
    cntxt.texState.activeTexUnitIndex = 0;
    unsigned int activeTexUnitIndex = cntxt.texState.activeTexUnitIndex;
    
    //Текстурный объект, привязанный к активному текстурному модулю
    cntxt.texState.texUnits[activeTexUnitIndex].boundTexObject = &cntxt.texState.texObjects[0];
    TexObject* boundTexObject = cntxt.texState.texUnits[activeTexUnitIndex].boundTexObject;
    
    float scaleFactor = 1.0;
	borderColor.r = 0.0f;
	borderColor.g = 0.0f;
	borderColor.b = 0.0f;
	borderColor.a = 0.0f;
    
    color pixelValue;
	pixelValue.r = 0;
	pixelValue.g = 0;
	pixelValue.b = 0;
    
	texEnvColor.x = 0.0f;
	texEnvColor.y = 0.0f;
	texEnvColor.y = 0.0f;
	texEnvColorAlpha = 0.0f;

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


    
	//Calculate some parameters from OpenGL 1.3 spec
	int n = log2(boundTexObject->texImages2D[0].width);
	int m = log2(boundTexObject->texImages2D[0].height);
	int p = max(n,m) + TEXTURE_BASE_LEVEL; //p = max{n,m,l} + TEXTURE_BASE_LEVEL
	int q = min(p,TEXTURE_MAX_LEVEL);//min{p,TEXTURE_MAX_LEVEL} page 140, glspec 1.3
    
    long long int temp;
    nm32s* dst = pDstTriangle;
    nm32s* src = pSrcTriangle;
    for(int cnt=0;cnt<count;cnt++){
        nm64s* pattern = (nm64s*) (patterns + cnt);
        pattern += windows[cnt].y;
        
        int width = windows[cnt].width;

        if (windows[cnt].x < 0) {
            width += windows[cnt].x;
        }
        
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
        
        float z0 = triangles->z[cnt];
        float z1 = triangles->z[cnt];
        float z2 = triangles->z[cnt];
        
        
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

		//Precompute reciprocal of vertex z-coordinate.
		//Part of calculation perspective correct attribute values using barycentric coordinates.
		z0 = 1 / z0;
		z1 = 1 / z1;
		z2 = 1 / z2;

		// Prepare vertex attributes. Divde them by their vertex z-coordinate
		// (though we use a multiplication here because v.z = 1 / v.z)
		//Part of calculation of perspective correct attribute values using barycentric coordinates.
        s0 *= z0;
        s1 *= z1;
        s2 *= z2;
        t0 *= z0;
        t1 *= z1;
        t2 *= z2;
        
        // Area of triangle.
        // Part of calculation attribute values using barycentric coordinates.
        edgeFunction(x0, y0, x1, y1, x2, y2, &area);
        int pixelCnt = 0;
        
        for(int y = 0; y < windows[cnt].height; y++){
            temp = pattern[y];
            nm32s* pDst = (nm32s*)(dst + y * windows[cnt].width);
            nm32s* pSrc = (nm32s*)(src + y * windows[cnt].width);
            if (windows[cnt].x < 0) {
                for(int i = 0; i > windows[cnt].x; i--){
                    pDst[0] = 0x00000000;
                    pDst+=1;
                    pSrc+=1;
                }
            }
            else {
                temp >>= (windows[cnt].x * 2);
            }
            
            for(int x = 0; x < width; x++){
                int mul = temp & 0x3;

                if (mul > 0)//pixel belongs to triangle
                {
                    //Calculate x and y of current pixel as float values
                    //TODO: Incorrect. xf and yf should be relative to triangle vertex coordinates
                    //(inside pattern or inside segment)
                    float xf = x + 0.5f; 
                    float yf = (y1-1-y) + 0.5f;//TODO: y1 = max vertex y, necessary to do -1 or -2 compare to windows opengl 
                                               //because of difference in rasterization algorithms 
                                               //Барицентрические координаты не соответствуют растеризованной картинке,
                                               //то есть растеризованные пиксели не должны быть растеризованы, если использовать барицентрические координаты.
                                               //Но они растеризованы -> для них вычисляются неверные барицентрическим координаты и неправильные значения текстурных координат
                                               //Нужно как-то соотнести алгоритм растеризации и вычисление текстурных координат
                    float w0 = 0;
                    float w1 = 0;
                    float w2 = 0;
                    //TODO: it works only for triangle in CW order
                    edgeFunction(x1, y1, x2, y2, xf, yf, &w0);
                    edgeFunction(x2, y2, x0, y0, xf, yf, &w1);
                    edgeFunction(x0, y0, x1, y1, xf, yf, &w2);
                    
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
                    
                    //Part of calculation of perspective correct attribute values using barycentric coordinates.
                    float oneOverZ = z0 * w0 + z1 * w1 + z2 * w2;
                    float z = 1 / oneOverZ;
                    
                    float s = 0.0;
                    float t = 0.0;
#ifdef USE_BARYCENTRIC
                    s = s0 * w0 + s1 * w1 + s2 * w2;
                    t = t0 * w0 + t1 * w1 + t2 * w2;
                    s *= z;
                    t *= z;
#else
                    float oneOverDenominator = 1 / (A2*xf + B2*yf + D2);
                    float rl_s = (A1_s*xf + B1_s*yf + D1_s) * oneOverDenominator;
                    float rl_t = (A1_t*xf + B1_t*yf + D1_t) * oneOverDenominator;
                    s = rl_s;
                    t = rl_t;
#endif
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


						float derivOneOverDenom = 1.0 / ((A2*x + B2*y + D2)*(A2*x + B2*y + D2));
						float dudx = (float)boundTexObject->texImages2D[0].width*((A1_s*B2 - A2*B1_s)*y + A1_s*D2 - A2*D1_s)*derivOneOverDenom;
						float dudy = (float)boundTexObject->texImages2D[0].width*((B1_s*A2 - B2*A1_s)*x + B1_s*D2 - B2*D1_s)*derivOneOverDenom;
						float dvdx = (float)boundTexObject->texImages2D[0].height*((A1_t*B2 - A2*B1_t)*y + A1_t*D2 - A2*D1_t)*derivOneOverDenom;
						float dvdy = (float)boundTexObject->texImages2D[0].height*((B1_t*A2 - B2*A1_t)*x + B1_t*D2 - B2*D1_t)*derivOneOverDenom;

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

						if (TEXTURE_MIN_LOD > TEXTURE_MAX_LOD)
						{
							printf("Error. TEXTURE_MIN_LOD > TEXTURE_MAX_LOD. LOD is undefined. Exit\n");
							return;
						}
						else if (lod_ > TEXTURE_MAX_LOD)
							lod = TEXTURE_MAX_LOD;
						else if (lod_ < TEXTURE_MIN_LOD)
							lod = TEXTURE_MIN_LOD;
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
							st.x = wrapCoord(textureWrapS, boundTexObject->texImages2D[0].width, st.x);
							st.y = wrapCoord(textureWrapT, boundTexObject->texImages2D[0].height, st.y);

							pixelValue = getPixelLinear(st, textureWrapS, textureWrapT, boundTexObject->texImages2D[0]);
						}
						else if ((minMagFlag == MINIFICATION) && ((textureMinFilter == NMGL_NEAREST_MIPMAP_NEAREST) || (textureMinFilter == NMGL_LINEAR_MIPMAP_NEAREST)))
						{
							d = 0;
							if (lod < 0.5f || (equalf(lod,0.5f) == 1))
								d = TEXTURE_BASE_LEVEL;
							else if ((TEXTURE_BASE_LEVEL + lod) <= ((float)q + 0.5f))
								d = ceil(TEXTURE_BASE_LEVEL + lod + 0.5f) - 1.0f;
							else if ((TEXTURE_BASE_LEVEL + lod) > ((float)q + 0.5f))
								d = q;
							else
							{
								printf("mipmap:NMGL_NEAREST_MIPMAP_NEAREST: d is undefined\n");
								getchar();
							}

							//Mipmapping. So wrap texture coordinates for texture of selected level d 
							st.x = wrapCoord(textureWrapS,boundTexObject->texImages2D[d].width, st.x);
							st.y = wrapCoord(textureWrapT,boundTexObject->texImages2D[d].height, st.y);

							if (textureMinFilter == NMGL_NEAREST_MIPMAP_NEAREST)
								pixelValue = getPixelNearest(st, boundTexObject->texImages2D[d]);
							else if (textureMinFilter == NMGL_LINEAR_MIPMAP_NEAREST)
								pixelValue = getPixelLinear(st, textureWrapS, textureWrapT, boundTexObject->texImages2D[d]);
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
							Vec2f st1;
							Vec2f st2;

							// st1 = st;
							// st2 = st;

							st1.x = wrapCoord(textureWrapS,boundTexObject->texImages2D[d1].width,st.x);
							st1.y = wrapCoord(textureWrapT,boundTexObject->texImages2D[d1].height,st.y);
							st2.x = wrapCoord(textureWrapS,boundTexObject->texImages2D[d2].width,st.x);
							st2.y = wrapCoord(textureWrapT,boundTexObject->texImages2D[d2].height,st.y);

							if (textureMinFilter == NMGL_NEAREST_MIPMAP_LINEAR)
							{
								t1 = getPixelNearest(st1, boundTexObject->texImages2D[d1]);
								t2 = getPixelNearest(st2, boundTexObject->texImages2D[d2]);
							}
							else if (textureMinFilter == NMGL_LINEAR_MIPMAP_LINEAR)
							{
								t1 = getPixelLinear(st1, textureWrapS, textureWrapT, boundTexObject->texImages2D[d1]);
								t2 = getPixelLinear(st2, textureWrapS, textureWrapT, boundTexObject->texImages2D[d2]);
							}
							else
							{
								printf("textureMinFilter has unsupported value for mipmapping\n");
								getchar();
							}
							
							double intpart = 0.0;
							float frac_lod = modf(lod, &intpart);

							pixelValue.r = (1.0 - frac_lod)*t1.r + frac_lod*t2.r;
							pixelValue.g = (1.0 - frac_lod)*t1.g + frac_lod*t2.g;
							pixelValue.b = (1.0 - frac_lod)*t1.b + frac_lod*t2.b;
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
                        
                        vertexRGB.x = ((pSrc[0] >> 8 ) & 0xff)/255.0;
                        vertexRGB.y = ((pSrc[0] >> 16) & 0xff)/255.0;
                        vertexRGB.z = ((pSrc[0] >> 24) & 0xff)/255.0;
                        vertexAlpha = ((pSrc[0]) & 0xff)/255.0;

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
						as = 0.0; //TODO: add using alpha channel if texture has alpha-channel 
						ac = texEnvColorAlpha;
						av = 0.0;

						if (texBaseInternalFormat == NMGL_RGB)
						{
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
						}
						else
						{
							printf ("Only RGB texture internal format is supported.\n");
							getchar();
							return;
						}
                        
                    nm32s color = 0;
                    color = color | (((nm32s)(cv.x * 255) & 0xff) << 8);
                    color = color | (((nm32s)(cv.y * 255) & 0xff) << 16);
                    color = color | (((nm32s)(cv.z * 255) & 0xff) << 24);
                    color = color | ((nm32s)(av * 255) & 0xff);
                    // pDst[0] = mul * pSrc[0];
                    pDst[0] = mul * color;
                }

                pDst += 1;
                pSrc += 1;
                temp >>= 2;
            }
        }
        src += windows[cnt].height * windows[cnt].width;
        dst += windows[cnt].height * windows[cnt].width;
    }
    
    printf ("End textureTriangle\n");     
    return;
}


void edgeFunction(float x0, float y0, float x1, float y1, float x2, float y2, float* res)
{
    *res = (x2 - x0) * (y1 - y0) - (y2 - y0) * (x1 - x0);
}