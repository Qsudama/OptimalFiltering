#ifndef CD_SCALAR_LINEAR
#define CD_SCALAR_LINEAR
#include "src/core/continuous_discrete_task.h"
#include "src/math/math.h"


namespace Tasks
{

//! Подмодуль, содержащий реализации задач для непрерывных фильтров оптимальной структуры.

namespace ContinuousDiscrete
{

/*!
 * \brief Тестовый скалярный пример (непрерывно-дискретный, линеаризованный) для фильтров оптимальной структуры.
 */

class ScalarLinear : public Core::ContinuousDiscreteTask
{

public:
    //! \brief Конструктор.
    ScalarLinear();

    Vector a(const Vector &x) const override;
    Matrix B(const Vector &x) const override;
    Vector c(const Vector &x, double measurementStep) const override;
    Vector tau(const Vector &m, const Matrix &D) const override;
    Matrix Theta(const Vector &m, const Matrix &D) const override;
    Matrix A(const Vector &m, const Matrix &D) const override;
    Vector h(const Vector &m, const Matrix &D, double measurementStep) const override;
    Matrix G(const Vector &m, const Matrix &D, double measurementStep) const override;
    Matrix F(const Vector &m, const Matrix &D, double measurementStep) const override;

protected:

    void loadParams() override;


protected:
    static constexpr double aa = 1.05;
    static constexpr double bb = 0.01;
    static constexpr double cc = 1.02;
    static constexpr double dd = 0.2;
    static constexpr double ee = 1.0;

    static constexpr double a1 = -0.5;
    //static constexpr double a3 = -1.0;
    //static constexpr double a3 = 0.0;
    static constexpr double a3 = 0.01;
    static constexpr double b0 = 1.5;
    static constexpr double c1 = 1.0;
    static constexpr double c2 = 0.5;
    static constexpr double d0 = 1.0;
    //static constexpr double d1 = 0.1;
     static constexpr double d1 = 0.0;
};


} // end Tasks::

} // end Tasks

#endif // CD_SCALAR_LINEAR

