#ifndef LOGIC_DYNAMIC_FOS_H
#define LOGIC_DYNAMIC_FOS_H

#include "src/core/logic_dynamic_filter.h"

namespace Filters
{

namespace LogicDynamic
{


//! \brief Класс, реализующий дискретный фильтр оптимальной структуры.

class FOS : public Core::LogicDynamicFilter
{

public:
    //! \brief Конструктор.
    FOS(Core::PtrFilterParameters params, Core::PtrTask task);


protected:
    /*!
     \brief Выполняет алгоритм.

     Вектор \f$Z_t\f$ оценки вычисляется следующим образом:
     \f[\Lambda_k = \tau_{k-1}(U_{k-1}, T_{k-1}),\f]
     \f[\Psi_k = \Theta_{k-1}(U_{k-1}, T_{k-1}),\f]
     \f[Z_k = \Lambda_k +
        \Psi_k \cdot G_k^T(\Lambda_k, \Psi_k) \cdot F_k^{-1}(\Lambda_k, \Psi_k) \cdot [Y_k - h_k(\Lambda_k, \Psi_k)].\f]
     \f[U_k = \Gamma_k^y \cdot Y_k + \Gamma_k^z \cdot Z_k + \chi_k.\f]

     Параметры \f$\Gamma_k^y,\ \Gamma_k^z,\ T_k,\ \chi_k\f$ вычисляются
     в computeParams(size_t, Array<Math::Vector> &, Math::Matrix &).

     \see Core::LogicDynamicFilter
     \see Core::LogicDynamicTask
    */
    void algorithm() override;
    void zeroIteration() override;

private:

    Array<Array<double>> Xi;
    Array<Array<Vector>> u;
    Array<double> Q;
    Array<Vector> kappa;
    Array<Matrix> T;
    Array<Vector> meanZ;
    Array<Matrix> Gamma;
    Array<Matrix> Dzz;

    void computeBlock1(long s);
    void computeBlock2(long s);
    void computeBlock3b();
    void computeBlock3c();
    void computeBlock4(long s);
    void computeBlock5(long s);
    void computeBlock6();
};


} // end LogicDynamic

} // end Filters

#endif // LOGIC_DYNAMIC_FOS_H
