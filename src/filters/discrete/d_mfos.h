#ifndef DISCRETE_MFOS_H
#define DISCRETE_MFOS_H

#include "src/core/discrete_filter.h"


namespace Filters
{

namespace Discrete
{


class MFOS : public Core::DiscreteFilter
{

public:
    MFOS(Core::PtrFilterParameters params, Core::PtrTask task);


protected:
    void algorithm() override;


protected:
    //    Vector ksi, lambda, u;
    //    Matrix T, Psy, GammaY, GammaZ;
};


} // end Discrete

} // end Filters


#endif // DISCRETE_MFOS_H
