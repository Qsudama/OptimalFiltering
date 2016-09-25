#include "continuous_task.h"


namespace Core
{

using Math::LinAlg::PinvSVD;


ContinuousTask::ContinuousTask()
    : Task()
{
}

ContinuousTask::~ContinuousTask()
{
}

Vector ContinuousTask::a(const Vector &x) const
{
    assert(x.size() == m_dimX);
    return funcA(x);
}

Matrix ContinuousTask::B(const Vector &x) const
{
    assert(x.size() == m_dimX);
    return funcB(x);
}

Vector ContinuousTask::c(const Vector &x) const
{
    assert(x.size() == m_dimX);
    return funcC(x);
}

Matrix ContinuousTask::D(const Vector &x) const
{
    assert(x.size() == m_dimX);
    return funcD(x);
}

Matrix ContinuousTask::A(const Vector &m, const Matrix &D) const
{
    assert(m.size() == m_dimX);
    return funcAA(m, D);
}

Matrix ContinuousTask::G(const Vector &m, const Matrix &D) const
{
    assert(m.size() == m_dimX);
    return funcG(m, D);
}

Matrix ContinuousTask::K(const Vector &m, const Matrix &D) const
{
    assert(m.size() == m_dimX);
    assert(D.rows() == m_dimX);
    assert(D.cols() == m_dimX);
    return (D * funcG(m, D).transpose() + funcS(m, D)) * PinvSVD(funcR(m, D));
}

Matrix ContinuousTask::Psy(const Vector &m, const Matrix &D) const
{
    assert(m.size() == m_dimX);
    assert(D.rows() == m_dimX);
    assert(D.cols() == m_dimX);
    Matrix k = K(m, D);
    Matrix a = funcAA(m, D);
    return a * D + D * a.transpose() + funcQ(m, D) - k * funcR(m, D) * k.transpose();
}


} // end Core
