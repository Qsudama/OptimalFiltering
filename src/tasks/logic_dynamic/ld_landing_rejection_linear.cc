#include "ld_landing_rejection_linear.h"
#include "src/math/convert.h"

#include "iostream"

namespace Tasks
{

namespace LogicDynamic
{


using Math::Convert::DegToRad;


LandingRejectionLinear::LandingRejectionLinear()
    : LogicDynamicTask()
    , m_turnTime(45.0)
    , m_p(1)
    , countIInTask(1)
    , gamMinX(0.5)
    , gamMinY(0.5)
{
    m_info->setName("6-мерный спуск ЛА со сбоями 2-х датчиков");
    m_info->setType("Л-");

    m_dimY = 2;

    m_dimX      = 6;
    m_meanX0    = Vector(m_dimX);
    m_meanX0[0] = 6000.0;
    m_meanX0[1] = DegToRad(-18.0);
    m_meanX0[2] = 100000.0;
    m_meanX0[3] = 0.5 * R0 * SX;
    m_meanX0[4] = KB;
    m_meanX0[5] = 0.0;

    m_dimV  = 3;
    m_meanV = Vector::Zero(m_dimV);

    m_dimW  = 4;
    m_meanW = Vector::Zero(m_dimW);

    m_varX0 = Matrix::Zero(m_dimX, m_dimX);
    m_varX0(0, 0) = pow(15, 2);
    m_varX0(1, 1) = pow(DegToRad(1.0), 2);
    m_varX0(2, 2) = pow(7000.0, 2);
    m_varX0(3, 3) = pow(0.1 * m_meanX0[3], 2);
    m_varX0(4, 4) = pow(0.02, 2);
    m_varX0(5, 5) = pow(DegToRad(1.0), 2);

    m_varV = Matrix::Zero(m_dimV, m_dimV);

    m_varW = Matrix::Zero(m_dimW, m_dimW);
    m_varW(0, 0) = pow(0.01, 2);
    m_varW(1, 1) = pow(0.01, 2);
    m_varW(2, 2) = pow(2E-5, 2);
    m_varW(3, 3) = pow(2E-5, 2);

    (*m_consts)["Kb"]     = KB;
    (*m_consts)["Beta"]   = BB;
    (*m_consts)["SigmaX"] = SX;
    (*m_consts)["rho0"]   = R0;
    (*m_consts)["g"]      = GG;
    (*m_consts)["R"]      = RR;
    (*m_consts)["e"]   = 0.4 * (1.0 - m_p);

    (*m_params)["tau"] = m_turnTime;
    (*m_params)["p"]   = m_p;
    (*m_params)["Кол-во режимов I"] = countIInTask;
    (*m_params)["GammaX_min"]   = gamMinX;
    (*m_params)["GammaY_min"]   = gamMinY;
}

void LandingRejectionLinear::loadParams()
{
    countI              = m_params->at("Кол-во режимов I");
    gamMinX             = m_params->at("GammaX_min");
    gamMinY             = m_params->at("GammaY_min");
    m_turnTime          = m_params->at("tau");
    m_p                 = m_params->at("p");
    (*m_consts)["e"]    = 0.4 * (1.0 - m_p);
}

double LandingRejectionLinear::Sk(double t) const
{
    return Math::sign(t - m_turnTime);
}

double LandingRejectionLinear::Ex(const Vector &x) const
{
    double E = x[0] * x[0] * x[3] * exp(-BB * x[2]);
    return E;
}

double LandingRejectionLinear::d(const Vector &x) const
{
    double d = cos(x[1]-x[5]) - x[4] * Sk(m_step*m_time) * sin(x[1]-x[5]);
    return d;
}

double LandingRejectionLinear::e(const Vector &x) const
{
    double e = sin(x[1]-x[5]) + x[4] * Sk(m_step*m_time) * cos(x[1]-x[5]);
    return e;
}

double LandingRejectionLinear::gammaX(int i) const
{
    if (i == 1 || i == 3) {
        return 1.0;
    } else {
        return gamMinX;
    }
}

double LandingRejectionLinear::gammaY(int i) const
{
    if (i == 1 || i == 2) {
        return 1.0;
    } else {
        return gamMinY;
    }
}

Vector LandingRejectionLinear::a(int /*i*/, const Vector &x) const
{
    double e = exp(-BB * x[2]);
    Vector dx(m_dimX);

    dx[0] = x[0] - m_step * x[0] * x[0] * x[3] * e - m_step * GG * sin(x[1]);
    dx[1] = x[1] + m_step * Sk(m_step*m_time) * x[0] * x[3] * x[4] * e + m_step * cos(x[1]) * (x[0] / (RR + x[2]) - GG / x[0]);
    dx[2] = x[2] + m_step * x[0] * sin(x[1]);
    dx[3] = x[3];
    dx[4] = x[4];
    dx[5] = x[5];

    return dx;
}

Vector LandingRejectionLinear::b(int i, const Vector &x) const
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

Vector LandingRejectionLinear::bForZeroW(int i, const Vector &x) const
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


double LandingRejectionLinear::A(int i, int l) const
{
    double p = m_p;
    double e = 0.4 * (1.0 - p);

    Matrix A(4, 4);
    A << p, p, p, p, e, e, e, e, e, e, e, e, 0.5 * e, 0.5 * e, 0.5 * e, 0.5 * e;

    return A(i - 1, l - 1);
}

double LandingRejectionLinear::nu(int i, int l, const Vector &/*m*/, const Matrix &/*D*/) const {
    return A(i, l);
}

Vector LandingRejectionLinear::tau(int i, int l, const Vector &z, const Matrix &/*P*/) const
{
    return A(i, l) * a(i, z);
}

Matrix LandingRejectionLinear::Theta(int i, int l, const Vector &z, const Matrix &P) const
{
    Matrix Ax = dadx(i, z);
    Vector a  = this->a(i, z);

    return A(i, l) * (Ax * P * Ax.transpose() + a * a.transpose());
}

Vector LandingRejectionLinear::h(int i, const Vector &m, const Matrix & /* D*/) const
{
    Vector b = bForZeroW(i, m);
    return b;
}

Matrix LandingRejectionLinear::G(int i, const Vector &m, const Matrix &D) const
{
    Matrix Bx = dbdx(i, m);
    Vector b  = h(i, m, m_varW);

    return D*Bx.transpose() + m*b.transpose();
}

Matrix LandingRejectionLinear::F(int i, const Vector &m, const Matrix &D) const
{
    Matrix Bx = dbdx(i, m);
    Vector b  = h(i, m, m_varW);
    Matrix BwDBwT = BwdbdwBwt(i, m);
    return b * b.transpose() + Bx * D * Bx.transpose() +  BwDBwT;
}

Matrix LandingRejectionLinear::dadx(int /*i*/, const Vector &x) const
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

Matrix LandingRejectionLinear::dadv(int /*i*/, const Vector & /*x*/) const
{
    return Matrix::Zero(m_dimX, m_dimV);
}

Matrix LandingRejectionLinear::dbdx(int i, const Vector &x) const
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

Matrix LandingRejectionLinear::dbdw(int i, const Vector &x) const
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

Matrix LandingRejectionLinear::BwdbdwBwt(int i, const Vector &x) const
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

double LandingRejectionLinear::Pr(int i) const
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

Array<int> LandingRejectionLinear::generateArrayI(int sizeS) const
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
