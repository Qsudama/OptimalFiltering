#ifndef CONVERT_H
#define CONVERT_H

#include "src/math/constants.h"


namespace Math
{

/*!
\brief Модуль, содержащий описание и реализацию методов конвертирования значений между системами.
*/

namespace Convert
{

/*!
 * \brief Переводит градусы в радианы.
 * \param[in] value - величина в градусах.
 * \return значение value в радианах.
 */

inline double DegToRad(double value)
{
    return value * Const::PI / 180.0;
}

/*!
 * \brief Переводит радианы в градусы.
 * \param[in] value - величина в радианах.
 * \return значение value в градусах.
 */

inline double RadToDeg(double value)
{
    return value * 180.0 / Const::PI;
}


} // end Convert

} // end Math


#endif // CONVERT_H
