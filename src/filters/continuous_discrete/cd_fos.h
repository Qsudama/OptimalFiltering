#ifndef CONTINUOUS_DISCRETE_FOS_H
#define CONTINUOUS_DISCRETE_FOS_H

#include "src/core/continuous_discrete_filter.h"


namespace Filters
{

namespace ContinuousDiscrete
{


//! \brief Класс, реализующий непрерывно-дискретный фильтр оптимальной структуры с непрерывным прогнозом.

class FOS : public Core::ContinuousDiscreteFilter
{

public:
    //! \brief Конструктор.
    FOS(Core::PtrFilterParameters params, Core::PtrTask task);


protected:
    /*!
     \brief Выполняет алгоритм.

     Вектор \f$Z_t\f$ оценки между измерениями вычисляется из уравнения:
     \f[\frac{d}{dt}Z_t = \tau(t,Z_t, \Gamma_t).\f]

     В моменты измерений:
     \f[Z_{t_k^+} = Z_{t_k} + \Gamma_{t_k} \cdot G_k^T(Z_{t_k}, \Gamma_{t_k}) \cdot
                                            F_k^{-1}(Z_{t_k}, \Gamma_{t_k}) \cdot [Y_k - h_k(Z_{t_k}, \Gamma_{t_k})].\f]

     Параметр \f$\Gamma_t\f$ вычисляется так:
     \f[\Gamma_t = D_t^x - D_t^z\f]

     \see Core::ContinuousDiscreteFilter
     \see Core::ContinuousDiscreteTask
    */
    void algorithm() override;
};


} // end ContinuousDiscrete

} // end Filters


#endif // CONTINUOUS_DISCRETE_FOS_H
