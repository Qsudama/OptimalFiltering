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


protected:
    // TODO
    // Vector funcA(const Vector &x) const override;
    // Matrix funcB(const Vector &x) const override;
    // Vector funcC(const Vector &x) const override;
    // Matrix funcD(const Vector &x) const override;
    Matrix funcAA(const Vector &m, const Matrix &D) const override;
    Matrix funcG(const Vector &m, const Matrix &D) const override;
    Matrix funcQ(const Vector &m, const Matrix &D) const override;
};


} // end Tasks::Continuous

} // end Tasks


#endif // CONTINUOUS_VANDERPOL_GAUSS_H
