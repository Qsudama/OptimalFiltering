#ifndef DISCRETE_AOF_H
#define DISCRETE_AOF_H

#include "src/core/discrete_filter.h"


namespace Filters
{

//! Подмодуль, содержащий реализации дискретных фильтров оптимальной структуры.

namespace Discrete
{


//! \brief Класс, реализующий дискретный абсолютно оптимальной фильтр.

class AOF : public Core::DiscreteFilter
{

public:
    //! \brief Конструктор.
    AOF(Core::PtrFilterParameters params, Core::PtrTask task);


protected:
    void zeroIteration() override;

    /*!
     \brief Выполняет алгоритм.

     Вектор \f$Z_t\f$ оценки вычисляется следующим образом:
     \f[\Lambda_k = \tau_{k-1}(Z_{k-1}, P_{k-1}),\f]
     \f[\Psi_k = \Theta_{k-1}(Z_{k-1}, P_{k-1}),\f]
     \f[H_k = \Psi_k \cdot G_k^T(\Lambda_k, \Psi_k) \cdot F_k^{-1}(\Lambda_k, \Psi_k),\f]
     \f[Z_k = \Lambda_k + H_k \cdot [Y_k - h_k(\Lambda_k, \Psi_k)],\f]
     \f[P_k = \Psi_k - H_k \cdot G_k(\Lambda_k, \Psi_k) \cdot \Psi_k.\f]

     \see Core::DiscreteFilter
     \see Core::DiscreteTask
    */
    void algorithm() override;


protected:
    Array<Matrix> m_sampleP; /*!< Массив апостериорных ковариаций \f$P\f$. */
};


} // end Discrete

} // end Filters


#endif // DISCRETE_AOF_H
