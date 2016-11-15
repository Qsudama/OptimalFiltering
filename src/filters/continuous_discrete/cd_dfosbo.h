#ifndef CONTINUOUS_DISCRETE_DFOSBO_H
#define CONTINUOUS_DISCRETE_DFOSBO_H

#include "src/core/continuous_discrete_filter.h"


namespace Filters
{

namespace ContinuousDiscrete
{


class DFOSBO : public Core::ContinuousDiscreteFilter
{

public:
    DFOSBO(Core::PtrFilterParameters params, Core::PtrTask task);


protected:
    void init() override;
    void zeroIteration() override;
    void algorithm() override;


protected:
    ulong ny;
    ulong p;

    Array<Vector> Zb;
};


} // end ContinuousDiscrete

} // end Filters


#endif // CONTINUOUS_DISCRETE_DFOSBO_H
