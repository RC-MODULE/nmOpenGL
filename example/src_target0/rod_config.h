#ifndef ROD_CONFIG_H_
#define ROD_CONFIG_H_
#include <rod_scenes.h>
#define ROD_PACKAGE_MAXSIZE (3<<20)

struct s_RodConfig;

/** \brief Функция инициализации библиотеки MGK и OpenGL
 */
typedef int32_t (*t_RodMGKInitializer)(struct s_RodConfig *);

/** \brief Функция деинициализации библиотеки MGK и OpenGL
 */
typedef int32_t (*t_RodMGKDeinitializer)(struct  s_RodConfig  *);

/** \brief  Функция смены буфера OpenGL
 */
typedef int32_t (*t_RodMGKSwapBuffers)(struct  s_RodConfig  *);

/** \brief Функция инициализации конфигурации
 */
typedef int32_t (*t_RodEnvInit)(struct  s_RodConfig *);

/** \brief Функция деинициализации конфигурации
 */
typedef int32_t (*t_RodEnvFree)(struct  s_RodConfig *);

/** \brief Функция чтения входных параметров
 */
typedef size_t (*t_RodReadInput)(struct  s_RodConfig *, void *, size_t);

/** \brief Функция записи выходных параметров
 */
typedef size_t (*t_RodWriteOutput)(struct  s_RodConfig *, const void *, size_t);

/** \brief Структура конфигурации ТЕСТА
 */
typedef struct s_RodConfig
{
  t_RodEnvInit             envInit;    /**< Функция инициализации конфигурации */
  t_RodEnvFree             envFree;    /**< Функция деинициализации конфигурации */
  t_RodReadInput           readInput;     /**< Функция чтения входных параметров */
  t_RodWriteOutput         writeOutput;   /**< Функция записи выходных параметров */

  t_RodMGKInitializer      initializer;   /**< Функция инициализации библиотеки MGK и OpenGL */
  t_RodMGKDeinitializer    deinitializer; /**< Функция деинициализации библиотеки MGK и OpenGL */
  t_RodMGKSwapBuffers      swapBuffers;   /**< Функция смены буфера OpenGL */

  int32_t xResolution; /**< Размер экрана по ширине */
  int32_t yResolution; /**< Размер экрана по высоте */

  void * pEnvContext; /**< Указатель на контекст окружения */
} t_RodConfig;


/** \brief Функция инициализации контекста
 *
 * \param cfg Указатель на конфигурацию
 */
#define ROD_INITIALIZER(cfg) (*(cfg)).initializer((struct s_RodConfig*)(cfg))

/** \brief Функция деинициализации контекста
 *
 * \param cfg Указатель на конфигурацию
 */
#define ROD_DEINITIALIZER(cfg) (*(cfg)).deinitializer((struct s_RodConfig*)(cfg))

/** \brief Функция смены видеобуфера
 *
 * \param cfg Указатель на конфигурацию
 */
#define ROD_SWAPBUFFERS(cfg) (*(cfg)).swapBuffers((struct s_RodConfig*)(cfg));


#endif // header guard
