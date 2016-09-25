#ifndef DISCRETE_FOS_H
#define DISCRETE_FOS_H

#include "src/core/discrete_filter.h"


namespace Filters
{

namespace Discrete
{


class FOS : public Core::DiscreteFilter
{

public:
    FOS(Core::PtrFilterParameters params, Core::PtrTask task);


protected:
    void algorithm() override;


protected:
    Vector ksi, lambda, u;
    Matrix T, Psy, GammaY, GammaZ;
};


} // end Discrete

} // end Filters


#endif // DISCRETE_FOS_H
