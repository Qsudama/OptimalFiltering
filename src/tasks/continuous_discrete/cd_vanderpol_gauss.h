#ifndef CONTINUOUS_DISCRETE_VANDERPOL_GAUSS_H
#define CONTINUOUS_DISCRETE_VANDERPOL_GAUSS_H

#include "cd_vanderpol_linear.h"


namespace Tasks
{

namespace ContinuousDiscrete
{


class VanDerPolGauss : public VanDerPolLinear
{

public:
    VanDerPolGauss();


protected:
    Vector tau(const Vector &m, const Matrix &D) const override;
    Matrix Theta(const Vector &m, const Matrix &D) const override;
    Matrix A(const Vector &m, const Matrix &D) const override;
    Matrix G(const Vector &m, const Matrix &D) const override;
    Matrix F(const Vector &m, const Matrix &D) const override;
};


} // end Tasks::ContinuousDiscrete

} // end Tasks


#endif // CONTINUOUS_DISCRETE_VANDERPOL_GAUSS_H
