#include "logic_dynamic_filter.h"

using Math::Statistic::Mean;
using Math::Statistic::Var;
using Math::Statistic::Cov;
using Math::LinAlg::Pinv;


namespace Core
{


LogicDynamicFilter::LogicDynamicFilter(PtrFilterParameters params, PtrTask task)
    : Filter(params)
    , m_task(std::dynamic_pointer_cast<LogicDynamicTask>(task))
{
    m_info->setType("лд");
}

void LogicDynamicFilter::init()
{
    m_sampleX.resize(m_params->sampleSize());
    m_sampleY.resize(m_params->sampleSize());
    m_sampleZ.resize(m_params->sampleSize());
    m_sampleE.resize(m_params->sampleSize());
    m_sampleI.resize(m_params->sampleSize());

    size_t size = size_t(m_params->measurementCount());
    m_result.resize(size);
    for (size_t i = 0; i < size; ++i) {
        m_result[i].time = m_params->measurementStep() * i;
    }
}

void LogicDynamicFilter::zeroIteration()
{
    int i = 1;
    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        m_sampleI[s] = m_task->nextI(i);
        m_sampleX[s] = m_task->x0();
        m_sampleY[s] = m_task->b(m_sampleI[s], m_sampleX[s]);

    }



    Vector mx0  = Mean(m_sampleX, m_sampleI, i);
    Matrix Dx0 = Cov(m_sampleX, m_sampleY, m_sampleI, i);

    Omega = m_task->Pr()[i-1];
    Lambda = mx0;
    Mu = m_task->h(i, mx0, Dx0);
    Psi = Dx0;
    Delta = m_task->G(i, mx0, Dx0) - Lambda * Mu.transpose();
    Phi = m_task->F(i, mx0, Dx0) - Mu*Mu.transpose();
   
    // тут по идее надо создать нулевые результаты... Но как?)

//    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
//        m_sampleZ[s] = H0 * m_sampleY[s] + e0;
//        m_sampleE[s] = m_sampleX[s] - m_sampleZ[s];
//    }

//    m_result[0].meanX = mx0;
//    m_result[0].meanZ = Mean(m_sampleZ);
//    m_result[0].meanE = Mean(m_sampleE);
//    m_result[0].varX  = Var(m_sampleX, m_result[0].meanX);
//    m_result[0].varZ  = Var(m_sampleZ, m_result[0].meanZ);
//    m_result[0].varE  = Var(m_sampleE, m_result[0].meanE);
//    m_result[0].time  = 0.0;
}


} // end Core
