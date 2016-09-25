#include "random.h"
#include "constants.h"


namespace Math
{

namespace Rand
{


void setRandomize()
{
    uint seed = uint(std::time(0));
    std::srand(seed);
}

void setRandSeed(uint seed)
{
    std::srand(seed);
}


double uniform() // in (0,1]
{
    return double(rand() + 1.0) / double(RAND_MAX + 1.0);
}

double uniform(double a, double b) // in [a,b]
{
    return (b - a) * uniform() + a;
}

double gaussian() // ~ N(0,1)
{
    double x = uniform();
    double y = uniform();
    return cos(2.0 * Math::Const::PI * y) * sqrt(-2.0 * log(x));
}

double gaussian(double m, double s) // ~ N (m,s^2)
{
    return m + s * gaussian();
}

double logNormal(double m, double s)
{
    return exp(gaussian(m, s));
}

double chiSquared(uint K)
{
    double sum_z = 0;
    for (uint i = 0; i < K; ++i) {
        sum_z += pow(gaussian(), 2);
    }
    return sum_z;
}

double students(uint N)
{
    double y0 = gaussian();
    double y  = chiSquared(N);
    return y0 * sqrt(double(N) / y);
}

double fishers(uint N1, uint N2)
{
    double y1 = chiSquared(N1);
    double y2 = chiSquared(N2);
    return double(N2) * y1 / (double(N1) * y2);
}

double rayleighs(double s)
{
    double x = gaussian(0, s);
    double y = gaussian(0, s);
    return sqrt(x * x + y * y);
}

double exponential(double L)
{
    return (-1.0 / L) * log(uniform());
}

Vector gaussianVector(long n)
{
    Vector v(n);
    for (long i = 0; i < n; ++i) {
        v[i] = gaussian();
    }
    return v;
}

Vector gaussianVector(long n, double m, double s)
{
    Vector v(n);
    for (long i = 0; i < n; ++i) {
        v[i] = gaussian(m, s);
    }
    return v;
}


Vector gaussianVector(const Vector &mean, const Matrix &cov)
{
    assert(cov.rows() == mean.size());
    return mean + LinAlg::Cholesky(cov) * gaussianVector(mean.size());
}


} // end Rand

} // end Math
