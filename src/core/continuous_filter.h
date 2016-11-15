#ifndef CONTINUOUS_FILTER_H
#define CONTINUOUS_FILTER_H

#include "continuous_task.h"
#include "filter.h"


namespace Core
{


class ContinuousFilter : public Filter
{

public:
    ContinuousFilter(PtrFilterParameters params, PtrTask task);


protected:
    void zeroIteration() override;


protected:
    PtrCTask m_task;
};


using PtrCFilter = std::shared_ptr<ContinuousFilter>;


} // end Core


#endif // CONTINUOUS_FILTER_H
