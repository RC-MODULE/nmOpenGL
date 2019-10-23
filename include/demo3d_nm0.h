#ifndef __DEMO3D_NM0_H__
#define __DEMO3D_NM0_H__

#include "demo3d_common.h"
#include "arraymanager.h"

struct Lines{
	float* x0;
	float* y0;
	float* x1;
	float* y1;
	int* z;
	v4nm32s* colors;
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

struct SegmentMask {
	int* bits;
	int hasNotZeroBits;
};

struct NMGL_Context_NM0 {
	Synchro* synchro;
	NMGLenum error;
	Patterns* patterns;
	int dummy;

	float* buffer0;
	float* buffer1;
	float* buffer2;
	float* buffer3;

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
	float isEnabledLight[MAX_LIGHTS];
	int isLighting;
	float specularExp;
};

struct MasksSeg
{
	int xLt[NMGL_SIZE / 32];
	int yLt[NMGL_SIZE / 32];
	int xGt[NMGL_SIZE / 32];
	int yGt[NMGL_SIZE / 32];
};



extern "C"{
	
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
	 *          <param> matrix </param> <values> imu0 </values>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void loadIdentify(mat4nm32f* matrix);
	 //! \}
	
	/**
	 *  \defgroup mul_mat4nm32f_v4nm32f mul_mat4nm32f_v4nm32f
	 *  \brief Функция умножение матрицы 4х4 на массив 4хмерных векторов. Матрица задается по столбцам
	 *  
	 *  \param srcMatrix4x4 [in] Входная матрица 4х4
	 *  \param srcVector4xN [in] Входной массив векторов
	 *  \param dstVector4xN [in] Выходной массив
	 *  \param count [in] Число векторов
	 *  \retval Return description
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param> srcMatrix4x4 </param> <values>  </values>
	 *          <param> srcVector4xN </param> <values>  </values>
	 *          <param> dstVector4xN </param> <values>  </values>
	 *          <param> count </param> <values>  </values>
	 *      </testperf>
	 *  \endxmlonly
	 */
	void mul_mat4nm32f_v4nm32f(mat4nm32f* srcMatrix4x4,	v4nm32f* srcVector4xN, v4nm32f* dstVector4xN, int count);

	/**
	 *  \defgroup split_v4nm32f split_v4nm32f
	 *  \brief Функция разбиения 4хмерного вектора на 4 отдельных значения
	 *  
	 *  \param srcVec [in] Входной массив векторов
	 *  \param step [in] Шаг чтения входного массива
	 *  \param dstX [out] Выходной массив первых компонент вектора
	 *  \param dstY [out] Выходной массив вторых компонент вектора
	 *  \param dstZ [out] Выходной массив третьих компонент вектора
	 *  \param dstW [out] Выходной массив четвертых компонент вектора
	 *  \param countVec [in] Размер массива в элементах
	 *  \retval Return description
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param> srcVec </param> <values> imu0 </values>
	 *          <param> step </param> <values> 0 </values>
	 *          <param> dstX </param> <values> imu0 imu1 </values>
	 *          <param> dstY </param> <values> imu0 imu1 imu2 </values>
	 *          <param> dstZ </param> <values> imu0 imu1 imu2 imu3 </values>
	 *          <param> dstW </param> <values> imu0 imu1 imu2 imu3 imu4 </values>
	 *          <param> countVec </param> <values> 128 512 1014 </values>
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
	 *          <param> srcVec </param> <values> imu0 </values>
	 *          <param> step </param> <values> imu0 imu1 </values>
	 *          <param> dstX </param> <values> imu0 imu1 imu2 </values>
	 *          <param> dstY </param> <values> imu0 imu1 imu2 imu3 </values>
	 *          <param> countVec </param> <values> 128 512 1024 </values>
	 *      </testperf>
	 *  \endxmlonly
	 */
	void split_v2nm32f(v2nm32f* srcVec, int step, float* dstX, float* dstY, int countVec);
	
	void copyOddOffset(float* src, float* dst, int first, int size);
	

	/**
	 *  \defgroup dotC_gt0 dotC_gt0
	 *  \brief Функция скалярного произведения массивa векторов и постоянного вектора с последующимся занулением отрицательных значений
	 *  
	 *  \param srcVec [in] Входной массив векторов
	 *  \param srcC [in] Указатель на постоянный вектор
	 *  \param dstValues [in] Выходной массив полученных значений (продублированный)
	 *  \param nSize [in] Число векторов в массиве
	 *  \retval Return description
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param> srcVec </param> <values> imu0 </values>
	 *          <param> srcC </param> <values> imu0 </values>
	 *          <param> dstValues </param> <values> imu0 imu1 </values>
	 *          <param> nSize </param> <values> 128 512 1024 </values>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void dotC_gt0_v4nm32f(v4nm32f* srcVec, v4nm32f* srcC, v2nm32f* dstValues, int nSize);
	 //! \}
	
	
	/**
	 *  \defgroup dotV_gt0 dotV_gt0
	 *  \brief Функция скалярного произведения массивов векторов с последующимся занулением отрицательных значений
	 *  
	 *  \param srcVec1 [in] Первый входной массив
	 *  \param srcVec2 [in] Второй входной массив
	 *  \param dstValues [in] Выходной массив полученных значений (продублированный)
	 *  \param nSize [in] Число элементов
	 *  \retval Return description
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param> srcVec1 </param> <values> imu0 </values>
	 *          <param> srcVec2 </param> <values> imu0 imu1 </values>
	 *          <param> dstValues </param> <values> imu0 imu1 imu2 </values>
	 *          <param> nSize </param> <values> 128 512 1024 </values>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void dotV_gt0_v4nm32f(v4nm32f* srcVec1, v4nm32f* srcVec2, v2nm32f* dstValues, int nSize);
	 //! \}
	
	
	void dotV_v4nm32f(v4nm32f* srcVec1, v4nm32f* srcVec2, v2nm32f* dstValues, int nSize);
	
	/**
	 *  \defgroup dotMulC_AddC dotMulC_AddC
	 *  \brief Функция умножения постоянного 4-хмерного вектора на массив продублированных констант с прибавлением другого постоянного вектора.
	 *  Массив констант должен быть продублированным
	 *  
	 *  \param n_dot_VP [in] Входной массив констант
	 *  \param mulC [in] Указатель на постоянный вектор-множитель
	 *  \param addC [in] Указатель на постоянный вектор-слагаемое
	 *  \param dst [out] Выходной массив векторов
	 *  \param nSize [in] Число векторов
	 *  \retval Return description
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param> n_dot_VP </param> <values> imu0 </values>
	 *          <param> mulC </param> <values> imu0 </values>
	 *          <param> addC </param> <values> imu0 </values>
	 *          <param> dst </param> <values> imu0 imu1 </values>
	 *          <param> nSize </param> <values> 128 512 1024 </values>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void dotMulC_AddC_v4nm32f(v2nm32f* srcVec, v4nm32f* mulC, v4nm32f* addC, v4nm32f* dst, int nSize);
	 //! \}
	
	/**
	 *  \defgroup dotMulC_Add dotMulC_Add
	 *  \brief Функция умножения постоянного 4-хмерного вектора на на массив констант с прибавлением массива других 4-хмерных векторов.
	 *  Массив констант должен быть продублированным
	 *  
	 *  \param n_dot_VP [in] Массив констант
	 *  \param mulC [in] Указатель на умножающийся постоянный вектор
	 *  \param addVec [in] Массив прибавляющихся векторов
	 *  \param dst [out] Выходной массив
	 *  \param nSize [in] Число элементов
	 *  \retval Return description
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param> n_dot_VP </param> <values> imu0 </values>
	 *          <param> mulC </param> <values> imu0 </values>
	 *          <param> addVec </param> <values> imu0 imu1 </values>
	 *          <param> dst </param> <values> imu0 imu1 imu2 </values>
	 *          <param> nSize </param> <values> 128 512 1024 </values>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void dotMulC_Add_v4nm32f(v2nm32f* n_dot_VP, v4nm32f* mulC, v4nm32f* addVec, v4nm32f* dst, int nSize);
	 //! \}

	/**
	 *  \defgroup subCRev subCRev
	 *  \brief Функция вычитания вектора из постоянного значения
	 *  
	 *  \param pSrcV [in] Входной массив
	 *  \param pSrcC [in] Указатель на константу
	 *  \param pDst [out] Выходной массив
	 *  \param nSize [in] Число элементов в массиве
	 *  \retval Return description
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param> pSrcV </param> <values> imu0 </values>
	 *          <param> pSrcC </param> <values> imu0 </values>
	 *          <param> pDst </param> <values> imu0 imu1 </values>
	 *          <param> nSize </param> <values> 128 512 1024 </values>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void subCRev_v4nm32f(v4nm32f* pSrcV, v4nm32f* pSrcC, v4nm32f* pDst, int nSize);
	 //! \}

	/**
	 *  \defgroup addC addC
	 *  \brief Функция прибавление постоянного значения к массиву
	 *  
	 *  \param pSrcV [in] Входной массив
	 *  \param pSrcC [in] Указатель на константу
	 *  \param pDst [out] Выходной массив
	 *  \param nSize [in] Число элементов в массиве
	 *  \retval Return description
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param> pSrcV </param> <values> imu0 </values>
	 *          <param> pSrcC </param> <values> imu0 </values>
	 *          <param> pDst </param> <values> imu0 imu1 </values>
	 *          <param> nSize </param> <values> 128 512 1024 </values>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void addC_v4nm32f(v4nm32f* pSrcV, v4nm32f* pSrcC, v4nm32f* pDst, int nSize);
	 //! \}
	
	/**
	 *  \defgroup mulC_f mulC_f
	 *  \brief Функция умножение массива на постоянное значение
	 *  
	 *  \param pSrcV [in] Входной массив
	 *  \param pSrcC [in] Указатель на постоянное значение
	 *  \param pDst [out] Выходной массив
	 *  \param nSize [in] Число элементов
	 *  \retval Return description
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param> pSrcV </param> <values> imu0 </values>
	 *          <param> pSrcC </param> <values> imu0 </values>
	 *          <param> pDst </param> <values> imu0 imu1 </values>
	 *          <param> nSize </param> <values> 128 512 1024 </values>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void mulC_v4nm32f(v4nm32f* pSrcV, v4nm32f* pSrcC, v4nm32f* pDst, int nSize);
	 //! \}

	/**
	 *  \defgroup clamp clamp
	 *  \brief Функция приведения чисел к фиксированному диапазону
	 *  
	 *  \param pSrcVec [in] Входной массив
	 *  \param min [in] Минимальный порог диапазона
	 *  \param max [in] Максимальный порог диапазона
	 *  \param pDstVec [out] Выходной массив
	 *  \param nSize [in] Число элементов массива
	 *  \retval Return description
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param> pSrcVec </param> <values> imu0 </values>
	 *          <param> min </param> <values> 2 </values>
	 *          <param> max </param> <values> 4 </values>
	 *          <param> pDstVec </param> <values> imu0 imu1 </values>
	 *          <param> nSize </param> <values> 128 512 1024 </values>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void clamp_32f(nm32f* pSrcVec, float min, float max, nm32f* pDstVec, int nSize);
	 //! \}
	
	
	/**
	 *  \defgroup meanOfThreeToInt meanOfThreeToInt
	 *  \brief Функция поиска среднего значения из трех элементов и сонвертирования его в целочисленный тип
	 *  
	 *  \param src1 [in] Входной массив первых значений
	 *  \param src2 [in] Входной массив вторых значений
	 *  \param src3 [in] Входной массив третьих значений
	 *  \param result [out] Выходной массив в целочисленном формате
	 *  \param nSize [in] Число элементов массива
	 *  \retval Return description
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param> src1 </param> <values> imu0 </values>
	 *          <param> src2 </param> <values> imu0 imu1 </values>
	 *          <param> src3 </param> <values> imu0 imu1 imu2 </values>
	 *          <param> result </param> <values> imu0 imu1 imu2 imu3 </values>
	 *          <param> nSize </param> <values> 128 512 1024 </values>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void meanToInt3(float* src1, float* src2, float* src3, int* result, int nSize);
	 //! \}
	 
	void meanToInt2(float* src1, float* src2, int* result, int nSize);

	/**
	 *  \defgroup sortByYinTriangle sortByYinTriangle
	 *  \brief Функция поэлементной сортировки двухмерных элементов по возрастанию 2-й компоненты
	 *  
	 *  \param srcAxy [in, out] Первый входной массив двухмерных векторов
	 *  \param srcBxy [in, out] Второй входной массив двухмерных векторов
	 *  \param srcCxy [in, out] Третий входной массив двухмерных векторов
	 *  \param nSize [in] Число элементов
	 *  \retval Return description
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param> srcAxy </param> <values> imu0 </values>
	 *          <param> srcBxy </param> <values> imu0 imu1 </values>
	 *          <param> srcCxy </param> <values> imu0 imu1 imu2 </values>
	 *          <param> nSize </param> <values> 128 512 1024 </values>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void sortByY3(float* srcAxy, float* srcBxy, float* srcCxy, int nSize);
	 //! \}
	 
	void sortByY2(float* srcXY0, float* srcXY1, int nSize);
	
	/**
	 *  \defgroup cnv32f cnv32f
	 *  \brief Функция конвертирования двухмерного вектора в четырехмерный 
	 *  с заданными значениями третьей и четвертой компоненты
	 *  
	 *  \param srcVec [in] Входный массив
	 *  \param stride [in] Шаг чтения
	 *  \param value3 [in] Значение третьей компоненты элемента выходного массива
	 *  \param value4 [in] Значение четвертой компоненты элемента выходного массива
	 *  \param dstVec [in] Выходной массив
	 *  \param countVec [in] Размер массива в элементах
	 *  \retval Return description
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param> srcVec </param> <values> imu0 </values>
	 *          <param> stride </param> <values> 0 1 </values>
	 *          <param> value3 </param> <values> 1 </values>
	 *          <param> value4 </param> <values> 2 </values>
	 *          <param> dstVec </param> <values> imu0 imu1 </values>
	 *          <param> countVec </param> <values> 128 512 1024 </values>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void cnv32f_v2v4(const v2nm32f* srcVec, int stride, float value3, float value4, v4nm32f* dstVec, int countVec);
	 //! \}
	 
	void cnv32f_v3v4(const nm32f* src_v3nm32f, nm32f* dst_v4nm32f, float value4, int size);

	/**
	 *  \defgroup set_v4nm32f set_v4nm32f
	 *  \brief Функция инициализации элементов массива постоянным значением.
	 *  
	 *  \param dstVec [out] Инициализируемый массив
	 *  \param valueC [in] Значение константы
	 *  \param nSize [in] Размер вектора в элементах
	 *  \retval Return description
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param> dstVec </param> <values> imu0 </values>
	 *          <param> valueC </param> <values> imu1 </values>
	 *          <param> nSize </param> <values> 128 512 1024 </values>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void set_v4nm32f(v4nm32f* dstVec, v4nm32f* valueC, int nSize);
	 //! \}

	/**
	 *  \defgroup replaceEq0_f replaceEq0
	 *  \brief Функция замены нуля другим значением
	 *  
	 *  \param srcVec [in] Входной массив
	 *  \param dstVec [out] Выходной массив
	 *  \param nSize [in] Число элементов в массиве
	 *  \param nReplaceC [in] Втсавляемое число
	 *  \retval Return description
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param> srcVec </param> <values> imu0 </values>
	 *          <param> dstVec </param> <values> imu0 imu1 </values>
	 *          <param> nSize </param> <values> 128 512 1024 </values>
	 *          <param> nReplaceC </param> <values> 1 </values>
	 *      </testperf>
	 *  \endxmlonly
	 */
	 //! \{
	void replaceEq0_32f(float* srcVec, float* dstVec, int nSize, float nReplaceC);
	 //! \}

	/**
	 *  \defgroup findMinMax findMinMax
	 *  \brief Поэлементный поиск минимум и максимума из трех массивов
	 *  
	 *  \param src1 [in] Первый входной массив
	 *  \param src2 [in] Второй входной массив
	 *  \param src3 [in] Третий входной массив
	 *  \param dstMin [out] Выходной массив с минимумом
	 *  \param dstMax [out] Выходной массив с максимумом
	 *  \param nSize [in] Description for size
	 *  \retval Return description
	 *  
	 *  \par
	 *  \xmlonly
	 *      <testperf>
	 *          <param> src1 </param> <values> imu0 </values>
	 *          <param> src2 </param> <values> imu0 </values>
	 *          <param> src3 </param> <values> imu0 </values>
	 *          <param> dstMin </param> <values> imu0 imu1 imu2 </values>
	 *          <param> dstMax </param> <values> imu0 imu1 imu2 </values>
	 *          <param> nSize </param> <values> 128 512 1024 </values>
	 *      </testperf>
	 *  \endxmlonly
	 */
	//! \{
	void findMinMax3(float* src1, float* src2, float* src3, float* dstMin, float* dstMax, int nSize);
	//! \}
	
	void findMinMax2(float* src1, float* src2, float* dstMin, float* dstMax, int nSize);

	int copyArraysByMask(void** srcPointers, nm1* mask, void** dstPointers, int nArrays, int size);
	int maskSelectionLight_RGBA_BGRA(v4nm32s* srcLight, nm1* mask, v4nm32s* dstLight, int size);

	void absIfNegElse0_32f(float* src, float* dst, int size);

	void remap_32u(nm32u* pSrcVec, nm32u* pDstVec, nm32s* pRemapTable, int nSize);
	
	void ternaryLt0_AddC_AddC_32f(nm32f* srcFlags, nm32f* srcVec, float valueLeft, float valueRight, float* dstVec, int size);
}
void reverseMatrix3x3in4x4(mat4nm32f* src, mat4nm32f* dst);

void getDrawInfo(Triangles* triangles, DrawInfo* drawInfo, int count);

void addInstrNMC1(HalRingBuffer* commandsRB, int instr, int param0 = 0, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0);

void setSegmentMask(v2nm32f* minXY, v2nm32f* maxXY, SegmentMask* masks, int primCount);
int pushToTriangles_t(const float *vertexX, const float *vertexY, const float *vertexZ, const v4nm32f* color, Triangles& triangles, int countVertex);
void rasterizeT(Triangles* triangles, SegmentMask* masks, int count);
void rasterizeL(Lines* lines, int count);

void fillPolygonsT(Polygons* poly, Triangles* triangles, int count, int segX, int segY);
void fillPolygonsL(Polygons* poly, Lines* lines, int count, int segX, int segY);

void pow_32f(nm32f* srcVec, nm32f* dstVec, float powC, int size, nm32f* pTmp1);

int cullFaceSortTriangles(Triangles* triangles, int count);

/**
 *  \defgroup color Light
 *  \brief Функция расчета освещения
 *  
 *  \param vertex [in] Входной массив вершин
 *  \param srcNormal_dstColor [in, out] Входной массив нормалей, выходной массив цветов
 *  \param size [in] Число вершин
 *  \retval Return description
 *  \details Функция использует массивы nmglBuffer0, nmglBuffer1, nmglBuffer2, nmglBuffer3
 *  
 *  \par
 *  \xmlonly
 *      <testperf>
 *          <param> vertex </param> <values> imu0  </values>
 *          <param> srcNormal_dstColor </param> <values> imu0 imu1 </values>
 *          <param> size </param> <values> 128 512 1024 </values>
 *      </testperf>
 *  \endxmlonly
 */
//! \{
void light(v4nm32f* vertex, v4nm32f* srcNormal_dstColor, int size);
//! \}

#endif
