#ifndef CONTINUOUS_LANDING_GAUSS_H
#define CONTINUOUS_LANDING_GAUSS_H

#include "c_landing_linear.h"


namespace Tasks
{

namespace Continuous
{


class LandingGauss : public LandingLinear
{
public:
    LandingGauss();


protected:
    // TODO :
    // Vector funcA(const Vector &x) const override;
    // Matrix funcB(const Vector &x) const override;
    // Vector funcC(const Vector &x) const override;
    // Matrix funcD(const Vector &x) const override;
    // Matrix funcAA(const Vector &m, const Matrix &D) const override;
    // Matrix funcG(const Vector &m, const Matrix &D) const override;
    // Matrix funcQ(const Vector &m, const Matrix &D) const override;
    // Matrix funcS(const Vector &m, const Matrix &D) const override;
    // Matrix funcR(const Vector &m, const Matrix &D) const override;
};


} // end Task::Continuous

} // end Continuous


#endif // CONTINUOUS_LANDING_GAUSS_H
