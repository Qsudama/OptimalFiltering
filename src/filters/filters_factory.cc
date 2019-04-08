#include "filters_factory.h"

using namespace Core;


namespace Filters
{


PtrFilter FilterFactory::create(FILTER_TYPE type, FILTER_ID id, PtrFilterParameters params, PtrTask task)
{
    switch (type) {
    case FILTER_TYPE::Continuous:
        return createContinuous(id, params, task);
    case FILTER_TYPE::ContinuousDiscrete:
        return createContinuousDiscrete(id, params, task);
    case FILTER_TYPE::Discrete:
        return createDiscrete(id, params, task);
    case FILTER_TYPE::LogicDynamic:
        return createLogicDynamic(id, params, task);
    }
    return PtrFilter(nullptr);
}


PtrFilter FilterFactory::createContinuous(FILTER_ID id, PtrFilterParameters params, PtrTask task)
{
    switch (id) {
    case FILTER_ID::AOF:
        return PtrFilter(new Filters::Continuous::AOF(params, task));
    case FILTER_ID::FOS:
        return PtrFilter(new Filters::Continuous::FOS(params, task));
    default:
        return PtrFilter(nullptr); // WARNING: такого фильтра нет
    }
    return PtrFilter(nullptr);
};

PtrFilter FilterFactory::createContinuousDiscrete(FILTER_ID id, PtrFilterParameters params, PtrTask task)
{
    switch (id) {
    case FILTER_ID::AOF:
        return PtrFilter(new Filters::ContinuousDiscrete::AOF(params, task));
    case FILTER_ID::FOS:
        return PtrFilter(new Filters::ContinuousDiscrete::FOS(params, task));
    case FILTER_ID::DFOS:
        return PtrFilter(new Filters::ContinuousDiscrete::DFOS(params, task));
    case FILTER_ID::DFOSBO:
        return PtrFilter(new Filters::ContinuousDiscrete::DFOSBO(params, task));
    default:
        return PtrFilter(nullptr); // WARNING: такого фильтра нет
    }
    return PtrFilter(nullptr);
};

PtrFilter FilterFactory::createDiscrete(FILTER_ID id, PtrFilterParameters params, PtrTask task)
{
    switch (id) {
    case FILTER_ID::AOF:
        return PtrFilter(new Filters::Discrete::AOF(params, task));
    case FILTER_ID::FOS:
        return PtrFilter(new Filters::Discrete::FOS(params, task));
    case FILTER_ID::MDFOS:
        return PtrFilter(new Filters::Discrete::MFOS(params, task));
    case FILTER_ID::DUOF:
        return PtrFilter(new Filters::Discrete::DUOF(params, task));
    case FILTER_ID::DFMP:
        return PtrFilter(new Filters::Discrete::DFMP(params, task));
    case FILTER_ID::FKP:
        return PtrFilter(new Filters::Discrete::FKP(params, task));
    case FILTER_ID::DFKP:
        return PtrFilter(new Filters::Discrete::DFKP(params, task));
    case FILTER_ID::MDFMP:
        return PtrFilter(new Filters::Discrete::MDFMP(params, task));
    default:
        return PtrFilter(nullptr); // WARNING: такого фильтра нет
    };
    return PtrFilter(nullptr);
}

PtrFilter FilterFactory::createLogicDynamic(FILTER_ID id, PtrFilterParameters params, PtrTask task)
{
    switch (id) {
    case FILTER_ID::AOF:
        return PtrFilter(new Filters::LogicDynamic::AOF(params, task, id));
    case FILTER_ID::FOS:
        return PtrFilter(new Filters::LogicDynamic::FOS(params, task, id));
    case FILTER_ID::LDFKP:
        return PtrFilter(new Filters::LogicDynamic::FKP_FBP(params, task, id));
    case FILTER_ID::LDFBP:
            return PtrFilter(new Filters::LogicDynamic::FKP_FBP(params, task, id));
    default:
        return PtrFilter(nullptr); // WARNING: такого фильтра нет
    };
    return PtrFilter(nullptr);
}


} // end Filters
