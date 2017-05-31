#include "d_landing_linear.h"
#include "src/math/convert.h"


namespace Tasks
{

namespace Discrete
{


using Math::Convert::DegToRad;


LandingLinear::LandingLinear()
    : DiscreteTask()
    , m_turnTime(45.0)
{
    m_info->setName("Спуск ЛА на планету");
    m_info->setType("Л-");

    m_dimY = 2;

    m_dimX      = 3;
    m_meanX0    = Vector(m_dimX);
    m_meanX0[0] = 6.0;
    m_meanX0[1] = DegToRad(-18.0);
    m_meanX0[2] = 100.0;

    m_dimV  = 3;
    m_meanV = Vector::Zero(m_dimV);

    m_dimW  = 4;
    m_meanW = Vector::Zero(m_dimW);

    m_varX0 = Matrix::Zero(m_dimX, m_dimX);
    m_varX0(0, 0) = pow(15E-3, 2);
    m_varX0(1, 1) = pow(DegToRad(1.0), 2);
    m_varX0(2, 2) = pow(7.0, 2);

    m_varV = Matrix::Zero(m_dimV, m_dimV);

    m_varW = Matrix::Zero(m_dimW, m_dimW);
    m_varW(0, 0) = pow(1E-2, 2);
    m_varW(1, 1) = pow(1E-2, 2);
    m_varW(2, 2) = pow(2E-7, 2);
    m_varW(3, 3) = pow(2E-7, 2);

    (*m_consts)["Kb"]   = KB;
    (*m_consts)["Beta"] = BB;
    (*m_consts)["c"]    = CC;
    (*m_consts)["g"]    = GG;
    (*m_consts)["R"]    = RR;

    (*m_params)["tau"] = m_turnTime;
}

void LandingLinear::loadParams()
{
    m_turnTime = m_params->at("tau");
}

double LandingLinear::k(double t) const
{
    return KB * Math::sign(t - m_turnTime);
}

Vector LandingLinear::a(const Vector &x) const
{
    double e = exp(-BB * x[2]);
    Vector dx(m_dimX);

    dx[0] = -CC * x[0] * x[0] * e - GG * sin(x[1]);
    dx[1] = CC * k(m_time) * x[0] * e + cos(x[1]) * (x[0] / (RR + x[2]) - GG / x[0]);
    dx[2] = x[0] * sin(x[1]);

    return x + m_step * dx;
}

Vector LandingLinear::b(const Vector &x) const
{
    double e = exp(-BB * x[2]);
    Vector w = m_normalRand(m_meanW, m_varW); // СКО, а не Дисперсия
    Vector res(m_dimY);

    res[0] = CC * (w[0] + 1.0) * x[0] * x[0] * e * (cos(x[1]) - k(m_time) * sin(x[1])) + w[2];
    res[1] = CC * (w[1] + 1.0) * x[0] * x[0] * e * (sin(x[1]) - k(m_time) * cos(x[1])) + w[3];

    return res;
}

Vector LandingLinear::b_m(const Vector &x) const
{
    double e = exp(-BB * x[2]);
    Vector w = m_meanW;
    Vector res(m_dimY);

    res[0] = CC * (w[0] + 1.0) * x[0] * x[0] * e * (cos(x[1]) - k(m_time) * sin(x[1])) + w[2];
    res[1] = CC * (w[1] + 1.0) * x[0] * x[0] * e * (sin(x[1]) - k(m_time) * cos(x[1])) + w[3];

    return res;
}

Vector LandingLinear::tau(const Vector &z, const Matrix & /*D*/) const
{
    return a(z);
}

Matrix LandingLinear::Theta(const Vector &z, const Matrix &P) const
{
    Matrix Ax = dadx(z);
    Matrix Av = dadv(z);

    return Ax * P * Ax.transpose() + Av * m_varV * Av.transpose();

    // TODO V = 0 --> m_meanV = 0, m_varV = 0 --> Av = 0....
}

Vector LandingLinear::h(const Vector &m, const Matrix & /* D*/) const
{
    double e = exp(-BB * m[2]);
    Vector res(m_dimY);

    res[0] = CC * (m_meanW[0] + 1.0) * m[0] * m[0] * e * (cos(m[1]) - k(m_time) * sin(m[1])) + m_meanW[2];
    res[1] = CC * (m_meanW[1] + 1.0) * m[0] * m[0] * e * (sin(m[1]) - k(m_time) * cos(m[1])) + m_meanW[3];

    return res;
}

Matrix LandingLinear::G(const Vector &m, const Matrix & /*D*/) const
{
    return dbdx(m);
}

Matrix LandingLinear::F(const Vector &m, const Matrix &D) const
{
    Matrix Bx = dbdx(m);
    Matrix Bw = dbdw(m);

    return Bx * D * Bx.transpose() + Bw * m_varW * Bw.transpose();
}

Matrix LandingLinear::dadx(const Vector &x) const
{
    double e = exp(-BB * x[2]);
    Matrix res(m_dimX, m_dimX);

    res(0, 0) = 1.0 - 2.0 * m_step * CC * x[0] * e;
    res(0, 1) = -m_step * GG * cos(x[1]);
    res(0, 2) = m_step * BB * CC * x[0] * x[0] * e;

    res(1, 0) = m_step * (CC * k(m_time) * e + (GG / (x[0] * x[0]) + 1.0 / (RR + x[2])) * cos(x[1]));
    res(1, 1) = 1.0 + m_step * ((GG / x[0] - x[0] / (RR + x[2])) * sin(x[1]));
    res(1, 2) = m_step * (-CC * BB * k(m_time) * x[0] * e - x[0] * cos(x[1]) / ((RR + x[2]) * (RR + x[2])));

    res(2, 0) = m_step * sin(x[1]);
    res(2, 1) = m_step * x[0] * cos(x[1]);
    res(2, 2) = 1.0;

    return res;
}

Matrix LandingLinear::dadv(const Vector & /*x*/) const
{
    return Matrix::Zero(m_dimX, m_dimV);
}

Matrix LandingLinear::dbdx(const Vector &x) const
{
    double e0 = CC * x[0] * x[0] * exp(-BB * x[2]) * (cos(x[1]) - k(m_time) * sin(x[1]));
    double e1 = CC * x[0] * x[0] * exp(-BB * x[2]) * (sin(x[1]) - k(m_time) * cos(x[1]));
    Matrix res(m_dimY, m_dimX);

    res(0, 0) = 2.0 * e0 * (1.0 + m_meanW[0]) / x[0];
    res(0, 1) = -e1 * (1.0 + m_meanW[0]);
    res(0, 2) = -BB * e0 * (1.0 + m_meanW[0]);

    res(1, 0) = 2.0 * e1 * (1.0 + m_meanW[1]) / x[0];
    res(1, 1) = e0 * (1.0 + m_meanW[1]);
    res(1, 2) = -BB * e1 * (1.0 + m_meanW[1]);

    return res;
}

Matrix LandingLinear::dbdw(const Vector &x) const
{
    Matrix res(m_dimY, m_dimW);

    res(0, 0) = CC * x[0] * x[0] * exp(-BB * x[2]) * (cos(x[1]) - k(m_time) * sin(x[1]));
    res(0, 1) = 0.0;
    res(0, 2) = 1.0;
    res(0, 3) = 0.0;

    res(1, 0) = 0.0;
    res(1, 1) = CC * x[0] * x[0] * exp(-BB * x[2]) * (sin(x[1]) - k(m_time) * cos(x[1]));
    res(1, 2) = 0.0;
    res(1, 3) = 1.0;

    return res;
}


} // end Tasks::Discrete

} // end Tasks
