#include "d_vanderpol_gauss.h"
#include "src/math/convert.h"


namespace Tasks
{

namespace Discrete
{


VanDerPolGauss::VanDerPolGauss()
    : DiscreteTask()
{
    m_info->setName("Осциллятор Ван-дер-Поля");
    m_info->setType("Г-");

    m_dimY = 2;

    m_dimX      = 2;
    m_meanX0    = Vector(m_dimX);
    m_meanX0[0] = 10.0;
    m_meanX0[1] = -3.0;

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

void VanDerPolGauss::loadParams() {
    deltaT = (*m_params).at("Δt");
}

Vector VanDerPolGauss::a(const Vector &x) const {
    Vector v = m_normalRand(m_meanV, m_varV);
    return a(x, v);
}

Vector VanDerPolGauss::a(const Vector &x, const Vector &v) const {
    Vector dx(m_dimX);

    dx[0] = x[0] + deltaT * x[1];
    dx[1] = x[1] + deltaT * (-pow(m_omega, 2) * x[0] + m_alpha * x[1] * (1 - m_beta * pow(x[0], 2)) + x[0] * v[0] );

    return dx;
}

Vector VanDerPolGauss::a_m(const Vector &x) const {
    Vector v = m_meanV;
    return a(x, v);
}

Vector VanDerPolGauss::b(const Vector &x) const {
    Vector w = m_normalRand(m_meanW, m_varW); // СКО, а не Дисперсия
    return b(x, w);
}

Vector VanDerPolGauss::b(const Vector &x, const Vector &w) const {
    Vector res(m_dimY);

    res[0] = pow(x[0], 2) + w[0];
    res[1] = x[1] + w[1];

    return res;
}

Vector VanDerPolGauss::b_m(const Vector &x) const {
    Vector w = m_meanW;
    return b(x, w);
}

Vector VanDerPolGauss::tau(const Vector &z, const Matrix & D) const {
    Vector tau(m_dimX);

    tau[0] = z[0] + deltaT * z[1];
    tau[1] = z[1] + deltaT * (-pow(m_omega, 2) * z[0] + m_alpha * z[1] - m_alpha * m_beta * (pow(z[0], 2) * z[1] + 2.0 * z[0] * D(0, 1) + z[1] * D(0, 0)));

    return a(z);
}

Matrix VanDerPolGauss::Theta(const Vector &z, const Matrix &P) const {
    Vector tau = VanDerPolGauss::tau(z, P);

    return mXi(z, P) - tau * tau.transpose();
}

Vector VanDerPolGauss::h(const Vector &m, const Matrix & D) const {
    Vector h(m_dimX);

    h[0] = pow(m[0], 2) + D(0, 0) + m_meanW[0];
    h[1] = m[1] + m_meanW[1];

    return h;
}

Matrix VanDerPolGauss::G(const Vector &m, const Matrix & /*D*/) const {
    Matrix G = Matrix::Zero(m_dimX, m_dimW);

    G(0, 0) = 2.0 * m[0];
    G(1, 1) = 1.0;

    return G;
}

Matrix VanDerPolGauss::F(const Vector &m, const Matrix &D) const {
    Matrix F = Matrix::Zero(m_dimX, m_dimX);

    F(0, 0) = 4.0 * pow(m[0], 2) * D(0, 0) + 2.0 * pow(D(0, 0), 2);
    F(0, 1) = 2.0 * m[0] * D(0, 1);
    F(1, 0) = 2.0 * m[0] * D(1, 0);
    F(1, 1) = D(1, 1);

    return F;
}



double VanDerPolGauss::moment(int k, int l, const Vector &m, const Matrix &D) const {
    if (k == 0 && l == 0) {
        return 1.0;
    } else if (k < 0 || l < 0) {
        return 0.0;
    }

    double arg2 = k > 1 ? (k - 1) * D(0, 0) * moment(k - 2, l, m, D) : 0.0;
    double arg3 = (l > 0 && k > 0) ? l * D(0, 1) * moment(k - 1, l - 1, m, D) : 0.0;

    return m[0] * moment(k - 1, l, m, D) + arg2 + arg3;
}

Matrix VanDerPolGauss::mXi(const Vector &m, const Matrix &D) const {
    double a11 = moment(1, 1, m, D);
    double a20 = moment(2, 0, m, D);
    double a02 = moment(0, 2, m, D);
    double a22 = moment(2, 2, m, D);
    double a31 = moment(3, 1, m, D);
    double a42 = moment(4, 2, m, D);

    Matrix mXi = Matrix::Zero(m_dimX, m_dimX);

    mXi(0, 0) = a20 + 2.0 * deltaT * a11 + pow(deltaT, 2) * a02;
    mXi(0, 1) = -a31 * m_alpha * m_beta * deltaT - a22 * m_alpha * m_beta * pow(deltaT, 2) + a02 * (m_alpha * pow(deltaT, 2) + deltaT) - a20 * deltaT * pow(m_omega, 2) + a11 * (m_alpha * deltaT - pow(deltaT, 2) * pow(m_omega, 2) + 1);
    mXi(1, 0) = mXi(0, 1);
    mXi(1, 1) = a42 * pow(deltaT, 2) * pow(m_alpha, 2) * pow(m_beta, 2) - a22 * deltaT * m_alpha * m_beta *(deltaT * m_alpha + 2.0) + a31 * (2.0 * pow(deltaT, 2) * pow(m_omega, 2) * m_alpha * m_beta) + a20 * pow(deltaT, 2) * pow(m_omega, 4) + a02 * (pow(deltaT, 2) * pow(m_alpha, 2) + 2.0 * deltaT * m_alpha + 1.0) - a11 * 2.0 * deltaT * pow(m_omega, 2) * (deltaT * m_alpha + 1.0);

    return mXi;
}


Matrix VanDerPolGauss::dadx(const Vector &/*x*/) const
{
    return Matrix::Zero(m_dimX, m_dimV);
}

Matrix VanDerPolGauss::dadv(const Vector & /*x*/) const
{
    return Matrix::Zero(m_dimX, m_dimV);
}

Matrix VanDerPolGauss::dbdx(const Vector &/*x*/) const
{
    return Matrix::Zero(m_dimX, m_dimV);
}

Matrix VanDerPolGauss::dbdw(const Vector &/*x*/) const
{
    return Matrix::Zero(m_dimX, m_dimV);
}


} // end Tasks::Discrete

} // end Tasks
