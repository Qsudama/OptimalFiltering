#ifndef CONTINUOUS_DISCRETE_FILTER_H
#define CONTINUOUS_DISCRETE_FILTER_H

#include "continuous_discrete_task.h"
#include "filter.h"
#include "src/math/linear_algebra.h"


namespace Core
{


/*! \brief Базовый класс для всех непрерывно-дискретных фильтров оптимальной структуры.

    \see ContinuousDiscreteTask.
*/

class ContinuousDiscreteFilter : public Filter
{

public:
    //! \brief Конструктор.
    ContinuousDiscreteFilter(PtrFilterParameters params, PtrTask task);


protected:
    /*! \brief Нулевая итерация алгоритма (инициализирует начальные состояния).

        Практически для всех дискретных фильтров оптимальной структуры имеет вид:
        \f[Z_0 = H_0 \cdot  Y_0 + e_0,\f]
        \f[H_0 = D_{00}^{xy} \cdot  (D_0^y)^{-1},\f]
        \f[e_0 = m_0^x - H_0 \cdot  m_0^y.\f]

        В гауссовском АОФ (ФНА) добавляется уравнение для апостериорной ковариации объекта:
        \f[P_0 = D_0^x - H_0 \cdot  (D_{00}^{xy})^T.\f]
    */
    void zeroIteration() override;

    double execute_time_filter() override;

protected:
    PtrCDTask m_task; /*!< Указатель на экземпляр задачи, с которой происходит работа. */
};


//! \brief Тип умного указателя на ContinuousDiscreteFilter.
using PtrCDFilter = std::shared_ptr<ContinuousDiscreteFilter>;


} // end Core


#endif // CONTINUOUS_DISCRETE_FILTER_H
