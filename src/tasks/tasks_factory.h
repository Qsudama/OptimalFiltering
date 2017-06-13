#ifndef TASKS_H
#define TASKS_H

#include "src/core/types_info.h"
#include "src/tasks/continuous/c_landing_gauss.h"
#include "src/tasks/continuous/c_landing_linear.h"
#include "src/tasks/continuous/c_vanderpol_gauss.h"
#include "src/tasks/continuous/c_vanderpol_linear.h"
#include "src/tasks/continuous_discrete/cd_landing_gauss.h"
#include "src/tasks/continuous_discrete/cd_landing_linear.h"
#include "src/tasks/continuous_discrete/cd_vanderpol_gauss.h"
#include "src/tasks/continuous_discrete/cd_vanderpol_linear.h"
#include "src/tasks/discrete/d_landing_gauss.h"
#include "src/tasks/discrete/d_landing_linear.h"
#include "src/tasks/discrete/d_scalar_linear.h"
#include "src/tasks/discrete/d_scalar_gauss.h"
#include "src/tasks/logic_dynamic/ld_landing_test_linear.h"
#include "src/tasks/logic_dynamic/ld_landing_rejection_linear.h"

//! \brief Модуль, содержащий реализации конкретных задач для фильтров.

namespace Tasks
{


//! \brief Набор идентификаторов имеющихся задач.

enum class TASK_ID {
    LandingLinear,    ///< Спуска ЛА на планету (линеаризованный).
    LandingGauss,     ///< Спуска ЛА на планету (гауссовский).
    VanDerPolLinear,  ///< Осциллятор Ван-дер-Поля (линеаризованный).
    VanDerPolGauss,   ///< Осциллятор Ван-дер-Поля (гауссовский).
    ScalarLinear,     ///< Тестовый скалярный пример (линеаризованный).
    ScalarGauss,      ///< Тестовый скалярный пример (гауссовский).
    LandingTest,      ///< Спуск ЛА с неполной информацией. Тестовый пример
    LandingRejection, ///< Спуск ЛА с отказами.
};


//! \brief Фабричный класс для создания экземпляров классов задач.

class TaskFactory
{

public:
    /*!
     \brief Создает экземпляр задачи по входным параметрам.

     \param ftype - тип фильтра (непрерывный, дискретный, ...).
     \param id - идентификатор задачи.
     \param atype - метод приближения (линейный, гауссвоский, ...).
     \return указатель на созданный экземпляр задачи.
    */
    static Core::PtrTask create(Core::FILTER_TYPE ftype, TASK_ID id);


private:
    //! \brief Вспомогательный метод для создания задачи для непрерывных фильтров оптимальной
    //! структуры.
    static Core::PtrTask createContinuous(TASK_ID id);

    //! \brief Вспомогательный метод для создания задачи для непрерывно-дискретных фильтров
    //! оптимальной структуры.
    static Core::PtrTask createContinuousDiscrete(TASK_ID id);

    //! \brief Вспомогательный метод для создания задачи для дискретных фильтров оптимальной
    //! структуры.
    static Core::PtrTask createDiscrete(TASK_ID id);

    //! \brief Вспомогательный метод для создания задачи для логико-динамических фильтров
    //! оптимальной структуры.
    static Core::PtrTask createLogicDynamic(TASK_ID id);
};


} // end Tasks


#endif // TASKS_H
