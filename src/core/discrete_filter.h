#ifndef DISCRETE_FILTER_H
#define DISCRETE_FILTER_H

#include "discrete_task.h"
#include "filter.h"
#include "src/math/linear_algebra.h"


namespace Core
{


class DiscreteFilter : public Filter
{

public:
    DiscreteFilter(PtrFilterParameters params, PtrTask task);


protected:
    void zeroIteration() override;


protected:
    PtrDTask m_task;
};


using PtrDFilter = std::shared_ptr<DiscreteFilter>;


} // end Core


#endif // DISCRETE_FILTER_H
