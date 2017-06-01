#ifndef D_DUOF_H
#define D_DUOF_H

#include "src/core/discrete_filter.h"


namespace Filters
{

namespace Discrete
{


class DUOF : public Core::DiscreteFilter
{

public:
    DUOF(Core::PtrFilterParameters params, Core::PtrTask task);


protected:

    void algorithm() override;

private:

    void computeParams(size_t k, Array<Math::Vector> &sampleU, Array<Math::Vector> &sampleS, Math::Matrix &T);


    void computeAdditionParams(const Array<Math::Vector> &sampleS, const Array<Math::Vector> &sampleLambda,
                               const Array<Math::Vector> &sampleSigma, Matrix &L, Matrix &K, Vector &n, Vector &e);
};


} // end Discrete

} // end Filters

#endif // D_DUOF_H
