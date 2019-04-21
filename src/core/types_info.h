#ifndef TYPES_INFO_H
#define TYPES_INFO_H


namespace Core
{


//! \brief Набор возможных типов фильтров.
enum FILTER_TYPE {
    Continuous,         /*!< Непрерывный фильтр. */
    ContinuousDiscrete, /*!< Непрерывно-дискретный фильтр. */
    Discrete,           /*!< Дискретный фильтр. */
    LogicDynamic        /*!< Логико-динамический фильтр. */
};

//! \brief Набор возможных приближений для фильтров оптимальной структуры.
enum APPROX_TYPE {
    Linear, /*!< Линейное приближение. */
    Gauss   /*!< Гауссовское приближение. */
};

//! \brief Набор возможных способов задания начальных данных для фильтров.
enum INITIAL_CONDITIONS {
    GaussApproximation,  /*!< Гауссовское приближение */
    MonteCarlo  /*!< Метод Монте–Карло. */
};

} // end Core


#endif // TYPES_INFO_H
