#include "filter_output.h"
#include "src/math/math.h"


namespace Core
{


void GetTime(const FilterOutput &fo, ArrayDbl &array)
{
    array.resize(int(fo.size()));
    for (size_t i = 0; i < size_t(fo.size()); ++i) {
        array[i] = fo[i].time;
    }
}

void GetMeanX(const FilterOutput &fo, long index, ArrayDbl &array, double coeff)
{
    assert(index >= 0 && index < fo[0].meanX.size() && "Core::GetMeanX(fo, index, array, coeff) : out of range");

    array.resize(int(fo.size()));
    for (size_t i = 0; i < size_t(fo.size()); ++i) {
        array[i] = coeff * fo[i].meanX[index];
    }
}

void GetMeanZ(const FilterOutput &fo, long index, ArrayDbl &array, double coeff)
{
    assert(index >= 0 && index < fo[0].meanZ.size() && "Core::GetMeanZ(fo, index, array, coeff) : out of range");

    array.resize(int(fo.size()));
    for (size_t i = 0; i < size_t(fo.size()); ++i) {
        array[i] = coeff * fo[i].meanZ[index];
    }
}

void GetMeanE(const FilterOutput &fo, long index, ArrayDbl &array, double coeff)
{
    assert(index >= 0 && index < fo[0].meanE.size() && "Core::GetMeanE(fo, index, array, coeff) : out of range");

    array.resize(int(fo.size()));
    for (size_t i = 0; i < size_t(fo.size()); ++i) {
        array[i] = coeff * fo[i].meanE[index];
    }
}

void GetStdDeviationX(const FilterOutput &fo, long index, ArrayDbl &array, double coeff)
{
    assert(index >= 0 && index < fo[0].varX.rows() && "Core::GetStdDeviationX(fo, index, array, coeff) : out of range");

    array.resize(int(fo.size()));
    for (size_t i = 0; i < size_t(fo.size()); ++i) {
        array[i] = coeff * Math::sqrt(fo[i].varX(index, index));
    }
}

void GetStdDeviationZ(const FilterOutput &fo, long index, ArrayDbl &array, double coeff)
{
    assert(index >= 0 && index < fo[0].varZ.rows() && "Core::GetStdDeviationZ(fo, index, array, coeff) : out of range");

    array.resize(int(fo.size()));
    for (size_t i = 0; i < size_t(fo.size()); ++i) {
        array[i] = coeff * Math::sqrt(fo[i].varZ(index, index));
    }
}

void GetStdDeviationE(const FilterOutput &fo, long index, ArrayDbl &array, double coeff)
{
    assert(index >= 0 && index < fo[0].varE.rows() && "Core::GetStdDeviationE(fo, index, array, coeff) : out of range");

    array.resize(int(fo.size()));
    for (size_t i = 0; i < size_t(fo.size()); ++i) {
        array[i] = coeff * Math::sqrt(fo[i].varE(index, index));
    }
}


} // end Core
