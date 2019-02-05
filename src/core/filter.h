#ifndef FILTER_H
#define FILTER_H

#include "src/config.h"

#ifdef QT_ENABLED
#include <QDebug>
#include <QObject>
#else
#include <iostream>
#endif

#include "filter_output.h"
#include "filter_parameters.h"
#include "info.h"
#include "src/array.h"
#include "src/math/random.h"
#include "src/math/statistic.h"
#include <memory>

#include "src/helpers/timer_manager.h"


using Math::Matrix;
using Math::Vector;
using Math::RowVector;


/*!
 \brief Модуль, содержащий основные классы и методы.

 Большая часть классов здесь абстрактные, т.е. предоставляют лишь общие интерфейсы.
*/

namespace Core
{


//! \brief Базовый класс для всех фильтров.

#ifdef QT_ENABLED
class Filter : public QObject
{
    Q_OBJECT
#else
class Filter
{
#endif

public:
    //! \brief Конструктор.
    explicit Filter(PtrFilterParameters params);

    //! \brief Деструктор.
    virtual ~Filter();


    Filter(const Filter &) = delete;
    Filter &operator=(const Filter &) = delete;

    /*!
     \brief Запускает фильтрацию.

     \return полное время работы.

     Схематически, полный цикл работы фильтра выглядит так:
     \code
     init();
     zeroIteration();
     algorithm();
     \endcode
    */
    void run();

    //! \brief Возвращает результат работы (не имеет смысла до вызова run()).
    const FilterOutput &result() const;

    //! \brief Возвращает текущие параметры фильтра.
    PtrFilterParameters params();

    //! \brief Возвращает информацию о фильтре (имя, тип).
    PtrInfo info() const;

    //! \brief Менеджер таймера работы фильтра.
    TimerManager& timerInstance;

protected:
    //! \brief Инициализирует массивы по входным данным.
    virtual void init();

    //! \brief Выполняет нулевую итерацию алгоритма.
    virtual void zeroIteration() = 0;

    //! \brief Выполняет основной алгоритм.
    virtual void algorithm() = 0;

    /*!
     \brief Вычисляет и записывает результаты для времени \f$t_n\f$.

     \param n - результат записывается в ячейку с этим индексом.
     \param copy - если false (по-умолчанию), то вычисляются все характеристики, если true,
                   то только характеристики для \f$x_{t_n}\f$, остальные копируются из предыдущей ячейки
                   (опция для дискретных фильтров - "порожки" на графиках).
    */
    void writeResult(size_t n, bool copy = false);
    void writeResult(size_t n, int countI);
#ifdef QT_ENABLED
signals:
#endif

    //! \brief Информирует о прогрессе выполнения алгоритма (в процентах).
    void updatePercent(int p) const;


protected:
    FilterOutput        m_result; /*!< Результаты работы. */
    PtrFilterParameters m_params; /*!< Параметры. */
    PtrInfo             m_info;   /*!< Информация (имя, тип). */

    Array<Vector> m_sampleX; /*!< Массив под выборку \f$X\f$. */
    Array<Vector> m_sampleY; /*!< Массив под выборку \f$Y\f$. */
    Array<Vector> m_sampleZ; /*!< Массив под выборку \f$Z\f$. */
    Array<Vector> m_sampleE; /*!< Массив под выборку \f$E\f$. */
    Array<int>    m_sampleI; /*!< Массив для выборки режимов (Только для логик-динамических фильтров). */

    Math::MultivariateNormalDistribution m_normalRand; /*!< Генератор гауссовских случайных векторов. */
};


//! \brief Тип умного указателя на Filter.
using PtrFilter = std::shared_ptr<Filter>;


} // end Core


#endif // FILTER_H
