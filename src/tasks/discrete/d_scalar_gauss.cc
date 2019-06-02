#include "d_scalar_gauss.h"
#include "src/math/convert.h"


namespace Tasks
{

namespace Discrete
{


using Math::Convert::DegToRad;


ScalarGauss::ScalarGauss()
    : DiscreteTask()
{
    m_info->setName("Тестовый скалярный пример");
    m_info->setType("Г-");

    m_dimY = 1;

    m_dimX      = 1;
    m_meanX0    = Vector(m_dimX);
    m_meanX0[0] = 0;
    m_varX0 = Matrix::Zero(m_dimX, m_dimX);
    m_varX0(0, 0) = 1.0;

    m_dimV  = 1;
    m_meanV = Vector::Zero(m_dimV);
    m_varV = Matrix::Zero(m_dimV, m_dimV);
    m_varV(0, 0) = 1.0;

    m_dimW  = 1;
    m_meanW = Vector::Zero(m_dimW);
    m_varW = Matrix::Zero(m_dimW, m_dimW);
    m_varW(0, 0) = 1.0;

    (*m_consts)["a"] = aa;
    (*m_consts)["c"] = cc;
    (*m_consts)["d"] = dd;

    (*m_params)["b"] = bb;
    (*m_params)["e"] = ee;
}

void ScalarGauss::loadParams()
{
    bb = (*m_params).at("b");
    ee = (*m_params).at("e");
}

Vector ScalarGauss::a(const Vector &x) const
{
    Vector dx(m_dimX);
    Vector v = m_normalRand(m_meanV, m_varV);

    dx[0] = aa * x[0] + bb * x[0] *  x[0] * v[0];

    return dx;
}

Vector ScalarGauss::a(const Vector &x, const Vector &v) const
{
    Vector dx(m_dimX);

    dx[0] = aa * x[0] + bb * x[0] *  x[0] * v[0];

    return dx;
}

Vector ScalarGauss::a_m(const Vector &x) const
{
    Vector dx(m_dimX);
    Vector v = m_meanV;

    dx[0] = aa * x[0] + bb * x[0] *  x[0] * v[0];

    return dx;
}

Vector ScalarGauss::b(const Vector &x) const
{
    Vector w = m_normalRand(m_meanW, m_varW); // СКО, а не Дисперсия
    Vector res(m_dimY);

    res[0] = cc * x[0] + dd * x[0] * x[0] + ee * w[0];

    return res;
}

Vector ScalarGauss::b(const Vector &x, const Vector &w) const
{
    Vector res(m_dimY);

    res[0] = cc * x[0] + dd * x[0] * x[0] + ee * w[0];

    return res;
}

Vector ScalarGauss::b_m(const Vector &x) const
{
    Vector w = m_meanW;
    Vector res(m_dimY);

    res[0] = cc * x[0] + dd * x[0] * x[0] + ee * w[0];

    return res;
}

Vector ScalarGauss::tau(const Vector &z, const Matrix & /*D*/) const
{
    Vector res(m_dimX);
    res[0] = aa * z[0];
    return res;
}

Matrix ScalarGauss::Theta(const Vector &z, const Matrix &P) const
{
    Matrix res(m_dimX, m_dimX);
    res = aa * aa * P + bb * bb * (z * z * z * z + 6.0 * z * z * P + 3.0 * P * P);

    return res;

    // TODO V = 0 --> m_meanV = 0, m_varV = 0 --> Av = 0....
}

Vector ScalarGauss::h(const Vector &m, const Matrix &D) const
{
    Vector res(m_dimY);

    res[0] = cc * m[0] + dd * (m[0] * m[0] + D(0, 0));

    return res;
}

Matrix ScalarGauss::G(const Vector &m, const Matrix &/*D*/) const
{
    Matrix res(m_dimX, m_dimX);

    res(0, 0) = cc + 2.0 * dd * m[0];

    return res;
}

Matrix ScalarGauss::F(const Vector &m, const Matrix &D) const
{
    Matrix res(m_dimX, m_dimX);

    res(0, 0) = cc * cc * D(0, 0) + 4.0 * cc * dd * m[0] * D(0, 0) + dd * dd * (4.0 * m[0] * m[0] * D(0, 0) + 2.0 * D(0, 0) * D(0, 0)) + ee * ee;

    return res;
}


Matrix ScalarGauss::dadx(const Vector &/*x*/) const
{
    return Matrix::Zero(m_dimX, m_dimV);
}

Matrix ScalarGauss::dadv(const Vector & /*x*/) const
{
    return Matrix::Zero(m_dimX, m_dimV);
}

Matrix ScalarGauss::dbdx(const Vector &/*x*/) const
{
    return Matrix::Zero(m_dimX, m_dimV);
}

Matrix ScalarGauss::dbdw(const Vector &/*x*/) const
{
    return Matrix::Zero(m_dimX, m_dimV);
}


} // end Tasks::Discrete

} // end Tasks
