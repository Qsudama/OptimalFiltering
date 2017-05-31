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
    case FILTER_ID::DFOS:
        return PtrFilter(nullptr); // WARNING: такого фильтра нет
    case FILTER_ID::DFOSBO:
        return PtrFilter(nullptr); // WARNING: такого фильтра нет
    case FILTER_ID::MDFOS:
        return PtrFilter(nullptr); // WARNING: такого фильтра нет
    case FILTER_ID::DUOF:
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
    case FILTER_ID::MDFOS:
        return PtrFilter(nullptr); // WARNING: такого фильтра нет
    case FILTER_ID::DUOF:
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
    case FILTER_ID::DFOS:
        return PtrFilter(nullptr); // WARNING: такого фильтра нет
    case FILTER_ID::DFOSBO:
        return PtrFilter(nullptr); // WARNING: такого фильтра нет
    case FILTER_ID::MDFOS:
        return PtrFilter(new Filters::Discrete::MFOS(params, task));
    case FILTER_ID::DUOF:
        return PtrFilter(new Filters::Discrete::DUOF(params, task));
    };
    return PtrFilter(nullptr);
}


} // end Filters
