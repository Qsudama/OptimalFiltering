#include "ld_scalar_rejection_gauss.h"
#include "src/math/convert.h"
#include "src/math/matrix.h"
#include "iostream"
#include "random"

using Math::MakeBlockVector;
using Math::MakeBlockMatrix;

namespace Tasks
{

namespace LogicDynamic
{

int count_k = 0;

using Math::Convert::DegToRad;

ScalarRejectionGauss::ScalarRejectionGauss()
    : LogicDynamicTask()
    , m_e(0.2)
    , m_cI2(10)
    , countIInTask(2)
{
    m_info->setName("Скалярный пример со сбоями измерителя");
    m_info->setType("Г-");

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

    (*m_params)["a1"] = A1;
    (*m_params)["a2"] = A2;

    (*m_params)["e"] = m_e;
    (*m_params)["с(2)"] = m_cI2;
    (*m_params)["Кол-во режимов I"] = countIInTask;
}

void ScalarRejectionGauss::loadParams()
{
    A1         = m_params->at("a1");
    A2         = m_params->at("a2");
    countI     = m_params->at("Кол-во режимов I");
    m_e        = m_params->at("e");
    m_cI2        = m_params->at("с(2)");
}

double ScalarRejectionGauss::C(int i) const
{
    if (i == 1) {
        return 1.0;
    } else {
        return m_cI2;
    }
}

Vector ScalarRejectionGauss::a(int /*i*/, const Vector &x) const
{
    Vector dx(m_dimX);
    Vector v  = m_normalRand(m_meanV, m_varV);
    dx[0] = A1 * x[0] + A2 * v[0];

    return dx;
}

Vector ScalarRejectionGauss::b(int i, const Vector &x) const
{
    Vector w  = m_normalRand(m_meanW, m_varW);
    Vector res(m_dimY);
    double c2 = C(i);

    res[0] = x[0] + c2 * w[0];

    return res;
}

double ScalarRejectionGauss::A(int i, int l) const
{
    double e = m_e;
    double p = 1 - e;

    Matrix A(2, 2);
    A << p, p, e, e;

    return A(i - 1, l - 1);
}

double ScalarRejectionGauss::nu(int i, int l, const Vector &/*m*/, const Matrix &/*D*/) const {
    return A(i, l);
}

Vector ScalarRejectionGauss::tau(int i, int l, const Vector &z, const Matrix &/*P*/) const
{
    Vector res = Vector::Zero(z.size());
    res[0] = A(i, l) * A1 * z[0];
    return res;
}

Matrix ScalarRejectionGauss::Theta(int i, int l, const Vector &z, const Matrix &P) const
{
    Matrix m = Matrix::Zero(P.rows(), P.cols());
    m(0, 0) = z[0]*z[0];
    Matrix aa2 = Matrix::Zero(P.rows(), P.cols());
    aa2(0, 0) = A2*A2;

    Matrix res = Matrix::Zero(P.rows(), P.cols());
    Matrix sum = P + m;
    Matrix SSS = Matrix::Zero(P.rows(), P.cols());
    SSS(0, 0) = A1 * sum(0, 0);
    res = A(i, l) * (SSS + aa2);
    return res;
}

Vector ScalarRejectionGauss::h(int /*i*/, const Vector &m, const Matrix & /* D*/) const
{
    return m;
}

Matrix ScalarRejectionGauss::G(int /*i*/, const Vector &m, const Matrix &D) const
{
    Matrix mm = Matrix::Zero(D.rows(), D.cols());
    mm(0, 0) = m[0]*m[0];
    Matrix res = Matrix::Zero(D.rows(), D.cols());
    res = mm + D;
    return res;
}

Matrix ScalarRejectionGauss::F(int i, const Vector &m, const Matrix &D) const
{
    Matrix mm = Matrix::Zero(D.rows(), D.cols());
    mm(0, 0) = m[0]*m[0];
    Matrix cPow2 = Matrix::Zero(D.rows(), D.cols());
    double cc = C(i);
    cPow2(0, 0) = cc*cc;

    Matrix res = Matrix::Zero(D.rows(), D.cols());
    res = D + mm + cPow2;

    return res;
}

Matrix ScalarRejectionGauss::dadx(int /*i*/, const Vector &/*x*/) const
{
    Matrix res = Matrix::Zero(m_dimX, m_dimX);
    res(0, 0) = A1;
    return res;
}

Matrix ScalarRejectionGauss::dadv(int /*i*/, const Vector & /*x*/) const
{
    Matrix res = Matrix::Zero(m_dimX, m_dimX);
    res(0, 0) = A2;
    return res;
}

Matrix ScalarRejectionGauss::dbdx(int /*i*/, const Vector &/*x*/) const
{
    Matrix res = Matrix::Zero(m_dimX, m_dimX);
    res(0, 0) = 1;
    return res;
}

Matrix ScalarRejectionGauss::dbdw(int i, const Vector &/*x*/) const
{
    Matrix res = Matrix::Zero(m_dimX, m_dimX);
    res(0, 0) = C(i);
    return res;
}

double ScalarRejectionGauss::Pr(int i) const
{
    if (i == 1) {
        return 1 - m_e;
    } else {
        return m_e;
    }
}

//Array<int> ScalarRejectionGauss::generateArrayI(int sizeS) const
//{
//    Array<int> array(sizeS);
//    double p = 1.0 - m_e;
//    int countI1;
//    countI1 = sizeS*p;
//    for (int i = 0; i < sizeS; i++) {
//        if (i < countI1) {
//            array[i] = 1;
//        } else {
//            array[i] = 2;
//        }
//    }
//    for (int i = 0; i < array.size(); i++) {
//        std::swap(array[i], array[rand() % sizeS]);
//    }

//    count_k++;
//    logInstance.logStringWithQDebug("НОМЕР: " + std::to_string(count_k));
//    logInstance.logArrayWithQDebug(array);

//    return array;
//}

Array<int> ScalarRejectionGauss::generateArrayI(int sizeS, int k) const
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

//    logInstance.logStringWithQDebug("НОМЕР: " + std::to_string(k));
//    logInstance.logArrayWithQDebug(array);

    return array;
}

} // end Tasks::LogicDynamic

} // end Tasks
