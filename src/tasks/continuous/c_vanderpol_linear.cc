#include "c_vanderpol_linear.h"
#include "src/math/constants.h"


namespace Tasks
{

namespace Continuous
{

VanDerPolLinear::VanDerPolLinear()
    : ContinuousTask()
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
    Matrix b = Matrix::Zero(m_dimX, m_dimV);
    b(1, 1) = x[0];

    return b;
}


Vector VanDerPolLinear::c(const Vector &x) const
{
    return x;
}

Matrix VanDerPolLinear::D(const Vector & /*x*/) const
{
    return Matrix::Identity(m_dimY, m_dimW);
}

Matrix VanDerPolLinear::Q(const Vector &x, const Matrix & /*D*/) const
{
    Matrix b = B(x);
    return b * b.transpose();
}

Matrix VanDerPolLinear::S(const Vector & /*x*/, const Matrix & /*D*/) const
{
    return Matrix::Zero(m_dimX, m_dimY);
}

Matrix VanDerPolLinear::R(const Vector &x, const Matrix & /*D*/) const
{
    Matrix d = D(x);
    return d * d.transpose();
}

Matrix VanDerPolLinear::A(const Vector &m, const Matrix & /*D*/) const
{
    Matrix aa(m_dimX, m_dimX);

    aa(0, 0) = 0.0;
    aa(0, 1) = 1.0;
    aa(1, 0) = -omega * omega - 2.0 * alpha * beta * m[0] * m[1];
    aa(1, 1) = alpha * (1.0 - beta * m[0] * m[0]);

    return aa;
}

Matrix VanDerPolLinear::G(const Vector & /*m*/, const Matrix & /*D*/) const
{
    return Matrix::Identity(m_dimY, m_dimX);
}


} // end Tasks::Continuous

} // end Tasks
