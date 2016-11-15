#include "continuous_filter.h"


using Math::Statistic::Mean;
using Math::Statistic::Var;


namespace Core
{


ContinuousFilter::ContinuousFilter(PtrFilterParameters params, PtrTask task)
    : Filter(params)
    , m_task(std::dynamic_pointer_cast<ContinuousTask>(task))
{
    m_info->setType("н");
}

void ContinuousFilter::zeroIteration()
{
    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        m_sampleX[s] = m_task->x0();
        m_sampleY[s] = Vector::Zero(m_task->dimY());
    }

    m_result[0].meanX = Mean(m_sampleX);
    m_result[0].varX  = Var(m_sampleX, m_result[0].meanX);
    m_result[0].meanZ = m_result[0].meanX;

    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        m_sampleZ[s] = m_result[0].meanX;
        m_sampleE[s] = m_sampleX[s] - m_sampleZ[s];
    }

    m_result[0].meanE = Mean(m_sampleE);
    m_result[0].varZ  = Var(m_sampleZ, m_result[0].meanZ);
    m_result[0].varE  = Var(m_sampleE, m_result[0].meanE);
    m_result[0].time  = 0.0;
}


} // end Core
