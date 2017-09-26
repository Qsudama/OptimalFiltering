#ifndef LD_LANDING_REJECTION_LINEAR_H
#define LD_LANDING_REJECTION_LINEAR_H

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

class LandingRejectionLinear : public Core::LogicDynamicTask
{

public:
    //! \brief Конструктор.
    LandingRejectionLinear();

    Vector a(int i, const Vector &x) const override;
    Vector b(int i, const Vector &x) const override;
    double nu(int i, int l, const Vector &m, const Matrix &D) const override;
    Vector tau(int i, int l, const Vector &z, const Matrix &P) const override;
    Matrix Theta(int i, int l, const Vector &z, const Matrix &P) const override;
    Vector h(int i, const Vector &m, const Matrix &D) const override;
    Matrix G(int i, const Vector &m, const Matrix &D) const override;
    Matrix F(int i, const Vector &m, const Matrix &D) const override;

    double A(int i, int l) const override;

    double Pr(int i) const override;

    Array<int> generateArrayI(int sizeS) const override;


protected:
    Matrix dadx(int i, const Vector &x) const override;
    Matrix dadv(int i, const Vector &x) const override;
    Matrix dbdx(int i, const Vector &x) const override;
    Matrix dbdw(int i, const Vector &x) const override;

    void loadParams() override;

    double e(const Vector &x) const;
    double d(const Vector &x) const;
    double Ex(const Vector &x) const;
    double Sk(double t) const;
    double gammaX(int i) const;
    double gammaY(int i) const;

    Matrix BwdbdwBwt(int i, const Vector &x) const;
    Vector bForZeroW(int i, const Vector &x) const;

protected:
    double m_turnTime;
    double m_p;
    double gamMinX; /*!< Для задачи со сбоями акселерометра */
    double gamMinY; /*!< Для задачи со сбоями акселерометра */
    int countIInTask; /*!< Количество режимов в задаче*/


    static constexpr double KB = 0.3; // b
    static constexpr double BB = 0.9; // Beta
    static constexpr double CC = 0.43333333333333333; // c = 0.5*pho_0*sigma_x
    static constexpr double GG = 3.72E-4; // g
    static constexpr double RR = 340.0; // R

};


} // end Tasks::

} // end Tasks

#endif // LD_LANDING_REJECTION_LINEAR_H
