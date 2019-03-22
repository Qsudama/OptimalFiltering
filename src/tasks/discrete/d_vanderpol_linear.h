#ifndef DISCRETE_VANDERPOL_LINEAR_H
#define DISCRETE_VANDERPOL_LINEAR_H

#include "src/core/discrete_task.h"
#include "src/math/math.h"


namespace Tasks
{

//! Подмодуль, содержащий реализации задач для дискретных фильтров оптимальной структуры.

namespace Discrete
{


/*!
 * \brief Дискретный линеаризованный осциллятор Ван-дер-Поля
 */

class VanDerPolLinear : public Core::DiscreteTask
{

public:
    //! \brief Конструктор.
    VanDerPolLinear();

    Vector a(const Vector &x) const override;
    Vector a(const Vector &x, const Vector &v) const override;
    Vector a_m(const Vector &x) const override;
    Vector b(const Vector &x) const override;
    Vector b(const Vector &x, const Vector &w) const override;
    Vector b_m(const Vector &x) const override;
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


protected:
    static constexpr double deltaT = 0.25;

    static constexpr double m_omega = 0.1 * M_PI;
    static constexpr double m_alpha = 2.0;
    static constexpr double m_beta = 1.0;
};


} // end Tasks::

} // end Tasks


#endif // DISCRETE_VANDERPOL_LINEAR_H
