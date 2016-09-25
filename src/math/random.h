#ifndef RAND_H
#define RAND_H

#include "linear_algebra.h"
#include "matrix.h"
#include "src/config.h"
#include <cstdlib>
#include <ctime>


namespace Math
{

namespace Rand
{


void setRandomize();
void setRandSeed(uint seed);

// случайные числа:
double uniform();
double uniform(double a, double b);
double gaussian();
double gaussian(double m, double s);
double logNormal(double m, double s);
double chiSquared(uint k);
double students(uint n);
double fishers(uint n1, uint n2);
double rayleighs(double s);
double exponential(double l);

// случайные векторы:
Vector gaussianVector(long n);
Vector gaussianVector(long n, double m, double s);
Vector gaussianVector(const Vector &m, const Matrix &D);


} // end Rand

} // end Math


#endif // RAND_H
