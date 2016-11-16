#ifndef CONTINUOUS_TASK_H
#define CONTINUOUS_TASK_H

#include "src/math/linear_algebra.h"
#include "task.h"


namespace Core
{


/*! \brief Базовый тип задач для непрерывных фильтров оптимальной структуры.

  Описываемая система имеет следующий вид

  * * Объект: \f[dX_t = a(t, X_t)dt + B(t, X_t)dV_t,\ \ \ X_0 \sim \pi_0(x),\f]

  * * Измеритель: \f[dY_t = c_k(t, X_t) + D(t, X_t)dW_t,\ \ \ Y_0 = 0.\f]

  Здесь \f$t\f$ - время, \f$X_t = X(t)\f$ - вектор состояния объекта в момент \f$t\f$,
  \f$Y_t\f$ - вектор измерения,
  \f$V_t, W_t\f$ - векторы независимых стандартных винеровских процессов.
*/

class ContinuousTask : public Task
{

public:
    //! \brief Конструктор.
    ContinuousTask();

    //! \brief Функция сноса объекта \f$a(t,x)\f$.
    virtual Vector a(const Vector &x) const = 0;

    //! \brief Функция диффузии объекта \f$B(t,x)\f$.
    virtual Matrix B(const Vector &x) const = 0;

    //! \brief Функция сноса измерителя \f$с(t,x)\f$.
    virtual Vector c(const Vector &x) const = 0;

    //! \brief Функция диффузии измерителя \f$D(t,x)\f$.
    virtual Matrix D(const Vector &x) const = 0;

    /*! \brief Структурная функция \f$A(t, m, D)\f$.

        Она имеет следующий вид для

        * * линеаризованного приближения: \f[A(t, m, D) = \frac{d}{dm} a(t, m).\f]
    */
    virtual Matrix A(const Vector &m, const Matrix &D) const = 0;

    /*! \brief Структурная функция \f$G(t, m, D)\f$.

        Она имеет следующий вид для

        * * линеаризованного приближения: \f[G(t, m, D) = \frac{d}{dm} c(t, m).\f]
    */
    virtual Matrix G(const Vector &m, const Matrix &D) const = 0;

    /*! \brief Структурная функция \f$K(t, m, D)\f$.

        Она имеет следующий вид для

        * * линеаризованного приближения: \f[K(t, m, D) = [D \cdot G^T(t,m) + S(t,m)] \cdot R^{-1}(t, m).\f]
    */
    virtual Matrix K(const Vector &m, const Matrix &D) const;

    /*! \brief Структурная функция \f$\Psi(t, m, D)\f$.

        Она имеет следующий вид для

        * * линеаризованного приближения:
        \f[\Psi(t, m, D) = A(t,m)\cdot D + D \cdot A^T(t,m) + Q(t,m) - K(t,m,D) \cdot R(t,m) \cdot K^T(t,m,D).\f]
    */
    virtual Matrix Psi(const Vector &m, const Matrix &D) const;


protected:
    //! \brief Возвращает интенсивность шумов \f$Q(\cdot) = B(\cdot) \cdot B^T(\cdot)\f$.
    virtual Matrix Q(const Vector &m, const Matrix &D) const = 0;

    /*! \brief Возвращает интенсивность шумов \f$S(\cdot) = B(\cdot) \cdot D^T(\cdot)\f$.

        \warning Так как шумы независимые, то должна возвращать нулевую матрицу (нулевая взаимная интенсивность).
     */
    virtual Matrix S(const Vector &m, const Matrix &D) const = 0;

    //! \brief Возвращает интенсивность шумов \f$R(\cdot) = D(\cdot) \cdot D^T(\cdot)\f$.
    virtual Matrix R(const Vector &m, const Matrix &D) const = 0;
};


//! \brief Тип умного указателя на ContinuousTask.
using PtrCTask = std::shared_ptr<ContinuousTask>;


} // end Core


#endif // CONTINUOUS_TASK_H
