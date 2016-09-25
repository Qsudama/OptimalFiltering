#include "cd_landing_linear.h"
#include "src/math/convert.h"


namespace Tasks
{

namespace ContinuousDiscrete
{


using Math::Rand::gaussianVector;
using Math::Convert::DegToRad;


LandingLinear::LandingLinear()
    : ContinuousDiscreteTask()
    , TT(45.0)
{
    m_info->setName("Спуск ЛА на планету");
    m_info->setType("Л-");

    m_dimY = 2;

    mx     = Vector(3);
    mx[0]  = 6.0;
    mx[1]  = DegToRad(-18.0);
    mx[2]  = 100.0;
    m_dimX = mx.size();

    mv     = Vector::Zero(3);
    m_dimV = mv.size();

    mw     = Vector::Zero(4);
    m_dimW = mw.size();

    Dx = Matrix::Zero(m_dimX, m_dimX);
    Dx(0, 0) = pow(15E-3, 2);
    Dx(1, 1) = pow(DegToRad(1.0), 2);
    Dx(2, 2) = pow(7.0, 2);

    Dv = Matrix::Zero(m_dimV, m_dimV);

    Dw = Matrix::Zero(m_dimW, m_dimW);
    Dw(0, 0) = pow(1E-2, 2);
    Dw(1, 1) = pow(1E-2, 2);
    Dw(2, 2) = pow(2E-7, 2);
    Dw(3, 3) = pow(2E-7, 2);

    (*m_consts)["Kb"]   = KB;
    (*m_consts)["Beta"] = BB;
    (*m_consts)["c"]    = CC;
    (*m_consts)["g"]    = GG;
    (*m_consts)["R"]    = RR;

    (*m_params)["tau"] = TT;
}

void LandingLinear::loadParams()
{
    TT = m_params->at("tau");
}

double LandingLinear::k(double t) const
{
    return KB * Math::sign(t - TT);
}

Vector LandingLinear::a(const Vector &x) const
{
    double e = exp(-BB * x[2]);
    Vector aa(m_dimX);

    aa[0] = -CC * x[0] * x[0] * e - GG * sin(x[1]);
    aa[1] = CC * k(m_time) * x[0] * e + cos(x[1]) * (x[0] / (RR + x[2]) - GG / x[0]);
    aa[2] = x[0] * sin(x[1]);

    return aa;
}

Matrix LandingLinear::B(const Vector & /*x*/) const
{
    return Matrix::Zero(3, 3);
}

Vector LandingLinear::tau(const Vector &m, const Matrix & /*D*/) const
{
    // tau (m, D, t) = a (m, t) :
    return a(m);
}

Matrix LandingLinear::Theta(const Vector & /*m*/, const Matrix & /*D*/) const
{
    return Matrix::Zero(3, 3);
}

Matrix LandingLinear::A(const Vector &m, const Matrix & /*D*/) const
{
    double e = exp(-BB * m[2]);
    Matrix aa(m_dimX, m_dimX);

    aa(0, 0) = -2.0 * CC * m[0] * e;
    aa(0, 1) = -GG * cos(m[1]);
    aa(0, 2) = BB * CC * m[0] * m[0] * e;

    aa(1, 0) = CC * k(m_time) * e + (GG / (m[0] * m[0]) + 1.0 / (RR + m[2])) * cos(m[1]);
    aa(1, 1) = (GG / m[0] - m[0] / (RR + m[2])) * sin(m[1]);
    aa(1, 2) = -CC * BB * k(m_time) * m[0] * e - m[0] * cos(m[1]) / ((RR + m[2]) * (RR + m[2]));

    aa(2, 0) = sin(m[1]);
    aa(2, 1) = m[0] * cos(m[1]);
    aa(2, 2) = 0.0;

    return aa;
}


Vector LandingLinear::c(const Vector &x) const
{
    double e = exp(-BB * x[2]);
    Vector w = gaussianVector(mw, Dw);
    Vector c(m_dimY);

    c[0] = CC * (w[0] + 1.0) * x[0] * x[0] * e * (cos(x[1]) - k(m_time) * sin(x[1])) + w[2];
    c[1] = CC * (w[1] + 1.0) * x[0] * x[0] * e * (sin(x[1]) - k(m_time) * cos(x[1])) + w[3];

    return c;
}

Vector LandingLinear::h(const Vector &m, const Matrix & /* D*/) const
{
    double e = exp(-BB * m[2]);
    Vector h(2); // WARNING (size = ?)

    h[0] = CC * m[0] * m[0] * e * (cos(m[1]) - k(m_time) * sin(m[1])) * (1.0 + mw[0]) + mw[2];
    h[1] = CC * m[0] * m[0] * e * (sin(m[1]) - k(m_time) * cos(m[1])) * (1.0 + mw[1]) + mw[3];

    return h;
}

Matrix LandingLinear::G(const Vector &m, const Matrix & /*D*/) const
{
    double e = exp(-BB * m[2]);
    Matrix g(2, 3); // WARNING (size = ?)

    g(0, 0) = 2 * CC * m[0] * (mw[0] + 1) * e * (cos(m[1]) - k(m_time) * sin(m[1]));
    g(0, 1) = -CC * m[0] * m[0] * (mw[0] + 1) * e * sin(m[1]);
    g(0, 2) = -BB * CC * m[0] * m[0] * (mw[0] + 1) * e * (cos(m[1]) - k(m_time) * sin(m[1]));

    g(1, 0) = 2 * CC * m[0] * (mw[1] + 1) * e * (sin(m[1]) - k(m_time) * cos(m[1]));
    g(1, 1) = CC * m[0] * m[0] * (mw[1] + 1) * e * cos(m[1]);
    g(1, 2) = -BB * CC * m[0] * m[0] * (mw[1] + 1) * e * (sin(m[1]) - k(m_time) * cos(m[1]));

    return g;
}

Matrix LandingLinear::F(const Vector &m, const Matrix &D) const
{
    double e  = exp(-BB * m[2]);
    Matrix cx = G(m, D);
    Matrix cw(2, 4); // WARNING (size = ?)

    cw(0, 0) = CC * m[0] * m[0] * e * (cos(m[1]) - k(m_time) * sin(m[1]));
    cw(0, 1) = 0.0;
    cw(0, 2) = 1.0;
    cw(0, 3) = 0.0;

    cw(1, 0) = 0.0;
    cw(1, 1) = CC * m[0] * m[0] * e * (sin(m[1]) - k(m_time) * cos(m[1]));
    cw(1, 2) = 0.0;
    cw(1, 3) = 1.0;

    return cx * D * cx.transpose() + cw * Dw * cw.transpose();
}


} // end Tasks::ContinuousDiscrete

} // end Tasks
