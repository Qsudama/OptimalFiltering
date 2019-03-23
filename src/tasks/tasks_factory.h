#ifndef TASKS_H
#define TASKS_H

#include "tasks_identifiers.h"
#include "src/core/types_info.h"
// continuous
#include "src/tasks/continuous/c_landing_gauss.h"
#include "src/tasks/continuous/c_landing_linear.h"
#include "src/tasks/continuous/c_vanderpol_gauss.h"
#include "src/tasks/continuous/c_vanderpol_linear.h"
#include "src/tasks/continuous/c_scalar_linear.h"
#include "src/tasks/continuous/c_scalar_gauss.h"

// continuous_discrete
#include "src/tasks/continuous_discrete/cd_landing_gauss.h"
#include "src/tasks/continuous_discrete/cd_landing_linear.h"
#include "src/tasks/continuous_discrete/cd_vanderpol_gauss.h"
#include "src/tasks/continuous_discrete/cd_vanderpol_linear.h"
#include "src/tasks/continuous_discrete/cd_scalar_linear.h"

// discrete
#include "src/tasks/discrete/d_landing_gauss.h"
#include "src/tasks/discrete/d_landing_linear.h"
#include "src/tasks/discrete/d_scalar_linear.h"
#include "src/tasks/discrete/d_scalar_gauss.h"
#include "src/tasks/discrete/d_vanderpol_linear.h"
#include "src/tasks/discrete/d_vanderpol_gauss.h"

// logic_dynamic
#include "src/tasks/logic_dynamic/ld_scalar_rejection_gauss.h"
#include "src/tasks/logic_dynamic/ld_landing_rejection_6d_linear.h"
#include "src/tasks/logic_dynamic/ld_landing_rejection_3d_linear.h"
#include "src/tasks/logic_dynamic/ld_vanderpol_rejection_linear.h"

//! \brief Модуль, содержащий реализации конкретных задач для фильтров.

namespace Tasks
{

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
