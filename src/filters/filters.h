#ifndef FILTERS_H
#define FILTERS_H

#include "src/core/types_info.h"
#include "src/filters/continuous/c_aof.h"
#include "src/filters/continuous/c_fos.h"
#include "src/filters/continuous_discrete/cd_aof.h"
#include "src/filters/continuous_discrete/cd_dfos.h"
#include "src/filters/continuous_discrete/cd_dfosbo.h"
#include "src/filters/continuous_discrete/cd_fos.h"
#include "src/filters/discrete/d_aof.h"
#include "src/filters/discrete/d_fos.h"
#include "src/filters/discrete/d_mfos.h"

namespace Filters
{


enum class FILTER_ID { AOF, FOS, DFOS, DFOSBO, MDFOS };


class FilterFactory
{
public:
    static Core::PtrFilter create(Core::FILTER_TYPE type, FILTER_ID id, Core::PtrFilterParameters params,
                                  Core::PtrTask task);

private:
    static Core::PtrFilter createContinuous(FILTER_ID id, Core::PtrFilterParameters params, Core::PtrTask task);
    static Core::PtrFilter createContinuousDiscrete(FILTER_ID id, Core::PtrFilterParameters params, Core::PtrTask task);
    static Core::PtrFilter createDiscrete(FILTER_ID id, Core::PtrFilterParameters params, Core::PtrTask task);
};


} // end Filters


#endif // FILTERS_H
