#ifndef DCI_TEST_H_
#define DCI_TEST_H_
#include <rod_config.h>

/** \brief Функция запуска сцены DCI
 *
 * \param pConfig t_RodConfig* Указатель на конфиг
 * \param input t_RodItemDciInput* Входная структура
 * \param output t_RodItemHeader* Выходная структура
 * \return int32_t Код ошибки
 *
 */
int32_t RodDciSceneBegin(t_RodConfig * pConfig, t_RodItemDciInput * input, t_RodItemHeader * output);

/** \brief Функция завершения сцены DCI
 * Параметр заголовка входной структуры param 0 - ожидание завершения, иначе принудительное завершение теста
 *
 * \param pConfig t_RodConfig* Указаьтель на конфиг
 * \param input t_RodItemHeader* Входная структура
 * \param output t_RodItemDciOutput* Выходная структура
 * \return int32_t Код ошибки
 *
 */
int32_t RodDciSceneEnd(t_RodConfig * pConfig, t_RodItemHeader * input, t_RodItemDciOutput * output);

/** \brief Функция прерывания сцены DCI
 *
 * \return int32_t Код ошибки
 *
 */
int32_t RodDciAbort();
#endif
