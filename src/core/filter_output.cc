#include "filter_output.h"
#include "src/math/math.h"

#include "src/helpers/alert_helper.h"

using std::sqrt;

namespace Core
{


void GetTime(const FilterOutput &fo, ArrayDbl &array)
{
    array.resize(IndexType(fo.size()));
    for (size_t i = 0; i < size_t(fo.size()); ++i) {
        array[IndexType(i)] = fo[i].time;
    }
}

void GetMeanX(const FilterOutput &fo, long index, ArrayDbl &array, double coeff)
{
//    assert(index >= 0 && index < fo[0].meanX.size() && "Core::GetMeanX(fo, index, array, coeff) : out of range");
    if (index < 0 && index >= fo[0].meanX.size()) {
        AlertHelper::showErrorAlertWithText("Core::GetMeanX\nВыход за пределы массива!");
        return;
    }
    array.resize(IndexType(fo.size()));
    for (size_t i = 0; i < size_t(fo.size()); ++i) {
        array[IndexType(i)] = coeff * fo[i].meanX[index];
    }
}

void GetMeanZ(const FilterOutput &fo, long index, ArrayDbl &array, double coeff)
{
//    assert(index >= 0 && index < fo[0].meanZ.size() && "Core::GetMeanZ(fo, index, array, coeff) : out of range");
    if (index < 0 && index >= fo[0].meanZ.size()) {
        AlertHelper::showErrorAlertWithText("Core::GetMeanZ\nВыход за пределы массива!");
        return;
    }
    array.resize(IndexType(fo.size()));
    for (size_t i = 0; i < size_t(fo.size()); ++i) {
        array[IndexType(i)] = coeff * fo[i].meanZ[index];
    }
}

void GetMeanE(const FilterOutput &fo, long index, ArrayDbl &array, double coeff)
{
//    assert(index >= 0 && index < fo[0].meanE.size() && "Core::GetMeanE(fo, index, array, coeff) : out of range");
    if (index < 0 && index >= fo[0].meanE.size()) {
        AlertHelper::showErrorAlertWithText("Core::GetMeanE\nВыход за пределы массива!");
        return;
    }
    array.resize(IndexType(fo.size()));
    for (size_t i = 0; i < size_t(fo.size()); ++i) {
        array[IndexType(i)] = coeff * fo[i].meanE[index];
    }
}

void GetStdDeviationX(const FilterOutput &fo, long index, ArrayDbl &array, double coeff)
{
//    assert(index >= 0 && index < fo[0].varX.rows() && "Core::GetStdDeviationX(fo, index, array, coeff) : out of range");
    if (index < 0 && index >= fo[0].varX.rows()) {
        AlertHelper::showErrorAlertWithText("Core::GetStdDeviationX\nВыход за пределы массива!");
        return;
    }
    array.resize(IndexType(fo.size()));
    for (size_t i = 0; i < size_t(fo.size()); ++i) {
        array[IndexType(i)] = coeff * sqrt(fo[i].varX(index, index));
    }
}

void GetStdDeviationZ(const FilterOutput &fo, long index, ArrayDbl &array, double coeff)
{
//    assert(index >= 0 && index < fo[0].varZ.rows() && "Core::GetStdDeviationZ(fo, index, array, coeff) : out of range");
    if (index < 0 && index >= fo[0].varZ.rows()) {
        AlertHelper::showErrorAlertWithText("Core::GetStdDeviationZ\nВыход за пределы массива!");
        return;
    }
    array.resize(IndexType(fo.size()));
    for (size_t i = 0; i < size_t(fo.size()); ++i) {
        array[IndexType(i)] = coeff * sqrt(fo[i].varZ(index, index));
    }
}

void GetStdDeviationE(const FilterOutput &fo, long index, ArrayDbl &array, double coeff)
{
//    assert(index >= 0 && index < fo[0].varE.rows() && "Core::GetStdDeviationE(fo, index, array, coeff) : out of range");
    if (index < 0 && index >= fo[0].varE.rows()) {
        AlertHelper::showErrorAlertWithText("Core::GetStdDeviationE\nВыход за пределы массива!");
        return;
    }
    array.resize(IndexType(fo.size()));
    for (size_t i = 0; i < size_t(fo.size()); ++i) {
        array[IndexType(i)] = coeff * sqrt(fo[i].varE(index, index));
    }
}

void GetMeanIntegralE(const FilterOutput &fo, long index, ArrayDbl &array, double coeff)
{
//    assert(index >= 0 && index < fo[0].meanX.size() && "Core::GetMeanX(fo, index, array, coeff) : out of range");
//    if (index < 0 && index >= fo[0].meanIntegralE.rows()) {
//        AlertHelper::showErrorAlertWithText("Core::GetMeanIntegralE\nВыход за пределы массива!");
//        return;
//    }
    array.resize(IndexType(fo.size()));
    for (size_t i = 0; i < size_t(fo.size()); ++i) {
        array[IndexType(i)] = coeff * fo[i].meanIntegralE;//[index];
    }
}

void GetUpE(const FilterOutput &fo, long index, ArrayDbl &array, double coeff)
{
//    assert(index >= 0 && index < fo[0].varE.rows() && "Core::GetStdDeviationE(fo, index, array, coeff) : out of range");
//    if (index < 0 && index >= fo[0].upE.rows()) {
//        AlertHelper::showErrorAlertWithText("Core::GetUpE\nВыход за пределы массива!");
//        return;
//    }
    array.resize(IndexType(fo.size()));
    for (size_t i = 0; i < size_t(fo.size()); ++i) {
        array[IndexType(i)] = coeff * fo[i].upE;
    }
}

void GetDownE(const FilterOutput &fo, long index, ArrayDbl &array, double coeff)
{
//    assert(index >= 0 && index < fo[0].varE.rows() && "Core::GetStdDeviationE(fo, index, array, coeff) : out of range");
//    if (index < 0 && index >= fo[0].varE.rows()) {
//        AlertHelper::showErrorAlertWithText("Core::GetDownE\nВыход за пределы массива!");
//        return;
//    }
    array.resize(IndexType(fo.size()));
    for (size_t i = 0; i < size_t(fo.size()); ++i) {
        array[IndexType(i)] = coeff * fo[i].downE;
    }
}

void GetRealizationE(const FilterOutput &fo, long index, ArrayDbl &array, double coeff)
{
//    assert(index >= 0 && index < fo[0].varE.rows() && "Core::GetStdDeviationE(fo, index, array, coeff) : out of range");
    if (index < 0 && index >= fo[0].specificE.rows()) {
        AlertHelper::showErrorAlertWithText("Core::GetRealizationE\nВыход за пределы массива!");
        return;
    }
    array.resize(IndexType(fo.size()));
    for (size_t i = 0; i < size_t(fo.size()); ++i) {
        array[IndexType(i)] = coeff * fo[i].specificE[index];
    }
}

void GetMeanIntegralX(const FilterOutput &fo, long index, ArrayDbl &array, double coeff)
{
//    assert(index >= 0 && index < fo[0].meanX.size() && "Core::GetMeanX(fo, index, array, coeff) : out of range");
//    if (index < 0 && index >= fo[0].meanIntegralE.rows()) {
//        AlertHelper::showErrorAlertWithText("Core::GetMeanIntegralE\nВыход за пределы массива!");
//        return;
//    }
    array.resize(IndexType(fo.size()));
    for (size_t i = 0; i < size_t(fo.size()); ++i) {
        array[IndexType(i)] = coeff * fo[i].meanIntegralX;//[index];
    }
}

void GetUpX(const FilterOutput &fo, long index, ArrayDbl &array, double coeff)
{
//    assert(index >= 0 && index < fo[0].varE.rows() && "Core::GetStdDeviationE(fo, index, array, coeff) : out of range");
//    if (index < 0 && index >= fo[0].upE.rows()) {
//        AlertHelper::showErrorAlertWithText("Core::GetUpE\nВыход за пределы массива!");
//        return;
//    }
    array.resize(IndexType(fo.size()));
    for (size_t i = 0; i < size_t(fo.size()); ++i) {
        array[IndexType(i)] = coeff * fo[i].upX;
    }
}

void GetDownX(const FilterOutput &fo, long index, ArrayDbl &array, double coeff)
{
//    assert(index >= 0 && index < fo[0].varE.rows() && "Core::GetStdDeviationE(fo, index, array, coeff) : out of range");
//    if (index < 0 && index >= fo[0].varE.rows()) {
//        AlertHelper::showErrorAlertWithText("Core::GetDownE\nВыход за пределы массива!");
//        return;
//    }
    array.resize(IndexType(fo.size()));
    for (size_t i = 0; i < size_t(fo.size()); ++i) {
        array[IndexType(i)] = coeff * fo[i].downX;
    }
}

void GetRealizationX(const FilterOutput &fo, long index, ArrayDbl &array, double coeff)
{
//    assert(index >= 0 && index < fo[0].varE.rows() && "Core::GetStdDeviationE(fo, index, array, coeff) : out of range");
    if (index < 0 && index >= fo[0].specificX.rows()) {
        AlertHelper::showErrorAlertWithText("Core::GetRealizationE\nВыход за пределы массива!");
        return;
    }
    array.resize(IndexType(fo.size()));
    for (size_t i = 0; i < size_t(fo.size()); ++i) {
        array[IndexType(i)] = coeff * fo[i].specificX[index];
    }
}


} // end Core
