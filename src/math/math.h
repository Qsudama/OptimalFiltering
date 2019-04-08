#ifndef MATH_H
#define MATH_H

#include <cmath>


/*!
\brief Модуль, содержащий описание и реализацию задач математических методов и типов.
*/

namespace Math
{

inline constexpr int sign(double r)
{
    return (0.0 < r) - (r < 0.0);
}

} // end Math


#endif // MATH_H
