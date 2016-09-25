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

    Vector a(const Vector &x) const override;
    Matrix B(const Vector &x) const override;
    Vector c(const Vector &x) const override;
    Matrix D(const Vector &x) const override;
    Matrix A(const Vector &m, const Matrix &D) const override;
    Matrix G(const Vector &m, const Matrix &D) const override;


protected:
    Matrix Q(const Vector &m, const Matrix &D) const override;
    Matrix S(const Vector &m, const Matrix &D) const override;
    Matrix R(const Vector &m, const Matrix &D) const override;

    void loadParams() override;


protected:
    double omega, alpha, beta;
};


} // end Tasks::Continuous

} // end Tasks


#endif // CONTINUOUS_VANDERPOL_LINEAR_H
