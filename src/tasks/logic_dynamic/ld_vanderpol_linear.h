#ifndef LD_VANDERPOL_LINEAR_H
#define LD_VANDERPOL_LINEAR_H

#include "src/core/logic_dynamic_task.h"
#include "src/math/math.h"


namespace Tasks
{

namespace LogicDynamic
{


/*!
 * \brief Осциллятор Ван-дер-Поля (непрерывно-дискретный, линеаризованный) для фильтров оптимальной структуры.
 */

class VanDerPolLinear : public Core::LogicDynamicTask
{

public:
    //! \brief Конструктор.
    VanDerPolLinear();

    Vector a(int i, const Vector &x) const override;
    Vector b(int i, const Vector &x) const override;
    double nu(int i, int l, const Vector &m, const Matrix &D) const override;
    Vector tau(int i, int l, const Vector &m, const Matrix &D) const override;
    Matrix Theta(int i, int l, const Vector &m, const Matrix &D) const override;
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

protected:
    double m_omega, m_alpha, m_beta, m_gamma, m_deltaT;
    int countIInTask; /*!< Количество режимов в задаче*/

    long m_dimE;
    Math::Matrix m_E;
};


} // end Tasks::LogicDynamic

} // end Tasks

#endif // LD_VANDERPOL_LINEAR_H
