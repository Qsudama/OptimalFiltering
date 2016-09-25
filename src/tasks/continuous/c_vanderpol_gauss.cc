#include "c_vanderpol_gauss.h"


namespace Tasks
{

namespace Continuous
{


VanDerPolGauss::VanDerPolGauss()
    : VanDerPolLinear()
{
    m_info->setType("Г-");
}

Matrix VanDerPolGauss::funcAA(const Vector &m, const Matrix &D) const
{
    Matrix A(m_dimX, m_dimX);
    A(0, 0) = 0.0;
    A(0, 1) = 1.0;
    A(1, 0) = -omega * omega - 2.0 * alpha * beta * (m[0] * m[1] + D(0, 1));
    A(1, 1) = alpha - alpha * beta * (m[0] * m[0] + D(0, 0));
    return A;
}

Matrix VanDerPolGauss::funcG(const Vector & /*m*/, const Matrix & /*D*/) const
{
    // WARNING G(0,0) = 2 * m[0] ??
    return Matrix::Identity(m_dimY, m_dimX);
}

Matrix VanDerPolGauss::funcQ(const Vector &m, const Matrix &D) const
{
    Matrix b = funcB(m);
    Matrix q = b * b.transpose();
    q(1, 1) += D(1, 1);
    return q;
}


} // end Tasks::Continuous

} // end Tasks
