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

Matrix VanDerPolGauss::A(const Vector &m, const Matrix &D) const
{
    Matrix res(m_dimX, m_dimX);

    res(0, 0) = 0.0;
    res(0, 1) = 1.0;
    res(1, 0) = -m_omega * m_omega - 2.0 * m_alpha * m_beta * (m[0] * m[1] + D(0, 1));
    res(1, 1) = m_alpha - m_alpha * m_beta * (m[0] * m[0] + D(0, 0));

    return res;
}

Matrix VanDerPolGauss::G(const Vector & /*m*/, const Matrix & /*D*/) const
{
    // WARNING G(0,0) = 2 * m[0] ??
    return Matrix::Identity(m_dimY, m_dimX);
}

Matrix VanDerPolGauss::Q(const Vector &m, const Matrix &D) const
{
    Matrix b   = B(m);
    Matrix res = b * b.transpose();

    res(1, 1) += D(1, 1);

    return res;
}


} // end Tasks::Continuous

} // end Tasks
