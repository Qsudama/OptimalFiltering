#include "cd_scalar_linear.h"

namespace Tasks
{

namespace ContinuousDiscrete
{


ScalarLinear::ScalarLinear()
    : ContinuousDiscreteTask()
{
    m_info->setName("Тестовый скалярный пример");
    m_info->setType("Л-");

    m_dimY = 1;

    m_dimX      = 1;
    m_meanX0    = Vector(m_dimX);
    m_meanX0[0] = 0;
    m_varX0 = Matrix::Zero(m_dimX, m_dimX);
    m_varX0(0, 0) = 1.0;

    m_dimV  = 1;
    m_meanV = Vector::Zero(m_dimV);
    m_varV = Matrix::Zero(m_dimV, m_dimV);
    m_varV(0, 0) = 1.0;

    m_dimW  = 1;
    m_meanW = Vector::Zero(m_dimW);
    m_varW = Matrix::Zero(m_dimW, m_dimW);
    m_varW(0, 0) = 1.0;

    /*(*m_consts)["a"] = aa;
    (*m_consts)["b"] = bb;
    (*m_consts)["c"] = cc;
    (*m_consts)["d"] = dd;
    (*m_consts)["e"] = ee;*/

    (*m_consts)["a1"] = a1;
    (*m_consts)["a3"] = a3;
    (*m_consts)["b0"] = b0;
    (*m_consts)["c1"] = c1;
    (*m_consts)["c2"] = c2;
    (*m_consts)["d0"] = d0;
    (*m_consts)["d1"] = d1;
}

void ScalarLinear::loadParams()
{

}

Vector ScalarLinear::a(const Vector &x) const
{
    Vector res(m_dimX);
    res[0] = a1 * x[0] + a3 * x[0] * x[0] * x[0] ;

    return res;
}

Matrix ScalarLinear::B(const Vector &x) const
{
    Matrix res = Matrix::Zero(m_dimX, m_dimV);
    res(0, 0) = b0;

    return res;
}

Vector ScalarLinear::tau(const Vector &m, const Matrix & /*D*/) const
{
    Vector res(m_dimX);
    res[0] = a1 * m[0] + a3 * m[0] * m[0] * m[0] ;

    return res;
}

Matrix ScalarLinear::Theta(const Vector &m, const Matrix & D) const
{
    Matrix res(m_dimX, m_dimX);

   //res(0, 0) =  (a1 + 3 * a3 * m[0] * m[0]) * (a1 + 3 * a3 * m[0] * m[0]) * D(0, 0)  + b0 * b0;
    res(0, 0) =  b0 * b0;

   return res;
}

Matrix ScalarLinear::A(const Vector &m, const Matrix & /*D*/) const
{
    Matrix res(m_dimX, m_dimX);

    res(0, 0) = a1 + 3 * a3 * m[0] * m[0];

    return res;
}


Vector ScalarLinear::c(const Vector &x, double measurementStep) const
{
    /*Vector w = Math::sqrt(measurementStep) * m_normalRand(m_meanW, m_varW); // СКО, а не Дисперсия
    Vector res(m_dimX);
    res[0] = (c1 * x[0] + c2 * x[0] * x[0]) * measurementStep + d0* w[0] + d1* x[0]* w[0];*/

    Vector w = 1.0 / Math::sqrt(measurementStep) * m_normalRand(m_meanW, m_varW); // СКО, а не Дисперсия
    Vector res(m_dimX);
    res[0] = (c1 * x[0] + c2 * x[0] * x[0]) + d0* w[0] + d1* x[0]* w[0];

    return res;
}

Vector ScalarLinear::h(const Vector &m, const Matrix & /* D*/, double measurementStep) const
{   Vector res(m_dimX);
     //res[0] = (c1 * m[0] + c2 * m[0] * m[0]) * measurementStep; // + m_meanW;
    res[0] = (c1 * m[0] + c2 * m[0] * m[0]);
    return res;
}

Matrix ScalarLinear::G(const Vector & m, const Matrix & /*D*/, double measurementStep) const
{
    Matrix res = Matrix::Zero(m_dimX, m_dimX); // WARNING (size = ?)

    //res(0, 0) = (c1 + 2 * c2 * m[0]) * measurementStep;
    res(0, 0) = (c1 + 2 * c2 * m[0]);

    return res;
}

Matrix ScalarLinear::F(const Vector & m, const Matrix &D, double measurementStep) const
{
    Matrix res(m_dimX, m_dimX);
    /*res(0, 0) = (c1 + 2 * c2 * m[0]) * (c1 + 2 * c2 * m[0]) * D(0, 0)* measurementStep * measurementStep
                + (d0 + d1 * m[0])*(d0 + d1 * m[0])*measurementStep;//+ m_varW;*/

    res(0, 0) = (c1 + 2 * c2 * m[0]) * (c1 + 2 * c2 * m[0]) * D(0, 0)
                + (d0 + d1 * m[0])*(d0 + d1 * m[0])/measurementStep;
    return res;
}


} // end Tasks::Continuous

} // end Tasks
