#ifndef CONTINUOUS_AOF_H
#define CONTINUOUS_AOF_H

#include "src/core/continuous_filter.h"


namespace Filters
{

namespace Continuous
{


class AOF : public Core::ContinuousFilter
{

public:
    AOF(Core::PtrFilterParameters params, Core::PtrTask task);


protected:
    void zeroIteration() override;
    void algorithm() override;


protected:
    Array<Math::Matrix> m_sampleP;
};


} // end Filters::Continuous

} // end Filters


#endif // CONTINUOUS_AOF_H
