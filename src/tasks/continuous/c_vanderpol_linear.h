#ifndef CONTINUOUS_VANDERPOL_LINEAR_H
#define CONTINUOUS_VANDERPOL_LINEAR_H

#include "src/core/continuous_task.h"
#include "src/math/math.h"


namespace Tasks
{

namespace Continuous
{


class VanDerPolLinear : public Core::ContinuousTask
{

public:
    VanDerPolLinear();


protected:
    Vector funcA(const Vector &x) const override;
    Matrix funcB(const Vector &x) const override;
    Vector funcC(const Vector &x) const override;
    Matrix funcD(const Vector &x) const override;
    Matrix funcAA(const Vector &m, const Matrix &D) const override;
    Matrix funcG(const Vector &m, const Matrix &D) const override;
    Matrix funcQ(const Vector &m, const Matrix &D) const override;
    Matrix funcS(const Vector &m, const Matrix &D) const override;
    Matrix funcR(const Vector &m, const Matrix &D) const override;

    void loadParams() override;


protected:
    double omega, alpha, beta;
};


} // end Tasks::Continuous

} // end Tasks


#endif // CONTINUOUS_VANDERPOL_LINEAR_H
