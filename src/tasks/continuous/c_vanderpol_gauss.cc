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
    Matrix aa(m_dimX, m_dimX);

    aa(0, 0) = 0.0;
    aa(0, 1) = 1.0;
    aa(1, 0) = -omega * omega - 2.0 * alpha * beta * (m[0] * m[1] + D(0, 1));
    aa(1, 1) = alpha - alpha * beta * (m[0] * m[0] + D(0, 0));

    return aa;
}

Matrix VanDerPolGauss::G(const Vector & /*m*/, const Matrix & /*D*/) const
{
    // WARNING G(0,0) = 2 * m[0] ??
    return Matrix::Identity(m_dimY, m_dimX);
}

Matrix VanDerPolGauss::Q(const Vector &m, const Matrix &D) const
{
    Matrix b = B(m);
    Matrix q = b * b.transpose();
    q(1, 1) += D(1, 1);

    return q;
}


} // end Tasks::Continuous

} // end Tasks
