#ifndef STATISTIC_H
#define STATISTIC_H

#include "src/array.h"
#include "src/math/matrix.h"


namespace Math
{

/*!
\brief Модуль, содержащий описание и реализацию методов работы с моментными характеристиками СВ.
*/

namespace Statistic
{

//! Возвращает математическое ожидание по выборке случайных чисел.
double Mean(const Array<double> &sampleX);

//! Возвращает вектор математического ожидания по выборке случайных векторов.
Vector Mean(const Array<Vector> &sampleX);

//! Возвращает дисперсию по выборке случайных чисел и математическому ожиданию.
double Var(const Array<double> &sampleX, double meanX);

//! Возвращает дисперсию по выборке случайных чисел.
double Var(const Array<double> &sampleX);

//! Возвращает матрицу дисперсии по выборке случайных векторов и вектору математического ожиданию.
Matrix Var(const Array<Vector> &sampleX, const Vector &meanX);

//! Возвращает матрицу дисперсии по выборке случайных векторов.
Matrix Var(const Array<Vector> &sampleX);

//! Возвращает ковариацию двух случайных величин по их выборкам.
double Cov(const Array<double> &sampleX, const Array<double> &sampleY);

//! Возвращает матрицу ковариации двух случайных векторов по их выборкам.
Matrix Cov(const Array<Vector> &sampleX, const Array<Vector> &sampleY);

// Var (x) = Cov (x, x)

// TODO:
// double Skewnewss (); 3й момент - Коэффициент асимметрии
// double Kurtosis (); 4й момент - Коэффициент эксцесса (мера остроты пика распределения)

//! Возвращает медиану по выборке случайных чисел.
double Median(const Array<double> &sampleX);


} // end Statistic

} // end Math


#endif // STATISTIC_H
