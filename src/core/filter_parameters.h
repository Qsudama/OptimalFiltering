#ifndef FILTER_PARAMETERS_H
#define FILTER_PARAMETERS_H

#include "src/config.h"
#include <cassert>
#include <memory>


namespace Core
{


/*!
 \brief Класс для работы с параметрами фильтра.

 Основное внимание уделено обработке временных интервалов.

 Пусть отрезок времени \f$t\f$, обрабатываемого фильтром, равен \f$[0, T_{max}]\f$.

 На этом отрезке времени производятся измерения с шагом \f$\delta t\f$.

 Между измерениями (в фильтрах с дискретными прогнозами) осуществляется прогнозирование.
 Интервал между прогнозами \f$\Delta t\f$.

 Шаг интегрирования системы \f$dt\f$.

 Эти величины корректируются (при изменении) так, чтобы
 количество интегрирований \f$N\f$ между соседними прогнозами,
 количество прогнозов \f$L\f$ между соседними измерениями и
 количесвтво измерений \f$K\f$ на всём временном интервале были целыми числами.

 То есть \f$dt, \Delta t, \delta t\f$ и \f$T_{max}\f$ немного подкручиваются так, что выражения ниже
 становятся корректными (в смысле делимости нацело) в совокупности:

 \f[K = \frac{T_{max}}{\delta t};\ \ \ L = \frac{\delta t}{\Delta t};\ \ \ N = \frac{\Delta t}{dt}\f]
*/

class FilterParameters
{

public:
    /*!
     \brief Конструктор.

     \param maxTime - время \f$T_{max}\f$ окончания фильтрации.
     \param measurementStep - интервал \f$\delta t\f$ между измерениями.
     \param predictionStep - интервал \f$\Delta t\f$ между (дискретными) прогнозами.
     \param integrationStep - шаг интегрирования \f$dt\f$.
     \param sampleSize - размер выборок.
     \param orderMult - кратность порядка фильтра для ФКП (ФОСпп).

     После автоматической коррекции значения могут несколько отличаться от заданных.
    */
    FilterParameters(double maxTime, double measurementStep, double predictionStep, double integrationStep,
                     ulong sampleSize, ulong orderMult = 1);

    //! \brief Возвращает время \f$T_{max}\f$ окончания фильтрации.
    const double &maxTime() const;

    //! \brief Возвращает интервал \f$\delta t\f$ между измерениями.
    const double &measurementStep() const;

    //! \brief Возвращает интервал \f$\Delta t\f$ между прогнозами.
    const double &predictionStep() const;

    //! \brief Возвращает шаг интегрирования \f$dt\f$.
    const double &integrationStep() const;


    //! \brief Возвращает количество \f$K\f$ измерений.
    const ulong &measurementCount() const;

    //! \brief Возвращает количество \f$L\f$ прогнозов между двумя измерениями.
    const ulong &predictionCount() const;

    //! \brief Возвращает количество \f$N\f$ интегрирований между двумя прогнозами.
    const ulong &integrationCount() const;

    //! \brief Возвращает размер выборок.
    const ulong &sampleSize() const;

    //! \brief Возвращает кратность порядка фильтра для ФКП (ФОСпп).
    const ulong &orderMult() const;

    //! \brief Устанавливает время \f$T_{max}\f$ окончания фильтрации.
    void setMaxTime(double tmax);

    //! \brief Устанавливает интервал \f$\delta t\f$ между измерениями.
    void setMeasurementStep(double step);

    //! \brief Устанавливает интервал \f$\Delta t\f$ между прогнозами.
    void setPredictionStep(double step);

    //! \brief Устанавливает шаг интегрирования \f$dt\f$.
    void setIntegrationStep(double step);

    //! \brief Устанавливает размер выборок.
    void setSampleSize(ulong size);

    //! \brief Устанавливает кратность порядка фильтра для ФКП (ФОСпп).
    void setOrderMult(ulong order);


private:
    double m_maxTime;         /*!< Время \f$T_{max}\f$ окончания фильтрации. */
    double m_measurementStep; /*!< Интервал \f$\delta t\f$ между измерениями. */
    double m_predictionStep;  /*!< Интервал \f$\Delta t\f$ между прогнозами. */
    double m_integrationStep; /*!< Шаг интегрирования \f$dt\f$. */

    ulong m_measurementCount; /*!< Количество \f$K\f$ измерений на \f$[0, T_{max}]\f$. */
    ulong m_predictionCount; /*!< Количество \f$L\f$ прогнозов между двумя измерениями. */
    ulong m_integrationCount; /*!< Количество \f$N\f$ интегрирований между двумя прогнозами. */
    ulong m_sampleSize; /*!< Размер выборок. */
    ulong m_orderMult;  /*!< Кратность порядка фильтра. */
};


//! \brief Тип умного указателя на FilterParameters.
using PtrFilterParameters = std::shared_ptr<FilterParameters>;


} // end Core


#endif // FILTER_PARAMETERS_H
