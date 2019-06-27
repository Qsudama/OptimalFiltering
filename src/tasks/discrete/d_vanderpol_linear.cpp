#include "d_vanderpol_linear.h"
#include "src/math/convert.h"


namespace Tasks
{

namespace Discrete
{


VanDerPolLinear::VanDerPolLinear()
    : DiscreteTask()
{
    m_info->setName("Осциллятор Ван-дер-Поля");
    m_info->setType("Л-");

    m_dimY = 2;

    m_dimX      = 2;
    m_meanX0    = Vector(m_dimX);
    m_meanX0[0] = 2.0;
    m_meanX0[1] = 0.0;

    m_dimV  = 1;
    m_meanV = Vector::Zero(m_dimV);

    m_dimW  = 2;
    m_meanW = Vector::Zero(m_dimW);
    m_meanW[0] = 0.0;
    m_meanW[1] = 0.0;


    m_varX0 = Matrix::Zero(m_dimX, m_dimX);
    m_varX0(0, 0) = 0.15;
    m_varX0(1, 1) = 0.15;

    m_varV = Matrix::Zero(m_dimV, m_dimV);

    m_varW = Matrix::Zero(m_dimW, m_dimW);
    m_varW(0, 0) = 1.0;
    m_varW(1, 1) = 1.0;

    (*m_consts)["Omega"]   = m_omega;
    (*m_consts)["Alpha"] = m_alpha;
    (*m_consts)["Beta"]    = m_beta;

    (*m_params)["Δt"] = deltaT;
}

void VanDerPolLinear::loadParams()
{
    deltaT = (*m_params).at("Δt");
}

Vector VanDerPolLinear::a(const Vector &x) const
{
    Vector v = m_normalRand(m_meanV, m_varV);
    return a(x, v);
}

Vector VanDerPolLinear::a(const Vector &x, const Vector &v) const
{
    Vector dx(m_dimX);

    dx[0] = x[0] + deltaT * x[1];
    dx[1] = x[1] + deltaT * (-pow(m_omega, 2) * x[0] + m_alpha * x[1] * (1 - m_beta * pow(x[0], 2)) + x[0] * v[0] );

    return dx;
}

Vector VanDerPolLinear::a_m(const Vector &x) const
{
    Vector v = m_meanV;
    return a(x, v);
}

Vector VanDerPolLinear::b(const Vector &x) const
{
    Vector w = m_normalRand(m_meanW, m_varW); // СКО, а не Дисперсия
    return b(x, w);
}

Vector VanDerPolLinear::b(const Vector &x, const Vector &w) const
{
    Vector res(m_dimY);

    res[0] = pow(x[0], 2) + w[0];
    res[1] = x[1] + w[1];

    return res;
}

Vector VanDerPolLinear::b_m(const Vector &x) const
{
    Vector w = m_meanW;
    return b(x, w);
}

Vector VanDerPolLinear::tau(const Vector &z, const Matrix & /*D*/) const
{
    return a(z, m_meanV);
}

Matrix VanDerPolLinear::Theta(const Vector &z, const Matrix &P) const
{
    Matrix Ax = dadx(z);
    Matrix Av = dadv(z);

    return Ax * P * Ax.transpose() + Av * m_varV * Av.transpose();
}

Vector VanDerPolLinear::h(const Vector &m, const Matrix & /* D*/) const
{
    Vector w = m_meanW;
    return b(m, w);
}

Matrix VanDerPolLinear::G(const Vector &m, const Matrix & /*D*/) const
{
    return dbdx(m);
}

Matrix VanDerPolLinear::F(const Vector &m, const Matrix &D) const
{
    Matrix Bx = dbdx(m);
    Matrix Bw = dbdw(m);

    return Bx * D * Bx.transpose() + Bw * m_varW * Bw.transpose();
}

Matrix VanDerPolLinear::dadx(const Vector &x) const
{
    Matrix res(m_dimX, m_dimX);

    res(0, 0) = 1.0;
    res(0, 1) = deltaT;

    res(1, 0) = - deltaT * pow(m_omega, 2) - 2 * deltaT * m_alpha * m_beta *x[0] * x[1];
    res(1, 1) = 1 + deltaT * m_alpha - deltaT * m_alpha * m_beta * pow(x[0], 2);

    return res;
}

Matrix VanDerPolLinear::dadv(const Vector & x) const
{
    Matrix res(m_dimX, m_dimV);

    res(0, 0) = 0.0;
    res(1, 0) = x[0];

    return res;
}

Matrix VanDerPolLinear::dbdx(const Vector &x) const
{
    Matrix res(m_dimY, m_dimX);

    res(0, 0) = 2.0 * x[0];
    res(0, 1) = 0.0;

    res(1, 0) = 0.0;
    res(1, 1) = 1.0;

    return res;
}

Matrix VanDerPolLinear::dbdw(const Vector &x) const
{
    Matrix res(m_dimY, m_dimW);

    res(0, 0) = 1.0;
    res(0, 1) = 0.0;

    res(1, 0) = 0.0;
    res(1, 1) = 1.0;

    return res;
}


} // end Tasks::Discrete

} // end Tasks
