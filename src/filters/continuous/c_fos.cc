#include "c_fos.h"


namespace Filters
{

namespace Continuous
{

using Math::Rand::gaussianVector;


FOS::FOS(Core::PtrFilterParameters params, Core::PtrTask task)
    : ContinuousFilter(params, task)
{
    m_info->setName(m_task->info()->type() + "ФОСн (" + std::to_string(task->dimX()) + ")");
}

void FOS::zeroIteration()
{
    ContinuousFilter::zeroIteration();

    dy.resize(m_params->sampleSize());
    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        dy[s] = Vector::Zero(m_task->dimY());
    }
}

void FOS::algorithm()
{
    double sqrtdt = std::sqrt(m_params->integrationStep());
    Matrix Gamma;

    for (size_t n = 1; n < m_result.size(); ++n) { // tn = t0 + n * dt
        m_task->setTime(m_result[n - 1].t);
        Gamma = m_result[n - 1].Dx - m_result[n - 1].Dz;
        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
            x[s] = x[s] + m_task->a(x[s]) * m_params->integrationStep() +
                   m_task->B(x[s]) * gaussianVector(m_task->dimV(), 0.0, sqrtdt);
            dy[s] = m_task->c(x[s]) * m_params->integrationStep() +
                    m_task->D(x[s]) * gaussianVector(m_task->dimW(), 0.0, sqrtdt);
            y[s] = y[s] + dy[s];
            z[s] = z[s] + m_task->a(z[s]) * m_params->integrationStep() +
                   m_task->K(z[s], Gamma) * (dy[s] - m_task->c(z[s]) * m_params->integrationStep());
        }
        writeResult(n);
    }
}


} // end Filters::Continuous

} // end Filters
