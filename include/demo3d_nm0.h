#ifndef __DEMO3D_NM0_H__
#define __DEMO3D_NM0_H__

#include "demo3d_common.h"
#include "arraymanager.h"
#include "nmgl.h"
#include "nmgltex_nm0.h"
#include "imagebuffer.h"
#include "nmsynchro.h"
#include "lighting.h"
#include "primitive.h"
#include "segment.h"
#include "framebuffer.h"

#define BIG_NMGL_SIZE (64 * NMGL_SIZE)

struct Lines{
	float* x0;
	float* y0;
	float* x1;
	float* y1;
	int* z;
	// TEXTURING PART
	//order is important
	float* s0;
	float* t0;
	float* s1;
	float* t1;
	float* w0;
	float* w1;
	// TEXTURING PART
	v4nm32s* colors;
	int size;
	int maxSize;
};

struct Points {
	float* x;
	float* y;
	int* z;
	float* s;
	float* t;
	float* w;
	v4nm32s* colors;
	int dummy;
	int size;
	int maxSize;
};

struct Triangles{
	float* x0;
	float* y0;
	float* x1;
	float* y1;
	float* x2;
	float* y2;
	int* z;
	// TEXTURING PART
	//order is important
	float* s0;
	float* t0;
	float* s1;
	float* t1;
	float* s2;
	float* t2;
	float* w0;
	float* w1;
	float* w2;
	int dummy;
	// TEXTURING PART
	v4nm32s* colors;
	int size;
	int maxSize;
};
void copyTriangles(const Triangles &src, int offsetSrc, Triangles &dst, int offsetDst, int size);

/*!
 *  \brief Структура для описывания блока glBegin/glEnd
 *  \author Жиленков Иван
 */
class NmglBeginEndInfo{
public:
	v4nm32f* vertex;
	v4nm32f* normal;
	v4nm32f* color;
	// TEXTURING PART
	v2nm32f* texcoord;//XXX: Only one texture unit is supported.
	// TEXTURING PART
	int vertexCounter;

	NMGLenum mode;
	bool inBeginEnd;

	int maxSize;

	NmglBeginEndInfo(){
		vertexCounter = 0;
		inBeginEnd = false;
	}

};

/*!
 *  \brief Структура, хранящая информацию о стеке матриц
 *  \author Жиленков Иван
 */
struct MatrixStack {
	mat4nm32f* base;	///< указатель на массив матриц
	int current;		///< Индекс текущей матрицы
	int size;			///< Размер массива
	int type;			///< Тип матриц

	mat4nm32f* top() {
		return &base[current];
	}
};





/*!
 *  \brief Контекст nmOpengl на ядре NMPU0
 *  \author Жиленков Иван
 *  
 *  \details Контекст сделан в виде статического класса и должен существовать только в единственном экземпляре.
 *  От значений различных параметров зависит работа функций Opengl
 *  
 */
class NMGL_Context_NM0 {
private:
	static NMGL_Context_NM0 *context;
	NMGL_Context_NM0() {};		// недоступный конструктор
	~NMGL_Context_NM0() {};		// и деструктор
public:	
	inline static void create() {
		context = (NMGL_Context_NM0*)halMalloc32(sizeof32(NMGL_Context_NM0));
		context->init();
	}
	inline static NMGL_Context_NM0 *getContext() {
		return context;
	}
	inline static void free() {
		halFree(context);
	}


	alignas(8) NMGL_SynchroMasterRingBuffer synchro;
	BitMask segmentMasks[36];
	v4nm32f *vertexResult;
	v4nm32f *colorResult;
	v4nm32f *normalResult;
	v2nm32f *texResult;
	v4nm32f *vertexResult2;
	v4nm32f *colorResult2;
	v4nm32f *normalResult2;
	v2nm32f *texResult2;

	ImageSegments* currentSegments;
	PolygonsConnector* triangleConnectors;
	PolygonsConnector* lineConnectors;
	PolygonsConnector* pointConnectors;
	float* buffer0;
	float* buffer1;
	float* buffer2;
	float* buffer3;
	float* buffer4;
	float* buffer5;	
	
	int isUseTwoSidedMode;
	NMGLenum error;
	int isCullFace;
	int cullFaceType;
	int frontFaceOrientation;
	int normalizeEnabled;				///< Состояние режима нормализации. NMGL_TRUE, если включен, иначе NMGL_FALSE
	float pointRadius;					///< Радиус точек при рисование примитивов типа NMGL_POINTS
	MatrixStack* currentMatrixStack;

	Triangles trianInner;
	Lines lineInner;
	Points pointInner;
	NmglBeginEndInfo beginEndInfo;
	alignas(8) v4nm32f currentColor;
	alignas(8) v4nm32f currentNormal;

	alignas(8) mat4nm32f modelviewMatrix[16];
	alignas(8) mat4nm32f projectionMatrix[2];
	alignas(8) mat4nm32f normalMatrix;
	alignas(8) MatrixStack modelviewMatrixStack;
	alignas(8) MatrixStack projectionMatrixStack;

	Array vertexArray;					///< Класс для работы со значением координат вершинам в nmglDrawArrays
	Array normalArray;					///< Класс для работы с нормалями в nmglDrawArrays
	Array colorArray;					///< Класс для работы с цветом в nmglDrawArrays

	
	WindowInfo windowInfo;				///< Информация о расположении и размерах сегментов в изображении. Модифицируется функцией nmglViewport
	NMGL_ScissorTest scissorTest;
	v4nm32f tmp;						

	LightingInfo lightingInfo;

	NMGL_Context_NM0_Texture texState; 	///< textures data
	int shadeModel;


	NMGL_Framebuffer defaultFramebuffer;
	ImageConnector imageConnector;


	NMGLint unpackAlignment;
	NMGLint packAlignment;

	Point_cntxt_t	point;
	Line_cntxt_t 	line;
	Polygon_cntxt_t polygon;
	Blend_cntxt_t	blend;

	NMGL_AlphaTest   alpha_test;

	NMGL_StencilTest stencil_test;

	NMGL_DepthTest 		 depth_test;

	NMGLboolean color_write_mask_flags[4];
	NMGLboolean depth_write_mask_enabled;

	NMGLenum perspectiveCorrectionHint;
	NMGLenum lineSmoothHint;
	NMGLenum pointSmoothHint;

	void init(){
		int i;
		unpackAlignment=4;
		packAlignment=4;
		
		point.smooth_enabled = NMGL_FALSE;
		line.smooth_enabled  = NMGL_FALSE;
		line.stipple.enabled = NMGL_FALSE;
		
		for(i=0;i<4;i++){
			color_write_mask_flags[i] = NMGL_TRUE;
		}
		
		depth_write_mask_enabled = NMGL_TRUE;


		line.width			 = 1.0;
		line.stipple.factor	 = 1;
		line.stipple.pattern = 0xFFFF;

		polygon.stipple.enabled 	= NMGL_FALSE;
		
		polygon.offset_fill_enabled = NMGL_FALSE;

		currentSegments = &windowInfo.segments;

		currentMatrixStack = &modelviewMatrixStack;
		isUseTwoSidedMode = NMGL_FALSE;
		isCullFace = NMGL_FALSE;
		cullFaceType = NMGL_BACK;
		frontFaceOrientation = NMGL_CCW;
		normalizeEnabled = NMGL_FALSE;

		scissorTest.isEnabled = NMGL_FALSE;

		alpha_test.enabled 	 = NMGL_FALSE;
		alpha_test.func		 = NMGL_ALWAYS;
		alpha_test.ref		 = 0;


		stencil_test.enabled = NMGL_FALSE;
		stencil_test.func = NMGL_ALWAYS;
		stencil_test.ref = 0;
		stencil_test.mask = (NMGLuint)-1;

		depth_test.enabled		 = NMGL_FALSE;
		depth_test.func			 = NMGL_LESS;

		blend.enabled = NMGL_FALSE;
		blend.sfactor = NMGL_ONE;
		blend.dfactor = NMGL_ZERO;

		perspectiveCorrectionHint = NMGL_DONT_CARE;
		lineSmoothHint            = NMGL_DONT_CARE;
		pointSmoothHint           = NMGL_DONT_CARE;


		lightingInfo.init();

		currentColor.vec[0] = (float)1.0;
		currentColor.vec[1] = (float)1.0;
		currentColor.vec[2] = (float)1.0;
		currentColor.vec[3] = (float)1.0;

		currentNormal.vec[0] = (float)0.0;
		currentNormal.vec[1] = (float)0.0;
		currentNormal.vec[2] = (float)1.0;
		currentNormal.vec[3] = (float)0.0;

		modelviewMatrixStack.base = modelviewMatrix;
		modelviewMatrixStack.current = 0;
		modelviewMatrixStack.size = 16;
		modelviewMatrixStack.type = NMGL_MODELVIEW_MATRIX;

		projectionMatrixStack.base = projectionMatrix;
		projectionMatrixStack.current = 0;
		projectionMatrixStack.size = 2;
		projectionMatrixStack.type = NMGL_PROJECTION_MATRIX;

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (i == j) {
					modelviewMatrix[0].matr[i * 4 + j] = 1.0f;
					projectionMatrix[0].matr[i * 4 + j] = 1.0f;
					normalMatrix.matr[i * 4 + j] = 1.0;
				}
				else {
					modelviewMatrix[0].matr[i * 4 + j] = 0.0f;
					projectionMatrix[0].matr[i * 4 + j] = 0.0f;
					normalMatrix.matr[i * 4 + j] = 0.0f;
				}
			}
		}
		normalMatrix.matr[15] = 0.0f;

		windowInfo.viewportMulZ = (1 - 0) * 0.5f * ZBUFF_MAX;
		windowInfo.viewportAddZ = (1 + 0) * 0.5f * ZBUFF_MAX;

		//массивы
		nmglDisableClientState(NMGL_VERTEX_ARRAY);
		nmglDisableClientState(NMGL_COLOR_ARRAY);
		nmglDisableClientState(NMGL_NORMAL_ARRAY);
		
		pointRadius = 0.5f;
		texState.init();
		shadeModel = NMGL_SMOOTH;
		
	}
};

extern "C"{
	

	// TEXTURING PART
	/**
	 *  \brief Функция поэлементной сортировки данных X,Y,S,T,W вершин 0,1,2
	 *  по возрастанию Y. Для i-го входного набора вершин 0,1,2 упорядочивает 
	 *  вершины (данные вершин) в порядке возрастания координаты Y.
	 *  
	 *  \param srcXY0 [in, out] Входной массив двухмерных векторов XY0
	 *  \param srcXY1 [in, out] Входной массив двухмерных векторов XY1
	 *  \param srcXY2 [in, out] Входной массив двухмерных векторов XY2
	 *  \param srcS0  [in, out] Входной массив одномерных векторов S0
	 *  \param srcT0  [in, out] Входной массив одномерных векторов T0
	 *  \param srcS1  [in, out] Входной массив одномерных векторов S1 
	 *  \param srcT1  [in, out] Входной массив одномерных векторов T1 
	 *  \param srcS2  [in, out] Входной массив одномерных векторов S2 
	 *  \param srcT2  [in, out] Входной массив одномерных векторов T2 
	 *  \param srcW0  [in, out] Входной массив одномерных векторов W0 
	 *  \param srcW1  [in, out] Входной массив одномерных векторов W1 
	 *  \param srcW2  [in, out] Входной массив одномерных векторов W2 
	 *  \param size [in] Число элементов в массивах
	 *  \retval Return description
	 */
	void sortByY5(float* srcXY0, float* srcXY1, float* srcXY2,
		float* srcS0, float* srcT0,
		float* srcS1, float* srcT1,
		float* srcS2, float* srcT2,
		float* srcW0, float* srcW1, float* srcW2,
		int size);
	// TEXTURING PART

	

    /*!                                                      
     *  \ingroup service_api                                 
     *  \defgroup repackToPrimitives_l[sl] repackToPrimitives_l[sl]
     *  \brief Функции переупаковки вершин линий
     *  \author Жиленков Иван                                
     *                                                       
 	 *  \param srcVertex [in] Входной массив вершин
 	 *  \param srcColor [in] Входной массив цветов 
 	 *  \param srcTex [in] Входной массив текстурных координат 
 	 *  \param dstVertex [out] Выходной массив линий
 	 *  \param vertexAmount [in] Количество вершин во входных массивах для обработки 
     *  \return Возвращает количество линий в выходном массиве                           
     *  \details Details                                     
     *                                                       
     */                                                      
     //! \{                                                  

	// Functions for TEXTURE_ENABLED macro enabled/disabled
	int repackToPrimitives_t_full(const v4nm32f *srcVertex,
		const v4nm32f *srcColor,
		const v2nm32f *srcTex,
		TrianglePointers *dstVertex,
		int vertexAmount);
	int repackToPrimitives_t_without_textures(const v4nm32f *srcVertex,
		const v4nm32f *srcColor,
		const v2nm32f *srcTex,
		TrianglePointers *dstVertex,
		int vertexAmount);

	 
	int repackToPrimitives_l(const v4nm32f *srcVertex,
		const v4nm32f *srcColor,
		const v2nm32f *srcTex,
		LinePointers *dstVertex,
		int vertexAmount);
	int repackToPrimitives_ls(const v4nm32f *srcVertex,
		const v4nm32f *srcColor,
		const v2nm32f *srcTex,
		LinePointers *dstVertex,
		int vertexAmount);
	int repackToPrimitives_ll(const v4nm32f *srcVertex,
		const v4nm32f *srcColor,
		const v2nm32f *srcTex,
		LinePointers *dstVertex,
		int vertexAmount);
	 //! \}

}

void pushToTriangles(const TrianglePointers &srcTriangles, Triangles& dstTriangles, int countVertex);
void pushToLines(const LinePointers &srcLines, Lines &dstLines, int countVertex);

int splitTriangles(TrianglePointers *srcVertex, int srcCount, int maxWidth, int maxHeight, int maxDstSize, TrianglePointers *dstVertex, int *srcTreatedCount);
void reverseMatrix3x3in4x4(mat4nm32f* src, mat4nm32f* dst);
void pow_32f(nm32f* srcVec, nm32f* dstVec, float powC, int size, nm32f* pTmp1);
void copyColorByIndices_BGRA_RGBA(v4nm32s* srcColor, int* indices, v4nm32s* dstColor, int size);


//functions that use NMGLSynchroL_ContextNM0

/*!
 *  \ingroup service_api
 *  \brief Brief description
 *  \author Жиленков Иван
 *  \warning Внутри функции используется контекст NMGL_Context_NM0.
 *  
 *  \param triangles [in] Description for triangles
 *  \param count [in] Description for triangles
 *  
 *  \return Description
 *  
 *  \details Функция реализует выборку треагольников в зависимости от порядка обхода точек в треугольниках.
 *  Режимы выборки определяются переменными frontFaceOrientation и cullFaceType в структуре NMGL_Context_NM0.
 *  
 */
int cullFaceSortTriangles(TrianglePointers &triangles, int count);

/*!
 *  \ingroup service_api
 *  \brief Функция проверки примитивов на принадлежность сегментам изображения
 *  \author Жиленков Иван
 *  \warning Внутри функции используется контекст NMGL_Context_NM0.
 *  
 *  \param minXY [in] Массив векторов минимумов координат *x* и *y*
 *  \param maxXY [in] Массив векторов максимумов координат *x* и *y*
 *  \param masks [out] Массив масок принадлежности.
 *  \param size [in] Число примитивов
 *  
 *  \details Функция используется контекст NMGL_Context_NM0
 *  
 */
void setSegmentMask(v2nm32f* minXY, v2nm32f* maxXY, BitMask* masks, int size);

/*!
 *  \ingroup service_api
 *  \defgroup rasterize rasterize
 *  \brief Функция растеризации примитивов
 *  \author Жиленков Иван
 *  \warning Внутри функции используется контекст NMGL_Context_NM0.
 *  
 *  \param triangles,lines,points [in] Значения вершин примитивов, их цвета и текстурных координат
 *  \param masks [in] Массив масок принадлежности примитивов сегментам
 *  
 *  \details Изображение делится на сегменты размером 128х128 пикселей, потому что такие сегменты помещаются во внутреннюю память. Принадлежность примитива какому-либо сегменту определяется посредство проверки маски данного сегмента. Если треугольник лежит на границе двух сегментов, то считается, что он принадлежит каждому из них. После выделения примитивов, принадлежащих сегментов происходит обработка вершин и запись полученных данных в структуру *DataForNmpu1*. При необходимости, посылается команда NMPU1 для отрисовки этой структуры.
 *  
 *  Функция использует контекст NMGL_Context_NM0. 
 *  
 */
  //! \{
void rasterizeT(const Triangles* triangles, const BitMask* masks);
void rasterizeL(const Lines* lines, const BitMask* masks);
void rasterizeP(const Points* points, const BitMask* masks);
  //! \}

void transferPolygons(PolygonsConnector *connector, int mode, int segNo);

/*!
 *  \ingroup service_api
 *  \defgroup updatePolygons updatePolygons
 *  \brief Функция дополнения структур для передачи данных от NMPU0 к NMPU1
 *  \author Жиленков Иван
 *  \warning Внутри функции используется контекст NMGL_Context_NM0.
 *  
 *  \param poly,data [in] Указатель на дополняемую структуру.
 *  \param triangles,lines,points [in] Указатель на входную структуру данных
 *  \param count [in] Число примитивов
 *  \param segX [in] Номер колонки сегмента изображения
 *  \param segY [in] Номер ряда сегмента изображения
 
 *  \details Функция берет массивы из структур *triangles*,*lines* или *points*, обрабатывает их в зависимости от положения сегмента где они находятс и кладет нужные данный в конец массивов в структуре *PolygonsOld* или *DataForNmpu1*, которые при необходимости передаются затем на NMPU1. 
 *  PolygonsOld устаревшая версия структуры и не рекомендуется к использованию. Переход между типами структур происходит посредством #define USED_OLD_POLYGONS.
 *  Функция использует контекст NMGL_Context_NM0.
 *  
 */
 //! \{
void updatePolygonsT(DataForNmpu1* data, Triangles* triangles, int count, v2nm32f lowerLeft);
void updatePolygonsL(DataForNmpu1* data, Lines* lines, int count, v2nm32f lowerLeft);
void updatePolygonsP(DataForNmpu1* data, Points* points, int count, v2nm32f lowerLeft);
//! \}

/*!
 *  \ingroup service_api
 *  \brief Функция расчета освещения
 *  \author Жиленков Иван
 *  \warning Внутри функции используется контекст NMGL_Context_NM0.
 *  
 *  \param vertex [in] Входной массив вершин
 *  \param srcNormal_dstColor [in, out] Входной массив нормалей, выходной массив цветов
 *  \param size [in] Число вершин
 *  
 *  \details Функция использует контекст NMGL_Context_NM0 и находящиеся в нем массивы buffer0, buffer1, buffer2, buffer3
 */
//! \{
void light(v4nm32f* vertex, v4nm32f* srcNormal, v4nm32f* src_dstColor, int size);
//! \}

void startCalculateColor(v4nm32f* srcVertex, v4nm32f* srcNormal, v4nm32f* srcColor, int vertexCount);
void getCalculatedColor(v4nm32f* dstColor, int vertexCount);


void printWindowInfo(WindowInfo* info, int mode);
void printDataForNmpu1(DataForNmpu1* data, int elementsAmount);
void printTriangles(Triangles* data, int elementsAmount);
void printBitMask(BitMask *bitmask, int nSeg, int elementAmount);
void printTrianglePointers(TrianglePointers* data, int elementsAmount);

namespace nm0_version {
void triangleShadeSmooth(TrianglesInfo* triangles, nm32s* pDstTriangle, int count);
}
#endif
