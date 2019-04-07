#ifndef FILTER_PARAMETERS_H
#define FILTER_PARAMETERS_H

#include "src/config.h"
#include "src/core/types_info.h"
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
 Интервал между прогнозами \f$\delta \tau\f$.

 Шаг интегрирования системы \f$\Delta t\f$.

 Эти величины корректируются (при изменении) так, чтобы
 количество интегрирований \f$N\f$ между соседними прогнозами,
 количество прогнозов \f$L\f$ между соседними измерениями и
 количесвтво измерений \f$K\f$ на всём временном интервале были целыми числами.

 То есть \f$\Delta t, \delta \tau, \delta t\f$ и \f$T_{max}\f$ немного подкручиваются так, что выражения ниже
 становятся корректными (в смысле делимости нацело) в совокупности:

 \f[K = \frac{T_{max}}{\delta t};\ \ \ L = \frac{\delta t}{\delta \tau};\ \ \ N = \frac{\delta \tau}{\Delta t}\f]
*/

class FilterParameters
{

public:
    /*!
    \brief Конструктор.

    \param maxTime - время \f$T_{max}\f$ окончания фильтрации.
    \param measurementStep - интервал \f$\delta t\f$ между измерениями.
    \param predictionStep - интервал \f$\delta \tau\f$ между (дискретными) прогнозами.
    \param integrationStep - шаг интегрирования \f$\Delta t\f$.
    \param sampleSize - размер выборок.
    \param specificRealization - номер запоминаемой реализации.
    \param orderMult - кратность порядка фильтра для ФКП (ФОСпп).
    \param argumentsCount - Количество аргументов в фильтре (дискретные фильтры).

    После автоматической коррекции значения могут несколько отличаться от заданных.
    */
    FilterParameters(double maxTime, double measurementStep, double predictionStep, double integrationStep,
                     Uint sampleSize, Uint specificRealization, Uint orderMult = 1, Uint argumentsCount = 2);

    //! \brief Возвращает время \f$T_{max}\f$ окончания фильтрации.
    const double &maxTime() const;

    //! \brief Возвращает интервал \f$\delta t\f$ между измерениями.
    const double &measurementStep() const;

    //! \brief Возвращает интервал \f$\delta \tau\f$ между прогнозами.
    const double &predictionStep() const;

    //! \brief Возвращает шаг интегрирования \f$\Delta t\f$.
    const double &integrationStep() const;


    //! \brief Возвращает количество \f$K\f$ измерений.
    const Uint &measurementCount() const;

    //! \brief Возвращает количество \f$L\f$ прогнозов между двумя измерениями.
    const Uint &predictionCount() const;

    //! \brief Возвращает количество \f$N\f$ интегрирований между двумя прогнозами.
    const Uint &integrationCount() const;

    //! \brief Возвращает размер выборок.
    const Uint &sampleSize() const;

    //! \brief Возвращает номер запоминаемой реализации.
    const Uint &specificRealization() const;

    //! \brief Возвращает кратность порядка фильтра для ФКП (ФОСпп).
    const Uint &orderMult() const;

    //! \brief Возвращает кол-во аргументов для ФМП, ДФМП, МФМП и МДФМП.
    const Uint &argumentsCount() const;

    //! \brief Возвращает cпособ задания начальных данных.
    const Core::INITIAL_CONDITIONS &initialCondition() const;

    //! \brief Устанавливает время \f$T_{max}\f$ окончания фильтрации.
    void setMaxTime(double tmax);

    //! \brief Устанавливает интервал \f$\delta t\f$ между измерениями.
    void setMeasurementStep(double step);

    //! \brief Устанавливает интервал \f$\delta \tau\f$ между прогнозами.
    void setPredictionStep(double step);

    //! \brief Устанавливает шаг интегрирования \f$\Delta t\f$.
    void setIntegrationStep(double step);

    //! \brief Устанавливает размер выборок.
    void setSampleSize(Uint size);

    //! \brief Устанавливает номер запоминаемой реализации.
    //! Возвращает true если получилось установить значение. Иначе false.
    bool setSpecificRealization(Uint realization);

    //! \brief Устанавливает кратность порядка фильтра для ФКП (ФОСпп).
    void setOrderMult(Uint order);

    //! \brief Устанавливает кол-во аргументов для ФМП, ДФМП, МФМП и МДФМП.
    void setArgumentsCount(Uint order);

    //! \brief Устанавливает cпособ задания начальных данных.
    void setInitialCondition(Core::INITIAL_CONDITIONS condition);


private:
    /*! \brief Корректирует величину шага и количество шагов в данном интервале.
     *
     * \param[in] intervalLength - длина \f$d\f$ интервала.
     * \param[in,out] step - шаг \f$h\f$ (корректируется, если необходимо).
     * \param[out] count - количиство \f$N\f$ шагов \f$h\f$, укладывающихся в \f$d\f$ (вычисляется).
     *
     * Корректировка работает следующим образом:
     *
     * Сначала вычисляется целое число \f$N' = \frac{d}{h}\f$ (дробная часть отбрасывается).
     *
     * Если \f$\frac{d}{h} - N' >= \frac{1}{2}\f$, то \f$N = N' + 1\f$, иначе \f$N = N'\f$.
     *
     * Теперь корректируется шаг: \f$h = \frac{d}{N}\f$.
     */
    void correctStepAndCount(const double &intervalLength, double &step, Uint &count);


private:
    double m_maxTime;         /*!< Время \f$T_{max}\f$ окончания фильтрации. */
    double m_measurementStep; /*!< Интервал \f$\delta t\f$ между измерениями. */
    double m_predictionStep;  /*!< Интервал \f$\delta \tau\f$ между прогнозами. */
    double m_integrationStep; /*!< Шаг интегрирования \f$\Delta t\f$. */

    Uint m_measurementCount; /*!< Количество \f$K\f$ измерений на \f$[0, T_{max}]\f$. */
    Uint m_predictionCount; /*!< Количество \f$L\f$ прогнозов между двумя измерениями. */
    Uint m_integrationCount; /*!< Количество \f$N\f$ интегрирований между двумя прогнозами. */
    Uint m_sampleSize; /*!< Размер выборок. */
    Uint m_specificRealization; /*!< Номер запоминаемой реализации. */
    Uint m_orderMult;  /*!< Кратность порядка фильтра. */
    Uint m_argumentsCount;  /*!< Кол-фо аргументов фильтра. */

    INITIAL_CONDITIONS m_initialCondition; /*! Cпособ задания начальных данных. */
};


//! \brief Тип умного указателя на FilterParameters.
using PtrFilterParameters = std::shared_ptr<FilterParameters>;


} // end Core


#endif // FILTER_PARAMETERS_H
