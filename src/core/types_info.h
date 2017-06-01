#ifndef TYPES_INFO_H
#define TYPES_INFO_H


namespace Core
{


//! \brief Набор возможных типов фильтров.
enum class FILTER_TYPE {
    Continuous,         /*!< Непрерывный фильтр. */
    ContinuousDiscrete, /*!< Непрерывно-дискретный фильтр. */
    Discrete,           /*!< Дискретный фильтр. */
    LogicDynamic        /*!< Логико-динамический фильтр. */
};

//! \brief Набор возможных приближений для фильтров оптимальной структуры.
enum class APPROX_TYPE {
    Linear, /*!< Линейное приближение. */
    Gauss   /*!< Гауссовское приближение. */
};


} // end Core


#endif // TYPES_INFO_H
