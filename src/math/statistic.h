#ifndef STATISTIC_H
#define STATISTIC_H

#include "src/array.h"
#include "src/math/matrix.h"


namespace Math
{


namespace Statistic
{


double Mean(const Array<double> &sampleX);
Vector Mean(const Array<Vector> &sampleX);

double Var(const Array<double> &sampleX, double meanX);
double Var(const Array<double> &sampleX);
Matrix Var(const Array<Vector> &sampleX, const Vector &meanX);
Matrix Var(const Array<Vector> &sampleX);

double Cov(const Array<double> &sampleX, const Array<double> &sampleY);
Matrix Cov(const Array<Vector> &sampleX, const Array<Vector> &sampleY);

// Var (x) = Cov (x, x)

// TODO:
// double Skewnewss (); 3й момент - Коэффициент асимметрии
// double Kurtosis (); 4й момент - Коэффициент эксцесса (мера остроты пика распределения)

double Median(const Array<double> &sampleX);


} // end Statistic

} // end Math


#endif // STATISTIC_H
