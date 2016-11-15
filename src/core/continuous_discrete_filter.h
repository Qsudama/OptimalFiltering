#ifndef CONTINUOUS_DISCRETE_FILTER_H
#define CONTINUOUS_DISCRETE_FILTER_H

#include "continuous_discrete_task.h"
#include "filter.h"
#include "src/math/linear_algebra.h"


namespace Core
{


class ContinuousDiscreteFilter : public Filter
{

public:
    ContinuousDiscreteFilter(PtrFilterParameters params, PtrTask task);


protected:
    void zeroIteration() override;


protected:
    PtrCDTask m_task;
};


using PtrCDFilter = std::shared_ptr<ContinuousDiscreteFilter>;


} // end Core


#endif // CONTINUOUS_DISCRETE_FILTER_H
