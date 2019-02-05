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
#endif
    , timerInstance(TimerManager::Instance())
    , m_params(params)
    , m_info(std::make_shared<Info>())
{
    m_normalRand.setSeed(Math::RandomProperties::defaultSeed());
}

Filter::~Filter()
{
}

void Filter::run()
{
    init();
    timerInstance.start_timer();
    zeroIteration();
    algorithm();
    timerInstance.stop_timer();
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

    timerInstance.interrupt_timer();

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
    timerInstance.continue_timer();
}

void Filter::writeResult(size_t n, int countI)
{
    assert(n < m_result.size() && "Core::Filter::writeResult(n, copy) : out of range (n >= size)");

    timerInstance.interrupt_timer();

    Array<Vector> mx(countI);
    Array<Matrix> varX(countI);
    Vector mz;
    Matrix varZ;
    Vector me;
    Matrix varE;

    for (int i = 0; i < countI; i++) {
        mx[i] = Mean(m_sampleX, m_sampleI, i+1);
        varX[i] = Var(m_sampleX, mx[i], m_sampleI, i+1);
    }
    Vector resMx = Vector::Zero(mx[0].size());
    Matrix resVarX = Matrix::Zero(varX[0].rows(), varX[0].cols());
    for (int i = 0; i < countI; i++) {
        resMx += mx[i];
        resVarX += varX[i];
    }
    m_result[n].meanX = resMx/countI;
    m_result[n].varX  = resVarX/countI;
    //qDebug() << "\n\nn = " << n;
    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        Vector rE = m_sampleX[s] - m_sampleZ[s];
        //qDebug() << "s = " << s << " VectorRes = "  << rE[0] << " " << rE[1] << " " << rE[2];
        m_sampleE[s] = rE;
    }
    mz = Mean(m_sampleZ);
    varZ = Var(m_sampleZ, mz);
    me = Mean(m_sampleE);
    varE = Var(m_sampleE, me);

    m_result[n].meanZ = mz;
    m_result[n].varZ  = varZ;
    m_result[n].meanE = me;
    m_result[n].varE  = varE;


#ifdef QT_ENABLED
    emit updatePercent(int(100 * n / m_result.size()));
#else
    updatePercent(int(100 * n / m_result.size()));
#endif
    timerInstance.continue_timer();
}

#ifndef QT_ENABLED
void Filter::updatePercent(int p) const
{
    std::cerr << "Progress: " << p << "% complete.\n";
}
#endif


} // end Core
