#include "d_landing_linear.h"
#include "src/math/convert.h"


namespace Tasks
{

namespace Discrete
{


using Math::Rand::gaussianVector;
using Math::Convert::DegToRad;


LandingLinear::LandingLinear()
    : DiscreteTask()
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
    Vector dx(m_dimX);

    dx[0] = -CC * x[0] * x[0] * e - GG * sin(x[1]);
    dx[1] = CC * k(m_time) * x[0] * e + cos(x[1]) * (x[0] / (RR + x[2]) - GG / x[0]);
    dx[2] = x[0] * sin(x[1]);

    return x + m_step * dx;
}

Vector LandingLinear::b(const Vector &x) const
{
    double e = exp(-BB * x[2]);
    Vector w = gaussianVector(mw, Dw);
    Vector y(m_dimY);

    y[0] = CC * (w[0] + 1.0) * x[0] * x[0] * e * (cos(x[1]) - k(m_time) * sin(x[1])) + w[2];
    y[1] = CC * (w[1] + 1.0) * x[0] * x[0] * e * (sin(x[1]) - k(m_time) * cos(x[1])) + w[3];

    return y;
}

Vector LandingLinear::tau(const Vector &z, const Matrix & /*D*/) const
{
    return a(z);
}

Matrix LandingLinear::Theta(const Vector &z, const Matrix &P) const
{
    Matrix Ax = dadx(z);
    Matrix Av = dadv(z);

    return Ax * P * Ax.transpose() + Av * Dv * Av.transpose();
}

Vector LandingLinear::h(const Vector &m, const Matrix & /* D*/) const
{
    double e = exp(-BB * m[2]);
    Vector hh(m_dimY);

    hh[0] = CC * (mw[0] + 1.0) * m[0] * m[0] * e * (cos(m[1]) - k(m_time) * sin(m[1])) + mw[2];
    hh[1] = CC * (mw[1] + 1.0) * m[0] * m[0] * e * (sin(m[1]) - k(m_time) * cos(m[1])) + mw[3];

    return hh;
}

Matrix LandingLinear::G(const Vector &m, const Matrix & /*D*/) const
{
    return dbdx(m);
}

Matrix LandingLinear::F(const Vector &m, const Matrix &D) const
{
    Matrix Bx = dbdx(m);
    Matrix Bw = dbdw(m);

    return Bx * D * Bx.transpose() + Bw * Dw * Bw.transpose();
}

Matrix LandingLinear::dadx(const Vector &x) const
{
    double e = exp(-BB * x[2]);
    Matrix da(m_dimX, m_dimX);

    da(0, 0) = 1.0 - 2.0 * m_step * CC * x[0] * e;
    da(0, 1) = -m_step * GG * cos(x[1]);
    da(0, 2) = m_step * BB * CC * x[0] * x[0] * e;

    da(1, 0) = m_step * (CC * k(m_time) * e + (GG / (x[0] * x[0]) + 1.0 / (RR + x[2])) * cos(x[1]));
    da(1, 1) = 1.0 + m_step * ((GG / x[0] - x[0] / (RR + x[2])) * sin(x[1]));
    da(1, 2) = m_step * (-CC * BB * k(m_time) * x[0] * e - x[0] * cos(x[1]) / ((RR + x[2]) * (RR + x[2])));

    da(2, 0) = m_step * sin(x[1]);
    da(2, 1) = m_step * x[0] * cos(x[1]);
    da(2, 2) = 1.0;

    return da;
}

Matrix LandingLinear::dadv(const Vector & /*x*/) const
{
    return Matrix::Zero(m_dimX, m_dimV);
}

Matrix LandingLinear::dbdx(const Vector &x) const
{
    double e0 = CC * x[0] * x[0] * exp(-BB * x[2]) * (cos(x[1]) - k(m_time) * sin(x[1]));
    double e1 = CC * x[0] * x[0] * exp(-BB * x[2]) * (sin(x[1]) - k(m_time) * cos(x[1]));
    Matrix db(m_dimY, m_dimX);

    db(0, 0) = 2 * e0 * (1.0 + mw[0]) / x[0];
    db(0, 1) = -e1 * (1.0 + mw[0]);
    db(0, 2) = -BB * e0 * (1.0 + mw[0]);

    db(1, 0) = 2 * e1 * (1.0 + mw[1]) / x[0];
    db(1, 1) = e0 * (1.0 + mw[1]);
    db(1, 2) = -BB * e1 * (1.0 + mw[1]);

    return db;
}

Matrix LandingLinear::dbdw(const Vector &x) const
{
    Matrix db(m_dimY, m_dimW);

    db(0, 0) = CC * x[0] * x[0] * exp(-BB * x[2]) * (cos(x[1]) - k(m_time) * sin(x[1]));
    db(0, 1) = 0.0;
    db(0, 2) = 1.0;
    db(0, 3) = 0.0;

    db(1, 0) = 0.0;
    db(1, 1) = CC * x[0] * x[0] * exp(-BB * x[2]) * (sin(x[1]) - k(m_time) * cos(x[1]));
    db(1, 2) = 0.0;
    db(1, 3) = 1.0;

    return db;
}


} // end Tasks::Discrete

} // end Tasks
