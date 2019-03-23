#include "cd_vanderpol_gauss.h"


namespace Tasks
{

namespace ContinuousDiscrete
{


VanDerPolGauss::VanDerPolGauss()
    : VanDerPolLinear()
{
    m_info->setType("Г-");
}

Vector VanDerPolGauss::tau(const Vector &m, const Matrix &D) const
{
    Vector res(m_dimX);

    res[0] = m[1];
    res[1] = -m_omega * m_omega * m[0] + m_alpha * m[1] -
             m_alpha * m_beta * (m[0] * m[0] * m[1] + m[1] * D(0, 0) + 2.0 * m[0] * D(0, 1));

    return res;
}

Matrix VanDerPolGauss::Theta(const Vector &m, const Matrix &D) const
{
    Matrix res = Matrix::Zero(2, 2); // WARNING (size = ?)

    res(1, 1) = m[0] * m[0] + D(0, 0);

    return res;
}

Matrix VanDerPolGauss::A(const Vector &m, const Matrix &D) const
{
    Matrix res(m_dimX, m_dimX);

    res(0, 0) = 0.0;
    res(0, 1) = 1.0;
    res(1, 0) = -m_omega * m_omega - 2.0 * m_alpha * m_beta * (m[0] * m[1] + D(0, 1));
    res(1, 1) = m_alpha - m_alpha * m_beta * (m[0] * m[0] + D(0, 0));

    return res;
}

Matrix VanDerPolGauss::G(const Vector & /*m*/, const Matrix & /*D*/, double measurementStep) const
{
    return Matrix::Identity(2, 2); // WARNING: size?
}

Matrix VanDerPolGauss::F(const Vector & /*m*/, const Matrix &D, double measurementStep) const
{
    Matrix res = D;

    res(0, 0) += 1.0 / measurementStep * m_varW(0, 0);
    res(1, 1) += 1.0 / measurementStep * m_varW(1, 1);
    return res;
}


} // end Tasks::ContinuousDiscrete

} // end Tasks
