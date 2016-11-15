#ifndef CONVERT_H
#define CONVERT_H

#include "src/math/constants.h"


namespace Math
{

namespace Convert
{


inline double DegToRad(double value)
{
    return value * Const::PI / 180.0;
}

inline double RadToDeg(double value)
{
    return value * 180.0 / Const::PI;
}


} // end Convert

} // end Math


#endif // CONVERT_H
