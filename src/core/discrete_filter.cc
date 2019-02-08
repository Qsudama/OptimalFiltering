#include "discrete_filter.h"


using Math::Statistic::Mean;
using Math::Statistic::Var;
using Math::Statistic::Cov;
using Math::LinAlg::Pinv;


namespace Core
{


DiscreteFilter::DiscreteFilter(PtrFilterParameters params, PtrTask task)
    : Filter(params)
    , m_task(std::dynamic_pointer_cast<DiscreteTask>(task))
{
    m_info->setType("д");
}

double DiscreteFilter::execute_time_filter()
{
    double result = timerInstance.result_execute_time / (m_params->measurementCount() * m_params->sampleSize());
    return result;
}

void DiscreteFilter::init()
{
    m_sampleX.resize(m_params->sampleSize());
    m_sampleY.resize(m_params->sampleSize());
    m_sampleZ.resize(m_params->sampleSize());
    m_sampleE.resize(m_params->sampleSize());

    size_t size = size_t(m_params->measurementCount());
    m_result.resize(size);
    for (size_t i = 0; i < size; ++i) {
        m_result[i].time = m_params->measurementStep() * i;
    }
}

void DiscreteFilter::zeroIteration()
{
    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        m_sampleX[s] = m_task->x0();
        m_sampleY[s] = m_task->b(m_sampleX[s]);
    }

    Vector mx0  = Mean(m_sampleX);
    Vector my0  = Mean(m_sampleY);
    Matrix Dy0  = Var(m_sampleY, my0);
    Matrix Dxy0 = Cov(m_sampleX, m_sampleY);
    Matrix H0   = Dxy0 * Pinv(Dy0);
    Vector e0   = mx0 - H0 * my0;

    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        m_sampleZ[s] = H0 * m_sampleY[s] + e0;
        m_sampleE[s] = m_sampleX[s] - m_sampleZ[s];
    }

    m_result[0].meanX = mx0;
    m_result[0].meanZ = Mean(m_sampleZ);
    m_result[0].meanE = Mean(m_sampleE);
    m_result[0].varX  = Var(m_sampleX, m_result[0].meanX);
    m_result[0].varZ  = Var(m_sampleZ, m_result[0].meanZ);
    m_result[0].varE  = Var(m_sampleE, m_result[0].meanE);
    m_result[0].time  = 0.0;
}


} // end Core
