#ifndef __DEMO3D_NM0_H__
#define __DEMO3D_NM0_H__

#include "demo3d_common.h"
#include "arraymanager.h"
#include "nmgl.h"
#include "nmgltex_nm0.h"
#include "imagebuffer.h"

#define BIG_NMGL_SIZE (128 * NMGL_SIZE)

/*!
 *  \brief Класс, хранящий побитовую маску.
 *  
 *  Побитовая маска используется для выставления и определения принадлежности примитива одному из сегментов изображения.
 *  
 *  Если в соответствующем бите стоит 1 - значит примитив принадлежит сегменту, иначе не принадлежит. Так же структура хранит в себе переменную, характеризующую
    наличие ненулевых битов в маске
 */
class BitMask {
public:
	int* bits;
	int hasNotZeroBits; 

	void init(nm1* mask) {
		bits = (int*)mask;
		hasNotZeroBits = 0;
	}

	int get(int index) {
		int word = bits[index / 32];
		word >>= index % 32;
		return word & 1;
	}

	void put(int index, int value) {
		int word = bits[index / 32];
		int mask = 1 << (index % 32);
		value <<= index % 32;
		word = ( word & ~mask ) | ( value & mask );
		bits[index / 32] = word;
	}
};

/*!
 *  \brief Класс, хранящий побитовую маску в раздельных массивах.
 *  Принцип схож с принципом структуры BitMask, однако в данной структуре маска разделена на четные и нечетные элементы
 */
class BitDividedMask{
public:
	BitMask even;
	BitMask odd;

	void init(nm1* evenMask, nm1* oddMask) {
		even.init(evenMask);
		odd.init(oddMask);
	}

	int get(int index) {
		if (index % 2) {
			return odd.get(index / 2);
		}
		else {
			return even.get(index / 2);
		}
	}

	void put(int index, int value) {
		if (index % 2) {
			odd.put(index / 2, value);
		}
		else {
			even.put(index / 2, value);
		}
	}
};

/*!
 *  \brief Структура, хранящая указатели на геометрические координаты, текстурные координаты и цвет вершины
 */
struct CombinePointers {
	float *x;
	float *y;
	float *z;
	float *w;
	float *s;
	float *t;
	v4nm32f* color;
	int dummy;
};


/*!
 *  \brief Структура, хранящая три вершины треугольника. Каждая вершина описывается структурой CombinePointers
 */
struct TrianglePointers {
	CombinePointers v0;
	CombinePointers v1;
	CombinePointers v2;
};

/*!
 *  \brief Структура, хранящая две вершины линии. Каждая вершина описывается структурой CombinePointers
 */
struct LinePointers {
	CombinePointers v0;
	CombinePointers v1;
};


struct Lines{
	float* x0;
	float* y0;
	float* x1;
	float* y1;
	int* z;
	v4nm32s* colors;
	int size;
	int maxSize;
};

struct Points {
	float* x0;
	float* y0;
	int* z;
	v4nm32s* colors;
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
	v4nm32s* colors;
	int size;
	int maxSize;
};
void copyTriangles(const Triangles &src, int offsetSrc, Triangles &dst, int offsetDst, int size);

/*!
 *  \brief Структура для описывания блока glBegin/glEnd
 */
class NmglBeginEndInfo{
public:
	v4nm32f* vertex;
	v4nm32f* normal;
	v4nm32f* color;
	int vertexCounter;
	int normalCounter;
	int colorCounter;

	NMGLenum mode;
	bool inBeginEnd;

	int dummy;
	int maxSize;

	NmglBeginEndInfo(){
		vertexCounter = 0;
		normalCounter = 0;
		colorCounter = 0;
		inBeginEnd = false;
	}

};

/*!
 *  \brief Структура, хранящая информацию о стеке матриц
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
	inline static void create(NMGLSynchroData* synchroData) {
		context = (NMGL_Context_NM0*)halMalloc32(sizeof32(NMGL_Context_NM0));
		context->init(synchroData);
	}
	inline static NMGL_Context_NM0 *getContext() {
		return context;
	}
	inline static void free() {
		halFree(context);
	}


	NMGLSynchro synchro;  ///< Класс, осуществляющий синхронизацию между процессорами
	PolygonsConnector* polygonsConnectors;  ///< Указатель на коннекторы структуры Polygons, необходимой для передачи данных с ядра на ядро
	NMGLenum error;			///< Переменная, хранящая код ошибки. 0, если ошибок нет
	PatternsArray* patterns;	///< Указатель на массив двухразрядных паттернов
	float* buffer0;				///< Указатель буфер 0, используемый как временный буфер в нескольких функциях. Размер буфера должен быть (12 * NMGL_SIZE)
	float* buffer1;				///< Указатель буфер 1, используемый как временный буфер в нескольких функциях. Размер буфера должен быть (12 * NMGL_SIZE)
	float* buffer2;				///< Указатель буфер 2, используемый как временный буфер в нескольких функциях. Размер буфера должен быть (12 * NMGL_SIZE)
	float* buffer3;				///< Указатель буфер 3, используемый как временный буфер в нескольких функциях. Размер буфера должен быть (12 * NMGL_SIZE)
	float* buffer4;				///< Указатель буфер 4, используемый как временный буфер в нескольких функциях. Размер буфера должен быть (12 * NMGL_SIZE)

	BitMask segmentMasks[36];	///< Массив структур однобитных масок. Каждая структура описывает один из сегментов изображения
	BitDividedMask dividedMasks[2];	 ///< Массив структур разделенных однобитных масок. 

	Triangles trianInner;		///< Структура Triangles, массивы которой должны лежать во внутренней памяти. Каждый массив должен быть размера NMGL_SIZE
	Lines lineInner;			///< Структура Lines, массивы которой должны лежать во внутренней памяти. Каждый массив должен быть размера NMGL_SIZE
	Points pointInner;			///< Структура Points, массивы которой должны лежать во внутренней памяти. Каждый массив должен быть размера NMGL_SIZE
	NmglBeginEndInfo beginEndInfo;	///< Структура для работы с блоком glBegin/glEnd

	mat4nm32f modelviewMatrix[16];		///< Стек видовых матриц(16 матриц)
	mat4nm32f projectionMatrix[2];		///< Стек проекционных матриц(2 матрицы)
	mat4nm32f normalMatrix;				///< Матрица для нормалей
	MatrixStack modelviewMatrixStack;	///< Структура для работы со стеком видовых матриц
	MatrixStack projectionMatrixStack;	///< Структура для работы со стеком проекционных матриц
	MatrixStack* currentMatrixStack;	///< Указатель на текущий стек матриц
	int isUseTwoSidedMode;				///< 

	int isCullFace;						///< Состояние режима отбрасывания треугольников с другим порядком обхода вершин. NMGL_TRUE, если включен, иначе NMGL_FALSE
	int cullFaceType;
	int frontFaceOrientation;
	int normalizeEnabled;				///< Состояние режима нормализации. NMGL_TRUE, если включен, иначе NMGL_FALSE
	float pointRadius;					///< Радиус точек при рисование примитивов типа NMGL_POINTS
	int dummy;

	Array vertexArray;					///< Класс для работы со значением координат вершинам в nmglDrawArrays
	Array normalArray;					///< Класс для работы с нормалями в nmglDrawArrays
	Array colorArray;					///< Класс для работы с цветом в nmglDrawArrays

	v4nm32f ambientMul[MAX_LIGHTS + 1];    ///< Общие значения окружающей интенсивности материала и источников освещения (элемент MAX_LIGHTS говорит об общей интенсивности материала и сцены)
	v4nm32f diffuseMul[MAX_LIGHTS];		   ///< Общие значения рассеяной интенсивности материала и источников освещения
	v4nm32f specularMul[MAX_LIGHTS];	   ///< Общие значения зеркальной интенсивности материала и источников освещения
	WindowInfo windowInfo;				///< Информация о расположении и размерах сегментов в изображении. Модифицируется функцией nmglViewport

	v4nm32f tmp;						

	v4nm32f materialAmbient;			///< Окружающий цвет материала
	v4nm32f materialDiffuse;			///< Рассеяный цвет материала
	v4nm32f materialSpecular;			///< Зеркальный цвет материала
	v4nm32f materialEmissive;			///< Эмиссионный цвет материала

	v4nm32f lightAmbient[MAX_LIGHTS + 1];   ///< Значения окружающей интенсивности источников света (элемент MAX_LIGHTS говорит об интенсивности сцены)
	v4nm32f lightDiffuse[MAX_LIGHTS];       ///< Значения рассеяной интенсивности источников света
	v4nm32f lightSpecular[MAX_LIGHTS];      ///< Значения зеркальной интенсивности источников света
	v4nm32f lightPosition[MAX_LIGHTS];		///< Значения положения источников освещения
	v4nm32f lightSpotDirection[MAX_LIGHTS];		///< Направление прожектора для источников света
	float lightSpotExp[MAX_LIGHTS];			///< Показатель степени прожектора для источников света
	float lightSpotCutoff[MAX_LIGHTS];		///< Угол отсечки прожектора для источников света
	float lightConstAtt[MAX_LIGHTS];		///< Постоянный коэффициент затухания для источника света (не используется)
	float lightLinAtt[MAX_LIGHTS];			///< Коэффициент линейного затухания  для источника света (не используется)
	float lightQuadAtt[MAX_LIGHTS];			///< Квадратичный коэффициент затухания для источника света (не используется)
	int isEnabledLight[MAX_LIGHTS];		///< Флаги активности источников света
	int isLighting;						///< Флаг активности расчета освещения
	float specularExp;					///< Показатель зеркальности

	NMGL_Context_NM0_Texture texState; 	///< textures data
	
	void init(NMGLSynchroData* syncroData){
		synchro.init(syncroData);

		currentMatrixStack = &modelviewMatrixStack;
		isUseTwoSidedMode = NMGL_FALSE;
		isCullFace = NMGL_FALSE;
		cullFaceType = NMGL_BACK;
		frontFaceOrientation = NMGL_CCW;
		normalizeEnabled = NMGL_FALSE;
		specularExp = 0;
		isLighting = NMGL_FALSE;


		modelviewMatrixStack.base = modelviewMatrix;
		modelviewMatrixStack.current = 0;
		modelviewMatrixStack.size = 16;
		modelviewMatrixStack.type = NMGL_MODELVIEW_MATRIX;

		projectionMatrixStack.base = projectionMatrix;
		projectionMatrixStack.current = 0;
		projectionMatrixStack.size = 2;
		projectionMatrixStack.type = NMGL_PROJECTION_MATRIX;

		materialAmbient.vec[0] = 0.2;
		materialAmbient.vec[1] = 0.2;
		materialAmbient.vec[2] = 0.2;
		materialAmbient.vec[3] = 1;
		materialDiffuse.vec[0] = 0.8;
		materialDiffuse.vec[1] = 0.8;
		materialDiffuse.vec[2] = 0.8;
		materialDiffuse.vec[3] = 1;
		materialSpecular.vec[0] = 0;
		materialSpecular.vec[1] = 0;
		materialSpecular.vec[2] = 0;
		materialSpecular.vec[3] = 1;
		materialEmissive.vec[0] = 0;
		materialEmissive.vec[1] = 0;
		materialEmissive.vec[2] = 0;
		materialEmissive.vec[3] = 1;

		for (int i = 0; i < MAX_LIGHTS; i++) {
			if (i == 0) {
				lightDiffuse[i].vec[0] = 1;
				lightDiffuse[i].vec[1] = 1;
				lightDiffuse[i].vec[2] = 1;
				lightDiffuse[i].vec[3] = 1;

				lightSpecular[i].vec[0] = 1;
				lightSpecular[i].vec[1] = 1;
				lightSpecular[i].vec[2] = 1;
				lightSpecular[i].vec[3] = 1;
			}
			else {
				lightDiffuse[i].vec[0] = 0;
				lightDiffuse[i].vec[1] = 0;
				lightDiffuse[i].vec[2] = 0;
				lightDiffuse[i].vec[3] = 1;

				lightSpecular[i].vec[0] = 0;
				lightSpecular[i].vec[1] = 0;
				lightSpecular[i].vec[2] = 0;
				lightSpecular[i].vec[3] = 1;
			}

			lightAmbient[i].vec[0] = 0;
			lightAmbient[i].vec[1] = 0;
			lightAmbient[i].vec[2] = 0;
			lightAmbient[i].vec[3] = 1;

			lightPosition[i].vec[0] = 0;
			lightPosition[i].vec[1] = 0;
			lightPosition[i].vec[2] = 1;
			lightPosition[i].vec[3] = 0;

			lightSpotDirection[i].vec[0] = 0;
			lightSpotDirection[i].vec[1] = 0;
			lightSpotDirection[i].vec[2] = -1;
			lightSpotDirection[i].vec[3] = 0;

			lightSpotExp[i] = 0;
			lightSpotCutoff[i] = 180;
			lightConstAtt[i] = 1;
			lightLinAtt[i] = 0;
			lightQuadAtt[i] = 0;
			isEnabledLight[i] = false;
		}

		lightAmbient[MAX_LIGHTS].vec[0] = 0.2;
		lightAmbient[MAX_LIGHTS].vec[1] = 0.2;
		lightAmbient[MAX_LIGHTS].vec[2] = 0.2;
		lightAmbient[MAX_LIGHTS].vec[3] = 1.0;

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (i == j) {
					modelviewMatrix[0].matr[i * 4 + j] = 1;
					projectionMatrix[0].matr[i * 4 + j] = 1;
					normalMatrix.matr[i * 4 + j] = 1;
				}
				else {
					modelviewMatrix[0].matr[i * 4 + j] = 0;
					projectionMatrix[0].matr[i * 4 + j] = 0;
					normalMatrix.matr[i * 4 + j] = 0;
				}
			}
		}
		normalMatrix.matr[15] = 0;

		windowInfo.segmentWidth = WIDTH_SEG;
		windowInfo.segmentHeight = HEIGHT_SEG;
		windowInfo.viewportMulZ = (1 - 0) * 0.5 * ZBUFF_MAX;
		windowInfo.viewportAddZ = (1 + 0) * 0.5 * ZBUFF_MAX;

		//массивы
		nmglDisableClientState(NMGL_VERTEX_ARRAY);
		nmglDisableClientState(NMGL_COLOR_ARRAY);
		nmglDisableClientState(NMGL_NORMAL_ARRAY);
		
		texState.init();
		
	}
};

extern "C"{
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция сложения массива 4-мерных векторов с одиночным 4-мерным вектором
	 *  
	 *  \param pSrcV [in] Входной массив 4-хмерных векторов
	 *  \param pSrcC [in] Указатель на одиночный 4-мерный вектор
	 *  \param pDst [out] Выходной массив
	 *  \param size [in] Число векторов
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="pSrcV"> im0 </param>
	 *          <param name="pSrcC"> im0 </param>
	 *          <param name="pDst"> im0 im1 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *          <size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void addC_v4nm32f(v4nm32f* pSrcV, v4nm32f* pSrcC, v4nm32f* pDst, int size);
	 //! \}
	

	/*!
	 *  \ingroup service_api
	 *  \brief Функция вычисления основной части освещения
	 *  
	 *  \param ambient [in] Указатель на амбиентную составляющую источника освещения
	 *  \param n_dot_vp [in] Указатель на продублированный массив скалярного произведения нормали и дистанцией между источником и точкой
	 *  \param diffuse [in] Указатель на амбиентную составляющую источника освещения
	 *  \param n_dot_h_in_srm [in] Указатель на возведенный в степень srm продублированный массив скалярного произведения нормали и вектора h
	 *  \param specular [in]  Указатель на массив specular
	 *  \param dst [out] Выходной массив
	 *  \param count [in] Число векторов
	 *  
	 *  \details Функция вычисляет основную часть освещения по формуле 
	 *  \f[res = a + nvp * d + f(nvp) * nh * s\f], где
	 *  *a* - *ambient*, *nvp* - *n_dot_vp*,  *d* - *diffuse*, *nh* - *n_dot_h_in_srm*, *s* - *specular*
	 *  \f[f(x) = 1, if (x!=0), else 0	\f]
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="ambient"> im0 </param>
	 *          <param name="n_dot_vp"> im0 </param>
	 *          <param name="diffuse"> im0 </param>
	 *          <param name="n_dot_h_in_srm"> im0 im1 </param>
	 *          <param name="specular"> im0 </param>
	 *          <param name="dst"> im0 im1 im2 </param>
	 *          <param name="count"> 128 512 1024 </param>
	 *          <size> count </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void baseLighti(v4nm32f* ambient, v2nm32f* n_dot_vp, v4nm32f* diffuse, v2nm32f* n_dot_h_in_srm, v4nm32f* specular, v4nm32f* dst, int count);
	 //! \}
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция приведения чисел к фиксированному диапазону
	 *  
	 *  \param pSrcVec [in] Входной массив
	 *  \param min [in] Минимальный порог диапазона
	 *  \param max [in] Максимальный порог диапазона
	 *  \param pDstVec [out] Выходной массив
	 *  \param size [in] Число элементов массива
	 *  
	 *  \details Функция вычисляет следующее выражение
	 *  \f[f(x)= \begin{Bmatrix}
	 *  \(min, & if x < min\) \\
	 *  \(x, & if min<x<max\) \\
	 *  \(max, & if x>max\) \f]
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="pSrcVec"> im0 </param>
	 *          <param name="min"> 0 </param>
	 *          <param name="max"> 1 </param>
	 *          <param name="pDstVec"> im0 im1 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *  		<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void clamp_32f(nm32f* pSrcVec, float min, float max, nm32f* pDstVec, int size);
	 //! \}
		
	/*!
	 *  \ingroup service_api
	 *  \brief Функция конвертации двухмерного вектора в четырехмерный 
	 *  с заданными значениями третьей и четвертой компоненты
	 *  
	 *  \param srcVec [in] Входный массив
	 *  \param dstVec [out] Выходной массив
	 *  \param value3 [in] Значение третьей компоненты элемента выходного массива
	 *  \param value4 [in] Значение четвертой компоненты элемента выходного массива
	 *  \param size [in] Размер массива в элементах
	 *  
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="srcVec"> im0 </param>
	 *          <param name="stride"> 0 1 </param>
	 *          <param name="value3"> 1 </param>
	 *          <param name="value4"> 2 </param>
	 *          <param name="dstVec"> im0 im1 </param>
	 *          <param name="countVec"> 128 512 1024 </param>
	 *  		<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void cnv32f_v2v4(const v2nm32f* srcVec, v4nm32f* dstVec, float value3, float value4, int size);
	 //! \}
	 
	 /*!
	 *  \ingroup service_api
	 *  \brief Функция конвертации трехмерного вектора в четырехмерный 
	 *  
	 *  \param srcVec [in] Входный массив
	 *  \param dstVec [out] Выходный массив
	 *  \param value4 [in] Значение четвертой компоненты элемента выходного массива
	 *  \param size [in] Размер массива в элементах
	 *  \retval Return description
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="src_v3nm32f"> </param>
	 *          <param name="dst_v4nm32f"> </param>
	 *          <param name="value4"> </param>
	 *          <param name="size"> </param>
	 *          <size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void cnv32f_v3v4(const nm32f* srcVec, nm32f* dstVec, float value4, int size);
	 //! \}
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функции выборки элементов из массивов
	 *  
	 *  \param srcPointers [in] Указатели входных массивов
	 *  \param indices [in] Массив индексов
	 *  \param dstPointers [in] Указатели выходных массивов
	 *  \param nArrays [in] Число массивов
	 *  \param size [in] Число копируемых элементов
	 *  
	 *  \details Функция производит выборку нужных элементов из входных массивов и кладет их подряд в выходных массивых. Индексы нужных элементов хранятся в массиве indices
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="srcPointers"> </param>
	 *          <param name="indices"> </param>
	 *          <param name="dstPointers"> </param>
	 *          <param name="nArrays"> </param>
	 *          <param name="size"> </param>
	 *          <size> </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	void copyArraysByIndices(void** srcPointers, int* indices, void** dstPointers, int nArrays, int size);
	
	/*!
	 *  \brief Функция выборки цветов
	 *  
	 *  \param srcColor [in] Входной массив цветных векторов
	 *  \param indices [in] Массив индексов
	 *  \param dstColor [in] Выходной массив цветных векторов
	 *  \param size [in] Число цветных векторов
	 *  
	 *  \details Функция берет элементы цветных векторов с индексами, получаемыми из *indices* и кладет их последовательно в выходной массив
	 *  
	 */
	void copyColorByIndices(v4nm32s* srcColor, int* indices, v4nm32s* dstColor, int size);
	

	/*!
	 *  \ingroup service_api
	 *  \brief Функция скалярного произведения массивa векторов и постоянного вектора с последующимся занулением отрицательных значений
	 *  
	 *  \param srcVec [in] Входной массив векторов
	 *  \param srcC [in] Указатель на постоянный вектор
	 *  \param dstValues [out] Выходной массив полученных значений (продублированный)
	 *  \param size [in] Число векторов в массиве
	 *  
	 *  \details Функция считает скалярное произведение двух векторов и проверяет знак. Если результат больше нуля, то пишется результат, если меньше
	 *  нуля, то пишется ноль. В выходном массиве результат пишется как два одинаковых числа, идующих подряд. Это сделано для удобства последующей работы
	 *  с векторным сопроцессором
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="srcVec"> im0 </param>
	 *          <param name="srcC"> im0 </param>
	 *          <param name="dstValues"> im0 im1 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *   		<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void dotC_gt0_v4nm32f(v4nm32f* srcVec, v4nm32f* srcC, v2nm32f* dstValues, int size);
	 //! \}
	
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция скалярного произведения массивов векторов с последующимся занулением отрицательных значений
	 *  
	 *  \param srcVec1 [in] Первый входной массив
	 *  \param srcVec2 [in] Второй входной массив
	 *  \param dstValues [out] Выходной массив полученных значений (продублированный)
	 *  \param size [in] Число элементов
	 *  
	 *  \details Функция считает скалярное произведение двух векторов и проверяет знак. Если результат больше нуля, то пишется результат, если меньше
	 *  нуля, то пишется ноль. В выходном массиве результат пишется в таком виде \f$ (r_0,r_0), (r_1,r_1)... (r_n,r_n) \f$. Это сделано для удобства последующей работы
	 *  с векторным сопроцессором
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="srcVec1"> im0 </param>
	 *          <param name="srcVec2"> im0 im1 </param>
	 *          <param name="dstValues"> im0 im1 im2 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *    		<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	void dotV_gt0_v4nm32f(v4nm32f* srcVec1, v4nm32f* srcVec2, v2nm32f* dstValues, int size);
	 
	 /*!
	 *  \ingroup service_api
	 *  \brief Функция скалярного произведения массивов векторов
	 *  
	 *  \param srcVec1 [in] Первый входной массив
	 *  \param srcVec2 [in] Второй входной массив
	 *  \param dstValues [out] Выходной массив полученных значений (продублированный)
	 *  \param size [in] Число элементов
	 *  
	 *  \details Функция считает скалярное произведение двух векторов в выходной массив получившийся результат. В выходном массиве результат 
	 *  пишется в таком виде \f$ (r_0,r_0), (r_1,r_1)... (r_n,r_n) \f$. Это сделано для удобства последующей работы
	 *  с векторным сопроцессором
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="srcVec1"> im0 </param>
	 *          <param name="srcVec2"> im0 im1 </param>
	 *          <param name="dstValues"> im0 im1 im2 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *    		<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	void dotV_v4nm32f(v4nm32f* srcVec1, v4nm32f* srcVec2, v2nm32f* dstValues, int size);
	
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция умножения постоянного 4-хмерного вектора на массив констант с прибавлением другого постоянного вектора.
	 *  Массив констант должен быть продублированным
	 *  
	 *  \param srcVec [in] Входной массив констант
	 *  \param mulC [in] Указатель на постоянный вектор-множитель
	 *  \param addC [in] Указатель на постоянный вектор-слагаемое
	 *  \param dst [out] Выходной массив векторов
	 *  \param size [in] Число векторов
	 *  
	 *  \details Массив констант должен быть  продублированным, то есть вида \f$ (c_0,c_0), (c_1,c_1)... (c_n,c_n) \f$
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="srcVec"> im0 </param>
	 *          <param name="mulC"> im0 </param>
	 *          <param name="addC"> im0 </param>
	 *          <param name="dst"> im0 im1 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *    		<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void dotMulC_AddC_v4nm32f(v2nm32f* srcVec, v4nm32f* mulC, v4nm32f* addC, v4nm32f* dst, int size);
	 //! \}

	 /*!
	 *  \ingroup service_api
	 *  \brief Функция умножения массива 4-хмерных векторов 
	 *  на массив констант
	 *
	 *  \param srcVec [in] Входной массив констант
	 *  \param mulVec [in] Указатель на массив векторов
	 *  \param dst [out] Выходной массив векторов
	 *  \param size [in] Число векторов
	 *  
	 *  \details Массив констант должен быть  продублированным, то есть вида \f$ (c_0,c_0), (c_1,c_1)... (c_n,c_n) \f$
	 *
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="srcVec"> im0 </param>
	 *          <param name="mulVec"> im0 </param>
	 *          <param name="dst"> im0 im1 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *  		<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void dotMulV_v4nm32f(v2nm32f* srcVec, v4nm32f* mulVec, v4nm32f* dst, int size);
	 //! \}
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция умножения постоянного 4-хмерного вектора на массив констант с прибавлением массива других 4-хмерных векторов.
	 *  Массив констант должен быть продублированным
	 *  
	 *  \param srcVec [in] Массив констант
	 *  \param mulC [in] Указатель на умножающийся постоянный вектор
	 *  \param addVec [in] Массив прибавляющихся векторов
	 *  \param dst [out] Выходной массив
	 *  \param size [in] Число элементов
	 *  
	 *  \details Функция делает следующую операцию \f$ f(x,y) = C * (x_0, x_1, x_2, x_3) +  (y_0, y_1, y_2, y_3)\f$. Константы C должен быть продублированным, то есть массив
	 *  констант должен быть вида \f$ (c_0,c_0), (c_1,c_1)... (c_n,c_n) \f$
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="srcVec"> im0 </param>
	 *          <param name="mulC"> im0 </param>
	 *          <param name="addVec"> im0 im1 </param>
	 *          <param name="dst"> im0 im1 im2 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *    		<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void dotMulC_Add_v4nm32f(v2nm32f* srcVec, v4nm32f* mulC, v4nm32f* addVec, v4nm32f* dst, int size);
	 //! \}

	/*!
	 *  \ingroup service_api
	 *  \brief Функция нахождение отрицательных смещений (с двумя параллельными обработками)
	 *  
	 *  \param src1 [in] Входной массив 1
	 *  \param src2 [in] Входной массив 2
	 *  \param dst1 [in] Выходный массив 1
	 *  \param dst2 [in] Выходный массив 2
	 *  \param size [in] Число обрабатываемых элементов (для обоих массивов одинаковое)
	 *  
	 *  \details Функция вычисляет следующее выражение
	 *  \f[f(x)= \begin{Bmatrix}
	 *  -x, & if x < 0 \\
	 *  0, & if x >= 0 \f]
	 *  
	 *  Функция берет данные от двух источников и если все массивы распределены по разным банкам памяти, то обрабатывает их параллельно
	 *  
	 */
	void doubleAbsIfNegElse0_32f(float* src1, float* src2, float* dst1, float* dst2, int size);
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция производит два параллельных сложения
	 *  
	 *  \param src1 [in] Первый входной массив 1
	 *  \param src2 [in] Первый входной массив 2
	 *  \param srcAdd1 [in] Второй входной массив 1
	 *  \param srcAdd2 [in] Второй входной массив 2
	 *  \param dst1 [in] Выходной массив 1
	 *  \param dst2 [in] Выходной массив 2
	 *  \param size [in] Число обрабатываемых элементов (для обоих массивов одинаковое)
	 *  
	 *  \details Функция паралелльно вычисляет два сложения. *dst1 = src1 + srcAdd1, dst2 = src2 + srcAdd2*
	 *  
	 */
	void doubleAdd_32f(float* src1, float* src2, float* srcAdd1, float* srcAdd2, float* dst1, float* dst2, int size);
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция приведения чисел к фиксированному диапазону
	 *  
	 *  \param src1 [in] Входной массив 1
	 *  \param src2 [in] Входной массив 2
	 *  \param min [in] Минимальный порог диапазона
	 *  \param max [in] Максимальный порог диапазона
	 *  \param dst1 [out] Выходной массив 1
	 *  \param dst2 [out] Выходной массив 2
	 *  \param size [in] Число элементов массива
	 *  
	 *  \details Функция осуществляет те же действия что и функия clamp_32f, но параллельно для двух массивов
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="pSrcVec"> im0 </param>
	 *          <param name="min"> 0 </param>
	 *          <param name="max"> 1 </param>
	 *          <param name="pDstVec"> im0 im1 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *  		<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	void doubleClamp_32f(float* src1, float* src2, float min, float max, float* dst1, float* dst2, int size);
	
	/*!
	 *  \ingroup service_api
	 *  \brief Brief description
	 *  
	 *  \param src1 [in] Description for src1
	 *  \param src2 [in] Description for src2
	 *  \param C1 [in] Description for C1
	 *  \param C2 [in] Description for C2
	 *  \param dst1 [in] Description for dst1
	 *  \param dst2 [in] Description for dst2
	 *  \param size [in] Description for size
	 *  
	 *  \details Details
	 *  
	 */
	void doubleMulC_32f(float* src1, float* src2, float C1, float C2, float* dst1, float* dst2, int size);
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция производит два параллельных вычитания
	 *  
	 *  \param src1 [in] Первый входной массив 1
	 *  \param src2 [in] Первый входной массив 2
	 *  \param srcSub1 [in] Второй входной массив 1
	 *  \param srcSub2 [in] Второй входной массив 2
	 *  \param dst1 [in] Выходной массив 1
	 *  \param dst2 [in] Выходной массив 2
	 *  \param size [in] Число обрабатываемых элементов (для обоих массивов одинаковое)
	 *  
	 *  \details Функция паралелльно осуществляет два вычитания. *dst1 = src1 - srcSub1, dst2 = src2 - srcSub2*
	 *  
	 */
	void doubleSub_32f(float* src1, float* src2, float* srcSub1, float* srcSub2, float* dst1, float* dst2, int size);
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция производит два параллельных вычитания
	 *  
	 *  \param src1 [in] Первый входной массив 1
	 *  \param src2 [in] Первый входной массив 2
	 *  \param С1 [in] Константа 1
	 *  \param С2 [in] Константа 2
	 *  \param dst1 [in] Выходной массив 1
	 *  \param dst2 [in] Выходной массив 2
	 *  \param size [in] Число обрабатываемых элементов (для обоих массивов одинаковое)
	 *  
	 *  \details Функция паралелльно вычитает два вычитания константы из массива. *dst1 = src1 - С1, dst2 = src2 - С2*
	 *  
	 */
	void doubleSubC_32f(float* src1, float* src2, float C1, float C2, float* dst1, float* dst2, int size);
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция быстрого обратного корня
	 *  
	 *  \param srcVec [in] Входной массив
	 *  \param dstVec [in] Выходной массив
	 *  \param size [in] Число элементов
	 *  
	 *  \details Функция вычисляет выражение \f[ \frac{1}{\sqrt{x}} \f]. 
	 *  Aлгоритм принимает 32-битное число с плавающей запятой (одинарной точности в формате IEEE 754) в качестве исходных данных и производит над ним следующие операции: 
	 *  1. Трактуя 32-битное дробное число как целое, проводит операцию \f$y_0 = 5F3759DF_16 − (x >> 1)\f$, где >> — битовый сдвиг вправо. Результат снова трактуется как 32-битное дробное число. 
	 *  2. Для уточнения проводит одну итерацию метода Ньютона: \f$y1 = y0(1,5 − 0,5xy0^2)\f$.
	 *  
	 */
	void fastInvSqrt(float* srcVec, float* dstVec, int size);
	
	/*!
	 *  \brief Находит индекс первого ненулевого элемента
	 *  
	 *  \param pSrcVec [in] Входной массив элемент
	 *  \param size [in] Число элементов
	 *  \return Индекс первого ненулевого элемента. Если не находит, то будет равно -1
	 *  
	 */
	int firstNonZeroIndx_32s(int* pSrcVec, int size);
	
	/*!
	 *  \ingroup service_api
	 *  \brief Поэлементный поиск минимум и максимума из двух массивов
	 *  
	 *  \param srcVec1 [in] Первый входной массив
	 *  \param srcVec2 [in] Второй входной массив
	 *  \param dstMin [out] Выходной массив минимумов
	 *  \param dstMax [out] Выходной массив максимумов
	 *  \param size [in] Число элементов
	 *  
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="srcVec1"> im0 </param>
	 *          <param name="srcVec2"> im0 im1 </param>
	 *          <param name="dstMin"> im0 im1 im2 </param>
	 *          <param name="dstMax"> im0 im1 im2 im3 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *          <size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void findMinMax2(float* srcVec1, float* srcVec2, float* dstMin, float* dstMax, int size);	
	 //! \}

	/*!
	 *  \ingroup service_api
	 *  \brief Поэлементный поиск минимум и максимума из трех массивов
	 *  
	 *  \param src1 [in] Первый входной массив
	 *  \param src2 [in] Второй входной массив
	 *  \param src3 [in] Третий входной массив
	 *  \param dstMin [out] Выходной массив с минимумом
	 *  \param dstMax [out] Выходной массив с максимумом
	 *  \param size [in] Число элементов
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="src1"> im0 </param>
	 *          <param name="src2"> im0 </param>
	 *          <param name="src3"> im0 </param>
	 *          <param name="dstMin"> im0 im1 </param>
	 *          <param name="dstMax"> im0 im1 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *    		<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void findMinMax3(float* src1, float* src2, float* src3, float* dstMin, float* dstMax, int size);
	 //! \}
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция инициализации матрицы едининчной матрицей
	 *  
	 *  \param matrix [out] Выходная матрица
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="matrix"> im0 </param>
	 *    		<size> 16 </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void loadIdentify(mat4nm32f* matrix);
	 //! \}
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция поиска среднего значения из трех элементов и конвертирования его в целочисленный тип
	 *  
	 *  \param src1 [in] Входной массив первых значений
	 *  \param src2 [in] Входной массив вторых значений
	 *  \param src3 [in] Входной массив третьих значений
	 *  \param result [out] Выходной массив в целочисленном формате
	 *  \param size [in] Число элементов массива
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="src1"> im0 </param>
	 *          <param name="src2"> im0 im1 </param>
	 *          <param name="src3"> im0 im1 im2 </param>
	 *          <param name="result"> im0 im1 im2 im3 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *    		<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void meanToInt3(float* src1, float* src2, float* src3, int* result, int size);
	 //! \}
	 
	 /*!
	 *  \ingroup service_api
	 *  \brief Функция поиска среднего значения из двух элементов и конвертирования его в целочисленный тип
	 *  
	 *  \param src1 [in] Входной массив первых значений
	 *  \param src2 [in] Входной массив вторых значений
	 *  \param result [out] Выходной массив
	 *  \param size [in] Число элементов массива
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="src1"> im0 </param>
	 *          <param name="src2"> im0 im1 </param>
	 *          <param name="result"> im0 im1 im2 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *          <size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void meanToInt2(float* src1, float* src2, int* result, int size);
	 //! \}
	
		
	/*!
	 *  \ingroup service_api
	 *  \brief Функция умножение массива на постоянное значение
	 *  
	 *  \param pSrcV [in] Входной массив
	 *  \param pSrcC [in] Указатель на постоянное значение
	 *  \param pDst [out] Выходной массив
	 *  \param size [in] Число элементов
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="pSrcV"> im0 </param>
	 *          <param name="pSrcC"> im0 </param>
	 *          <param name="pDst"> im0 im1 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *    		<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void mulC_v4nm32f(v4nm32f* pSrcV, v4nm32f* pSrcC, v4nm32f* pDst, int size);
	 //! \}
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция умножение матрицы 4х4 на массив 4хмерных векторов. Матрица задается по столбцам
	 *  
	 *  \param srcMat4x4 [in] Входная матрица 4х4 (column-major order)
	 *  \param srcVec4xN [in] Входной массив векторов
	 *  \param dstVec4xN [in] Выходной массив
	 *  \param count [in] Число векторов
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="srcMat4x4"> im0 </param>
	 *          <param name="srcVec4xN"> im0 im1 </param>
	 *          <param name="dstVec4xN"> im0 im1 im2 </param>
	 *          <param name="count"> 128 512 1024 </param>
	 *    		<size> count </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void mul_mat4nm32f_v4nm32f(mat4nm32f* srcMat4x4,	v4nm32f* srcVec4xN, v4nm32f* dstVec4xN, int count);
	 //! \}

	 /*!
	 *  \ingroup service_api
	 *  \brief Функция массива 4хмерных векторов на матрицу 4х4. Матрица задается по строкам
	 *
	 *  \param srcMat4x4 [in] Входная матрица 4х4 (row-major order)
	 *  \param srcVec4xN [in] Входной массив векторов
	 *  \param dstVec4xN [in] Выходной массив
	 *  \param count [in] Число векторов
	 *
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="srcMat4x4"> im0 </param>
	 *          <param name="srcVec4xN"> im0 im1 </param>
	 *          <param name="dstVec4xN"> im0 im1 im2 </param>
	 *          <param name="count"> 128 512 1024 </param>
	 *     		<size> count </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void mul_v4nm32f_mat4nm32f(v4nm32f* srcVec4xN, mat4nm32f* srcMat4x4, v4nm32f* dstVec4xN, int count);
	 //! \}
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция чтение однобитного массива масок
	 *  
	 *  \param mask [in] Входной массив масок
	 *  \param dstIndices [in] Выходной массив индексов
	 *  \param size [in] Число элементов
	 *  
	 *  \details Функция читает входные данные как однобитный массив проверяя значение каждого бита. Если бит маски равен 1, то его порядковый номер 
	 *  записывается в *dstIndices*, иначе он пропускается
	 *  
	 */
	int readMask(nm1* mask, int* dstIndices, int size);
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция чтение однобитного массива масок, разделенного на четные и нечетные маски
	 *  
	 *  \param maskEven [in] Входной массив масок четных элементов
	 *  \param maskOdd [in] Входной массив масок нечетных элементов
	 *  \param dstIndices [in] Выходной массив индексов
	 *  \param size [in] Число элементов
	 *  
	 *  \details Функция читает входные данные как однобитный массив проверяя значение каждого бита. Если бит маски равен 1, то его порядковый номер вычисляется для массива четных масок 
	 *  как 2i, для нечетных масок как 2i+1, где i - порядковый номер бита в массиве. Затем этот индекс записывается в *dstIndices*
	 *  
	 */
	int readDividedMask(nm1* maskEven, nm1* maskOdd, int* dstIndices, int size);
	
	//int readMaskToLimitDst(nm1* mask, int* dstIndices, int* treated, int size, int maxSize);
	void remap_32u(nm32u* pSrcVec, nm32u* pDstVec, nm32s* pRemapTable, int size);

	/*!
	 *  \ingroup service_api
	 *  \brief Функция замены нуля другим значением
	 *  
	 *  \param srcVec [in] Входной массив
	 *  \param dstVec [out] Выходной массив
	 *  \param size [in] Число элементов в массиве
	 *  \param nReplaceC [in] Втсавляемое число
	 *  \retval Return description
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="srcVec"> im0 </param>
	 *          <param name="dstVec"> im0 im1 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *          <param name="nReplaceC"> 1 </param>
	 *     		<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void replaceEq0_32f(float* srcVec, float* dstVec, int size, float nReplaceC);
	 //! \}
	
	

	/*!
	 *  \ingroup service_api
	 *  \brief Функция инициализации элементов массива постоянным значением.
	 *  
	 *  \param dstVec [out] Инициализируемый массив
	 *  \param valueC [in] Значение константы
	 *  \param size [in] Размер вектора в элементах
	 *  \retval Return description
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="dstVec"> im0 </param>
	 *          <param name="valueC"> im0 im1 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *      		<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void set_v4nm32f(v4nm32f* dstVec, v4nm32f* valueC, int size);
	 //! \}
	
	

	/*!
	 *  \ingroup service_api
	 *  \brief Функция поэлементной сортировки двухмерных элементов по возрастанию 2-й компоненты
	 *  
	 *  \param srcAxy [in, out] Первый входной массив двухмерных векторов
	 *  \param srcBxy [in, out] Второй входной массив двухмерных векторов
	 *  \param srcCxy [in, out] Третий входной массив двухмерных векторов
	 *  \param size [in] Число элементов
	 *  \retval Return description
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="srcAxy"> im0 </param>
	 *          <param name="srcBxy"> im0 im1 </param>
	 *          <param name="srcCxy"> im0 im1 im2 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *      	<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void sortByY3(float* srcAxy, float* srcBxy, float* srcCxy, int size);
	 //! \}
	 
	 /*!
	 *  \ingroup service_api
	 *  \brief Функция поэлементной сортировки двухмерных элементов по возрастанию 2-й компоненты
	 *  
	 *  \param srcXY0 [in, out] Первый входной массив двухмерных векторов
	 *  \param srcXY1 [in, out] Второй входной массив двухмерных векторов
	 *  \param size [in] Число элементов
	 *  \retval Return description
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="srcXY0"> im0 </param>
	 *          <param name="srcXY1"> im0 im1 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *      	<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	void sortByY2(float* srcXY0, float* srcXY1, int size);

	/*!
	 *  \ingroup service_api
	 *  \brief Функция разбивки 4х(-)мерного вектора на 4 отдельных значения(вектор на значения не бьется)
	 *  
	 *  \param srcVec [in] Входной массив векторов (массив векторов?)
	 *  \param step [in] Шаг чтения входного массива (шаг в чем , в элементах v4nm32f?)
	 *  \param dstX [out] Выходной массив первых компонент вектора
	 *  \param dstY [out] Выходной массив вторых компонент вектора
	 *  \param dstZ [out] Выходной массив третьих компонент вектора
	 *  \param dstW [out] Выходной массив четвертых компонент вектора
	 *  \param countVec [in] Размер массива в элементах (в v4nm32f?)
	 *  \retval Return description (description?)
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="srcVec"> im0 </param>
	 *          <param name="step"  > 2 </param>
	 *          <param name="dstX"  > im0 im1 </param>
	 *          <param name="dstY"  > im0 im1 im2 </param>
	 *          <param name="dstZ"  > im0 im1 im2 im3 </param>
	 *          <param name="dstW"  > im0 im1 im2 im3 im4 </param>
	 *          <param name="countVec"> 128 512 1014 </param>
	 *      	<size> countVec </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void split_v4nm32f(v4nm32f* srcVec, int step, float* dstX, float* dstY, float* dstZ, float* dstW, int countVec);
	 //! \}
	 
	/*!
	 *  \ingroup service_api
	 *  \brief Функция разбиения 2хмерного вектора на 2 отдельных значения
	 *  
	 *  \param srcVec [in] Входной массив векторов
	 *  \param step [in] Шаг чтения входного массива
	 *  \param dstX [out] Выходной массив первых компонент вектора
	 *  \param dstY [out] Выходной массив первых компонент вектора
	 *  \param countVec [in] Description for countVec
	 *  \retval Return description
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="srcVec"> im0 </param>
	 *          <param name="step"> im0 im1 </param>
	 *          <param name="dstX"> im0 im1 im2 </param>
	 *          <param name="dstY"> im0 im1 im2 im3 </param>
	 *          <param name="countVec"> 128 512 1024 </param>
	 *       	<size> countVec </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void split_v2nm32f(v2nm32f* srcVec, int step, float* dstX, float* dstY, int countVec);
	//! \}
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция вычитания вектора из постоянного значения
	 *  
	 *  \param pSrcV [in] Входной массив
	 *  \param pSrcC [in] Указатель на константу
	 *  \param pDst [out] Выходной массив
	 *  \param size [in] Число элементов в массиве
	 *  \retval Return description
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param name="pSrcV"> im0 </param>
	 *          <param name="pSrcC"> im0 </param>
	 *          <param name="pDst"> im0 im1 </param>
	 *          <param name="size"> 128 512 1024 </param>
	 *       	<size> size </size>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void subCRev_v4nm32f(v4nm32f* pSrcV, v4nm32f* pSrcC, v4nm32f* pDst, int size);
	 //! \}
	
	/*!
	 *  \ingroup service_api
	 *  \brief Brief description
	 *  
	 *  \param srcFlags [in] Description for srcFlags
	 *  \param srcVec [in] Description for srcVec
	 *  \param valueLeft [in] Description for valueLeft
	 *  \param valueRight [in] Description for valueRight
	 *  \param dstVec [in] Description for dstVec
	 *  \param size [in] Description for size
	 *  \return Return description
	 *  \details Details
	 *  
	 */
	void ternaryLt0_AddC_AddC_32f(nm32f* srcFlags, nm32f* srcVec, float valueLeft, float valueRight, float* dstVec, int size);
	
	/*!
	 *  \ingroup service_api
	 *  \brief Функция умножения трех массивов на константу
	 *  
	 *  \param src1 [in] Входной массив 1
	 *  \param src2 [in] Входной массив 2
	 *  \param src3 [in] Входной массив 2
	 *  \param C [in] Умножаемая константа
	 *  \param dst1 [in] Выходной массив 1
	 *  \param dst2 [in] Выходной массив 2
	 *  \param dst3 [in] Выходной массив 3
	 *  \param size [in] Число элементов
	 *  
	 *  \details Функция умножает три входных массива на одну константу и выгружает результат в три разных массива. Если все массивы лежат в разных банках памяти,
	 *  то умножение каждого массива происходит параллельно друг другу
	 *  
	 */
	void tripleMulC_32f(float* src1, float* src2, float* src3, float C, float* dst1, float* dst2, float* dst3, int size);

	/*!
	 *  \ingroup service_api
	 *  \defgroup repackToPrimitives repackToPrimitives
	 *  \brief Функции переупаковки вершин
	 *  
	 *  \param srcVertex [in] Description for srcVertex
	 *  \param srcColor [in] Description for srcColor
	 *  \param srcTex [in] Description for srcTex
	 *  \param dstVertex [in] Description for dstVertex
	 *  \param vertexAmount [in] Description for vertexAmount
	 *  \return Return description
	 *  \details Details
	 *  
	 */
	 //! \{
	int repackToPrimitives_t(const v4nm32f *srcVertex,
		const v4nm32f *srcColor,
		const v2nm32f *srcTex,
		TrianglePointers *dstVertex,
		int vertexAmount);
	int repackToPrimitives_ts(const v4nm32f *srcVertex,
		const v4nm32f *srcColor,
		const v2nm32f *srcTex,
		TrianglePointers *dstVertex,
		int vertexAmount);
	int repackToPrimitives_tf(const v4nm32f *srcVertex,
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

int splitTriangles(TrianglePointers *srcVertex, int srcCount, int maxWidth, int maxHeight, int maxDstSize, TrianglePointers *dstVertex, int *srcTreatedCount);



//int getVertexPart(Array* array, v4nm32f* dst, float* tmpBuffer, int count);
//int getNormalPart(Array* array, v4nm32f* dst, float* tmpBuffer, int count);
//int getColorPart(Array* array, v4nm32f* dst, float* tmpBuffer, int count);

void pushToLines_l(const float *vertexX, const float *vertexY, const float *vertexZ, const v4nm32f* color, Lines& lines, int countVertex);
void pushToLines(const float *vertexX, const float *vertexY, const float *vertexZ, const v4nm32f* color, Lines& lines, int mode, int countVertex);

void pushToTriangles_t(const float *vertexX, const float *vertexY, const float *vertexZ, const v4nm32f* color, Triangles& triangles, int countVertex);
void reverseMatrix3x3in4x4(mat4nm32f* src, mat4nm32f* dst);
void pow_32f(nm32f* srcVec, nm32f* dstVec, float powC, int size, nm32f* pTmp1);
void copyColorByIndices_BGRA_RGBA(v4nm32s* srcColor, int* indices, v4nm32s* dstColor, int size);


//functions that use NMGLSynchroL_ContextNM0

/*!
 *  \ingroup service_api
 *  \brief Brief description
 *  
 *  \param triangles [in] Description for triangles
 *  
 *  \details Функция реализует выборку треагольников в зависимости от порядка обхода точек в треугольниках. Функция использует контекст NMGL_Context_NM0.
 *  Режимы выборки определяются переменными frontFaceOrientation и cullFaceType в структуре NMGL_Context_NM0.
 *  
 */
void cullFaceSortTriangles(Triangles &triangles);

/*!
 *  \ingroup service_api
 *  \brief Функция проверки примитивов на принадлежность сегментам изображения
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

/*!
 *  \ingroup service_api
 *  \defgroup updatePolygons updatePolygons
 *  \brief Функция дополнения структур для передачи данных от NMPU0 к NMPU1
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
void updatePolygonsT(PolygonsOld* poly, Triangles* triangles, int count, int segX, int segY);
void updatePolygonsL(PolygonsOld* poly, Lines* lines, int count, int segX, int segY);
void updatePolygonsP(PolygonsOld* poly, Points* points, int count, int segX, int segY);
void updatePolygonsT(DataForNmpu1* data, Triangles* triangles, int count, int segX, int segY);
void updatePolygonsL(DataForNmpu1* data, Lines* lines, int count, int segX, int segY);
void updatePolygonsP(DataForNmpu1* data, Points* points, int count, int segX, int segY);
//! \}

/*!
 *  \ingroup service_api
 *  \brief Функция расчета освещения
 *  
 *  \param vertex [in] Входной массив вершин
 *  \param srcNormal_dstColor [in, out] Входной массив нормалей, выходной массив цветов
 *  \param size [in] Число вершин
 *  
 *  \details Функция использует контекст NMGL_Context_NM0 и находящиеся в нем массивы buffer0, buffer1, buffer2, buffer3
 */
//! \{
void light(v4nm32f* vertex, v4nm32f* srcNormal_dstColor, int size);
//! \}


#endif
