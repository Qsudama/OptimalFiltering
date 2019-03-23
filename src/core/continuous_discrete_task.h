#ifndef CONTINUOUS_DISCRETE_TASK_H
#define CONTINUOUS_DISCRETE_TASK_H

#include "task.h"


namespace Core
{


/*! \brief Базовый тип задач для непрерывно-дискретных фильтров оптимальной структуры.

  Описываемая система имеет следующий вид

  * * Объект: \f[dX_t = a(t, X_t)dt + B(t, X_t)dV_t,\ \ \ X_0 \sim \pi_0(x),\f]

  * * Измеритель: \f[Y_k = с_k(X_{t_k}, W_k),\ \ \ k = 0, 1, \ldots\f]

  Здесь \f$t\f$ - время, \f$t_k\f$ - моменты измерений,
  \f$X_t = X(t)\f$ - вектор состояния объекта в момент \f$t\f$,
  \f$Y_k\f$ - вектор измерения,
  \f$V_t\f$ - вектор стандартного винеровского процессов,
  \f$W_k\f$ - вектор дискретного белого шума.
*/

class ContinuousDiscreteTask : public Task
{

public:
    //! \brief Конструктор.
    ContinuousDiscreteTask();

    //! \brief Функция сноса объекта \f$a(t,x)\f$.
    virtual Vector a(const Vector &x) const = 0;

    //! \brief Функция диффузии объекта \f$B(t,x)\f$.
    virtual Matrix B(const Vector &x) const = 0;

    /*! \brief Функция измерителя \f$c_k(X_k) = c_k(X_k, W_k)\f$.

        Шум \f$W_k\f$ генерируется внутри.
    */
    virtual Vector c(const Vector &x, double measurementStep) const = 0;

    /*! \brief Структурная функция прогноза \f$\tau(t, m, D)\f$.

        Она имеет следующий вид для

        * * гауссовского приближения: \f[\tau(t, m, D) = \int a(t,x)\cdot N(x\ |\ m, D)dx,\f]

        * * линеаризованного приближения: \f[\tau(t, m, D) = a(t, m).\f]
    */
    virtual Vector tau(const Vector &m, const Matrix &D) const = 0;

    /*! \brief Структурная функция прогноза \f$\Theta(t,m,D)\f$.

        Она имеет следующий вид для

        * * гауссовского приближения: \f[\Theta(t,m,D) = \int B(t,x)\cdot B^T(t,x)\cdot N(x\ |\ m,D)dx,\f]

        * * линеаризованного приближения: \f[\Theta(t,m,D) = B(t,m)\cdot B^T(t,m).\f]
    */
    virtual Matrix Theta(const Vector &m, const Matrix &D) const = 0;

    /*! \brief Возвращает матрицу якоби \f$A(t, m, D)\f$ для функции \f$\tau(t, m, D)\f$.

        Она имеет следующий вид для

        * * гауссовского приближения: \f[A(t, m, D) = \frac{d}{dm} \tau(t, m),\f]

        * * линеаризованного приближения: \f[A(t, m, D) = \frac{d}{dm} a(t, m).\f]
    */
    virtual Matrix A(const Vector &m, const Matrix &D) const = 0;

    /*! \brief Структурная функция коррекции \f$h_k(m, D)\f$.

        Она имеет следующий вид для

        * * гауссовского приближения: \f[h_k(m, D) = \int (M[c_k(x, W_k)] \cdot N(x\ |\ m, D))dx,\f]

        * * линеаризованного приближения: \f[h_k(m, D) = c_k(m, M[W_k]).\f]
    */
    virtual Vector h(const Vector &m, const Matrix &D, double measurementStep) const = 0;

    /*! \brief Структурная функция коррекции \f$G_k(m, D)\f$.

        Она имеет следующий вид для

        * * гауссовского приближения: \f[G_k(m, D) = \frac{d}{dm}h_k(m,D),\f]

        * * линеаризованного приближения: \f[G_k(m, D) = \frac{d}{dm}c_k(m,M[W_k]).\f]
    */
    virtual Matrix G(const Vector &m, const Matrix &D, double measurementStep) const = 0;

    /*! \brief Структурная функция коррекции \f$F_k(m, D)\f$.

        Она имеет следующий вид для

        * * гауссовского приближения:
        \f[F_k(m, D) = \int (M[c_k(x,W_k)\cdot c_k^T(x, W_k)] \cdot N(x\ |\ m,D))dx - h_k(m,D)\cdot h_k^T(m,D),\f]

        * * линеаризованного приближения:
        \f[F_k(m, D) = C_k^x(m)\cdot D\cdot C_k^{xT}(m) + C_k^w(m)\cdot D[W_k]\cdot C_k^{wT}(m).\f]

        Здесь \f$C_k^x(x) = \frac{d}{dx} c_k(x, M[W_k]),\ C_k^w(x) = \frac{d}{dw} c_k(x, M[W_k])\f$.
    */
    virtual Matrix F(const Vector &m, const Matrix &D, double measurementStep) const = 0;
};


//! \brief Тип умного указателя на ContinuousDiscreteTask.
using PtrCDTask = std::shared_ptr<ContinuousDiscreteTask>;


} // end Core


#endif // CONTINUOUS_DISCRETE_TASK_H
