#ifndef CONTINUOUS_DISCRETE_AOF_H
#define CONTINUOUS_DISCRETE_AOF_H

#include "src/core/continuous_discrete_filter.h"


namespace Filters
{

//! Подмодуль, содержащий реализации непрерывно-дискретных фильтров оптимальной структуры.

namespace ContinuousDiscrete
{


//! \brief Класс, реализующий непрерывно-дискретный абсолютно оптимальной фильтр.

class AOF : public Core::ContinuousDiscreteFilter
{

public:
    //! \brief Конструктор.
    AOF(Core::PtrFilterParameters params, Core::PtrTask task);


protected:
    void zeroIteration() override;

    /*!
     \brief Выполняет алгоритм.

     Вектор \f$Z_t\f$ оценки между измерениями вычисляется из системы:
     \f[\frac{d}{dt}Z_t = \tau(t,Z_t, P_t),\f]
     \f[\frac{d}{dt}P_t = A(t,Z_t, P_t) \cdot P_t + P_t \cdot A^T(t,Z_t, P_t) + \Theta(t,Z_t,P_t).\f]

     В моменты измерений:
     \f[K_k = P_{t_k} \cdot G_k^T(Z_{t_k}, P_{t_k}) \cdot F_k^{-1}(Z_{t_k}, P_{t_k}),\f]
     \f[Z_{t_k^+} = Z_{t_k} + K_k \cdot [Y_k - h_k(Z_{t_k}, P_{t_k})],\f]
     \f[P_{t_k^+} = P_{t_k} - K_k \cdot G_k(Z_{t_k},P_{t_k}) \cdot P_{t_k}.\f]

     \see Core::ContinuousDiscreteFilter
     \see Core::ContinuousDiscreteTask
    */
    void algorithm() override;


protected:
    Array<Matrix> m_sampleP; /*!< Массив апостериорных ковариаций \f$P\f$. */
};


} // end ContinuousDiscrete

} // end Filters


#endif // CONTINUOUS_DISCRETE_AOF_H
