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
    dy.resize(m_params->sampleSize());
    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        x[s]  = m_task->x0();
        y[s]  = Vector::Zero(m_task->dimY());
        dy[s] = y[s];
    }

    m_result[0].mx = Mean(x);
    m_result[0].Dx = Var(x, m_result[0].mx);
    m_result[0].mz = m_result[0].mx;

    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        z[s] = m_result[0].mx;
        e[s] = x[s] - z[s];
    }

    m_result[0].me = Mean(e);
    m_result[0].Dz = Var(z, m_result[0].mz);
    m_result[0].De = Var(e, m_result[0].me);
    m_result[0].t  = 0.0;
}


} // end Core
