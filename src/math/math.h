#ifndef MATH_H
#define MATH_H

#include <cmath>


namespace Math
{


// --- trigonometric functions ---------------------------------------

using std::sin;
using std::cos;
using std::tan;

inline double sec(double x)
{
    return 1.0 / cos(x);
}

inline double csc(double x)
{
    return 1.0 / sin(x);
}

inline double cot(double x)
{
    return cos(x) / sin(x);
}

using std::asin;
using std::acos;
using std::atan;
using std::atan2;

inline double atan(double y, double x)
{
    return atan2(y, x);
}


// --- hyperbolic functions ------------------------------------------

using std::sinh;
using std::cosh;
using std::tanh;

inline double sech(double x)
{
    return 1.0 / cosh(x);
}

inline double csch(double x)
{
    return 1.0 / sinh(x);
}

inline double coth(double x)
{
    return cosh(x) / sinh(x);
}

inline double asinh(double x)
{
    return std::log(x + std::sqrt(x * x + 1.0));
}

inline double acosh(double x)
{
    return std::log(x + std::sqrt(x * x - 1.0));
}

inline double atanh(double x)
{
    return 0.5 * std::log((1.0 + x) / (1.0 - x));
}

inline double asech(double x)
{
    return std::log((1.0 + std::sqrt(1.0 - x * x)) / x);
}

inline double acsch(double x)
{
    double t = 1.0 / x;
    return std::log(t + std::sqrt(1.0 + t * t));
}

inline double acoth(double x)
{
    return 0.5 * std::log((x + 1.0) / (x - 1.0));
}


// --- exponential functions -----------------------------------------

using std::exp;

inline double exp10(double x)
{
    return std::pow(10.0, x);
}

inline double exp2(double x)
{
    return std::pow(2.0, x);
}

using std::frexp;
using std::ldexp;
using std::log;

inline double ln(double x)
{
    return log(x);
}

inline double log(double b, double x)
{
    return log(x) * log(b);
}

using std::log10;

inline double log2(double x)
{
    return log(x) * 1.44269504088896340735992;
}

using std::pow;
using std::sqrt;

inline double sqrt(double r, double x)
{
    return std::pow(x, 1.0 / r);
}


// --- nearest integer functions -------------------------------------

using std::ceil;
using std::floor;
using std::modf;

inline double round(double x)
{
    return floor(x + 0.5);
}


// --- misc functions ------------------------------------------------

using std::fmod;
using std::abs;
using std::fabs;

inline double frac(double x)
{
    return x - floor(x);
}


// --- other ---------------------------------------------------------

inline double min(double r1, double r2)
{
    return r1 < r2 ? r1 : r2;
}

inline double min(double r1, double r2, double r3)
{
    return min(min(r1, r2), r3);
}

inline double max(double r1, double r2)
{
    return r1 > r2 ? r1 : r2;
}

inline double max(double r1, double r2, double r3)
{
    return max(max(r1, r2), r3);
}

inline constexpr int sign(double r)
{
    return (0.0 < r) - (r < 0.0);
}


} // end Math


#endif // MATH_H
