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

};


} // end Discrete

} // end Filters


#endif // DISCRETE_MFOS_H
