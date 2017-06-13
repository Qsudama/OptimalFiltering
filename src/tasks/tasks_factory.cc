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
        return PtrTask(nullptr); // WARNING
    case TASK_ID::ScalarGauss:
        return PtrTask(nullptr); // WARNING
    case TASK_ID::LandingTest:
        return PtrTask(nullptr); // WARNING
    case TASK_ID::LandingRejection:
        return PtrTask(nullptr); // WARNING
    }
    return PtrTask(nullptr);
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
        return PtrTask(nullptr); // WARNING
    case TASK_ID::ScalarGauss:
        return PtrTask(nullptr); // WARNING
    case TASK_ID::LandingTest:
        return PtrTask(nullptr); // WARNING
    case TASK_ID::LandingRejection:
        return PtrTask(nullptr); // WARNING
    }
    return PtrTask(nullptr);
}

PtrTask TaskFactory::createDiscrete(TASK_ID id)
{
    switch (id) {
    case TASK_ID::LandingLinear:
        return PtrTask(new Tasks::Discrete::LandingLinear);
    case TASK_ID::LandingGauss:
        return PtrTask(new Tasks::Discrete::LandingGauss);
    case TASK_ID::VanDerPolLinear:
        return PtrTask(nullptr); // WARNING
    case TASK_ID::VanDerPolGauss:
        return PtrTask(nullptr); // WARNING
    case TASK_ID::ScalarLinear:
        return PtrTask(new Tasks::Discrete::ScalarLinear);
    case TASK_ID::ScalarGauss:
        return PtrTask(new Tasks::Discrete::ScalarGauss);
    case TASK_ID::LandingTest:
        return PtrTask(nullptr); // WARNING
    case TASK_ID::LandingRejection:
        return PtrTask(nullptr); // WARNING
    }
    return PtrTask(nullptr);
}


PtrTask TaskFactory::createLogicDynamic(TASK_ID id)
{
    switch (id) {
    case TASK_ID::LandingLinear:
        return PtrTask(nullptr); // WARNING
    case TASK_ID::LandingGauss:
        return PtrTask(nullptr); // WARNING
    case TASK_ID::VanDerPolLinear:
        return PtrTask(nullptr); // WARNING
    case TASK_ID::VanDerPolGauss:
        return PtrTask(nullptr); // WARNING
    case TASK_ID::ScalarLinear:
        return PtrTask(nullptr); // WARNING
    case TASK_ID::ScalarGauss:
        return PtrTask(nullptr); // WARNING
    case TASK_ID::LandingTest:
        return PtrTask(new Tasks::LogicDynamic::LandingTestLinear);
    case TASK_ID::LandingRejection:
        return PtrTask(new Tasks::LogicDynamic::LandingRejectionLinear);
    }
    return PtrTask(nullptr);
}

} // end Tasks
