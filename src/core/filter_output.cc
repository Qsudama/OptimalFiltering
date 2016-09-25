#include "filter_output.h"
#include "src/math/math.h"


namespace Core
{


void GetTime(const FilterOutput &fo, ArrayDbl &array)
{
    array.resize(fo.size());
    for (size_t i = 0; i < size_t(fo.size()); ++i) {
        array[i] = fo[i].t;
    }
}

void GetMeanX(const FilterOutput &fo, long index, ArrayDbl &array, double coeff)
{
    assert(index >= 0 && index < fo[0].mx.size());
    array.resize(fo.size());
    for (size_t i = 0; i < size_t(fo.size()); ++i) {
        array[i] = coeff * fo[i].mx[index];
    }
}

void GetMeanZ(const FilterOutput &fo, long index, ArrayDbl &array, double coeff)
{
    assert(index >= 0 && index < fo[0].mz.size());
    array.resize(fo.size());
    for (size_t i = 0; i < size_t(fo.size()); ++i) {
        array[i] = coeff * fo[i].mz[index];
    }
}

void GetMeanE(const FilterOutput &fo, long index, ArrayDbl &array, double coeff)
{
    assert(index >= 0 && index < fo[0].me.size());
    array.resize(fo.size());
    for (size_t i = 0; i < size_t(fo.size()); ++i) {
        array[i] = coeff * fo[i].me[index];
    }
}

void GetStdDeviationX(const FilterOutput &fo, long index, ArrayDbl &array, double coeff)
{
    assert(index >= 0 && index < fo[0].Dx.rows());
    array.resize(fo.size());
    for (size_t i = 0; i < size_t(fo.size()); ++i) {
        array[i] = coeff * Math::sqrt(fo[i].Dx(index, index));
    }
}

void GetStdDeviationZ(const FilterOutput &fo, long index, ArrayDbl &array, double coeff)
{
    assert(index >= 0 && index < fo[0].Dz.rows());
    array.resize(fo.size());
    for (size_t i = 0; i < size_t(fo.size()); ++i) {
        array[i] = coeff * Math::sqrt(fo[i].Dz(index, index));
    }
}

void GetStdDeviationE(const FilterOutput &fo, long index, ArrayDbl &array, double coeff)
{
    assert(index >= 0 && index < fo[0].De.rows());
    array.resize(fo.size());
    for (size_t i = 0; i < size_t(fo.size()); ++i) {
        array[i] = coeff * Math::sqrt(fo[i].De(index, index));
    }
}


} // end Core
