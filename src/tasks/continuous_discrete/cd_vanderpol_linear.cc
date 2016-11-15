#include "cd_vanderpol_linear.h"
#include "src/math/constants.h"


namespace Tasks
{

namespace ContinuousDiscrete
{

using Math::Rand::gaussianVector;


VanDerPolLinear::VanDerPolLinear()
    : ContinuousDiscreteTask()
    , m_omega(0.1 * Math::Const::PI)
    , m_alpha(1.0)
    , m_beta(0.25)
{
    m_info->setName("Осциллятор Ван-дер-Поля");
    m_info->setType("Л-");

    m_dimY = 2;

    m_dimX     = 2;
    m_meanX    = Vector(m_dimX);
    m_meanX[0] = 10.0;
    m_meanX[1] = -3.0;

    m_dimV  = 2;
    m_meanV = Vector::Zero(m_dimV);

    m_dimW     = 2;
    m_meanW    = Vector(m_dimW);
    m_meanW[0] = 1.0;
    m_meanW[1] = 1.5;

    m_varX = Matrix::Zero(m_dimX, m_dimX);
    m_varX(0, 0) = 5.0;
    m_varX(1, 1) = 5.0;

    m_varV = Matrix::Identity(m_dimV, m_dimV);

    m_varW = Matrix::Identity(m_dimW, m_dimW);
    m_varW(0, 0) = 4.0;
    m_varW(1, 1) = 4.0;

    (*m_params)["Omega"] = m_omega;
    (*m_params)["Alpha"] = m_alpha;
    (*m_params)["Beta"]  = m_beta;
}

void VanDerPolLinear::loadParams()
{
    m_omega = m_params->at("Omega");
    m_alpha = m_params->at("Alpha");
    m_beta  = m_params->at("Beta");
}

Vector VanDerPolLinear::a(const Vector &x) const
{
    Vector res(m_dimX);

    res[0] = x[1];
    res[1] = -m_omega * m_omega * x[0] + m_alpha * x[1] - m_alpha * m_beta * x[0] * x[0] * x[1];

    return res;
}

Matrix VanDerPolLinear::B(const Vector &x) const
{
    Matrix res = Matrix::Zero(m_dimX, m_dimV);

    res(1, 1) = x[0];

    return res;
}

Vector VanDerPolLinear::tau(const Vector &m, const Matrix & /*D*/) const
{
    Vector res(m_dimX);

    res[0] = m[1];
    res[1] = -m_omega * m_omega * m[0] + m_alpha * m[1] - m_alpha * m_beta * m[0] * m[0] * m[1];

    return res;
}

Matrix VanDerPolLinear::Theta(const Vector &m, const Matrix & /*D*/) const
{
    Matrix b = B(m);

    return b * b.transpose();
}

Matrix VanDerPolLinear::A(const Vector &m, const Matrix & /*D*/) const
{
    Matrix res(m_dimX, m_dimX);

    res(0, 0) = 0.0;
    res(0, 1) = 1.0;
    res(1, 0) = -m_omega * m_omega - 2.0 * m_alpha * m_beta * m[0] * m[1];
    res(1, 1) = m_alpha * (1 - m_beta * m[0] * m[0]);

    return res;
}


Vector VanDerPolLinear::c(const Vector &x) const
{
    return x + gaussianVector(m_meanW, m_varW);
}

Vector VanDerPolLinear::h(const Vector &m, const Matrix & /* D*/) const
{
    return m + m_meanW;
}

Matrix VanDerPolLinear::G(const Vector & /*m*/, const Matrix & /*D*/) const
{
    Matrix res = Matrix::Zero(2, 2); // WARNING (size = ?)

    res(0, 0) = 1.0;

    return res;
}

Matrix VanDerPolLinear::F(const Vector & /*m*/, const Matrix &D) const
{
    return D + m_varW;
}


} // end Tasks::ContinuousDiscrete

} // end Tasks
