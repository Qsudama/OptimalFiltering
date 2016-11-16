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

    m_sampleP.resize(m_params->sampleSize());
    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        m_sampleP[s] = m_result[0].varX;
    }
}

void AOF::algorithm()
{
    double sqrtdt = std::sqrt(m_params->integrationStep());
    Vector prevZ, dy;

    // Индекс n соответствует моменту времени tn = t0 + n * dt  (dt - шаг интегрирования):
    for (size_t n = 1; n < m_result.size(); ++n) {
        m_task->setTime(m_result[n - 1].time);

        // Индекс s пробегает по всем элементам выборки:
        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
            m_sampleX[s] = m_sampleX[s] + m_task->a(m_sampleX[s]) * m_params->integrationStep() +
                           m_task->B(m_sampleX[s]) * gaussianVector(m_task->dimV(), 0.0, sqrtdt);

            dy = m_task->c(m_sampleX[s]) * m_params->integrationStep() +
                 m_task->D(m_sampleX[s]) * gaussianVector(m_task->dimW(), 0.0, sqrtdt);
            m_sampleY[s] = m_sampleY[s] + dy;

            prevZ = m_sampleZ[s];

            m_sampleZ[s] =
                m_sampleZ[s] + m_task->a(m_sampleZ[s]) * m_params->integrationStep() +
                m_task->K(m_sampleZ[s], m_sampleP[s]) * (dy - m_task->c(m_sampleZ[s]) * m_params->integrationStep());

            m_sampleP[s] = m_sampleP[s] + m_task->Psi(prevZ, m_sampleP[s]) * m_params->integrationStep();
            m_sampleP[s] = 0.5 * (m_sampleP[s] + m_sampleP[s].transpose());
        }
        writeResult(n);
    }
}


} // end Filters::Continuous

} // end Filters
