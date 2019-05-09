#include "ld_landing_rejection_3d_linear.h"
#include "src/math/convert.h"

#include "iostream"

namespace Tasks
{

namespace LogicDynamic
{

using Math::Convert::DegToRad;


LandingRejection3DLinear::LandingRejection3DLinear() : LogicDynamicTask()
    , m_turnTime(45.0) // Время t_y
    , m_e(0.0) // Вероятность сбоя
    , gamMinX(0.01)
    , gamMinY(0.01)
    , countIInTask(1) // Количество режимов
{
    m_info->setName("3-мерный спуск ЛА со сбоями 2-х датчиков");
    m_info->setType("Л-");

    m_dimY = 2;
    m_dimX = 3;

    m_dimV = 3;
    m_dimW = 4;

    m_meanX0    =  Vector(m_dimX);
    m_meanX0[0] = 6.0; //M_v_0
    m_meanX0[1] = -0.314;//DegToRad(-18.0); // M_theta_0
    m_meanX0[2] = 100.0; // M_H_0

    m_meanV = Vector::Zero(m_dimV);
    m_meanW = Vector::Zero(m_dimW);

    static double radian = 0.017453;

    m_varX0 = Matrix::Zero(m_dimX, m_dimX);
    m_varX0(0, 0) = pow(0.015, 2); // Sigma_V_0
    m_varX0(1, 1) = pow(radian, 2); // Sigma_theta_0
    m_varX0(2, 2) = pow(1, 2); // Sigma_H_0

    m_varV = Matrix::Zero(m_dimV, m_dimV);
    m_varW = Matrix::Zero(m_dimW, m_dimW);

    m_varW(0, 0) = pow(0.01, 2); // Sigma_U
    m_varW(1, 1) = pow(0.01, 2); // Sigma_U
    m_varW(2, 2) = pow(0.00002, 2); // Sigma_W
    m_varW(3, 3) = pow(0.00002, 2); // Sigma_W

    (*m_consts)["Kb"] = KB;
    (*m_consts)["Beta"] = BB;
    (*m_consts)["g"] = GG;
    (*m_consts)["R"] = RR;

    (*m_consts)["p"] = 1 - m_e;
// 4-х режимность
//    (*m_consts)["p"]   = 1 - 2.5*m_e;

    (*m_params)["tau"] = m_turnTime;
    (*m_params)["e"] = m_e;
    (*m_params)["Кол-во режимов I"] = countIInTask;
    (*m_params)["GammaX_min"] = gamMinX;
    (*m_params)["GammaY_min"] = gamMinY;

}

void LandingRejection3DLinear::loadParams()
{
    countI              = m_params->at("Кол-во режимов I");
    gamMinX             = m_params->at("GammaX_min");
    gamMinY             = m_params->at("GammaY_min");
    m_turnTime          = m_params->at("tau");
    m_e                 = m_params->at("e");

    (*m_consts)["p"]    = 1 - m_e;
// 4-х режимность
//    (*m_consts)["p"]    = 1 - 2.5*m_e;
}

double LandingRejection3DLinear::Sk(double t) const
{
    return Math::sign(t - m_turnTime);
}

double LandingRejection3DLinear::Ex(const Vector &x) const
{
    double E = x[0] * x[0] * CC * exp(-BB * x[2]);
    return E;
}

double LandingRejection3DLinear::d(const Vector &x) const
{
    double d = cos(x[1]) - KB * Sk(m_step*m_time) * sin(x[1]);
    return d;
}

double LandingRejection3DLinear::e(const Vector &x) const
{
    double e = sin(x[1]) + KB * Sk(m_step*m_time) * cos(x[1]);
    return e;
}

double LandingRejection3DLinear::gammaX(int i) const
{
    if (i == 1) {
        return 1.0;
    }
    return gamMinX;

// 4-х режимность
//    if (i == 1 || i == 2) {
//        return 1.0;
//    } else {
//        return gamMinX;
//    }
}

double LandingRejection3DLinear::gammaY(int i) const
{
    if (i == 1) {
        return 1.0;
    }
    return gamMinY;

// 4-х режимность
//    if (i == 1 || i == 3) {
//        return 1.0;
//    } else {
//        return gamMinY;
//    }
}

Vector LandingRejection3DLinear::a(int /*i*/, const Vector &x) const
{
    double e = exp(-BB * x[2]);
    Vector dx(m_dimX);

    dx[0] = x[0] - m_step * x[0] * x[0] * CC * e - m_step * GG * sin(x[1]);
    dx[1] = x[1] + m_step * Sk(m_step*m_time) * x[0] * CC * KB * e + m_step * cos(x[1]) * (x[0] / (RR + x[2]) - GG / x[0]);
    dx[2] = x[2] + m_step * x[0] * sin(x[1]);

    return dx;
}

Vector LandingRejection3DLinear::b(int i, const Vector &x) const
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

Vector LandingRejection3DLinear::bForZeroW(int i, const Vector &x) const
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


double LandingRejection3DLinear::A(int i, int l) const
{
    double e = m_e;

    double p = 1 - m_e;
    Matrix A(2, 2);
    A << p, p, e, e;

// 4-х режимность
//    double p = 1 - 2.5*m_e;
//    Matrix A(4, 4);
//    A << p, p, p, p, e, e, e, e, e, e, e, e, 0.5 * e, 0.5 * e, 0.5 * e, 0.5 * e;

    return A(i - 1, l - 1);
}

double LandingRejection3DLinear::nu(int i, int l, const Vector &/*m*/, const Matrix &/*D*/) const {
    return A(i, l);
}

Vector LandingRejection3DLinear::tau(int i, int l, const Vector &z, const Matrix &/*P*/) const
{
    return A(i, l) * a(i, z);
}

Matrix LandingRejection3DLinear::Theta(int i, int l, const Vector &z, const Matrix &P) const
{
    Matrix Ax = dadx(i, z);
    Vector a  = this->a(i, z);

    return A(i, l) * (Ax * P * Ax.transpose() + a * a.transpose());
}

Vector LandingRejection3DLinear::h(int i, const Vector &m, const Matrix & /* D*/) const
{
    Vector b = bForZeroW(i, m);
    return b;
}

Matrix LandingRejection3DLinear::G(int i, const Vector &m, const Matrix &D) const
{
    Matrix Bx = dbdx(i, m);
    Vector b  = h(i, m, m_varW);

    return D*Bx.transpose() + m*b.transpose();
}

Matrix LandingRejection3DLinear::F(int i, const Vector &m, const Matrix &D) const
{
    Matrix Bx = dbdx(i, m);
    Vector b  = h(i, m, m_varW);
    Matrix BwDBwT = BwdbdwBwt(i, m);
    Matrix result =  b * b.transpose() + Bx * D * Bx.transpose() +  BwDBwT;
    return result;
}

Matrix LandingRejection3DLinear::dadx(int /*i*/, const Vector &x) const
{
    double _E = Ex(x);
    double h   = m_step;
    Matrix res = Matrix::Zero(m_dimX, m_dimX);

    double sk1 = Sk(m_step*m_time);

    res(0, 0) = 1.0 - 2.0 * h * _E / x[0];
    res(0, 1) = -1 * h * GG * cos(x[1]);
    res(0, 2) = h * BB * _E;

    res(1, 0) = h * (sk1 * KB * _E / pow(x[0], 2) + cos(x[1]) * (1/(RR + x[2]) + GG / pow(x[0], 2)));
    res(1, 1) = 1.0 - h * sin(x[1]) * (x[0] / (RR + x[2]) - GG / x[0]);
    res(1, 2) = -1*h * ((BB * sk1 * KB * _E/x[0]) + x[0] * cos(x[1]) / pow(RR + x[2], 2));

    res(2, 0) = h * sin(x[1]);
    res(2, 1) = h * x[0] * cos(x[1]);

    res(2, 2) = 1.0;

    return res;
}

Matrix LandingRejection3DLinear::dadv(int /*i*/, const Vector & /*x*/) const
{
    return Matrix::Zero(m_dimX, m_dimV);
}

Matrix LandingRejection3DLinear::dbdx(int i, const Vector &x) const
{
    double _e = e(x);
    double _d = d(x);
    double _E = Ex(x);

    Matrix gamma = Matrix::Zero(2, 2);
    gamma(0,0) = gammaX(i);
    gamma(1,1) = gammaY(i);

    Matrix tmp(m_dimY, m_dimX);

    tmp(0, 0) = 2.0 * _d / x[0];
    tmp(0, 1) = -1 * _e;
    tmp(0, 2) = -1 * BB * _d;

    tmp(1, 0) = 2.0 * _e / x[0];
    tmp(1, 1) = _d;
    tmp(1, 2) = -1 * BB * _e;

    return _E * gamma * tmp;
}

Matrix LandingRejection3DLinear::dbdw(int i, const Vector &x) const
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

Matrix LandingRejection3DLinear::BwdbdwBwt(int i, const Vector &x) const
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

double LandingRejection3DLinear::Pr(int i) const
{
    double e = m_e;

    double p = 1 - m_e;
    if (i == 1) {
        return p;
    } else {
        return e;
    }

// 4-х режимность
//    double p = 1 - 2.5*m_e;
//    if (i == 1) {
//        return p;
//    } else if (i == 2) {
//        return e;
//    } else if (i == 3) {
//        return e;
//    } else {
//        return 0.5*e;
//    }
}

Array<int> LandingRejection3DLinear::generateArrayI(int sizeS, int k) const // только для 2-х режимов!
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
