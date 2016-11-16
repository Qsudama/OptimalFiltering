#ifndef CONTINUOUS_FOS_H
#define CONTINUOUS_FOS_H

#include "src/core/continuous_filter.h"


namespace Filters
{

namespace Continuous
{


//! \brief Класс, реализующий непрерывный фильтр оптимальной структуры.

class FOS : public Core::ContinuousFilter
{

public:
    //! \brief Конструктор.
    FOS(Core::PtrFilterParameters params, Core::PtrTask task);


protected:
    /*!
     \brief Выполняет алгоритм.

     Вектор \f$Z_t\f$ оценки вычисляется следующим образом:
     \f[dZ_t = a(t,Z_t)dt + K(t,Z_t, \Gamma_t)\cdot [ dY_t - c(t,Z_t)dt],\f]
     \f[\Gamma_t = D_t^x - D_t^z,\f]

     \see Core::ContinuousFilter
     \see Core::ContinuousTask
    */
    void algorithm() override;
};


} // end Filters::Continuous

} // end Filters


#endif // CONTINUOUS_FOS_H
