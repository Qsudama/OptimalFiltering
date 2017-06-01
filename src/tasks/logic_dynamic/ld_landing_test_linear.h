#ifndef LD_LANDING_TEST_LINEAR_H
#define LD_LANDING_TEST_LINEAR_H

#include "src/core/logic_dynamic_task.h"
#include "src/math/math.h"

namespace Tasks
{

//! Подмодуль, содержащий реализации задач для логико-динамических фильтров оптимальной структуры.

namespace LogicDynamic
{


/*!
 * \brief Задача спуска ЛА на планету (логико-динамическая, линеаризованная) для фильтров оптимальной структуры.
 */

class LandingTestLinear : public Core::LogicDynamicTask
{

public:
    //! \brief Конструктор.
    LandingTestLinear();

    Vector a(const Vector &x) const override;
    Vector b(const Vector &x) const override;
    Vector tau(const Vector &z, const Matrix &P) const override;
    Matrix Theta(const Vector &z, const Matrix &P) const override;
    Vector h(const Vector &m, const Matrix &D) const override;
    Matrix G(const Vector &m, const Matrix &D) const override;
    Matrix F(const Vector &m, const Matrix &D) const override;


protected:
    Matrix dadx(const Vector &x) const override;
    Matrix dadv(const Vector &x) const override;
    Matrix dbdx(const Vector &x) const override;
    Matrix dbdw(const Vector &x) const override;

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


} // end Tasks::

} // end Tasks


#endif // LD_LANDING_TEST_LINEAR_H
