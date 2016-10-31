#ifndef FILTER_H
#define FILTER_H

#include "src/config.h"

#ifdef QT_ENABLED
#include <QDebug>
#include <QObject>
#else
#include <iostream>
#endif

#include "filter_output.h"
#include "filter_parameters.h"
#include "info.h"
#include "src/array.h"
#include "src/math/random.h"
#include "src/math/statistic.h"
#include <memory>


using Math::Matrix;
using Math::Vector;
using Math::RowVector;


namespace Core
{


#ifdef QT_ENABLED
class Filter : public QObject
{
    Q_OBJECT
#else
class Filter
{
#endif

public:
    explicit Filter(PtrFilterParameters params);
    virtual ~Filter();

    Filter(const Filter &) = delete;
    Filter &operator=(const Filter &) = delete;

    double run();

    const FilterOutput &result() const;
    PtrFilterParameters params();
    PtrInfo             info() const;

protected:
    virtual void init();
    virtual void zeroIteration() = 0;
    virtual void algorithm()     = 0;
    void writeResult(size_t n, bool copy = false);

#ifdef QT_ENABLED
signals:
#endif

    void updatePercent(int p) const;


protected:
    FilterOutput        m_result;
    PtrFilterParameters m_params;
    PtrInfo             m_info;

    Array<Vector> x, y, z, e; // массивы под выборки для промежуточных вычислений
};


using PtrFilter = std::shared_ptr<Filter>;


} // end Core


#endif // FILTER_H
