#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <limits>


namespace Math
{


class Const
{
public:
    static constexpr double PI    = 3.14159265358979323846264;
    static constexpr double E     = 2.71828182845904523536029;
    static constexpr double GAMMA = 0.57721566490153286060651;
    static constexpr double EPS   = std::numeric_limits<double>::epsilon();
    static constexpr double MIN   = std::numeric_limits<double>::min();
    static constexpr double MAX   = std::numeric_limits<double>::max();
    static constexpr double INF   = std::numeric_limits<double>::infinity();
};


} // end Math


#endif // CONSTANTS_H
