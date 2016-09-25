#include "c_aof.h"


using Math::Rand::gaussianVector;


namespace Filters
{

namespace Continuous
{


AOF::AOF(Core::PtrFilterParameters params, Core::PtrTask task)
    : ContinuousFilter(params, task)
{
    long n = task->dimX();
    m_info->setName(m_task->info()->type() + "AОФн (" + std::to_string(n * (n + 3) / 2) + ")");
}

void AOF::zeroIteration()
{
    ContinuousFilter::zeroIteration();

    P.resize(m_params->sampleSize());
    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        P[s] = m_result[0].Dx;
    }
}

void AOF::algorithm()
{
    double sqrtdt = std::sqrt(m_params->integrationStep());
    Vector z1;

    for (size_t n = 1; n < m_result.size(); ++n) { // tn = t0 + n * dt
        m_task->setTime(m_result[n - 1].t);
        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
            x[s] = x[s] + m_task->a(x[s]) * m_params->integrationStep() +
                   m_task->B(x[s]) * gaussianVector(m_task->dimV(), 0.0, sqrtdt);
            dy[s] = m_task->c(x[s]) * m_params->integrationStep() +
                    m_task->D(x[s]) * gaussianVector(m_task->dimW(), 0.0, sqrtdt);

            y[s] = y[s] + dy[s];
            z1   = z[s];
            z[s] = z[s] + m_task->a(z[s]) * m_params->integrationStep() +
                   m_task->K(z[s], P[s]) * (dy[s] - m_task->c(z[s]) * m_params->integrationStep());
            P[s] = P[s] + m_task->Psy(z1, P[s]) * m_params->integrationStep();
            P[s] = 0.5 * (P[s] + P[s].transpose());
        }
        writeResult(n);
    }
}


} // end Filters::Continuous

} // end Filters
