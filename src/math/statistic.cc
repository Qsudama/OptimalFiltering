#include "statistic.h"
#include <algorithm>


namespace Math
{

namespace Statistic
{


double Mean(const Array<double> &sampleX)
{
    double sumX = 0.0;
    for (size_t i = 0; i < sampleX.size(); ++i) {
        sumX += sampleX[i];
    }
    return sumX / sampleX.size();
}

// Логико - динамика

double Mean(const Array<double> &sampleX, const Array<int> sampleI, int i)
{
    assert(sampleX.size() == sampleI.size());

    double sumXi   = 0.0;
    int    countXi = 0;
    for (size_t j = 0; j < sampleI.size(); ++j) {
        if (sampleI[j] == i) {
            sumXi += sampleX[j];
            ++countXi;
        }
    }
    return sumXi / countXi;
}

Vector Mean(const Array<Vector> &sampleX)
{
    Vector sumX = Vector::Zero(sampleX[0].size());
    for (size_t i = 0; i < sampleX.size(); ++i) {
        sumX += sampleX[i];
    }
    return sumX / sampleX.size();
}

// Логико - динамика

Vector Mean(const Array<Vector> &sampleX, const Array<int> sampleI, int i)
{
    assert(sampleX.size() == sampleI.size());

    Vector sumXi   = Vector::Zero(sampleX[0].size());
    int    countXi = 0;
    for (size_t j = 0; j < sampleI.size(); ++j) {
        if (sampleI[j] == i) {
            sumXi += sampleX[j];
            ++countXi;
        }
    }
    return sumXi / countXi;
}

double Var(const Array<double> &sampleX, double meanX)
{
    double sum = 0.0;
    for (size_t i = 0; i < sampleX.size(); ++i) {
        sum += pow(sampleX[i] - meanX, 2.0);
    }
    return sum / (sampleX.size() - 1);
}

// Логико - динамика

double Var(const Array<double> &sampleX, double meanXi, const Array<int> sampleI, int i)
{
    assert(sampleX.size() == sampleI.size());

    double sumXi = 0.0;
    int    count = 0;
    for (size_t j = 0; j < sampleI.size(); ++j) {
        if (sampleI[j] == i) {
            sumXi += pow(sampleX[j] - meanXi, 2.0);
            ++count;
        }
    }
    return sumXi / (count - 1);
}

double Var(const Array<double> &sampleX)
{
    double meanX = Mean(sampleX);
    return Var(sampleX, meanX);
}

double Var(const Array<double> &sampleX, const Array<int> sampleI, int i)
{
    assert(sampleX.size() == sampleI.size());

    double meanXi = Mean(sampleX, sampleI, i);
    return Var(sampleX, meanXi, sampleI, i);
}

Matrix Var(const Array<Vector> &sampleX, const Vector &meanX)
{
    assert(sampleX[0].size() == meanX.size());

    Matrix sumXX = Matrix::Zero(sampleX[0].size(), sampleX[0].size());
    for (size_t i = 0; i < sampleX.size(); ++i) {
        sumXX += sampleX[i] * sampleX[i].transpose();
    }
    return sumXX / sampleX.size() - meanX * meanX.transpose();
}

// Логико - динамика

Matrix Var(const Array<Vector> &sampleX, const Vector &meanX, const Array<int> sampleI, int i)
{
    assert(sampleX.size() == sampleI.size());
    assert(sampleX[0].size() == meanX.size());

    Matrix sumXX = Matrix::Zero(sampleX[0].size(), sampleX[0].size());
    int    count = 0;
    for (size_t j = 0; j < sampleX.size(); ++j) {
        if (sampleI[j] == i) {
            sumXX += sampleX[j] * sampleX[j].transpose();
            ++count;
        }
    }
    return sumXX / count - meanX * meanX.transpose();
}

Matrix Var(const Array<Vector> &sampleX)
{
    Vector meanX = Mean(sampleX);
    return Var(sampleX, meanX);
}

// Логико - динамика

Matrix Var(const Array<Vector> &sampleX, const Array<int> sampleI, int i)
{
    Vector meanXi = Mean(sampleX, sampleI, i);
    return Var(sampleX, meanXi, sampleI, i);
}

double Cov(const Array<double> &sampleX, const Array<double> &sampleY)
{
    assert(sampleX.size() == sampleY.size());

    size_t size  = sampleX.size();
    double sumX  = 0.0;
    double sumY  = 0.0;
    double sumXY = 0.0;
    for (size_t i = 0; i < size; ++i) {
        sumX += sampleX[i];
        sumY += sampleY[i];
        sumXY += sampleX[i] * sampleY[i];
    }
    return sumXY / size - (sumX / size) * (sumY / size);
}

// Логико - динамика

double Cov(const Array<double> &sampleX, const Array<double> &sampleY, const Array<int> sampleI,
           int i)
{
    assert(sampleX.size() == sampleI.size());
    assert(sampleY.size() == sampleI.size());

    double meanXi = Mean(sampleX, sampleI, i);
    double meanYi = Mean(sampleY, sampleI, i);

    return Cov(sampleX, sampleY, meanXi, meanYi, sampleI, i);
}

// Логико - динамика

double Cov(const Array<double> &sampleX, const Array<double> &sampleY, const double &meanX,
           const double &meanY, const Array<int> sampleI, int i)
{
    assert(sampleX.size() == sampleI.size());
    assert(sampleY.size() == sampleI.size());

    double res = 0.0;
    int count = 0;
    for (size_t j = 0; j < sampleI.size(); ++j) {
        if (sampleI[j] == i) {
            double cov = (sampleX[j] - meanX)*(sampleY[j] - meanY);
            res += cov;
            ++count;
        }
    }
    return res/(count - 1);
}

Matrix Cov(const Array<Vector> &sampleX, const Array<Vector> &sampleY)
{
    assert(sampleX.size() == sampleY.size());

    size_t size  = sampleX.size();
    long   dimX  = sampleX[0].size();
    long   dimY  = sampleY[0].size();
    Vector sumX  = Vector::Zero(dimX);
    Vector sumY  = Vector::Zero(dimY);
    Matrix sumXY = Matrix::Zero(dimX, dimY);
    for (size_t i = 0; i < size; ++i) {
        sumX += sampleX[i];
        sumY += sampleY[i];
        sumXY += sampleX[i] * sampleY[i].transpose();
    }
    return sumXY / size - (sumX / size) * (sumY / size).transpose();
}

// Логико - динамика

Matrix Cov(const Array<Vector> &sampleX, const Array<Vector> &sampleY, const Array<int> sampleI,
           int i)
{
    assert(sampleX.size() == sampleI.size());
    assert(sampleY.size() == sampleI.size());

    Vector meanXi = Mean(sampleX, sampleI, i);
    Vector meanYi = Mean(sampleY, sampleI, i);

    return Cov(sampleX, sampleY, meanXi, meanYi, sampleI, i);
}

// Логико - динамика

Matrix Cov(const Array<Vector> &sampleX, const Array<Vector> &sampleY, const Vector &meanXi,
           const Vector &meanYi,  const Array<int> sampleI, int i)
{
    assert(sampleX.size() == sampleI.size());
    assert(sampleY.size() == sampleI.size());

    long   dimX  = sampleX[0].size();
    long   dimY  = sampleY[0].size();
    Matrix sumXY = Matrix::Zero(dimX, dimY);
    int count = 0;
    for (size_t j = 0; j < sampleI.size(); ++j) {

        if (sampleI[j] == i) {
            sumXY += sampleX[j] * sampleY[j].transpose();
            ++count;
        }
    }
    return sumXY / count - meanXi * meanYi.transpose();
}

double Median(const Array<double> &sampleX)
{
    if (sampleX.size() == 1) {
        return sampleX[0];
    }

    Array<double> tmp = sampleX;
    std::sort(tmp.begin(), tmp.end());
    if (tmp.size() % 2 == 0) {
        return 0.5 * (tmp[tmp.size() / 2] + tmp[tmp.size() / 2 + 1]);
    }
    return tmp[tmp.size() / 2 + 1];
}


} // end Stat

} // end Math
