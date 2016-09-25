#ifndef CONTINUOUS_FOS_H
#define CONTINUOUS_FOS_H

#include "src/core/continuous_filter.h"


namespace Filters
{

namespace Continuous
{


class FOS : public Core::ContinuousFilter
{

public:
    FOS(Core::PtrFilterParameters params, Core::PtrTask task);


protected:
    void zeroIteration() override;
    void algorithm() override;
};


} // end Filters::Continuous

} // end Filters


#endif // CONTINUOUS_FOS_H
