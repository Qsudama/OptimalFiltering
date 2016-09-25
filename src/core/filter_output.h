#ifndef FILTER_OUTPUT_H
#define FILTER_OUTPUT_H

#include "src/array.h"
#include "src/math/matrix.h"

#ifdef QT_ENABLED
#include <QVector>
#endif


namespace Core
{


struct SingleFilterOutput {
    Math::Vector mx;
    Math::Vector mz;
    Math::Vector me;
    Math::Matrix Dx;
    Math::Matrix Dz;
    Math::Matrix De;
    double       t;
};

using FilterOutput = Array<SingleFilterOutput>;


#ifdef QT_ENABLED
using ArrayDbl = QVector<double>;
#else
using ArrayDbl = Array<double>;
#endif

void GetTime(const FilterOutput &fo, ArrayDbl &array);
void GetMeanX(const FilterOutput &fo, long index, ArrayDbl &array, double coeff = 1.0);
void GetMeanZ(const FilterOutput &fo, long index, ArrayDbl &array, double coeff = 1.0);
void GetMeanE(const FilterOutput &fo, long index, ArrayDbl &array, double coeff = 1.0);
void GetStdDeviationX(const FilterOutput &fo, long index, ArrayDbl &array, double coeff = 1.0);
void GetStdDeviationZ(const FilterOutput &fo, long index, ArrayDbl &array, double coeff = 1.0);
void GetStdDeviationE(const FilterOutput &fo, long index, ArrayDbl &array, double coeff = 1.0);


} // end Core


#endif // FILTER_OUTPUT_H
