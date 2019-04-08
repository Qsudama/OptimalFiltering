#include "ld_vanderpol_rejection_linear.h"
#include "src/math/constants.h"


namespace Tasks
{

namespace LogicDynamic
{

//int count_k = 0;

VanDerPolRejectionLinear::VanDerPolRejectionLinear()
    : LogicDynamicTask()
    , m_e(0.0)
    , m_cI2(10)
    , m_omega(0.1 * Math::Const::PI)
    , m_alpha(2.0)
    , m_beta(1.0)
    , m_gamma(1.0)
    , m_deltaT(0.0007)
    , countIInTask(1)
{
    m_info->setName("Осциллятор Ван-дер-Поля");
    m_info->setType("Л-");

    m_dimY = 1;
    m_dimX = 2;
    m_dimV = 1;
    m_dimW = 1;
    m_dimE = 1;

    m_meanX0 = Vector(m_dimX);
    m_meanX0[0] = 2.0;
    m_meanX0[1] = 0.0;

    m_varX0 = Matrix::Zero(m_dimX, m_dimX);
    m_varX0(0, 0) = 0.15;
    m_varX0(1, 1) = 0.15;


    m_meanV = Vector::Zero(m_dimV);

    m_meanW    = Vector(m_dimW);
    m_meanW[0] = 0.0;
//    m_meanW[1] = 0.0;


    m_varV = Matrix::Identity(m_dimV, m_dimV);
    m_varV(0, 0) = m_deltaT;

    m_varW = Matrix::Identity(m_dimW, m_dimW);
    m_varW(0, 0) = 1.0;
//    m_varW(1, 1) = 1.0;

    m_E = Matrix::Identity(m_dimE, m_dimE);
    m_E(0, 0) = /*m_E(1, 1) =*/ 1.0;

    (*m_params)["Omega"] = m_omega;
    (*m_params)["Alpha"] = m_alpha;
    (*m_params)["Beta"] = m_beta;
    (*m_params)["Gamma"] = m_gamma;
    (*m_params)["Delta*t"] = m_deltaT;
    (*m_params)["a1"] = A1;
    (*m_params)["a2"] = A2;
    (*m_params)["с(2)"] = m_cI2;
    (*m_params)["e"] = m_e;
    (*m_params)["Кол-во режимов I"] = countIInTask;
}

void VanDerPolRejectionLinear::loadParams()
{
    m_omega  = m_params->at("Omega");
    m_alpha  = m_params->at("Alpha");
    m_beta   = m_params->at("Beta");
    m_gamma  = m_params->at("Gamma");
    m_deltaT = m_params->at("Delta*t");
    A1       = m_params->at("a1");
    A2       = m_params->at("a2");
    m_cI2    = m_params->at("с(2)");
    m_e      = m_params->at("e");
    countI   = m_params->at("Кол-во режимов I");
}

double VanDerPolRejectionLinear::C(int i) const
{
    if (i == 1) {
        return 1.0;
    } else {
        return m_cI2;
    }
}

Vector VanDerPolRejectionLinear::a(int /*i*/, const Vector &x) const
{
    Vector dx(m_dimX);

    Vector zer = Vector::Zero(1);
    Matrix E = m_deltaT * m_E;
    Vector v = m_normalRand(zer, E);

    dx[0] = x[0] + m_deltaT * x[1];
    dx[1] = x[1] + m_deltaT * (-1 * m_omega * m_omega * x[0] + m_alpha * x[1] * (1 - m_beta * x[0] * x[0]) + m_gamma * x[0] * v[0]);

    return dx;
}

Vector VanDerPolRejectionLinear::b(int i, const Vector &x) const
{
    Vector w  = m_normalRand(m_meanW, m_varW);
    Vector res = Vector::Zero(m_dimY);
    double c = C(i);

    res[0] = A1 * x[0] * x[0] + A2 * x[1] + c * w[0];

    return res;
}


double VanDerPolRejectionLinear::A(int i, int l) const
{
    double e = m_e;
    double p = 1 - e;

    Matrix A(2, 2);
    A << p, p, e, e;

    return A(i - 1, l - 1);
}

double VanDerPolRejectionLinear::nu(int i, int l, const Vector &/*m*/, const Matrix &/*D*/) const {
    return A(i, l);
}

Vector VanDerPolRejectionLinear::tau(int i, int l, const Vector &m, const Matrix &/*D*/) const
{
    return A(i, l) * a(i, m);
}

Matrix VanDerPolRejectionLinear::Theta(int i, int l, const Vector &m, const Matrix &D) const
{
    Matrix Ax = dadx(i, m);
    Matrix Av = dadv(i, m);
    Vector a  = this->a(i, m);

    return A(i, l) * (Ax * D * Ax.transpose() + Av * m_varV * Av.transpose() + a * a.transpose());
}

Vector VanDerPolRejectionLinear::h(int i, const Vector &m, const Matrix & /* D*/) const
{
    return b(i, m);
}

Matrix VanDerPolRejectionLinear::G(int i, const Vector &m, const Matrix &D) const
{
    Matrix Bx = dbdx(i, m);
    Vector b  = this->b(i, m);

    return D * Bx.transpose() + m * b.transpose();
}

Matrix VanDerPolRejectionLinear::F(int i, const Vector &m, const Matrix &D) const
{
    Matrix Bx = dbdx(i, m);
    Matrix Bw = dbdw(i, m);
    Vector b  = this->b(i, m);

    return Bx * D * Bx.transpose() + Bw * m_varW * Bw.transpose() + b * b.transpose();
}

Matrix VanDerPolRejectionLinear::dadx(int /*i*/, const Vector &x) const
{

    Matrix res = Matrix::Zero(m_dimX, m_dimX);

    res(0, 0) = 1.0;
    res(0, 1) = m_deltaT;

    res(1, 0) = m_deltaT * (-1 * m_omega * m_omega - m_alpha * x[1] * 2 * m_beta * x[0]);
    res(1, 1) = 1 + m_deltaT * m_alpha * (1 - m_beta * x[0] * x[0]);

    return res;
}

Matrix VanDerPolRejectionLinear::dadv(int /*i*/, const Vector &x) const
{
    Matrix res = Matrix::Zero(m_dimX, m_dimV);
    res(1, 0) = m_gamma * x[0];
    return res;
}

Matrix VanDerPolRejectionLinear::dbdx(int /*i*/, const Vector &x) const
{

    Matrix res = Matrix::Zero(m_dimY, m_dimX);

    res(0, 0) = 2.0 * A1 * x[0] + A2;

    return res;
}

Matrix VanDerPolRejectionLinear::dbdw(int i, const Vector &/*x*/) const
{

    Matrix res = Matrix::Zero(m_dimY, m_dimW);

    res(0, 0) = C(i);

    return res;
}

double VanDerPolRejectionLinear::Pr(int i) const
{
    if (i == 1) {
        return 1 - m_e;
    } else {
        return m_e;
    }
}

Array<int> VanDerPolRejectionLinear::generateArrayI(int sizeS, int k) const
{
    Array<int> array(sizeS);
    double p = 1.0 - m_e;

    std::default_random_engine generator;
    generator.seed(k);
    std::bernoulli_distribution distribution(p);

    for (int i = 0; i < sizeS; ++i) {
        if (distribution(generator)) {
            array[i] = 1;
        } else {
           array[i] = 2;
        }
    }

    return array;
}

} // end Tasks::LogicDynamic

} // end Tasks

