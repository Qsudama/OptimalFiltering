#ifndef RAND_H
#define RAND_H

#include "linear_algebra.h"
#include "matrix.h"
#include "src/config.h"
#include <chrono>
#include <random>


namespace Math
{

//!
//! \brief Статический класс, помогающий управлять генератором ПСЧ.
//!
class RandomProperties
{
public:
    //! \brief Возвращает стартовое число ГПСЧ по-умолчанию.
    //! \details Оно равно половине наибольшего unsigned long.
    static Uint defaultSeed();

    //! \brief Возвращает стартовое число ГПСЧ, которое берется из системного времени.
    //! \details Оно равно количеству наносекунд, прошедших с 01.01.1970.
    static Uint randomSeed();
};


//!
//! \brief Класс-генератор случайных нормальных(гауссовских) векторов.
//!
class MultivariateNormalDistribution
{
public:
    //! Конструктор.
    MultivariateNormalDistribution();

    //! Установливает число, на основании которого генерируется последовательсть ПСЧ.
    void setSeed(Uint seed);

    //! Возвращает гауссовский случайный вектор размерности dim c независимыми компонентами.
    Vector operator()(long dim) const;

    //! Возвращает гауссовский случайный вектор \f$x \sim N(m, D)\f$ (компоненты зависимые).
    //! \param mean - математическое ожидание.
    //! \param var - дисперсия
    //! \details Вектор \f$x = m + C * g\f$, где
    //!
    //! \f$m\f$ - математическое ожидание;
    //!
    //! \f$C\f$ - результат разложения Холецкого матрицы дисперсии;
    //!
    //! \f$g\f$ - гауссвоский случайный вектор с независимыми коорданатами (\f$g_i \sim N(0,1)\f$).
    Vector operator()(const Vector &mean, const Matrix &var) const;


private:
    Vector normal01(long dim) const;


private:
#if defined(ARCHITECTURE_64)
    mutable std::mt19937_64 m_generator;
#else
    mutable std::mt19937 m_generator;
#endif

    mutable std::normal_distribution<double> m_univariateNormalDistribution;
};


} // end Math


#endif // RAND_H
