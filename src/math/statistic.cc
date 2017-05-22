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

double Cov(const Array<double> &sampleX, const Array<double> &sampleY, const Array<int> sampleI,
           int i)
{
    assert(sampleX.size() == sampleI.size());
    assert(sampleY.size() == sampleI.size());

    Array<double> tmpX, tmpY;
    for (size_t j = 0; j < sampleI.size(); ++j) {
        if (sampleI[j] == i) {
            tmpX.push_back(sampleX[j]);
            tmpY.push_back(sampleY[j]);
        }
    }

    return Cov(tmpX, tmpY);
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

Vector Cov(const Array<Vector> &sampleX, const Array<Vector> &sampleY, const Array<int> sampleI,
           int i)
{
    assert(sampleX.size() == sampleI.size());
    assert(sampleY.size() == sampleI.size());

    Array<Vector> tmpX, tmpY;
    for (size_t j = 0; j < sampleI.size(); ++j) {
        if (sampleI[j] == i) {
            tmpX.push_back(sampleX[j]);
            tmpY.push_back(sampleY[j]);
        }
    }

    return Cov(tmpX, tmpY);
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
