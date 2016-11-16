#ifndef RAND_H
#define RAND_H

#include "linear_algebra.h"
#include "matrix.h"
#include "src/config.h"
#include <cstdlib>
#include <ctime>


namespace Math
{

/*!
\brief Модуль, содержащий описание и реализацию методов генерации случайных величин.
*/

namespace Rand
{


//! Устонавливает случайную последовательность для генерации СВ.
void setRandomize();

//! Позволяет создавать повторяющиеся последовательности для генерации СВ.
void setRandSeed(uint seed);

//! Возвращает значение \f$ x \sim U(0,1)\f$ (равномерное распределение).
double uniform();

//! Возвращает значение \f$ x \sim U(a,b)\f$ (равномерное распределение).
double uniform(double a, double b);

//! Возвращает значение \f$ x \sim N(0,1)\f$ (гауссовское распределение).
double gaussian();

//! Возвращает значение \f$ x \sim N(m,s^2)\f$ (гауссовское распределение).
double gaussian(double m, double s);

//! Возвращает значение \f$ x \sim lnN(m,s^2)\f$ (логнормальное распределение).
double logNormal(double m, double s);

//! Возвращает значение \f$ x \sim \chi^2(k)\f$ (распределение хи-квадрат).
double chiSquared(uint k);

//! Возвращает значение \f$ x \sim t(n)\f$ (распределение Стьюдента).
double students(uint n);

//! Возвращает значение \f$ x \sim F(n_1,n_2)\f$ (распределение Фишера).
double fishers(uint n1, uint n2);

//! Возвращает значение \f$ x \sim R(s)\f$ (распределение Рэлея).
double rayleighs(double s);

//! Возвращает значение \f$ x \sim Exp(l)\f$ (экспоненциальное распределение).
double exponential(double l);


//! Возвращает вектор размерности \f$n\f$ c независимыми компонентами, распределенными как \f$x_i \sim N(0,1)\f$.
Vector gaussianVector(long n);

//! Возвращает вектор размерности \f$n\f$ c независимыми компонентами, распределенными как \f$x_i \sim N(m,s^2)\f$.
Vector gaussianVector(long n, double m, double s);

//! Возвращает вектор размерности \f$n\f$ с математическим ожаданием \f$n\f$ и дисперсией \f$D\f$ (компоненты
//! зависимые).
Vector gaussianVector(const Vector &m, const Matrix &D);


} // end Rand

} // end Math


#endif // RAND_H
