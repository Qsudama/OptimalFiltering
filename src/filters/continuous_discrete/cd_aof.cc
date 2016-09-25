#include "cd_aof.h"


namespace Filters
{

namespace ContinuousDiscrete
{

using Math::Rand::gaussianVector;
using Math::LinAlg::PinvSVD;


AOF::AOF(Core::PtrFilterParameters params, Core::PtrTask task)
    : ContinuousDiscreteFilter(params, task)
{
    long n = task->dimX();
    m_info->setName(m_task->info()->type() + "AОФнд (" + std::to_string(n * (n + 3) / 2) + ")");
}

void AOF::zeroIteration()
{
    ContinuousDiscreteFilter::zeroIteration();

    P.resize(m_params->sampleSize());
    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        P[s] = m_result[0].Dx;
    }
}

void AOF::algorithm()
{
    double sqrtdt = std::sqrt(m_params->integrationStep());

    for (size_t n = 1; n < m_result.size(); ++n) { // tn = t0 + n * dt
        m_task->setTime(m_result[n - 1].t);
        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
            x[s] = x[s] + m_task->a(x[s]) * m_params->integrationStep() +
                   m_task->B(x[s]) * gaussianVector(m_task->dimV(), 0.0, sqrtdt);
            A     = m_task->A(z[s], P[s]);
            Theta = m_task->Theta(z[s], P[s]);
            z[s]  = z[s] + m_task->tau(z[s], P[s]) * m_params->integrationStep();
            P[s]  = P[s] + (A * P[s] + P[s] * A.transpose() + Theta) * m_params->integrationStep();
            P[s]  = 0.5 * (P[s] + P[s].transpose());
        }
        if (n % (m_params->predictionCount() * m_params->integrationCount()) == 0) { // t = tk - приходит измерение
            for (size_t s = 0; s < m_params->sampleSize(); ++s) {
                y[s]  = m_task->c(x[s]);
                h     = m_task->h(z[s], P[s]);
                G     = m_task->G(z[s], P[s]);
                F     = m_task->F(z[s], P[s]);
                Kappa = P[s] * G.transpose() * PinvSVD(F);
                z[s]  = z[s] + Kappa * (y[s] - h);
                P[s]  = P[s] - Kappa * G * P[s];
                P[s]  = 0.5 * (P[s] + P[s].transpose());
            }
        }
        writeResult(n);
    }
}


} // end ContinuousDiscrete

} // end Filters
