#ifndef MATH_H
#define MATH_H

#include <cmath>
#include "src/math/matrix.h"

using Math::Vector;
using Math::Matrix;

/*!
\brief Модуль, содержащий описание и реализацию задач математических методов и типов.
*/

namespace Math
{

inline Vector sqrt(Vector x)
{
    Vector result(x.size());
    for (int i = 0; i < x.size(); i++) {
        result[i] = std::sqrt(x[i]);
    }
    return result;
}

inline Matrix sqrt(Matrix x)
{
    Matrix result(x.rows(), x.cols());
    for (int i = 0; i < x.rows(); i++) {
        for (int j = 0; j < x.cols(); j++) {
            result(i, j) = std::sqrt(x(i, j));
        }
    }
    return result;
}

inline constexpr int sign(double r)
{
    return (0.0 < r) - (r < 0.0);
}

} // end Math


#endif // MATH_H
