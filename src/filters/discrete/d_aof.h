#ifndef DISCRETE_AOF_H
#define DISCRETE_AOF_H

#include "src/core/discrete_filter.h"


namespace Filters
{

namespace Discrete
{


class AOF : public Core::DiscreteFilter
{

public:
    AOF(Core::PtrFilterParameters params, Core::PtrTask task);


protected:
    void zeroIteration() override;
    void algorithm() override;


protected:
    Array<Matrix> P;
    Vector        lambda;
    Matrix        H, Psy;
};


} // end Discrete

} // end Filters


#endif // DISCRETE_AOF_H
