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

void Filter::writeResult(size_t n, int countI, bool copy)
{
    assert(n < m_result.size() && "Core::Filter::writeResult(n, copy) : out of range (n >= size)");

    Array<Vector> mx(countI);
    Array<Matrix> varX(countI);
    Array<Vector> mz(countI);
    Array<Matrix> varZ(countI);
    Array<Vector> me(countI);
    Array<Matrix> varE(countI);

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

    if (copy) {
        m_result[n].meanZ = m_result[n - 1].meanZ;
        m_result[n].meanE = m_result[n - 1].meanE;
        m_result[n].varZ  = m_result[n - 1].varZ;
        m_result[n].varE  = m_result[n - 1].varE;
    } else {
        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
            m_sampleE[s] = m_sampleX[s] - m_sampleZ[s];
        }
        for (int i = 0; i < countI; i++) {
            mz[i] = Mean(m_sampleZ, m_sampleI, i+1);
            me[i] = Mean(m_sampleE, m_sampleI, i+1);
            varZ[i] = Var(m_sampleZ, mz[i], m_sampleI, i+1);
            varE[i] = Var(m_sampleE, me[i], m_sampleI, i+1);
        }
        Vector resMz = Vector::Zero(mz[0].size());
        Matrix resVarZ = Matrix::Zero(varZ[0].rows(), varZ[0].cols());
        Vector resMe = Vector::Zero(me[0].size());
        Matrix resVarE = Matrix::Zero(varZ[0].rows(), varZ[0].cols());
        for (int i = 0; i < countI; i++) {
            resMz += mz[i];
            resVarZ += varZ[i];
            resMe += me[i];
            resVarE += varE[i];
        }

        m_result[n].meanZ = resMz/countI;
        m_result[n].varZ  = resVarZ/countI;
        m_result[n].meanE = resMe/countI;
        m_result[n].varE  = resVarE/countI;
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
