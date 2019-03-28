#ifndef CONTINUOUS_FILTER_H
#define CONTINUOUS_FILTER_H

#include "continuous_task.h"
#include "filter.h"


namespace Core
{


/*! \brief Базовый класс для всех непрерывных фильтров оптимальной структуры.

    \see ContinuousTask.
*/

class ContinuousFilter : public Filter
{

public:
    //! \brief Конструктор.
    ContinuousFilter(PtrFilterParameters params, PtrTask task);


protected:
    /*! \brief Нулевая итерация алгоритма (инициализирует начальные состояния).

        Практически для всех дискретных фильтров оптимальной структуры имеет вид:
        \f[Z_0 = m_0^x = M[X_0],\f]

        В гауссовском АОФ (ФНА) добавляется уравнение для апостериорной ковариации объекта:
        \f[P_0 = D_0^x = D[X_0].\f]
    */
    void zeroIteration() override;

    FilterTimeResult execute_time_filter() override;

protected:
    PtrCTask m_task; /*!< Указатель на экземпляр задачи, с которой происходит работа. */
};


//! \brief Тип умного указателя на ContinuousFilter.
using PtrCFilter = std::shared_ptr<ContinuousFilter>;


} // end Core


#endif // CONTINUOUS_FILTER_H
