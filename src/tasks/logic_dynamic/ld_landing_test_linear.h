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
 * \brief Задача спуска ЛА на планету (логико-динамическая, линеаризованная) для фильтров
 * оптимальной структуры.
 */

class LandingTestLinear : public Core::LogicDynamicTask
{

public:
    //! \brief Конструктор.
    LandingTestLinear();

    Vector a(const Vector &x) const override;
    Vector b(int i, const Vector &x) const override;
    Vector tau(const Vector &z, const Matrix &P) const override;
    Matrix Theta(const Vector &z, const Matrix &P) const override;
    Vector h(int i, const Vector &m, const Matrix &D) const override;
    Matrix G(int i, const Vector &m, const Matrix &D) const override;
    Matrix F(int i, const Vector &m, const Matrix &D) const override;


protected:
    Matrix dadx(const Vector &x) const override;
    Matrix dadv(const Vector &x) const override;
    Matrix dbdx(int i, const Vector &x) const override;
    Matrix dbdw(int i, const Vector &x) const override;

    void loadParams() override;

    Matrix e(int i, const Vector &x) const;
    double gammaX(int i) const;
    double gammaY(int i) const;
    double k(double t) const;


protected:
    double m_turnTime;

    static constexpr double KB = 0.3;
    static constexpr double BB = 0.09;
    static constexpr double SX = 1.0 / 150.0;
    static constexpr double R0 = 0.013;
    static constexpr double GG = 3.711E-3;
    static constexpr double RR = 3390.0;
};


} // end Tasks::

} // end Tasks


#endif // LD_LANDING_TEST_LINEAR_H
