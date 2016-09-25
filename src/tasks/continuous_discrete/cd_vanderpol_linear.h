#ifndef CONTINUOUS_DISCRETE_VANDERPOL_LINEAR_H
#define CONTINUOUS_DISCRETE_VANDERPOL_LINEAR_H


#include "src/core/continuous_discrete_task.h"
#include "src/math/math.h"


namespace Tasks
{

namespace ContinuousDiscrete
{


class VanDerPolLinear : public Core::ContinuousDiscreteTask
{

public:
    VanDerPolLinear();


protected:
    Vector funcA(const Vector &x) const override;
    Matrix funcB(const Vector &x) const override;
    Vector funcTau(const Vector &m, const Matrix &D) const override;
    Matrix funcTheta(const Vector &m, const Matrix &D) const override;
    Matrix funcAA(const Vector &m, const Matrix &D) const override;
    Vector funcC(const Vector &x) const override;
    Vector funcH(const Vector &m, const Matrix &D) const override;
    Matrix funcG(const Vector &m, const Matrix &D) const override;
    Matrix funcF(const Vector &m, const Matrix &D) const override;

    void loadParams() override;


protected:
    double omega, alpha, beta;
};


} // end Tasks::ContinuousDiscrete

} // end Tasks


#endif // CONTINUOUS_DISCRETE_VANDERPOL_LINEAR_H
