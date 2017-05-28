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
    , m_info(std::make_shared<Info>())
{
    m_normalRand.setSeed(Math::RandomProperties::defaultSeed());
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
    m_sampleX.resize(m_params->sampleSize());
    m_sampleY.resize(m_params->sampleSize());
    m_sampleZ.resize(m_params->sampleSize());
    m_sampleE.resize(m_params->sampleSize());

    size_t size = size_t(m_params->measurementCount() * m_params->predictionCount() * m_params->integrationCount());
    m_result.resize(size);
    for (size_t i = 0; i < size; ++i) {
        m_result[i].time = m_params->integrationStep() * i;
    }
}

void Filter::writeResult(size_t n, bool copy)
{
    assert(n < m_result.size() && "Core::Filter::writeResult(n, copy) : out of range (n >= size)");

    m_result[n].meanX = Mean(m_sampleX);
    m_result[n].varX  = Var(m_sampleX, m_result[n].meanX);

    if (copy) {
        m_result[n].meanZ = m_result[n - 1].meanZ;
        m_result[n].meanE = m_result[n - 1].meanE;
        m_result[n].varZ  = m_result[n - 1].varZ;
        m_result[n].varE  = m_result[n - 1].varE;
    } else {
        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
            m_sampleE[s] = m_sampleX[s] - m_sampleZ[s];
        }
        m_result[n].meanZ = Mean(m_sampleZ);
        m_result[n].varZ  = Var(m_sampleZ, m_result[n].meanZ);
        m_result[n].meanE = Mean(m_sampleE);
        m_result[n].varE  = Var(m_sampleE, m_result[n].meanE);
    }

#ifdef QT_ENABLED
    emit updatePercent(int(100 * n / m_result.size()));
#else
    updatePercent(int(100 * n / m_result.size()));
#endif
}

void Filter::writeResult(size_t n, bool logicDynamic, bool copy)
{
    assert(n < m_result.size() && "Core::Filter::writeResult(n, copy) : out of range (n >= size)");

    m_result[n].meanX = Mean(m_sampleX);
    m_result[n].varX  = Var(m_sampleX, m_result[n].meanX);

    if (copy) {
        m_result[n].meanZ = m_result[n - 1].meanZ;
        m_result[n].meanE = m_result[n - 1].meanE;
        m_result[n].varZ  = m_result[n - 1].varZ;
        m_result[n].varE  = m_result[n - 1].varE;
    } else {
        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
            m_sampleE[s] = m_sampleX[s] - m_sampleZ[s];
        }
        m_result[n].meanZ = Mean(m_sampleZ);
        m_result[n].varZ  = Var(m_sampleZ, m_result[n].meanZ);
        m_result[n].meanE = Mean(m_sampleE);
        m_result[n].varE  = Var(m_sampleE, m_result[n].meanE);
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
