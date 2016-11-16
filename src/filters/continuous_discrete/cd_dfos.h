#ifndef CONTINUOUS_DISCRETE_DFOS_H
#define CONTINUOUS_DISCRETE_DFOS_H

#include "src/core/continuous_discrete_filter.h"


namespace Filters
{

namespace ContinuousDiscrete
{


//! \brief Класс, реализующий непрерывно-дискретный фильтр оптимальной структуры с дискретным прогнозом.

class DFOS : public Core::ContinuousDiscreteFilter
{

public:
    //! \brief Конструктор.
    DFOS(Core::PtrFilterParameters params, Core::PtrTask task);


protected:
    /*!
     \brief Выполняет алгоритм.

     Вектор \f$Z_t\f$ оценки между измерениями (прогнозирование) вычисляется следующим образом:
     \f[Z_{\tau_k^i} = \Gamma_k^i \cdot Z_{\tau_k^{i-1}} + \kappa_k^i,\ \ \ i = 1 \ldots L,\f]

     В моменты измерений:
     \f[\Lambda_k = \Gamma_k \cdot Z_{\tau_{k-1}^L} + \kappa_k,\f]
     \f[Z_{\tau_k^0} = \Lambda_k + T_k \cdot G_k^T(\Lambda_k, T_k) \cdot F_k^{-1}(\Lambda_k, T_k)
                                                                                   \cdot [Y_k - h_k(\Lambda_k, T_k)],\f]

     Параметры вычисляется так:
     \f[\Gamma_k^i = D_{\tau_k^i,\tau_k^{i-1}}^{x,z} \cdot (D_{\tau_k^{i-1}}^z)^{-1},\ \
        \kappa_k^i = m_{\tau_k^i}^x - \Gamma_k^i \cdot m_{\tau_k^{i-1}}^z,\f]
     \f[\Gamma_k = D_{t_k,\tau_{k-1}^L}^{x,z} \cdot (D_{\tau_{k-1}^L}^z)^{-1},\ \
        \kappa_k = m_{t_k}^x - \Gamma_k \cdot m_{\tau_{k-1}^L}^z,\f]
     \f[T_k = D_{t_k}^x - \Gamma_k \cdot (D_{t_k,\tau_{k-1}^L}^{x,z})^T.\f]

     \see Core::ContinuousDiscreteFilter
     \see Core::ContinuousDiscreteTask
    */
    void algorithm() override;
};


} // end ContinuousDiscrete

} // end Filters


#endif // CONTINUOUS_DISCRETE_DFOS_H
