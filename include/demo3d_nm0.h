#ifndef __DEMO3D_NM0_H__
#define __DEMO3D_NM0_H__

#include "demo3d_common.h"
#include "arraymanager.h"
#include "nmgl.h"
#include "nmgltex_nm0.h"
#include "imagebuffer.h"

#define BIG_NMGL_SIZE (128 * NMGL_SIZE)

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

struct MatrixStack {
	mat4nm32f* base;
	int current;
	int size;
	int type;

	mat4nm32f* top() {
		return &base[current];
	}
};


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


	NMGLSynchro synchro;
	PolygonsConnector* polygonsConnectors;
	NMGLenum error;
	PatternsArray* patterns;
	float* buffer0;
	float* buffer1;
	float* buffer2;
	float* buffer3;
	float* buffer4;

	BitMask segmentMasks[36];
	BitDividedMask dividedMasks[2];	

	Triangles trianInner;
	Lines lineInner;
	Points pointInner;
	NmglBeginEndInfo beginEndInfo;

	mat4nm32f modelviewMatrix[16];
	mat4nm32f projectionMatrix[2];
	mat4nm32f normalMatrix;
	MatrixStack modelviewMatrixStack;
	MatrixStack projectionMatrixStack;
	MatrixStack* currentMatrixStack;
	int isUseTwoSidedMode;

	int isCullFace;
	int cullFaceType;
	int frontFaceOrientation;
	int normalizeEnabled;
	float pointRadius;
	int dummy;

	Array vertexArray;
	Array normalArray;
	Array colorArray;

	v4nm32f ambientMul[MAX_LIGHTS + 1];
	v4nm32f diffuseMul[MAX_LIGHTS];
	v4nm32f specularMul[MAX_LIGHTS];
	WindowInfo windowInfo;

	v4nm32f tmp;

	v4nm32f materialAmbient;
	v4nm32f materialDiffuse;
	v4nm32f materialSpecular;
	v4nm32f materialEmissive;

	v4nm32f lightAmbient[MAX_LIGHTS + 1];
	v4nm32f lightDiffuse[MAX_LIGHTS];
	v4nm32f lightSpecular[MAX_LIGHTS];
	v4nm32f lightPosition[MAX_LIGHTS];
	v4nm32f lightSpotDirection[MAX_LIGHTS];
	float lightSpotExp[MAX_LIGHTS];
	float lightSpotCutoff[MAX_LIGHTS];
	float lightConstAtt[MAX_LIGHTS];
	float lightLinAtt[MAX_LIGHTS];
	float lightQuadAtt[MAX_LIGHTS];
	bool isEnabledLight[MAX_LIGHTS];
	int isLighting;
	float specularExp;

	NMGL_Context_NM0_Texture texState; //textures data
	
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
		materialDiffuse.vec[0] = 0.8;
		materialDiffuse.vec[1] = 0.8;
		materialDiffuse.vec[2] = 0.8;
		materialSpecular.vec[0] = 0;
		materialSpecular.vec[1] = 0;
		materialSpecular.vec[2] = 0;
		materialEmissive.vec[0] = 0;
		materialEmissive.vec[1] = 0;
		materialEmissive.vec[2] = 0;
		materialAmbient.vec[3] = 1;
		materialDiffuse.vec[3] = 1;
		materialSpecular.vec[3] = 1;
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
				}
				else {
					modelviewMatrix[0].matr[i * 4 + j] = 0;
					projectionMatrix[0].matr[i * 4 + j] = 0;
				}
			}
		}

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
	
	/**
	 *  \defgroup addC_v4nm32f addC_v4nm32f
	 *  \brief Функция сложения массива 4-мерных векторов с 4-мерным вектором
	 *  
	 *  \param pSrcV [in] Входной массив 4-хмерных векторов
	 *  \param pSrcC [in] Указатель на 4-мерный вектор
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
	

	/**
	 *  \defgroup baseLighti baseLighti
	 *  \brief Функция вычисления основной части освещения
	 *  
	 *  \param ambient [in] Указатель на амбиентную составляющую источника освещения
	 *  \param n_dot_vp [in] Указатель на продублированный массив скалярного произведения нормали и дистанцией между источником и точкой
	 *  \param diffuse [in] Указатель на амбиентную составляющую источника освещения
	 *  \param n_dot_h_in_srm [in] Что-то пока что не очень ясное
	 *  \param specular [in]  Тоже неясно
	 *  \param dst [out] Выходной массив
	 *  \param count [in] Число векторов
	 *  
	 *  \details Функция вычисляет основную часть освещения по формуле 
	 *  res = a + nvp * d + f(nvp) * nh * s, где
	 *  a - ambient
	 *  nvp - n_dot_vp
	 *  d - diffuse
	 *  nh - n_dot_h_in_srm
	 *  s - specular
	 *  f(x) = 1, if (x!=0), else 0	
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
	
	/**
	 *  \defgroup clamp clamp
	 *  \brief Функция приведения чисел к фиксированному диапазону
	 *  
	 *  \param pSrcVec [in] Входной массив
	 *  \param min [in] Минимальный порог диапазона
	 *  \param max [in] Максимальный порог диапазона
	 *  \param pDstVec [out] Выходной массив
	 *  \param size [in] Число элементов массива
	 *  \retval Return description
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
		
	/**
	 *  \defgroup cnv32f_v2v4 cnv32f_v2v4
	 *  \brief Функция конвертации двухмерного вектора в четырехмерный 
	 *  с заданными значениями третьей и четвертой компоненты
	 *  
	 *  \param srcVec [in] Входный массив
	 *  \param stride [in] Шаг чтения
	 *  \param value3 [in] Значение третьей компоненты элемента выходного массива
	 *  \param value4 [in] Значение четвертой компоненты элемента выходного массива
	 *  \param dstVec [out] Выходной массив
	 *  \param size [in] Размер массива в элементах
	 *  \retval Return description
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
	void cnv32f_v2v4(const v2nm32f* srcVec, int stride, float value3, float value4, v4nm32f* dstVec, int size);
	 //! \}
	 
	 /**
	 *  \defgroup cnv32f_v3v4 cnv32f_v3v4
	 *  \brief Функция конвертации трехмерного вектора в четырехмерный 
	 *  
	 *  \param src [in] Входный массив
	 *  \param dst [out] Выходный массив
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
	void cnv32f_v3v4(const nm32f* src, nm32f* dst, float value4, int size);
	 //! \}
	
	/**
	 *  \defgroup Group Group name
	 *  \brief Brief description
	 *  
	 *  \param srcPointers [in] Description for srcPointers
	 *  \param indices [in] Description for indices
	 *  \param dstPointers [in] Description for dstPointers
	 *  \param nArrays [in] Description for nArrays
	 *  \param size [in] Description for size
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
	void copyColorByIndices(v4nm32s* srcColor, int* indices, v4nm32s* dstColor, int size);
	

	/**
	 *  \defgroup dotC_gt0 dotC_gt0
	 *  \brief Функция скалярного произведения массивa векторов и постоянного вектора с последующимся занулением отрицательных значений
	 *  
	 *  \param srcVec [in] Входной массив векторов
	 *  \param srcC [in] Указатель на постоянный вектор
	 *  \param dstValues [out] Выходной массив полученных значений (продублированный)
	 *  \param size [in] Число векторов в массиве
	 *  \retval Return description
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
	
	
	/**
	 *  \defgroup dotV_gt0 dotV_gt0
	 *  \brief Функция скалярного произведения массивов векторов с последующимся занулением отрицательных значений
	 *  
	 *  \param srcVec1 [in] Первый входной массив
	 *  \param srcVec2 [in] Второй входной массив
	 *  \param dstValues [out] Выходной массив полученных значений (продублированный)
	 *  \param size [in] Число элементов
	 *  \retval Return description
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
	 //! \{
	void dotV_gt0_v4nm32f(v4nm32f* srcVec1, v4nm32f* srcVec2, v2nm32f* dstValues, int size);
	void dotV_v4nm32f(v4nm32f* srcVec1, v4nm32f* srcVec2, v2nm32f* dstValues, int size);
	 //! \}
	
	
	/**
	 *  \defgroup dotMulC_AddC dotMulC_AddC
	 *  \brief Функция умножения постоянного 4-хмерного вектора на массив продублированных констант с прибавлением другого постоянного вектора.
	 *  Массив констант должен быть продублированным
	 *  
	 *  \param srcVec [in] Входной массив констант
	 *  \param mulC [in] Указатель на постоянный вектор-множитель
	 *  \param addC [in] Указатель на постоянный вектор-слагаемое
	 *  \param dst [out] Выходной массив векторов
	 *  \param size [in] Число векторов
	 *  \retval Return description
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

	 /**
	 *  \defgroup dotMulC_AddC dotMulC_AddC
	 *  \brief Функция умножения массива 4-хмерных векторов 
	 *  на массив продублированных констант
	 *
	 *  \param srcVec [in] Входной массив констант
	 *  \param mulVec [in] Указатель на массив векторов
	 *  \param dst [out] Выходной массив векторов
	 *  \param size [in] Число векторов
	 *  \retval Return description
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
	
	/**
	 *  \defgroup dotMulC_Add dotMulC_Add
	 *  \brief Функция умножения постоянного 4-хмерного вектора на на массив констант с прибавлением массива других 4-хмерных векторов.
	 *  Массив констант должен быть продублированным
	 *  
	 *  \param srcVec [in] Массив констант
	 *  \param mulC [in] Указатель на умножающийся постоянный вектор
	 *  \param addVec [in] Массив прибавляющихся векторов
	 *  \param dst [out] Выходной массив
	 *  \param size [in] Число элементов
	 *  \retval Return description
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

	 /**
	 *  \defgroup dotMulC_Add dotMulC_Add
	 *  \brief Функция умножения постоянного 4-хмерного вектора на на массив констант с прибавлением массива других 4-хмерных векторов.
	 *  Массив констант должен быть продублированным
	 *
	 *  \param srcVec [in] Массив констант
	 *  \param mulC [in] Указатель на умножающийся постоянный вектор
	 *  \param addVec [in] Массив прибавляющихся векторов
	 *  \param dst [out] Выходной массив
	 *  \param size [in] Число элементов
	 *  \retval Return description
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
	
	void doubleAbsIfNegElse0_32f(float* src1, float* src2, float* dst1, float* dst2, int size);
	void doubleAdd_32f(float* src1, float* src2, float* srcAdd1, float* srcAdd2, float* dst1, float* dst2, int size);
	void doubleClamp_32f(float* src1, float* src2, float min, float max, float* dst1, float* dst2, int size);
	void doubleMulC_32f(float* src1, float* src2, float C1, float C2, float* dst1, float* dst2, int size);
	void doubleSub_32f(float* src1, float* src2, float* srcSub1, float* srcSub2, float* dst1, float* dst2, int size);
	void doubleSubC_32f(float* src1, float* src2, float C1, float C2, float* dst1, float* dst2, int size);
	void fastInvSqrt(float* srcVec, float* dstVec, int size);
	int firstNonZeroIndx_32s(int* pSrcVec, int size);
	
	/**
	 *  \defgroup findMinMax2 findMinMax2
	 *  \brief Поэлементный поиск минимум и максимума из двух массивов
	 *  
	 *  \param srcVec1 [in] Description for src1
	 *  \param srcVec2 [in] Description for src2
	 *  \param dstMin [out] Description for dstMin
	 *  \param dstMax [in] Description for dstMax
	 *  \param size [in] Description for size
	 *  \retval Return description
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

	/**
	 *  \defgroup findMinMax3 findMinMax3
	 *  \brief Поэлементный поиск минимум и максимума из трех массивов
	 *  
	 *  \param src1 [in] Первый входной массив
	 *  \param src2 [in] Второй входной массив
	 *  \param src3 [in] Третий входной массив
	 *  \param dstMin [out] Выходной массив с минимумом
	 *  \param dstMax [out] Выходной массив с максимумом
	 *  \param size [in] Description for size
	 *  \retval Return description
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
	
	/**
	 *  \defgroup loadIdentify loadIdentify
	 *  \brief Функция инициализации матрицы едининчной матрицей
	 *  
	 *  \param matrix [out] Выходная матрица
	 *  \retval Return description
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
	
	/**
	 *  \defgroup meanToInt3 meanToInt3
	 *  \brief Функция поиска среднего значения из трех элементов и конвертирования его в целочисленный тип
	 *  
	 *  \param src1 [in] Входной массив первых значений
	 *  \param src2 [in] Входной массив вторых значений
	 *  \param src3 [in] Входной массив третьих значений
	 *  \param result [out] Выходной массив в целочисленном формате
	 *  \param size [in] Число элементов массива
	 *  \retval Return description
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
	 
	 /**
	 *  \defgroup meanToIn23 meanToInt2
	 *  \brief Функция поиска среднего значения из двух элементов и конвертирования его в целочисленный тип
	 *  
	 *  \param src1 [in] Входной массив первых значений
	 *  \param src2 [in] Входной массив вторых значений
	 *  \param result [out] Выходной массив
	 *  \param size [in] Число элементов массива
	 *  \retval Return description
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
	
		
	/**
	 *  \defgroup mulC_f mulC_f
	 *  \brief Функция умножение массива на постоянное значение
	 *  
	 *  \param pSrcV [in] Входной массив
	 *  \param pSrcC [in] Указатель на постоянное значение
	 *  \param pDst [out] Выходной массив
	 *  \param size [in] Число элементов
	 *  \retval Return description
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
	
	/**
	 *  \defgroup mul_mat4nm32f_v4nm32f mul_mat4nm32f_v4nm32f
	 *  \brief Функция умножение матрицы 4х4 на массив 4хмерных векторов. Матрица задается по столбцам
	 *  
	 *  \param srcMat4x4 [in] Входная матрица 4х4 (column-major order)
	 *  \param srcVec4xN [in] Входной массив векторов
	 *  \param dstVec4xN [in] Выходной массив
	 *  \param count [in] Число векторов
	 *  \retval Return description
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

	 /**
	 *  \defgroup mul_v4nm32f_mat4nm32f mul_v4nm32f_mat4nm32f
	 *  \brief Функция массива 4хмерных векторов на матрицу 4х4. Матрица задается по строкам
	 *
	 *  \param srcMat4x4 [in] Входная матрица 4х4 (row-major order)
	 *  \param srcVec4xN [in] Входной массив векторов
	 *  \param dstVec4xN [in] Выходной массив
	 *  \param count [in] Число векторов
	 *  \retval Return description
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
	
	
	int readMask(nm1* mask, int* dstIndices, int size);
	int readMaskToLimitDst(nm1* mask, int* dstIndices, int* treated, int size, int maxSize);
	void remap_32u(nm32u* pSrcVec, nm32u* pDstVec, nm32s* pRemapTable, int size);

	/**
	 *  \defgroup replaceEq0_f replaceEq0
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
	
	

	/**
	 *  \defgroup set_v4nm32f set_v4nm32f
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
	
	

	/**
	 *  \defgroup sortByYinTriangle sortByYinTriangle
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
	 
	void sortByY2(float* srcXY0, float* srcXY1, int size);

	/**
	 *  \defgroup split_v4nm32f split_v4nm32f
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
	 
	/**
	 *  \defgroup split_v2nm32f split_v2nm32f
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
	/**
	 *  \defgroup subCRev subCRev
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
	
	void ternaryLt0_AddC_AddC_32f(nm32f* srcFlags, nm32f* srcVec, float valueLeft, float valueRight, float* dstVec, int size);
	void tripleMulC_32f(float* src1, float* src2, float* src3, float C, float* dst1, float* dst2, float* dst3, int size);


}

int getVertexPart(Array* array, v4nm32f* dst, float* tmpBuffer, int count);
int getNormalPart(Array* array, v4nm32f* dst, float* tmpBuffer, int count);
int getColorPart(Array* array, v4nm32f* dst, float* tmpBuffer, int count);

void pushToLines_l(const float *vertexX, const float *vertexY, const float *vertexZ, const v4nm32f* color, Lines& lines, int countVertex);

void pushToTriangles_t(const float *vertexX, const float *vertexY, const float *vertexZ, const v4nm32f* color, Triangles& triangles, int countVertex);
void reverseMatrix3x3in4x4(mat4nm32f* src, mat4nm32f* dst);
void pow_32f(nm32f* srcVec, nm32f* dstVec, float powC, int size, nm32f* pTmp1);
void copyColorByIndices_BGRA_RGBA(v4nm32s* srcColor, int* indices, v4nm32s* dstColor, int size);


//functions that use NMGLSynchroL_ContextNM0
void cullFaceSortTriangles(Triangles &triangles);
void setSegmentMask(v2nm32f* minXY, v2nm32f* maxXY, BitMask* masks, int size);
void rasterizeT(const Triangles* triangles, const BitMask* masks);
void rasterizeL(const Lines* lines, const BitMask* masks);
void rasterizeP(const Points* points, const BitMask* masks);
void updatePolygonsT(PolygonsOld* poly, Triangles* triangles, int count, int segX, int segY);
void updatePolygonsL(PolygonsOld* poly, Lines* lines, int count, int segX, int segY);
void updatePolygonsP(PolygonsOld* poly, Points* lines, int count, int segX, int segY);
void updatePolygonsT(DataForNmpu1* data, Triangles* triangles, int count, int segX, int segY);
void updatePolygonsL(DataForNmpu1* data, Lines* triangles, int count, int segX, int segY);
void updatePolygonsP(DataForNmpu1* data, Points* triangles, int count, int segX, int segY);

/**
 *  \defgroup color Light
 *  \brief Функция расчета освещения
 *  
 *  \param vertex [in] Входной массив вершин
 *  \param srcNormal_dstColor [in, out] Входной массив нормалей, выходной массив цветов
 *  \param size [in] Число вершин
 *  \retval Return description
 *  \details Функция использует массивы nmglBuffer0, nmglBuffer1, nmglBuffer2, nmglBuffer3
 */
//! \{
void light(v4nm32f* vertex, v4nm32f* srcNormal_dstColor, int size);
//! \}


#endif
