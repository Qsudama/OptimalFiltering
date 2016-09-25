#include "continuous_discrete_task.h"


namespace Core
{


ContinuousDiscreteTask::ContinuousDiscreteTask()
    : Task()
{
}

Vector ContinuousDiscreteTask::a(const Vector &x) const
{
    return funcA(x);
}

Matrix ContinuousDiscreteTask::B(const Vector &x) const
{
    return funcB(x);
}

Vector ContinuousDiscreteTask::tau(const Vector &m, const Matrix &D) const
{
    return funcTau(m, D);
}

Matrix ContinuousDiscreteTask::Theta(const Vector &m, const Matrix &D) const
{
    return funcTheta(m, D);
}

Matrix ContinuousDiscreteTask::A(const Vector &m, const Matrix &D) const
{
    return funcAA(m, D);
}

Vector ContinuousDiscreteTask::c(const Vector &x) const
{
    return funcC(x);
}

Vector ContinuousDiscreteTask::h(const Vector &m, const Matrix &D) const
{
    return funcH(m, D);
}

Matrix ContinuousDiscreteTask::G(const Vector &m, const Matrix &D) const
{
    return funcG(m, D);
}

Matrix ContinuousDiscreteTask::F(const Vector &m, const Matrix &D) const
{
    return funcF(m, D);
}


} // end Core
