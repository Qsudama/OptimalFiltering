#include "filter.h"
#include <ctime>


using Math::Statistic::Mean;
using Math::Statistic::Var;

namespace Core
{


Filter::Filter(PtrFilterParameters params)
    :
#ifdef QT_ENABLED
    QObject(nullptr)
    ,
#endif
    m_params(params)
    , m_info(new Info())
{
}

Filter::~Filter()
{
}

double Filter::run()
{
    init();
    std::clock_t timeStart = std::clock();
    zeroIteration();
    algorithm();
    return double((std::clock() - timeStart) / double(CLOCKS_PER_SEC));
}

const FilterOutput &Filter::result() const
{
    return m_result;
}

PtrFilterParameters Filter::params()
{
    return m_params;
}

PtrInfo Filter::info() const
{
    return m_info;
}

void Filter::init()
{
    x.resize(m_params->sampleSize());
    y.resize(m_params->sampleSize());
    z.resize(m_params->sampleSize());
    e.resize(m_params->sampleSize());

    size_t size = size_t(m_params->measurementCount() * m_params->predictionCount() * m_params->integrationCount());
    m_result.resize(size);
    for (size_t i = 0; i < size; ++i) {
        m_result[i].t = m_params->integrationStep() * i;
    }
}

void Filter::writeResult(size_t n, bool copy)
{
    assert(n < m_result.size());
    m_result[n].mx = Mean(x);
    m_result[n].Dx = Var(x, m_result[n].mx);

    if (copy) {
        m_result[n].mz = m_result[n - 1].mz;
        m_result[n].me = m_result[n - 1].me;
        m_result[n].Dz = m_result[n - 1].Dz;
        m_result[n].De = m_result[n - 1].De;
    } else {
        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
            e[s] = x[s] - z[s];
        }
        m_result[n].mz = Mean(z);
        m_result[n].Dz = Var(z, m_result[n].mz);
        m_result[n].me = Mean(e);
        m_result[n].De = Var(e, m_result[n].me);
    }

#ifdef QT_ENABLED
    emit updatePercent(int(100 * n / m_result.size()));
#else
    updatePercent(int(100 * n / m_result.size()));
#endif
}

#ifndef QT_ENABLED
void Filter::updatePercent(int p) const
{
    std::cerr << "Progress: " << p << "% complete.\n";
}
#endif


} // end Core
