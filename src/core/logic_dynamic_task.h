#ifndef LOGIC_DYNAMIC_TASK_H
#define LOGIC_DYNAMIC_TASK_H

#include "task.h"

namespace Core
{

/*! \brief Базовый тип задач для логико-динамических фильтров оптимальной структуры.

  Описываемая система имеет следующий вид

  * * Объект: \f[X_{k+1} = a_k(X_k, V_k),\ \ \ k = 0, 1, \ldots\f]

  * * Измеритель: \f[Y_k = b_k(X_k, W_k),\ \ \ k = 0, 1, \ldots\f]

  Здесь \f$X_k\f$ - вектор состояния объекта, \f$Y_k\f$ - вектор измерений,
  \f$V_k, W_k\f$ - векторы независимых дискретных белых шумов.
*/

class LogicDynamicTask : public Task
{

public:
    //! \brief Конструктор.
    LogicDynamicTask();

    /*! \brief Функция объекта \f$a_k(X_k) = a_k(X_k, V_k)\f$.

        Шум \f$V_k\f$ генерируется внутри.
    */
    virtual Vector a(int i, const Vector &x) const = 0;

    /*! \brief Функция измерителя \f$b(X_k) = b_k(X_k, W_k)\f$.

        Шум \f$W_k\f$ генерируется внутри.
    */
    virtual Vector b(int i, const Vector &x) const = 0;

    /*! \brief Структурная функция прогноза \f$\nu_k(m, D)\f$.

        Она имеет следующий вид для

    */

    virtual double nu(int i, int i0, const Vector &m, const Matrix &D) const = 0;

    /*! \brief Структурная функция прогноза \f$\tau_k(m, D)\f$.

        Она имеет следующий вид для

        * * гауссовского приближения: \f[\tau_k(m, D) = M_N[M[a_k(m, V_k)] \ |\  m, D],\f]

        * * линеаризованного приближения: \f[\tau_k(m, D) = a_k(m, M[V_k]).\f]
    */
    virtual Vector tau(int i, int i0, const Vector &m, const Matrix &D) const = 0;

    /*! \brief Структурная функция прогноза \f$\Theta_k(m, D)\f$.

        Она имеет следующий вид для

        * * гауссовского приближения:
        \f[\Theta_k(m, D) = M_N[M[a_k(m, V_k)\cdot a_k^T(m, V_k)]\ |\  m, D] - \tau_k(m,D) \cdot
       \tau_k^T(m, D),\f]

        * * линеаризованного приближения:
         \f[\Theta_k(m, D) = A_k^x(m) \cdot D \cdot A_k^{xT}(m) + A_k^v(m) \cdot D[V_k] \cdot A_k^{v
       T}(m)\f]

        Здесь \f$A_k^x(x) = \nabla_x a_k(x,M[V_k]),\ A_k^v(x) = \nabla_v a_k(x,M[V_k])\f$.
    */
    virtual Matrix Theta(int i, int i0, const Vector &m, const Matrix &D) const = 0;

    /*! \brief Структурная функция коррекции \f$h_k(m, D)\f$.

        Она имеет следующий вид для

        * * гауссовского приближения: \f[h_k(m, D) = M_N[M[b_k(m, W_k)] \ |\  m, D],\f]

        * * линеаризованного приближения: \f[h_k(m, D) = b_k(m, M[W_k]).\f]
    */
    virtual Vector h(int i, const Vector &m, const Matrix &D) const = 0;

    /*! \brief Структурная функция коррекции \f$G_k(m, D)\f$.

        Она имеет следующий вид для

        * * гауссовского приближения: \f[G_k(m, D) = \nabla_m h_k(m,D),\f]

        * * линеаризованного приближения: \f[G_k(m, D) = \nabla_m b_k(m, M[W_k]).\f]
    */
    virtual Matrix G(int i, const Vector &m, const Matrix &D) const = 0;

    /*! \brief Структурная функция коррекции \f$F_k(m, D)\f$.

        Она имеет следующий вид для

        * * гауссовского приближения:
        \f[F_k(m, D) = M_N[M[b_k(m, W_k)\cdot b_k^T(m, W_k)]  \ |\  m, D] - h_k(m,D) \cdot h_k^T(m,
       D),\f]

        * * линеаризованного приближения:
        \f[F_k(m, D) = B_k^x(m) \cdot D \cdot B_k^{xT}(m) + B_k^w(m) \cdot D[W_k] \cdot
       B_k^{wT}(m).\f]

        Здесь \f$B_k^x(x) = \nabla_x b_k(x, M[W_k]),\ B_k^w(x) = \nabla_w b_k(x, M[W_k])\f$.
    */
    virtual Matrix F(int i, const Vector &m, const Matrix &D) const = 0;

    //! Возващает вектор вероятностей изменения режима.
    virtual double Pr(int i) const = 0;

    //! Возвращает случайное значение режима по предыдущему значению.
    virtual Array<int> generateArrayI(int sizeS) const = 0;

    virtual double A(int i, int l, const Vector &m, const Matrix &D) const = 0;

    int countI; /*!< Количество режимов в конкретной задаче */

    double gamMinX;
    double gamMinY;

protected:
    //! Вспомогательная функция, вычисляет \f$\nabla_x a_k(x,v)\f$.
    virtual Matrix dadx(int i, const Vector &x) const = 0;

    //! Вспомогательная функция, вычисляет \f$\nabla_v a_k(x,v)\f$.
    virtual Matrix dadv(int i, const Vector &x) const = 0;

    //! Вспомогательная функция, вычисляет \f$\nabla_x b_k(x,w)\f$.
    virtual Matrix dbdx(int i, const Vector &x) const = 0;

    //! Вспомогательная функция, вычисляет \f$\nabla_w b_k(x,w)\f$.
    virtual Matrix dbdw(int i, const Vector &x) const = 0;
};


//! \brief Тип умного указателя на LogicDynamicTask.
using PtrLDTask = std::shared_ptr<LogicDynamicTask>;


} // end Core

#endif // LOGIC_DYNAMIC_TASK_H
