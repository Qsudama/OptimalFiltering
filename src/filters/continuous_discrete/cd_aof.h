#ifndef CONTINUOUS_DISCRETE_AOF_H
#define CONTINUOUS_DISCRETE_AOF_H

#include "src/core/continuous_discrete_filter.h"


namespace Filters
{

namespace ContinuousDiscrete
{


class AOF : public Core::ContinuousDiscreteFilter
{

public:
    AOF(Core::PtrFilterParameters params, Core::PtrTask task);


protected:
    void zeroIteration() override;
    void algorithm() override;


protected:
    Array<Matrix> m_sampleP;
};


} // end ContinuousDiscrete

} // end Filters


#endif // CONTINUOUS_DISCRETE_AOF_H
