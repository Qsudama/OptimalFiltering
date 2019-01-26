#ifndef FUNCTION_TIME_H
#define FUNCTION_TIME_H

#include "src/helpers/log_in_file_manager.h"
#include "src/helpers/log_in_console_manager.h"

namespace Core
{


/*!
 \brief Базовый класс для классов, представляющих собой функции времени.

 Зачем нужен? Позволяет работать со временем там, где нет возможности передать время
 явно в качестве параметра функции.

 Обычный класс, имеющий метод с явной передачей \f$t\f$ в параметры:
 \code
    SomeType func;
    double t = 10.3;
    double x = func.foo(..., t);
 \endcode
 Класс, унаследованный от FunctionTime:
 \code
    FunctionTimeChild func;
    double t = 10.3;
    func.setTime(t);
    double x = func.foo(...);
 \endcode
*/

class FunctionTime
{

public:
    //! \brief Конструктор.
    FunctionTime(double initialTime = 0.0, double initialStep = 0.001);

    //! \brief Деструктор.
    virtual ~FunctionTime();

    //! \brief Возвращает текущее значение времени.
    double time() const;

    //! \brief Возвращает шаг на который увеличивается значение времени.
    double step() const;

    //! \brief Устанавливает новое значение времени.
    void setTime(double newTime);

    //! \brief Устанавливает шаг на который увеличивается значение времени.
    void setStep(double newStep);

    /*!
     \brief Увеличивает значение времни на step.

     При этом step не сохраняется: шаг по-умолчанию не изменяется.
    */
    void advanceTime(double step);

    /*!
     \brief Увеличивает значение времени на величину шага по-умолчанию.

     Этот шаг задается в конструкторе и может быть изменен вызовом setStep(double newStep).
    */
    void advanceTime();

    //! \brief Менеджер для логирования в консоль.
    LogInConsoleManager& logInstance;

protected:
    double m_time; /*!< Время. */
    double m_step; /*!< Шаг. */
};


} // end Core


#endif // FUNCTION_TIME_H
