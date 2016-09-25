#ifndef CONVERT_H
#define CONVERT_H

#include "src/math/constants.h"


namespace Math
{

namespace Convert
{


inline double DegToRad(double x)
{
    return x * Const::PI / 180.0;
}

inline double RadToDeg(double x)
{
    return x * 180.0 / Const::PI;
}


} // end Convert

} // end Math


#endif // CONVERT_H
