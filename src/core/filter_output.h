#ifndef FILTER_OUTPUT_H
#define FILTER_OUTPUT_H

#include "src/array.h"
#include "src/math/matrix.h"

#ifdef QT_ENABLED
#include <QVector>
#endif


namespace Core
{


//! \brief Структура, хранящая значение времени и моментные характеристики, соответствующие ему.
struct SingleFilterOutput {
    Math::Vector meanX; /*!< Математическое ожидание \f$M[X_t]\f$. */
    Math::Vector meanZ; /*!< Математическое ожидание \f$M[Z_t]\f$. */
    Math::Vector meanE; /*!< Математическое ожидание \f$M[E_t], E_t = |X_t - Z_t|\f$. */
    Math::Matrix varX;  /*!< Дисперсия \f$M[X_t]\f$. */
    Math::Matrix varZ;  /*!< Дисперсия \f$M[Z_t]\f$. */
    Math::Matrix varE;  /*!< Дисперсия \f$M[E_t], E_t = |X_t - Z_t|\f$. */
    double       time;  /*!< Время \f$t\f$. */
    Math::Vector upE; /*!< Верхняя граница E*/
    Math::Vector downE; /*!< Нижняя граница E*/
    Math::Vector upX; /*!< Верхняя граница X*/
    Math::Vector downX; /*!< Нижняя граница X*/
    Math::Vector specificX; /*!< Конкретная реализация X выбранной траектории*/
    Math::Vector specificE; /*!< Конкретная реализация E выбранной траектории*/
    Math::Vector specificZ; /*!< Конкретная реализация Z выбранной траектории*/


    /*Только для логико-динамических фильтров*/
    Array<int>   I; /*!< I всех траекторий*/
    Array<int>   evaluationI; /*!< I с крышкой всех траекторий*/
    Array<int>   deltaI; /*!< ΔI всех траекторий*/
    double       PdeltaI; /*!< Вероятность ΔI*/
    double       PI; /*!< Вероятность I*/
};

//struct SpecificParameter {
//    std::string name;
//    Math::Matrix parameter;
//};


//! \brief Тип результата работы фильтра.
using FilterOutput = Array<SingleFilterOutput>;


#ifdef QT_ENABLED
using ArrayDbl  = QVector<double>;
using IndexType = int;
#else
using ArrayDbl  = Array<double>;
using IndexType = std::size_t;
#endif


/*!
 \brief Извлекает время \f$t\f$ из результатов.

 \param[in] fo - массив с результатами работы фильтра.
 \param[out] array - сюда записываются значения.
*/
void GetTime(const FilterOutput &fo, ArrayDbl &array);

/*!
 \brief Извлекает математическое ожидание \f$M[X]\f$ вектора состояния объекта \f$X\f$ из результатов.

 \param[in] fo - массив с результатами работы фильтра.
 \param[in] index - номер координаты.
 \param[out] array - сюда записываются значения.
 \param[in] coeff - коээфициент, на который будет умножен результат оперции (если необходим масштабированный вывод).
*/
void GetMeanX(const FilterOutput &fo, long index, ArrayDbl &array, double coeff = 1.0);

/*!
 \brief Извлекает математическое ожидание \f$M[Z]\f$ вектора оценки \f$Z\f$ из результатов.

 \param[in] fo - массив с результатами работы фильтра.
 \param[in] index - номер координаты.
 \param[out] array - сюда записываются значения.
 \param[in] coeff - коээфициент, на который будет умножен результат оперции (если необходим масштабированный вывод).
*/
void GetMeanZ(const FilterOutput &fo, long index, ArrayDbl &array, double coeff = 1.0);

/*!
 \brief Извлекает математическое ожидание \f$M[E]\f$ вектора погрешности \f$E\f$ из результатов.

 \param[in] fo - массив с результатами работы фильтра.
 \param[in] index - номер координаты.
 \param[out] array - сюда записываются значения.
 \param[in] coeff - коээфициент, на который будет умножен результат оперции (если необходим масштабированный вывод).
*/
void GetMeanE(const FilterOutput &fo, long index, ArrayDbl &array, double coeff = 1.0);

/*!
 \brief Извлекает среднеквадратическое отклонение \f$\sqrt{D[X]}\f$ вектора состояния объекта \f$X\f$ из результатов.

 \param[in] fo - массив с результатами работы фильтра.
 \param[in] index - номер координаты.
 \param[out] array - сюда записываются значения.
 \param[in] coeff - коээфициент, на который будет умножен результат оперции (если необходим масштабированный вывод).
*/
void GetStdDeviationX(const FilterOutput &fo, long index, ArrayDbl &array, double coeff = 1.0);

/*!
 \brief Извлекает среднеквадратическое отклонение \f$\sqrt{D[Z]}\f$ вектора оценки \f$Z\f$ из результатов.

 \param[in] fo - массив с результатами работы фильтра.
 \param[in] index - номер координаты.
 \param[out] array - сюда записываются значения.
 \param[in] coeff - коээфициент, на который будет умножен результат оперции (если необходим масштабированный вывод).
*/
void GetStdDeviationZ(const FilterOutput &fo, long index, ArrayDbl &array, double coeff = 1.0);

/*!
 \brief Извлекает среднеквадратическое отклонение \f$\sqrt{D[E]}\f$ вектора погрешности \f$E\f$ из результатов.

 \param[in] fo - массив с результатами работы фильтра.
 \param[in] index - номер координаты.
 \param[out] array - сюда записываются значения.
 \param[in] coeff - коээфициент, на который будет умножен результат оперции (если необходим масштабированный вывод).
*/
void GetStdDeviationE(const FilterOutput &fo, long index, ArrayDbl &array, double coeff = 1.0);



void GetUpE(const FilterOutput &fo, long index, ArrayDbl &array, double coeff = 1.0);

void GetDownE(const FilterOutput &fo, long index, ArrayDbl &array, double coeff = 1.0);

void GetRealizationE(const FilterOutput &fo, long index, ArrayDbl &array, double coeff = 1.0);

void GetUpX(const FilterOutput &fo, long index, ArrayDbl &array, double coeff = 1.0);

void GetDownX(const FilterOutput &fo, long index, ArrayDbl &array, double coeff = 1.0);

void GetRealizationX(const FilterOutput &fo, long index, ArrayDbl &array, double coeff = 1.0);

void GetRealizationZ(const FilterOutput &fo, long index, ArrayDbl &array, double coeff = 1.0);

void GetI(const FilterOutput &fo, long numberTraektor, ArrayDbl &array, double coeff);

void GetEvaluationI(const FilterOutput &fo, long numberTraektor, ArrayDbl &array, double coeff);

void GetDeltaI(const FilterOutput &fo, long numberTraektor, ArrayDbl &array, double coeff);

void GetPI(const FilterOutput &fo, ArrayDbl &array, double coeff);

void GetPDeltaI(const FilterOutput &fo, ArrayDbl &array, double coeff);

} // end Core


#endif // FILTER_OUTPUT_H
