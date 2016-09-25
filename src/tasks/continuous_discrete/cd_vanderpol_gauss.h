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
    Vector funcTau(const Vector &m, const Matrix &D) const override;
    Matrix funcTheta(const Vector &m, const Matrix &D) const override;
    Matrix funcAA(const Vector &m, const Matrix &D) const override;
    Matrix funcG(const Vector &m, const Matrix &D) const override;
    Matrix funcF(const Vector &m, const Matrix &D) const override;
};


} // end Tasks::ContinuousDiscrete

} // end Tasks


#endif // CONTINUOUS_DISCRETE_VANDERPOL_GAUSS_H
