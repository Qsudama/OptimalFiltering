#include "tasks_factory.h"

using namespace Core;


namespace Tasks
{


PtrTask TaskFactory::create(FILTER_TYPE ftype, TASK_ID id, APPROX_TYPE atype)
{
    switch (ftype) {
    case FILTER_TYPE::Continuous:
        return createContinuous(id, atype);
    case FILTER_TYPE::ContinuousDiscrete:
        return createContinuousDiscrete(id, atype);
    case FILTER_TYPE::Discrete:
        return createDiscrete(id, atype);
    case FILTER_TYPE::LogicDynamic:
        return createLogicDynamic(id, atype);
    }
    return PtrTask(nullptr);
}

PtrTask TaskFactory::createContinuous(TASK_ID id, APPROX_TYPE type)
{
    switch (type) {
    case APPROX_TYPE::Linear:
        switch (id) {
        case TASK_ID::Landing:
            return PtrTask(new Tasks::Continuous::LandingLinear);
        case TASK_ID::VanDerPol:
            return PtrTask(new Tasks::Continuous::VanDerPolLinear);
        case TASK_ID::Scalar:
            return PtrTask(new Tasks::Discrete::ScalarLinear); // WARNING
        case TASK_ID::LandingTest:
            return PtrTask(nullptr); // WARNING
        case TASK_ID::LandingRejection:
            return PtrTask(nullptr); // WARNING
        }
    case APPROX_TYPE::Gauss:
        switch (id) {
        case TASK_ID::Landing:
            return PtrTask(new Tasks::Continuous::LandingGauss);
        case TASK_ID::VanDerPol:
            return PtrTask(new Tasks::Continuous::VanDerPolGauss);
        case TASK_ID::Scalar:
            return PtrTask(new Tasks::Discrete::ScalarLinear); // WARNING
        case TASK_ID::LandingTest:
            return PtrTask(nullptr); // WARNING
        case TASK_ID::LandingRejection:
            return PtrTask(nullptr); // WARNING
        }
    }
    return PtrTask(nullptr);
}

PtrTask TaskFactory::createContinuousDiscrete(TASK_ID id, APPROX_TYPE type)
{
    switch (type) {
    case APPROX_TYPE::Linear:
        switch (id) {
        case TASK_ID::Landing:
            return PtrTask(new Tasks::ContinuousDiscrete::LandingLinear);
        case TASK_ID::VanDerPol:
            return PtrTask(new Tasks::ContinuousDiscrete::VanDerPolLinear);
        case TASK_ID::Scalar:
            return PtrTask(nullptr);
        case TASK_ID::LandingTest:
            return PtrTask(nullptr); // WARNING
        case TASK_ID::LandingRejection:
            return PtrTask(nullptr); // WARNING
        }
    case APPROX_TYPE::Gauss:
        switch (id) {
        case TASK_ID::Landing:
            return PtrTask(new Tasks::ContinuousDiscrete::LandingGauss);
        case TASK_ID::VanDerPol:
            return PtrTask(new Tasks::ContinuousDiscrete::VanDerPolGauss);
        case TASK_ID::Scalar:
            return PtrTask(nullptr);
        case TASK_ID::LandingTest:
            return PtrTask(nullptr); // WARNING
        case TASK_ID::LandingRejection:
            return PtrTask(nullptr); // WARNING
        }
    }
    return PtrTask(nullptr);
}

PtrTask TaskFactory::createDiscrete(TASK_ID id, APPROX_TYPE type)
{
    switch (type) {
    case APPROX_TYPE::Linear:
        switch (id) {
        case TASK_ID::Landing:
            return PtrTask(new Tasks::Discrete::LandingLinear);
        case TASK_ID::VanDerPol:
            return PtrTask(nullptr); // WARNING
        case TASK_ID::Scalar:
            return PtrTask(new Tasks::Discrete::ScalarLinear);
        case TASK_ID::LandingTest:
            return PtrTask(nullptr); // WARNING
        case TASK_ID::LandingRejection:
            return PtrTask(nullptr); // WARNING
        }
    case APPROX_TYPE::Gauss:
        switch (id) {
        case TASK_ID::Landing:
            return PtrTask(new Tasks::Discrete::LandingGauss);
        case TASK_ID::VanDerPol:
            return PtrTask(nullptr); // WARNING
        case TASK_ID::Scalar:
            return PtrTask(new Tasks::Discrete::ScalarLinear);
        case TASK_ID::LandingTest:
            return PtrTask(nullptr); // WARNING
        case TASK_ID::LandingRejection:
            return PtrTask(nullptr); // WARNING
        }
    }
    return PtrTask(nullptr);
}


PtrTask TaskFactory::createLogicDynamic(TASK_ID id, APPROX_TYPE type)
{
    switch (type) {
    case APPROX_TYPE::Linear:
        switch (id) {
        case TASK_ID::Landing:
            return PtrTask(nullptr); // WARNING
        case TASK_ID::VanDerPol:
            return PtrTask(nullptr); // WARNING
        case TASK_ID::Scalar:
            return PtrTask(nullptr);
        case TASK_ID::LandingTest:
            return PtrTask(new Tasks::LogicDynamic::LandingTestLinear);
        case TASK_ID::LandingRejection:
            return PtrTask(new Tasks::LogicDynamic::LandingRejectionLinear);
        }
    case APPROX_TYPE::Gauss:
        switch (id) {
        case TASK_ID::Landing:
            return PtrTask(nullptr); // WARNING
        case TASK_ID::VanDerPol:
            return PtrTask(nullptr); // WARNING
        case TASK_ID::Scalar:
            return PtrTask(nullptr);
        case TASK_ID::LandingTest:
            return PtrTask(nullptr); // WARNING
        case TASK_ID::LandingRejection:
            return PtrTask(nullptr); // WARNING
        }
    }
    return PtrTask(nullptr);
}

} // end Tasks
