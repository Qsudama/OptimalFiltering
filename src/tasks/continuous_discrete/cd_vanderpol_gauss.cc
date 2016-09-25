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

Vector VanDerPolGauss::funcTau(const Vector &m, const Matrix &D) const
{
    Vector tau(m_dimX);
    tau[0] = m[1];
    tau[1] = -omega * omega * m[0] + alpha * m[1] -
             alpha * beta * (m[0] * m[0] * m[1] + m[1] * D(0, 0) + 2.0 * m[0] * D(0, 1));
    return tau;
}

Matrix VanDerPolGauss::funcTheta(const Vector &m, const Matrix &D) const
{
    Matrix theta = Matrix::Zero(2, 2); // WARNING (size = ?)
    theta(1, 1) = m[0] * m[0] + D(0, 0);
    return theta;
}

Matrix VanDerPolGauss::funcAA(const Vector &m, const Matrix &D) const
{
    Matrix a(m_dimX, m_dimX);
    a(0, 0) = 0.0;
    a(0, 1) = 1.0;
    a(1, 0) = -omega * omega - 2.0 * alpha * beta * (m[0] * m[1] + D(0, 1));
    a(1, 1) = alpha - alpha * beta * (m[0] * m[0] + D(0, 0));
    return a;
}

Matrix VanDerPolGauss::funcG(const Vector & /*m*/, const Matrix & /*D*/) const
{
    return Matrix::Identity(2, 2);
}

Matrix VanDerPolGauss::funcF(const Vector & /*m*/, const Matrix &D) const
{
    Matrix f = D;
    f(0, 0) += Dw(0, 0);
    f(1, 1) += Dw(1, 1);
    return f;
}


} // end Tasks::ContinuousDiscrete

} // end Tasks
