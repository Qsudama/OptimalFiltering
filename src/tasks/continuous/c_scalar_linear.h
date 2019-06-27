#ifndef C_SCALAR_LINEAR
#define C_SCALAR_LINEAR
#include "src/core/continuous_task.h"
#include "src/math/math.h"


namespace Tasks
{

//! Подмодуль, содержащий реализации задач для непрерывных фильтров оптимальной структуры.

namespace Continuous
{

/*!
 * \brief Тестовый скалярный пример (линеаризованный) для фильтров оптимальной структуры.
 */

class ScalarLinear : public Core::ContinuousTask
{

public:
    //! \brief Конструктор.
    ScalarLinear();

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

#endif // C_SCALAR_LINEAR

