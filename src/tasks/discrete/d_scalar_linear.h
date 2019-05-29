#ifndef DISCRETE_SCALAR_LINEAR_H
#define DISCRETE_SCALAR_LINEAR_H

#include "src/core/discrete_task.h"
#include "src/math/math.h"


namespace Tasks
{

//! Подмодуль, содержащий реализации задач для дискретных фильтров оптимальной структуры.

namespace Discrete
{


/*!
 * \brief Тестовый скалярный пример (линеаризованный) для фильтров оптимальной структуры.
 */

class ScalarLinear : public Core::DiscreteTask
{

public:
    //! \brief Конструктор.
    ScalarLinear();

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
    static constexpr double aa = 1.05;
    static constexpr double bb = 0.01;
    static constexpr double cc = 1.02;
    static constexpr double dd = 0.2;
    static constexpr double ee = 1.0;

//    double dd = 0.2;
//    double ee = 0.2;
};


} // end Tasks::

} // end Tasks

#endif // DISCRETE_SCALAR_LINEAR_H
