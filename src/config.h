/*! \file
 * \brief Файл конфигурации проекта.
 */

#ifndef CONFIG_H
#define CONFIG_H


//#define NDEBUG  /* раскомментировать чтобы выключить assert-ы в релизе */
#define QT_ENABLED /* раскомментировать если используется Qt */


using uint  = unsigned int;
using ulong = unsigned long;

static constexpr ulong TASK_RAND_SEED   = 1638493;
static constexpr ulong FILTER_RAND_SEED = 1052704;


#endif // CONFIG_H
