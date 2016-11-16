#ifndef CONTINUOUS_AOF_H
#define CONTINUOUS_AOF_H

#include "src/core/continuous_filter.h"


namespace Filters
{

//! Подмодуль, содержащий реализации непрерывных фильтров оптимальной структуры.

namespace Continuous
{


//! \brief Класс, реализующий непрерывный абсолютно оптимальный фильтр.

class AOF : public Core::ContinuousFilter
{

public:
    //! \brief Конструктор.
    AOF(Core::PtrFilterParameters params, Core::PtrTask task);


protected:
    void zeroIteration() override;

    /*!
     \brief Выполняет алгоритм.

     Вектор \f$Z_t\f$ оценки вычисляется следующим образом:
     \f[dZ_t = a(t,Z_t)dt + K(t,Z_t, P_t)\cdot [ dY_t - c(t,Z_t)dt],\f]
     \f[dP_t = \Psi(t,Z_t,P_t)dt.\f]

     \see Core::ContinuousFilter
     \see Core::ContinuousTask
    */
    void algorithm() override;


protected:
    Array<Math::Matrix> m_sampleP; /*!< Массив апостериорных ковариаций \f$P\f$. */
};


} // end Filters::Continuous

} // end Filters


#endif // CONTINUOUS_AOF_H
