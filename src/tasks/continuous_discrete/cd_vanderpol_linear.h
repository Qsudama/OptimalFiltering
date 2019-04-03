#ifndef CONTINUOUS_DISCRETE_VANDERPOL_LINEAR_H
#define CONTINUOUS_DISCRETE_VANDERPOL_LINEAR_H


#include "src/core/continuous_discrete_task.h"
#include "src/math/math.h"


namespace Tasks
{

namespace ContinuousDiscrete
{


/*!
 * \brief Осциллятор Ван-дер-Поля (непрерывно-дискретный, линеаризованный) для фильтров оптимальной структуры.
 */

class VanDerPolLinear : public Core::ContinuousDiscreteTask
{

public:
    //! \brief Конструктор.
    VanDerPolLinear();

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
    double m_omega, m_alpha, m_beta;
};


} // end Tasks::ContinuousDiscrete

} // end Tasks


#endif // CONTINUOUS_DISCRETE_VANDERPOL_LINEAR_H
