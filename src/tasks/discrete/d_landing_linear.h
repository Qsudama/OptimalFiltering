#ifndef DISCRETE_LANDING_LINEAR_H
#define DISCRETE_LANDING_LINEAR_H

#include "src/core/discrete_task.h"
#include "src/math/math.h"


namespace Tasks
{

namespace Discrete
{


class LandingLinear : public Core::DiscreteTask
{

public:
    LandingLinear();

    Vector a(const Vector &x) const override;
    Vector b(const Vector &x) const override;
    Vector tau(const Vector &z, const Matrix &P) const override;
    Matrix Theta(const Vector &z, const Matrix &P) const override;
    Vector h(const Vector &m, const Matrix &D) const override;
    Matrix G(const Vector &m, const Matrix &D) const override;
    Matrix F(const Vector &m, const Matrix &D) const override;


protected:
    Matrix dadx(const Vector &x) const override;
    Matrix dadv(const Vector &x) const override;
    Matrix dbdx(const Vector &x) const override;
    Matrix dbdw(const Vector &x) const override;

    void loadParams() override;

    double k(double t) const;


protected:
    double TT;

    static constexpr double KB = 0.3;
    static constexpr double BB = 0.09;
    static constexpr double CC = 0.043333333333333333333333333333333333333333333333333;
    static constexpr double GG = 3.711E-3;
    static constexpr double RR = 3390.0;
};


} // end Tasks::

} // end Tasks


#endif // DISCRETE_LANDING_LINEAR_H
