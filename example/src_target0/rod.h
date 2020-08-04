#ifndef _ROD_H
#define _ROD_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
//#include <GLSC/gl.h>
#include <nmgladapter.h>
#ifdef __cplusplus
extern "C"
{
#endif

/*endian h*/
#include <rod_endian.h>

#ifndef ROD_FRONTEND
  #ifndef ROD_NANOTIME
    #define ROD_NANOTIME
  #endif
#else
  #ifdef ROD_UNIX
    #ifndef ROD_NANOTIME
      #define ROD_NANOTIME
      #define ROD_EPOCH_TIME
    #endif
  #endif
#endif
/**common defines */
#define ROD_LOG_ERROR 1
#define ROD_LOG_WARNING 2
#define ROD_LOG_DEBUG 3
#define ROD_LOG_INFO 4
#define ROD_LOG_OUTPUT 5

#if LOG_LEVEL > 0
  #define LOGFT(type, format, ...) {if (type <= LOG_LEVEL) {fprintf(stderr, "%s " format, LogTypeToString(type), __VA_ARGS__);}}
  #define LOGF(format, ...) fprintf(stderr, format, __VA_ARGS__)

  #define LOGT(type, s, ...) {if (type <= LOG_LEVEL) {fprintf(stderr, "%s %s", LogTypeToString(type), s);}}
  #define LOG(s, ...) fprintf(stderr,"%s", s)

  #define LOG_FLUSH() fflush(stderr)
#else
  #define LOGFT(...)
  #define LOGF(...)
  #define LOGT(...)
  #define LOG(...)

  #define LOG_FLUSH(...)
#endif

#define VARZERO(v) memset(&(v), 0, sizeof(v));
#define COUNT(a) sizeof(a)/sizeof(a[0])


#define ROD_AUX_BACKGROUND_THREAD_COUNT 20 /**<  Количество фоновых потоков */
#define ROD_AUX_BACKGROUND_THREAD_EPS 0.05 /**<  Точность калибрации загрузки фонового потока */
#define ROD_AUX_BACKGROUND_THREAD_PERIOD 0.1 /**<  Период загрузки фонового потока */
#define ROD_AUX_DELAY_MEMORY_SIZE (4<<20) /**< Размер памяти для фоновых потоков */

#define ROD_INFO_TEST_MAX_TEXTURE_COUNT (1<<15) /**< Максимальное измеряемое число идентификатов тестур */
#define ROD_INFO_TEST_MAX_MEMORY (1<<30) /**< Максимальный измеряемый размер видео памяти */
#define ROD_INFO_TEST_MIN_TEXTURE_SIZE  (1<<4) /**< Минимальный измеряемый размер текстуры */



#define ROD_VIDEOBUFFER_W (512)
#define ROD_VIDEOBUFFER_H (512)
#define ROD_VIDEOBUFFER_Z (512)
#define ROD_DATA_ARRAY_SIZE (128)

#define ROD_OK                (0)
#define ROD_ERROR            (-1)
#define ROD_ERROR_VALUE      (-2)
#define ROD_ERROR_MEMORY     (-3)
#define ROD_ERROR_OPENGL     (-4)
#define ROD_ERROR_CONNECTION (-5)
#define ROD_ERROR_PROTOCOL   (-6)
#define ROD_ERROR_ACTION     (-7)
#define ROD_ERROR_SYSFUNC    (-8)
#define ROD_ERROR_THREAD     (-9)



#define ROD_AUX_DELAYTYPE_MEMORY   1
#define ROD_AUX_DELAYTYPE_DEFAULT  0

#define ROD_AUX_MODULATE_NONE    0
#define ROD_AUX_MODULATE_SIN     1
#define ROD_AUX_MODULATE_IMPULSE  2

#define ROD_RENDER_DIRECT 0
#define ROD_RENDER_VARRAY 1
#define ROD_RENDER_DIRECT_SIMPLE 2
#define ROD_RENDER_VARRAY_SIMPLE 3
#define ROD_RENDER_LIST_OPTIMIZED 4

#define  ROD_LOAD_LOW 0
#define  ROD_LOAD_MEDIUM 1
#define  ROD_LOAD_HIGH 2
#define  ROD_LOAD_VERY_HIGH 3
#define  ROD_LOAD_ULTIMATE 4

#define ROD_MODE_PERFORMANCE 0
#define ROD_MODE_CORRECT 1



/** \brief Заголовок пакета запуска сценария
 * Некоторые соглашения:
 * param в выходной структуре backend'a означает код ошибки сценария.
 * param в выходной структуре frontend'a в функциях завершения сцены означает принудительное (!= 0) - без ожидаения завершение сцены.
 * При других вариантах использования параметра - обязательно писать комментарий, желательное в таком случае использовать payload
 */
typedef struct s_RodItemHeader
{
  int64_t id;   /**< Идентификатор сценария */
  int64_t size; /**< Размер ВСЕГО пакета в байтах */
  int64_t param;/**< Произвольный параметр */
} t_RodItemHeader;


typedef struct s_RodColor
{
  union
  {
    uint32_t u32;
    struct
    {
      uint8_t r;
      uint8_t g;
      uint8_t b;
      uint8_t a;
    } rgba;
  };
} t_RodColor;

/** \brief Функция округления размера до десятичной степени двойки
 *
 * \param n double Размер в байтах
 * \return double Размер в максимальной десятичной степени двойки
 *
 */
double SizeRoundBin(double n);

/** \brief Функция возвращения строки суффикса байт
 *
 * \param n double Размер в байтах
 * \return const char* Строки суффикса
 *
 */
const char* SizeSuffixBin(double n);

/** \brief Функция округления до степени тысячи
 *
 * \param n double Число
 * \return double Число, округленное до степени тысячи
 *
 */
double NumberRoundDec(double n);

/** \brief Функция возвращения десятичного суффикса
 *
 * \param n double Число
 * \return const char* Десятичный суффик
 *
 */
const char* NumberSuffixDec(double n);


#ifdef ROD_NANOTIME
/** \brief Функция возвращения текущего времени с точностью до наносекунд
 *
 * \return double Текущее время в секундах
 *
 */
  double
   Nanotime();
#endif

int32_t RodCheckGLFilter(int32_t filter);
int32_t RodCheckGLWrap(int32_t filter);
int32_t RodCheckGLAlpha(int32_t func);
int32_t RodCheckGLBlend(int32_t sfactor, int32_t dfactor);
int32_t RodCheckGLDepth(int32_t func);

/** \brief Функция преобразования GLenum в строку
 *
 * \param which GLenum Примитив
 * \return const char* Строка
 *
 */
const char * WhichToString(GLenum which);

/** \brief Преобразхование типа лога в строку
 *
 * \param type int Тип лога
 * \return const char* Строка
 *
 */
const char * LogTypeToString(int type);
#ifdef __cplusplus
}
#endif

#endif /*rod.h*/
