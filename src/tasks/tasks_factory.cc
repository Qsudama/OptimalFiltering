#include "tasks_factory.h"

using namespace Core;


namespace Tasks
{


PtrTask TaskFactory::create(FILTER_TYPE ftype, TASK_ID id)
{
    switch (ftype) {
    case FILTER_TYPE::Continuous:
        return createContinuous(id);
    case FILTER_TYPE::ContinuousDiscrete:
        return createContinuousDiscrete(id);
    case FILTER_TYPE::Discrete:
        return createDiscrete(id);
    case FILTER_TYPE::LogicDynamic:
        return createLogicDynamic(id);
    }
    return PtrTask(nullptr);
}

PtrTask TaskFactory::createContinuous(TASK_ID id)
{
    switch (id) {
    case TASK_ID::LandingLinear:
        return PtrTask(new Tasks::Continuous::LandingLinear);
    case TASK_ID::LandingGauss:
        return PtrTask(new Tasks::Continuous::LandingGauss);
    case TASK_ID::VanDerPolLinear:
        return PtrTask(new Tasks::Continuous::VanDerPolLinear);
    case TASK_ID::VanDerPolGauss:
        return PtrTask(new Tasks::Continuous::VanDerPolGauss);
    case TASK_ID::ScalarLinear:
        return PtrTask(new Tasks::Continuous::ScalarLinear);
    case TASK_ID::ScalarGauss:
        return PtrTask(new Tasks::Continuous::ScalarGauss);
    default:
        return PtrTask(nullptr); // WARNING
    }
}

PtrTask TaskFactory::createContinuousDiscrete(TASK_ID id)
{
    switch (id) {
    case TASK_ID::LandingLinear:
        return PtrTask(new Tasks::ContinuousDiscrete::LandingLinear);
    case TASK_ID::LandingGauss:
        return PtrTask(new Tasks::ContinuousDiscrete::LandingGauss);
    case TASK_ID::VanDerPolLinear:
        return PtrTask(new Tasks::ContinuousDiscrete::VanDerPolLinear);
    case TASK_ID::VanDerPolGauss:
        return PtrTask(new Tasks::ContinuousDiscrete::VanDerPolGauss);
    case TASK_ID::ScalarLinear:
        return PtrTask(new Tasks::ContinuousDiscrete::ScalarLinear);
    default:
        return PtrTask(nullptr); // WARNING
    }
}

PtrTask TaskFactory::createDiscrete(TASK_ID id)
{
    switch (id) {
    case TASK_ID::LandingLinear:
        return PtrTask(new Tasks::Discrete::LandingLinear);
    case TASK_ID::LandingGauss:
        return PtrTask(new Tasks::Discrete::LandingGauss);
    case TASK_ID::DVanDerPolLinear:
        return PtrTask(new Tasks::Discrete::VanDerPolLinear);
    case TASK_ID::ScalarLinear:
        return PtrTask(new Tasks::Discrete::ScalarLinear);
    case TASK_ID::ScalarGauss:
        return PtrTask(new Tasks::Discrete::ScalarGauss);
    default:
        return PtrTask(nullptr); // WARNING
    }
}


PtrTask TaskFactory::createLogicDynamic(TASK_ID id)
{
    switch (id) {
    case TASK_ID::LDScalarRejectionGauss:
        return PtrTask(new Tasks::LogicDynamic::ScalarRejectionGauss);
    case TASK_ID::LDLandingRejection3DLinear:
        return PtrTask(new Tasks::LogicDynamic::LandingRejection3DLinear);
    case TASK_ID::LDLandingRejection6DLinear:
        return PtrTask(new Tasks::LogicDynamic::LandingRejection6DLinear);
    default:
        return PtrTask(nullptr); // WARNING
    }
}

} // end Tasks
