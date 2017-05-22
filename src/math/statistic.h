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

//! Возвращает математическое ожидание для i-го режима (многорежимные фильтры).
double Mean(const Array<double> &sampleX, const Array<int> sampleI, int i);

//! Возвращает вектор математического ожидания по выборке случайных векторов.
Vector Mean(const Array<Vector> &sampleX);

//! Возвращает вектор математического ожидания для i-го режима (многорежимные фильтры).
Vector Mean(const Array<Vector> &sampleX, const Array<int> sampleI, int i);

//! Возвращает дисперсию по выборке случайных чисел и математическому ожиданию.
double Var(const Array<double> &sampleX, double meanX);

//! Возвращает дисперсию для i-го режима (многорежимные фильтры).
double Var(const Array<double> &sampleX, double meanXi, const Array<int> sampleI, int i);

//! Возвращает дисперсию по выборке случайных чисел.
double Var(const Array<double> &sampleX);

//! Возвращает дисперсию для i-го режима (многорежимные фильтры).
double Var(const Array<double> &sampleX, const Array<int> sampleI, int i);

//! Возвращает матрицу дисперсии по выборке случайных векторов и вектору математического ожиданию.
Matrix Var(const Array<Vector> &sampleX, const Vector &meanX);

//! Возвращает матрицу дисперсии для i-го режима (многорежимные фильтры).
Matrix Var(const Array<Vector> &sampleX, const Vector &meanX, const Array<int> sampleI, int i);

//! Возвращает матрицу дисперсии по выборке случайных векторов.
Matrix Var(const Array<Vector> &sampleX);

//! Возвращает матрицу дисперсии для i-го режима (многорежимные фильтры).
Matrix Var(const Array<Vector> &sampleX, const Array<int> sampleI, int i);

//! Возвращает ковариацию двух случайных величин по их выборкам.
double Cov(const Array<double> &sampleX, const Array<double> &sampleY);

//! Возвращает ковариацию двух случайных величин для i-го режима (многорежимные фильтры).
double Cov(const Array<double> &sampleX, const Array<double> &sampleY, const Array<int> sampleI,
           int i);

//! Возвращает матрицу ковариации двух случайных векторов по их выборкам.
Matrix Cov(const Array<Vector> &sampleX, const Array<Vector> &sampleY);

//! Возвращает матрицу ковариации двух случайных векторов для i-го режима (многорежимные фильтры).
Vector Cov(const Array<Vector> &sampleX, const Array<Vector> &sampleY, const Array<int> sampleI,
           int i);

// Var (x) = Cov (x, x)


//! Возвращает медиану по выборке случайных чисел.
double Median(const Array<double> &sampleX);


} // end Statistic

} // end Math


#endif // STATISTIC_H
