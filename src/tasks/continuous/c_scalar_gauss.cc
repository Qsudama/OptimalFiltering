#include "c_scalar_gauss.h"

namespace Tasks
{

namespace Continuous
{


ScalarGauss::ScalarGauss()
    :ScalarLinear()
{
    m_info->setType("Г-");
}

Matrix ScalarGauss::A(const Vector &m, const Matrix &D) const
{
    Matrix res(m_dimX, m_dimX);

    res(0, 0) = a1 + 3 * a3 * m[0] * m[0] + 3 * a3 * D(0, 0);

    return res;
}

Matrix ScalarGauss::G(const Vector & m, const Matrix & /*D*/) const
{
    Matrix res(m_dimX, m_dimX);

    res(0, 0) = c1 + 2 * c2 * m[0];

    return res;
}

Matrix ScalarGauss::Q(const Vector &m, const Matrix &D) const
{
    Matrix b   = B(m);
    Matrix res = b * b.transpose();

    res(0, 0) += 0.0;  // b1 * b1 * D(0, 0)

    return res;
}

} // end Tasks::Continuous

} // end Tasks
