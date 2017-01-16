#include "c_fos.h"


namespace Filters
{

namespace Continuous
{


FOS::FOS(Core::PtrFilterParameters params, Core::PtrTask task)
    : ContinuousFilter(params, task)
{
    m_info->setName(m_task->info()->type() + "ФМПн (p=" + std::to_string(task->dimX()) + ")");
}

void FOS::algorithm()
{
    double sqrtdt = std::sqrt(m_params->integrationStep());
    Vector dy;
    Matrix Gamma;

    // Индекс n соответствует моменту времени tn = t0 + n * dt  (dt - шаг интегрирования):
    for (size_t n = 1; n < m_result.size(); ++n) {
        m_task->setTime(m_result[n - 1].time);
        Gamma = m_result[n - 1].varX - m_result[n - 1].varZ;

        // Индекс s пробегает по всем элементам выборки:
        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
            m_sampleX[s] = m_sampleX[s] + m_task->a(m_sampleX[s]) * m_params->integrationStep() +
                           m_task->B(m_sampleX[s]) * sqrtdt * m_normalRand(m_task->dimV());

            dy = m_task->c(m_sampleX[s]) * m_params->integrationStep() +
                 m_task->D(m_sampleX[s]) * sqrtdt * m_normalRand(m_task->dimW());
            m_sampleY[s] = m_sampleY[s] + dy;

            m_sampleZ[s] =
                m_sampleZ[s] + m_task->a(m_sampleZ[s]) * m_params->integrationStep() +
                m_task->K(m_sampleZ[s], Gamma) * (dy - m_task->c(m_sampleZ[s]) * m_params->integrationStep());
        }
        writeResult(n);
    }
}


} // end Filters::Continuous

} // end Filters
