#include "c_landing_linear.h"
#include "src/math/convert.h"


namespace Tasks
{

namespace Continuous
{

using Math::Convert::DegToRad;


LandingLinear::LandingLinear()
    : ContinuousTask()
    , m_turnTime(45.0)
{
    m_info->setName("3-мерный спуск ЛА на планету");
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
    m_varX0(2, 2) = pow(1.0, 2);

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
    Vector res(m_dimX);

    res[0] = -CC * x[0] * x[0] * e - GG * sin(x[1]);
    res[1] = CC * k(m_time) * x[0] * e + cos(x[1]) * (x[0] / (RR + x[2]) - GG / x[0]);
    res[2] = x[0] * sin(x[1]);

    return res;
}

Matrix LandingLinear::B(const Vector & /*x*/) const
{
    return Matrix::Zero(m_dimX, m_dimV);
}


Vector LandingLinear::c(const Vector &x) const
{
    double e = exp(-BB * x[2]);
    Vector res(m_dimY);

    res[0] = CC * x[0] * x[0] * e * (cos(x[1]) - k(m_time) * sin(x[1]));
    res[1] = CC * x[0] * x[0] * e * (sin(x[1]) + k(m_time) * cos(x[1]));

    return res;
}

Matrix LandingLinear::D(const Vector &x) const
{
    Matrix res = Matrix::Zero(m_dimY, m_dimW);
    Vector cc  = c(x);

    res(0, 0) = cc[0] * sigma_m;
    res(0, 2) = 1.0 * sigma_a;
    res(1, 1) = cc[1] * sigma_m;
    res(1, 3) = 1.0 * sigma_a;

    return res;
}

Matrix LandingLinear::Q(const Vector &x, const Matrix & /*D*/) const
{
    Matrix b = B(x);

    return b * b.transpose();
}

Matrix LandingLinear::S(const Vector & /*x*/, const Matrix & /*D*/) const
{
    return Matrix::Zero(m_dimX, m_dimY);
}

Matrix LandingLinear::R(const Vector &x, const Matrix & /*D*/) const
{
    Matrix d = D(x);

    return d * d.transpose();
}

Matrix LandingLinear::A(const Vector &m, const Matrix & /*D*/) const
{
    double e = exp(-BB * m[2]);
    Matrix res(m_dimX, m_dimX);

    res(0, 0) = -2.0 * CC * m[0] * e;
    res(0, 1) = -GG * cos(m[1]);
    res(0, 2) = BB * CC * m[0] * m[0] * e;

    res(1, 0) = CC * k(m_time) * e + (GG / (m[0] * m[0]) + 1.0 / (RR + m[2])) * cos(m[1]);
    res(1, 1) = (GG / m[0] - m[0] / (RR + m[2])) * sin(m[1]);
    res(1, 2) = -CC * BB * k(m_time) * m[0] * e - m[0] * cos(m[1]) / ((RR + m[2]) * (RR + m[2]));

    res(2, 0) = sin(m[1]);
    res(2, 1) = m[0] * cos(m[1]);
    res(2, 2) = 0.0;

    return res;
}

Matrix LandingLinear::G(const Vector &m, const Matrix & /*D*/) const
{
    double e  = exp(-BB * m[2]);
    double kt = k(m_time);
    Matrix res(m_dimY, m_dimX);

    res(0, 0) = 2.0 * CC * m[0] * e * (cos(m[1]) - kt * sin(m[1]));
    res(0, 1) = -CC * m[0] * m[0] * e * sin(m[1]);
    res(0, 2) = -BB * CC * m[0] * m[0] * e * (cos(m[1]) - kt * sin(m[1]));

    res(1, 0) = 2.0 * CC * m[0] * e * (sin(m[1]) + kt * cos(m[1]));
    res(1, 1) = CC * m[0] * m[0] * e * cos(m[1]);
    res(1, 2) = -BB * CC * m[0] * m[0] * e * (sin(m[1]) + kt * cos(m[1]));

    return res;
}


} // end Tasks::Continuous

} // end Tasks
