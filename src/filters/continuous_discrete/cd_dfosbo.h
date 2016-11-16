#ifndef CONTINUOUS_DISCRETE_DFOSBO_H
#define CONTINUOUS_DISCRETE_DFOSBO_H

#include "src/core/continuous_discrete_filter.h"


namespace Filters
{

namespace ContinuousDiscrete
{


//! \brief Класс, реализующий непрерывно-дискретный фильтр оптимальной структуры повышеного порядка.

class DFOSBO : public Core::ContinuousDiscreteFilter
{

public:
    //! \brief Конструктор.
    DFOSBO(Core::PtrFilterParameters params, Core::PtrTask task);


protected:
    void zeroIteration() override;

    /*!
     \brief Выполняет алгоритм.

     Алгоритм схож с DFOS. Отличие в вычислении вектора состояния между измерениями (прогнозы).

     Вводится расширенный вектор состояния \f$S_k\f$:
     \f[ S_k = [Y_k^T\ \ Y_{k-1}^T\ \cdots Y_{k-l+1}^T]^T,\f]
     где \f$l\f$ - кратность порядка \f$p = l \cdot dim(Y)\f$ фильтра.

     Вектор \f$Z_t\f$ оценки между измерениями (прогнозирование) вычисляется следующим образом:
     \f[Z_{\tau_k^i} = \Gamma_k^i \cdot S_k + \kappa_k^i,\ \ \ i = 1 \ldots L,\f]

     В моменты измерений:
     \f[\Lambda_k = \Gamma_k \cdot Z_{k-1} + \kappa_k,\f]
     \f[Z_{\tau_k^0} = \Lambda_k + T_k \cdot G_k^T(\Lambda_k, T_k) \cdot F_k^{-1}(\Lambda_k, T_k)
                                                                                   \cdot [Y_k - h_k(\Lambda_k, T_k)],\f]

     Параметры вычисляется так:
     \f[\Gamma_k^i = D_{\tau_k^i,k}^{x,s} \cdot (D_k^s)^{-1},\ \
        \kappa_k^i = m_{\tau_k^i}^x - \Gamma_k^i \cdot m_k^s,\f]
     \f[\Gamma_k = D_{\tau_k^0,k-1}^{x,s} \cdot (D_{k-1}^s)^{-1},\ \
        \kappa_k = m_{\tau_k^0}^x - \Gamma_k \cdot m_{k-1}^s,\f]
     \f[T_k = D_{\tau_k^0}^x - \Gamma_k \cdot (D_{\tau_k^0,k-1}^{x,s})^T.\f]

     \see Core::ContinuousDiscreteFilter
     \see Core::ContinuousDiscreteTask
    */
    void algorithm() override;


protected:
    Array<Vector> m_sampleS; /*!< Массив для хранения расширенных векторов состояния \f$S_k\f$. */
};


} // end ContinuousDiscrete

} // end Filters


#endif // CONTINUOUS_DISCRETE_DFOSBO_H
