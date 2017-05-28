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
    , m_p(0.8)
{
    m_info->setName("Спуск ЛА с неполной информацией. Тестовый пример");
    m_info->setType("Л-");

    m_dimY = 2;

    m_dimX      = 6;
    m_meanX0    = Vector(m_dimX);
    m_meanX0[0] = 6.0;
    m_meanX0[1] = DegToRad(-18.0);
    m_meanX0[2] = 100.0;
    m_meanX0[3] = 0.5 * R0 * SX;
    m_meanX0[4] = KB;
    m_meanX0[5] = 0.0;

    m_dimV  = 3;
    m_meanV = Vector::Zero(m_dimV);

    m_dimW  = 4;
    m_meanW = Vector::Zero(m_dimW);

    m_varX0 = Matrix::Zero(m_dimX, m_dimX);
    m_varX0(0, 0) = pow(15E-3, 2);
    m_varX0(1, 1) = pow(DegToRad(1.0), 2);
    m_varX0(2, 2) = pow(7.0, 2);
    m_varX0(3, 3) = pow(0.1 * m_meanX0[3], 2);
    m_varX0(4, 4) = pow(0.02, 2);
    m_varX0(5, 5) = pow(DegToRad(1.0), 2);

    m_varV = Matrix::Zero(m_dimV, m_dimV);

    m_varW = Matrix::Zero(m_dimW, m_dimW);
    m_varW(0, 0) = pow(1E-2, 2);
    m_varW(1, 1) = pow(1E-2, 2);
    m_varW(2, 2) = pow(2E-7, 2);
    m_varW(3, 3) = pow(2E-7, 2);

    (*m_consts)["Kb"]     = KB;
    (*m_consts)["Beta"]   = BB;
    (*m_consts)["SigmaX"] = SX;
    (*m_consts)["rho0"]   = R0;
    (*m_consts)["g"]      = GG;
    (*m_consts)["R"]      = RR;

    (*m_params)["tau"] = m_turnTime;
    (*m_params)["p"]   = m_p;
}

void LandingTestLinear::loadParams()
{
    countI = 4;
    m_turnTime = m_params->at("tau");
    m_p        = m_params->at("p");
}

double LandingTestLinear::k(double t) const
{
    return Math::sign(t - m_turnTime);
}

double LandingTestLinear::Ex(const Vector &x) const
{
    double E = x[0] * x[0] * x[3] * exp(-BB * x[2]);
    return E;
}

double LandingTestLinear::d(const Vector &x) const
{
    double d = cos(x[1]-x[5]) - x[4] * k(m_time) * sin(x[1]-x[5]);
    return d;
}

double LandingTestLinear::e(const Vector &x) const
{
    double e = sin(x[1]-x[5]) + x[4] * k(m_time) * cos(x[1]-x[5]);
    return e;
}

double LandingTestLinear::gammaX(int i) const
{
    if (i == 1 || i == 3) {
        return 1.0;
    } else {
        return 0.0;
    }
}

double LandingTestLinear::gammaY(int i) const
{
    if (i == 1 || i == 2) {
        return 1.0;
    } else {
        return 0.0;
    }
}

Vector LandingTestLinear::a(int /*i*/, const Vector &x) const
{
    double e = exp(-BB * x[2]);
    Vector dx(m_dimX);

    dx[0] = -x[0] * x[0] * x[3] * e - GG * sin(x[1]);
    dx[1] = k(m_time) * x[0] * x[3] * x[4] * e + cos(x[1]) * (x[0] / (RR + x[2]) - GG / x[0]);
    dx[2] = x[0] * sin(x[1]);

    dx[3] = dx[4] = dx[5] = 0.0; // x = x + h * dx

    return x + m_step * dx;
}

Vector LandingTestLinear::b(int i, const Vector &x) const
{
    double _e = e(x);
    double _d = d(x);
    double _E = Ex(x);
    double gamX = gammaX(i);
    double gamY = gammaY(i);
    Vector w  = m_normalRand(m_meanW, m_varW);
    Vector res(m_dimY);

    res[0] = gamX * (_E * _d * (w[0] + 1.0) + w[2]);
    res[1] = gamY * (_E * _e * (w[1] + 1.0) + w[3]);

    return res;
}

Vector LandingTestLinear::bForZeroW(int i, const Vector &x) const
{
    double _e = e(x);
    double _d = d(x);
    double _E = Ex(x);
    double gamX = gammaX(i);
    double gamY = gammaY(i);
    Vector res(m_dimY);

    res[0] = gamX * _E* _d;
    res[1] = gamY * _E* _e;

    return res;
}


double LandingTestLinear::A(int i, int i0, const Vector &m, const Matrix &D) const
{
    double p = m_p;
    double e = 0.4 * (1.0 - p);

    Matrix A(4, 4);
    A << p, p, p, p, e, e, e, e, e, e, e, e, 0.5 * e, 0.5 * e, 0.5 * e, 0.5 * e;

    return A(i - 1, i0 - 1);
}

double LandingTestLinear::nu(int i, int i0, const Vector &m, const Matrix &D) const {
    return A(i, i0, m, D);
}

Vector LandingTestLinear::tau(int i, int i0, const Vector &z, const Matrix &P) const
{
    return A(i, i0, z, P) * a(i, z);
}

Matrix LandingTestLinear::Theta(int i, int i0, const Vector &z, const Matrix &P) const
{
    Matrix Ax = dadx(i, z);
    Vector a  = this->a(i, z);

    return A(i, i0, z, P) * (Ax * P * Ax.transpose() + a * a.transpose());
}

Vector LandingTestLinear::h(int i, const Vector &m, const Matrix & /* D*/) const
{
    Vector b = bForZeroW(i, m);
//    Matrix _e = e(i, m);
//    Vector res(m_dimY);

//    res[0] = (_e(0, 0) - m[4] * _e(0, 1)) * (m_meanW[0] + 1.0) + m_meanW[2];
//    res[1] = (_e(1, 0) + m[4] * _e(1, 1)) * (m_meanW[1] + 1.0) + m_meanW[3];

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
    Matrix Bw = dbdw(i, m);
    Vector b  = h(i, m, m_varW);

    return Bx * D * Bx.transpose() + Bw * m_varW * Bw.transpose() + b * b.transpose();
}

Matrix LandingTestLinear::dadx(int /*i*/, const Vector &x) const
{
    double E   = x[0] * x[0] * x[3] * exp(-BB * x[2]);
    double h   = m_step;
    double kt  = k(m_time);
    Matrix res = Matrix::Zero(m_dimX, m_dimX);

    res(0, 0) = 1.0 - 2.0 * h * E;
    res(0, 1) = -h * GG * cos(x[1]);
    res(0, 2) = h * BB * E;
    res(0, 3) = -h * E / x[3];
    res(0, 4) = 0.0;
    res(0, 5) = 0.0;

    res(1, 0) = h * (E * x[4] * kt / pow(x[0], 2) + (GG / pow(x[0], 2) + 1.0 / (RR + x[2])) * cos(x[1]));
    res(1, 1) = 1.0 - h * sin(x[1]) * (x[0] / (RR + x[2]) - GG / x[0]);
    res(1, 2) = -h * (BB * x[4] * kt * E + x[0] * cos(x[1]) / pow(RR + x[2], 2));
    res(1, 3) = h * x[4] * kt * E / (x[0] * x[3]);
    res(1, 4) = h * kt * E;
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
    double e0 = cos(x[1] - x[5]) - x[4] * sin(x[1] - x[5]) * k(m_time);
    double e1 = sin(x[1] - x[5]) + x[4] * cos(x[1] - x[5]) * k(m_time);
    Matrix tmp(m_dimY, m_dimX);

    tmp(0, 0) = 2.0 * e0 / x[0];
    tmp(0, 1) = -e1;
    tmp(0, 2) = -BB * e0;
    tmp(0, 3) = e0 / x[3];
    tmp(0, 4) = -k(m_time) * sin(x[1] - x[5]);
    tmp(0, 5) = e1;

    tmp(1, 0) = 2.0 * e1 / x[0];
    tmp(1, 1) = e0;
    tmp(1, 2) = -BB * e1;
    tmp(1, 3) = e0 / x[3];
    tmp(1, 4) = k(m_time) * cos(x[1] - x[5]);
    tmp(1, 5) = -e0;

    Matrix gamma = Matrix::Zero(2, 2);
    gamma(0, 0) = gammaX(i);
    gamma(1, 1) = gammaY(i);

    return gamma * tmp;
}

Matrix LandingTestLinear::dbdw(int i, const Vector &x) const
{
    double E  = x[0] * x[0] * x[3] * exp(-BB * x[2]);
    double e0 = cos(x[1] - x[5]) - x[4] * sin(x[1] - x[5]) * k(m_time);
    double e1 = sin(x[1] - x[5]) + x[4] * cos(x[1] - x[5]) * k(m_time);
    Matrix tmp(m_dimY, m_dimW);

    tmp(0, 0) = E * e0;
    tmp(0, 1) = 0.0;
    tmp(0, 2) = 1.0;
    tmp(0, 3) = 0.0;

    tmp(1, 0) = 0.0;
    tmp(1, 1) = E * e1;
    tmp(1, 2) = 0.0;
    tmp(1, 3) = 1.0;

    Matrix gamma = Matrix::Zero(2, 2);
    gamma(0, 0) = gammaX(i);
    gamma(1, 1) = gammaY(i);

    return gamma * tmp;
}

double LandingTestLinear::Pr(int i) const
{
    double e = 0.4 * (1.0 - m_p);
    if (i == 1) {
        return m_p;
    } else if (i == 2) {
        return e;
    } else if (i == 3) {
        return e;
    } else {
        return 0.5*e;
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
//    double p = m_p;
//    double e = 0.4 * (1.0 - p);
//    double u = uniform(generator);
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
