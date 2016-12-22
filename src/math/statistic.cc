#include "statistic.h"
#include <algorithm>


namespace Math
{

namespace Statistic
{


double Mean(const Array<double> &sampleX)
{
    size_t size = sampleX.size();
    double sumX = 0.0;
    for (size_t i = 0; i < size; ++i) {
        sumX += sampleX[i];
    }
    return sumX / size;
}

Vector Mean(const Array<Vector> &sampleX)
{
    size_t size = sampleX.size();
    long   dim  = sampleX[0].size();
    Vector sumX = Vector::Zero(dim);
    for (size_t i = 0; i < size; ++i) {
        sumX += sampleX[i];
    }
    return sumX / size;
}

double Var(const Array<double> &sampleX, double meanX)
{
    size_t size = sampleX.size();
    double sum  = 0.0;
    for (size_t i = 0; i < size; ++i) {
        sum += pow(sampleX[i] - meanX, 2.0);
    }
    return sum / (size - 1);
}

double Var(const Array<double> &sampleX)
{
    double meanX = Mean(sampleX);
    return Var(sampleX, meanX);
}

double Cov(const Array<double> &sampleX, const Array<double> &sampleY)
{
    assert(sampleX.size() == sampleY.size() && "Math::Statistic::Cov(sampleX, sampleY) : different sizes of arrays");

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

Matrix Var(const Array<Vector> &sampleX, const Vector &meanX)
{
    size_t size = sampleX.size();
    long   dimX = sampleX[0].size();

    assert(dimX == meanX.size() &&
           "Math::Statistic::Var(sampleX, meanX) : corrupt dimension of meanX (or array's elements)");

    Matrix sumXX = Matrix::Zero(dimX, dimX);
    for (size_t i = 0; i < size; ++i) {
        sumXX += sampleX[i] * sampleX[i].transpose();
    }
    return sumXX / size - meanX * meanX.transpose();
}

Matrix Var(const Array<Vector> &sampleX)
{
    Vector meanX = Mean(sampleX);
    return Var(sampleX, meanX);
}

Matrix Cov(const Array<Vector> &sampleX, const Array<Vector> &sampleY)
{
    assert(sampleX.size() == sampleY.size() && "Math::Statistic::Cov(sampleX, sampleY) : different sizes of arrays");

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
