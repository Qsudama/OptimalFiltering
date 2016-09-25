#include "cd_vanderpol_linear.h"
#include "src/math/constants.h"


namespace Tasks
{

namespace ContinuousDiscrete
{

using Math::Rand::gaussianVector;


VanDerPolLinear::VanDerPolLinear()
    : ContinuousDiscreteTask()
    , omega(0.1 * Math::Const::PI)
    , alpha(1.0)
    , beta(0.25)
{
    m_info->setName("Осциллятор Ван-дер-Поля");
    m_info->setType("Л-");

    m_dimY = 2;

    mx[0] = 10.0;
    mx[1] = -3.0;

    mv = Vector::Zero(2);

    mw[0] = 1.0;
    mw[1] = 1.5;

    Dx(0, 0) = 5.0;
    Dx(1, 1) = 5.0;

    Dv = Matrix::Identity(2, 2);

    Dw(0, 0) = 4.0;
    Dw(1, 1) = 4.0;

    (*m_params)["Omega"] = omega;
    (*m_params)["Alpha"] = alpha;
    (*m_params)["Beta"]  = beta;
}

void VanDerPolLinear::loadParams()
{
    omega = m_params->at("Omega");
    alpha = m_params->at("Alpha");
    beta  = m_params->at("Beta");
}

Vector VanDerPolLinear::a(const Vector &x) const
{
    Vector aa(m_dimX);
    aa[0] = x[1];
    aa[1] = -omega * omega * x[0] + alpha * x[1] - alpha * beta * x[0] * x[0] * x[1];
    return aa;
}

Matrix VanDerPolLinear::B(const Vector &x) const
{
    Matrix B = Matrix::Zero(m_dimX, m_dimV);
    B(1, 1) = x[0];
    return B;
}

Vector VanDerPolLinear::tau(const Vector &m, const Matrix & /*D*/) const
{
    Vector tau(m_dimX);
    tau[0] = m[1];
    tau[1] = -omega * omega * m[0] + alpha * m[1] - alpha * beta * m[0] * m[0] * m[1];
    return tau;
}

Matrix VanDerPolLinear::Theta(const Vector &m, const Matrix & /*D*/) const
{
    Matrix b = B(m);
    return b * b.transpose();
}

Matrix VanDerPolLinear::A(const Vector &m, const Matrix & /*D*/) const
{
    Matrix aa(m_dimX, m_dimX);
    aa(0, 0) = 0.0;
    aa(0, 1) = 1.0;
    aa(1, 0) = -omega * omega - 2.0 * alpha * beta * m[0] * m[1];
    aa(1, 1) = alpha * (1 - beta * m[0] * m[0]);
    return aa;
}


Vector VanDerPolLinear::c(const Vector &x) const
{
    Vector w = gaussianVector(mw, Dw);
    return x + w;
}

Vector VanDerPolLinear::h(const Vector &m, const Matrix & /* D*/) const
{
    return m + mw;
}

Matrix VanDerPolLinear::G(const Vector & /*m*/, const Matrix & /*D*/) const
{
    Matrix g = Matrix::Zero(2, 2); // WARNING (size = ?)
    g(0, 0) = 1.0;
    return g;
}

Matrix VanDerPolLinear::F(const Vector & /*m*/, const Matrix &D) const
{
    return D + Dw;
}


} // end Tasks::ContinuousDiscrete

} // end Tasks
