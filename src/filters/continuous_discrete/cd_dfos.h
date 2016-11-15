#ifndef CONTINUOUS_DISCRETE_DFOS_H
#define CONTINUOUS_DISCRETE_DFOS_H

#include "src/core/continuous_discrete_filter.h"


namespace Filters
{

namespace ContinuousDiscrete
{


class DFOS : public Core::ContinuousDiscreteFilter
{

public:
    DFOS(Core::PtrFilterParameters params, Core::PtrTask task);


protected:
    void algorithm() override;
};


} // end ContinuousDiscrete

} // end Filters


#endif // CONTINUOUS_DISCRETE_DFOS_H
