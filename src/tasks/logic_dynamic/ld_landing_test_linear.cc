#include "ld_landing_test_linear.h"
#include "src/math/convert.h"


namespace Tasks
{

namespace LogicDynamic
{


using Math::Convert::DegToRad;


LandingTestLinear::LandingTestLinear()
    : LogicDynamicTask()
    , m_turnTime(45.0)
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
    m_varX0(3, 3) = pow(0.1, 2); // WARNING ... = 0.1 * m0, m0 - ???
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
}

void LandingTestLinear::loadParams()
{
    m_turnTime = m_params->at("tau");
}

double LandingTestLinear::k(double t) const
{
    return KB * Math::sign(t - m_turnTime);
}

Matrix LandingTestLinear::e(int i, const Vector &x) const
{
    double _exp = exp(-BB * x[2]);
    Matrix e    = Matrix::Zero(2, 2);

    e(0, 0) = gammaX(i) * _exp * cos(x[1] - x[5]);
    e(0, 1) = gammaX(i) * _exp * k(m_time) * sin(x[1] - x[5]);
    e(1, 0) = gammaY(i) * _exp * sin(x[1] - x[5]);
    e(1, 1) = gammaY(i) * _exp * k(m_time) * cos(x[1] - x[5]);

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

Vector LandingTestLinear::a(const Vector &x) const
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
    Matrix _e = e(i, x);
    Vector w  = m_normalRand(m_meanW, m_varW);
    Vector res(m_dimY);

    res[0] = (_e(0, 0) - x[4] * _e(0, 1)) * (w[0] + 1.0) + w[2];
    res[1] = (_e(1, 0) + x[4] * _e(1, 1)) * (w[1] + 1.0) + w[3];

    return res;
}

Vector LandingTestLinear::tau(const Vector &z, const Matrix & /*D*/) const
{
    return a(z);
}

Matrix LandingTestLinear::Theta(const Vector &z, const Matrix &P) const
{
    Matrix Ax = dadx(z);
    Matrix Av = dadv(z);

    return Ax * P * Ax.transpose() + Av * m_varV * Av.transpose();

    // TODO V = 0 --> m_meanV = 0, m_varV = 0 --> Av = 0....
}

Vector LandingTestLinear::h(int i, const Vector &m, const Matrix & /* D*/) const
{
    Matrix _e = e(i, m);
    Vector res(m_dimY);

    res[0] = (_e(0, 0) - m[4] * _e(0, 1)) * (m_meanW[0] + 1.0) + m_meanW[2];
    res[1] = (_e(1, 0) + m[4] * _e(1, 1)) * (m_meanW[1] + 1.0) + m_meanW[3];

    return res;
}

Matrix LandingTestLinear::G(int i, const Vector &m, const Matrix & /*D*/) const
{
    return dbdx(i, m);
}

Matrix LandingTestLinear::F(int i, const Vector &m, const Matrix &D) const
{
    Matrix Bx = dbdx(i, m);
    Matrix Bw = dbdw(i, m);

    return Bx * D * Bx.transpose() + Bw * m_varW * Bw.transpose();
}

Matrix LandingTestLinear::dadx(const Vector &x) const
{
    double e   = exp(-BB * x[2]);
    double h   = m_step;
    double kt  = k(m_time);
    Matrix res = Matrix::Zero(m_dimX, m_dimX);

    res(0, 0) = 1.0 - 2.0 * h * x[0] * x[3] * e;
    res(0, 1) = -h * GG * cos(x[1]);
    res(0, 2) = h * BB * x[0] * x[0] * x[3] * e;
    res(0, 3) = -h * x[0] * x[0] * e;
    res(0, 4) = 0.0;
    res(0, 5) = 0.0;

    res(1, 0) = h * (x[3] * x[4] * kt * e + (GG / (x[0] * x[0]) + 1.0 / (RR + x[2])) * cos(x[1]));
    res(1, 1) = 1.0 - h * sin(x[1]) * (x[0] / (RR + x[2]) - GG / x[0]);
    res(1, 2) = -h * (BB * x[0] * x[3] * x[4] * kt * e + x[0] * cos(x[1]) / (RR + x[2]));
    res(1, 3) = h * x[0] * x[4] * kt * e;
    res(1, 4) = h * x[0] * x[3] * kt * e;
    res(1, 5) = 0.0;

    res(2, 0) = h * sin(x[1]);
    res(2, 1) = h * x[0] * cos(x[1]);
    res(2, 2) = 1.0;

    res(3, 3) = res(4, 4) = res(5, 5) = 1.0;

    return res;
}

Matrix LandingTestLinear::dadv(const Vector & /*x*/) const
{
    return Matrix::Zero(m_dimX, m_dimV);
}

Matrix LandingTestLinear::dbdx(int i, const Vector &x) const
{
    Matrix _e = e(i, x);
    double e0 = _e(0, 0) - x[4] * _e(0, 1);
    double e1 = _e(1, 0) + x[4] * _e(1, 1);
    Matrix res(m_dimY, m_dimX);

    res(0, 0) = 2.0 * e0 / x[0];
    res(0, 1) = -e1;
    res(0, 2) = -BB * e0;
    res(0, 3) = e0 / x[3];
    res(0, 4) = -_e(0, 1);
    res(0, 5) = BB * e0;

    res(1, 0) = 2.0 * e1 / x[0];
    res(1, 1) = e0;
    res(1, 2) = -BB * e1;
    res(1, 3) = e1 / x[3];
    res(1, 4) = _e(1, 1);
    res(1, 5) = BB * e1;

    return res;
}

Matrix LandingTestLinear::dbdw(int i, const Vector &x) const
{
    Matrix _e = e(i, x);
    double e0 = _e(0, 0) - x[4] * _e(0, 1);
    double e1 = _e(1, 0) + x[4] * _e(1, 1);
    Matrix res(m_dimY, m_dimW);

    res(0, 0) = e0;
    res(0, 1) = 0.0;
    res(0, 2) = 1.0;
    res(0, 3) = 0.0;

    res(1, 0) = 0.0;
    res(1, 1) = e1;
    res(1, 2) = 0.0;
    res(1, 3) = 1.0;

    return res;
}


} // end Tasks::LogicDynamic

} // end Tasks
