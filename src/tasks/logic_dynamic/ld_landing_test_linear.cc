#include "ld_landing_test_linear.h"
#include "src/math/convert.h"

#include "iostream"

namespace Tasks
{

namespace LogicDynamic
{


using Math::Convert::DegToRad;


LandingTestLinear::LandingTestLinear()
    : LogicDynamicTask()
    , m_turnTime(45.0)
    , m_e(0.05)
{
    m_info->setName("Спуск ЛА с неполной информацией. Тестовый пример");
    m_info->setType("Л-");

    m_dimY = 1;
    m_dimX = 1;
    m_dimV  = 1;
    m_dimW  = 1;

    m_meanX0 = Vector(m_dimX);
    m_meanV = Vector::Zero(m_dimV);
    m_meanW = Vector::Zero(m_dimW);
    m_varX0 = Matrix::Zero(m_dimX, m_dimX);
    m_varV = Matrix::Zero(m_dimV, m_dimV);
    m_varW = Matrix::Zero(m_dimW, m_dimW);

    m_meanX0[0] = 10.0;
    m_varX0(0, 0) = 25.0;
    m_varV(0, 0) = 1.0;
    m_varW(0, 0) = 1.0;

    (*m_consts)["a1"]     = A1;
    (*m_consts)["a2"]     = A2;

    (*m_params)["tau"] = m_turnTime;
    (*m_params)["e"]   = m_e;
}

void LandingTestLinear::loadParams()
{
    countI = 1;
    cI2 = 3;
    m_turnTime = m_params->at("tau");
    m_e        = m_params->at("e");
}


double LandingTestLinear::c(int i) const
{
    if (i == 1) {
        return 1.0;
    } else {
        return cI2;
    }
}


Vector LandingTestLinear::a(int /*i*/, const Vector &x) const
{
    Vector dx(m_dimX);
    Vector v  = m_normalRand(m_meanV, m_varV);
    dx[0] = A1 * x[0] + A2 * v[0];

    return dx;
}

Vector LandingTestLinear::b(int i, const Vector &x) const
{
    Vector w  = m_normalRand(m_meanW, m_varW);
    Vector res(m_dimY);
    double c2 = c(i);

    res[0] = x[0] + c2 * w[0];

    return res;
}


double LandingTestLinear::A(int i, int l) const
{
    double e = m_e;
    double p = 1 - e;

    Matrix A(2, 2);
    A << p, p, e, e;

    return A(i - 1, l - 1);
}

double LandingTestLinear::nu(int i, int l, const Vector &m, const Matrix &D) const {
    return A(i, l);
}

Vector LandingTestLinear::tau(int i, int l, const Vector &z, const Matrix &P) const
{
    return A(i, l) * A1 * z;
}

Matrix LandingTestLinear::Theta(int i, int l, const Vector &z, const Matrix &P) const
{

    return A(i, l) * (A1 * (P + z) + A2*A2);
}

Vector LandingTestLinear::h(int i, const Vector &m, const Matrix & /* D*/) const
{
//    Vector b = bForZeroW(i, m);
    return b;
}

Matrix LandingTestLinear::G(int i, const Vector &m, const Matrix &D) const
{
    Matrix Bx = dbdx(i, m);
    Vector b  = h(i, m, m_varW);

    return D*Bx.transpose() + m*b.transpose();
}

Matrix LandingTestLinear::F(int i, const Vector &m, const Matrix &D) const
{
    Matrix Bx = dbdx(i, m);
    Vector b  = h(i, m, m_varW);
    Matrix BwDBwT = BwdbdwBwt(i, m);
    return b * b.transpose() + Bx * D * Bx.transpose() +  BwDBwT;
}

Matrix LandingTestLinear::dadx(int /*i*/, const Vector &x) const
{
    double _E = Ex(x);
    double h   = m_step;
    Matrix res = Matrix::Zero(m_dimX, m_dimX);

    double sk1 = Sk(m_step*m_time);

    res(0, 0) = 1.0 - 2.0 * h * _E / x[0];
    res(0, 1) = -1 * h * GG * cos(x[1]);
    res(0, 2) = h * BB * _E;
    res(0, 3) = -1 * h * _E / x[3];
    res(0, 4) = 0.0;
    res(0, 5) = 0.0;

    res(1, 0) = h * (sk1 * x[4] * _E / pow(x[0], 2) + cos(x[1]) * (1/(RR + x[2]) + GG / pow(x[0], 2)));
    res(1, 1) = 1.0 - h * sin(x[1]) * (x[0] / (RR + x[2]) - GG / x[0]);
    res(1, 2) = -1*h * ((BB * sk1 * x[4] * _E/x[0]) + x[1] * cos(x[1]) / pow(RR + x[2], 2));
    res(1, 3) = h * sk1 * x[4] * _E / (x[0] * x[3]);
    res(1, 4) = h * sk1 * _E/x[0];
    res(1, 5) = 0.0;

    res(2, 0) = h * sin(x[1]);
    res(2, 1) = h * x[0] * cos(x[1]);

    res(2, 2) = res(3, 3) = res(4, 4) = res(5, 5) = 1.0;

    return res;
}

Matrix LandingTestLinear::dadv(int /*i*/, const Vector & /*x*/) const
{
    return Matrix::Zero(m_dimX, m_dimV);
}

Matrix LandingTestLinear::dbdx(int i, const Vector &x) const
{
    double _e = e(x);
    double _d = d(x);
    double _E = Ex(x);

    Matrix gamma = Matrix::Zero(2, 2);
    gamma(0,0) = gammaX(i);
    gamma(1,1) = gammaY(i);

    double sk1 = Sk(m_step*m_time);

    Matrix tmp(m_dimY, m_dimX);

    tmp(0, 0) = 2.0 * _d / x[0];
    tmp(0, 1) = -1 * _e;
    tmp(0, 2) = -1 * BB * _d;
    tmp(0, 3) = _d / x[3];
    tmp(0, 4) = -1 * sk1 * sin(x[1] - x[5]);
    tmp(0, 5) = _e;

    tmp(1, 0) = 2.0 * _e / x[0];
    tmp(1, 1) = _d;
    tmp(1, 2) = -1 * BB * _e;
    tmp(1, 3) = _e / x[3];
    tmp(1, 4) = sk1 * sin(x[1] - x[5]);
    tmp(1, 5) = -1 * _d;

    return _E * gamma * tmp;
}

Matrix LandingTestLinear::dbdw(int i, const Vector &x) const
{
    double _e = e(x);
    double _d = d(x);
    double _E = Ex(x);
    double gamX = gammaX(i);
    double gamY = gammaY(i);
    Matrix tmp(m_dimY, m_dimW);

    tmp(0, 0) = gamX * _E * _d;
    tmp(0, 1) = 0.0;
    tmp(0, 2) = gamX;
    tmp(0, 3) = 0.0;

    tmp(1, 0) = 0.0;
    tmp(1, 1) = gamY * _E * _e;
    tmp(1, 2) = 0.0;
    tmp(1, 3) = gamY;

    return tmp;
}

Matrix LandingTestLinear::BwdbdwBwt(int i, const Vector &x) const
{
    double _e = e(x);
    double _d = d(x);
    double _E = Ex(x);
    double gamX = gammaX(i);
    double gamY = gammaY(i);
    Matrix tmp(m_dimY, m_dimY);

    tmp(0, 0) = gamX * (_E * _E * _d * _d * m_varW(0, 0) + m_varW(2, 2));
    tmp(0, 1) = 0.0;


    tmp(1, 0) = 0.0;
    tmp(1, 1) = gamY * (_E * _E * _e * _e * m_varW(0, 0) + m_varW(2, 2));

    return tmp;
}

double LandingTestLinear::Pr(int i) const
{
    if (i == 1) {
        return 1 - m_e;
    } else {
        return m_e;
    }
}

Array<int> LandingTestLinear::generateArrayI(int sizeS) const
{
    Array<int> array(sizeS);
    double e = 0.4 * (1.0 - m_p);
    int countI1, countI2, countI3;
    countI1 = sizeS*m_p;
    countI2 = sizeS*e + countI1;
    countI3 = sizeS*e + countI2;
    for (int i = 0; i < sizeS; i++) {
        if (i < countI1) {
            array[i] = 1;
        } else if (i < countI2) {
            array[i] = 2;
        } else if (i < countI3) {
            array[i] = 3;
        } else {
            array[i] = 4;
        }
    }

    for (int i = 0; i < sizeS; i++) {
      std::swap(array[i], array[rand() % sizeS]);
    }
    return array;
//#if defined(ARCHITECTURE_64)
//    std::mt19937_64 generator;
//#else
//    std::mt19937 generator;
//#endif
//    static std::uniform_real_distribution<double> uniform(0.0, 1.0);
//    double u = uniform(generator);

//    double p = m_p;
//    double e = 0.4 * (1.0 - p);
//
//    if (u < p) {
//        return 1;
//    } else if (u < p + e) {
//        return 2;
//    } else if (u < p + 2 * e) {
//        return 3;
//    } else {
//        return 4;
//    }
}

} // end Tasks::LogicDynamic

} // end Tasks
