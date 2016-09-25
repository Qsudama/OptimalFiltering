#ifndef CONTINUOUS_VANDERPOL_GAUSS_H
#define CONTINUOUS_VANDERPOL_GAUSS_H

#include "c_vanderpol_linear.h"


namespace Tasks
{

namespace Continuous
{


class VanDerPolGauss : public VanDerPolLinear
{

public:
    VanDerPolGauss();

    Matrix A(const Vector &m, const Matrix &D) const override;
    Matrix G(const Vector &m, const Matrix &D) const override;

protected:
    Matrix Q(const Vector &m, const Matrix &D) const override;
};


} // end Tasks::Continuous

} // end Tasks


#endif // CONTINUOUS_VANDERPOL_GAUSS_H
