#ifndef CONTINUOUS_LANDING_LINEAR_H
#define CONTINUOUS_LANDING_LINEAR_H

#include "src/core/continuous_task.h"
#include "src/math/math.h"


namespace Tasks
{

//! Подмодуль, содержащий реализации задач для непрерывных фильтров оптимальной структуры.

namespace Continuous
{

/*!
 * \brief Задача спуска ЛА на планету (непрерывная, линеаризованная) для фильтров оптимальной структуры.
 */

class LandingLinear : public Core::ContinuousTask
{

public:
    //! \brief Конструктор.
    LandingLinear();

    Vector a(const Vector &x) const override;
    Matrix B(const Vector &x) const override;
    Vector c(const Vector &x) const override;
    Matrix D(const Vector &x) const override;
    Matrix A(const Vector &m, const Matrix &D) const override;
    Matrix G(const Vector &m, const Matrix &D) const override;


protected:
    Matrix Q(const Vector &m, const Matrix &D) const override;
    Matrix S(const Vector &m, const Matrix &D) const override;
    Matrix R(const Vector &m, const Matrix &D) const override;

    void loadParams() override;

    double k(double t) const;


protected:
    double m_turnTime;

    static constexpr double KB = 0.3;
    static constexpr double BB = 0.09;
    static constexpr double CC = 0.043333333333333333333333333333333333333333333333333;
    static constexpr double GG = 3.711E-3;
    static constexpr double RR = 3390.0;
};


} // end Tasks::Continuous

} // end Tasks


#endif // CONTINUOUS_LANDING_LINEAR_H
