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


namespace Tasks
{


enum class TASK_ID { Landing, VanDerPol };


class TaskFactory
{
public:
    static Core::PtrTask create(Core::FILTER_TYPE ftype, TASK_ID id, Core::APPROX_TYPE atype);

private:
    static Core::PtrTask createContinuous(TASK_ID id, Core::APPROX_TYPE type);
    static Core::PtrTask createContinuousDiscrete(TASK_ID id, Core::APPROX_TYPE type);
    static Core::PtrTask createDiscrete(TASK_ID id, Core::APPROX_TYPE type);
};


} // end Tasks


#endif // TASKS_H
