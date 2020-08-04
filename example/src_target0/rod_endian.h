#ifndef _ROD_ENDIAN_H
#define _ROD_ENDIAN_H
#include <stdint.h>
#include <stdlib.h>
#ifdef __cplusplus
extern "C"
{
#endif

/**ENDIAN defines */
#define ROD_BIG_ENDIAN    1234
#define ROD_LITTLE_ENDIAN 3412


/** \brief Дефайн переворота байт
 *
 * \param arg Переменная
 *
 */
#define ENDIAN_VARIABLE(arg) EndianVariable((uint8_t*)&arg, sizeof(arg))

#if defined(__i386__) || defined(__x86_64__) || defined(__ia64__)
  #define ROD_ENDIAN	ROD_LITTLE_ENDIAN
#else
  #define ROD_ENDIAN	ROD_BIG_ENDIAN
#endif

#if ROD_ENDIAN == ROD_BIG_ENDIAN
  #define ROD_SWAP_BE 0
  #define ROD_SWAP_LE 1
  /** \brief Функция преобразования переменной в big endian
   *
   * \param arg переменная
   *
   */
  #define vh2be(arg)
  /** \brief Функция преобразования переменной из big endian
   *
   * \param arg переменная
   *
   */
  #define vbe2h(arg)
   /** \brief Функция преобразования переменной в little endian
   *
   * \param arg переменная
   *
   */
  #define vh2le(arg) ENDIAN_VARIABLE(arg)
  /** \brief Функция преобразования переменной из little endian
   *
   * \param arg переменная
   *
   */
  #define vle2h(arg) ENDIAN_VARIABLE(arg)
#elif ROD_ENDIAN == ROD_LITTLE_ENDIAN
  #define ROD_SWAP_BE 1
  #define ROD_SWAP_LE 0
  /** \brief Функция преобразования переменной в big endian
   *
   * \param arg переменная
   *
   */
  #define vh2be(arg) ENDIAN_VARIABLE(arg)
  /** \brief Функция преобразования переменной из big endian
   *
   * \param arg переменная
   *
   */
  #define vbe2h(arg) ENDIAN_VARIABLE(arg)
   /** \brief Функция преобразования переменной в little endian
   *
   * \param arg переменная
   *
   */
  #define vh2le(arg)
  /** \brief Функция преобразования переменной из little endian
   *
   * \param arg переменная
   *
   */
  #define vle2h(arg)
#else
  #error "Not defined endian"
#endif

/** \brief Функция переворота байт
 *
 * \param mem uint8_t* Указатель на память
 * \param size size_t Размер байта
 * \return void
 *
 */
void EndianVariable(uint8_t* mem, size_t size);

#ifdef __cplusplus
}
#endif


#endif

