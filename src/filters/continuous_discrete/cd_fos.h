#ifndef CONTINUOUS_DISCRETE_FOS_H
#define CONTINUOUS_DISCRETE_FOS_H

#include "src/core/continuous_discrete_filter.h"


namespace Filters
{

namespace ContinuousDiscrete
{


class FOS : public Core::ContinuousDiscreteFilter
{

public:
    FOS(Core::PtrFilterParameters params, Core::PtrTask task);


protected:
    void algorithm() override;
};


} // end ContinuousDiscrete

} // end Filters


#endif // CONTINUOUS_DISCRETE_FOS_H
