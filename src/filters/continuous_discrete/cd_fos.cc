#include "cd_fos.h"


namespace Filters
{

namespace ContinuousDiscrete
{

using Math::Rand::gaussianVector;
using Math::LinAlg::PinvSVD;
using Math::Statistic::Cov;
using Math::Statistic::Var;
using Math::Statistic::Mean;


FOS::FOS(Core::PtrFilterParameters params, Core::PtrTask task)
    : ContinuousDiscreteFilter(params, task)
{
    m_info->setName(m_task->info()->type() + "ФКПнд-нп (" + std::to_string(task->dimX()) + ")");
}

void FOS::algorithm()
{
    Gamma         = Var(x) - Var(z);
    double sqrtdt = std::sqrt(m_params->integrationStep());

    for (size_t n = 1; n < m_result.size(); ++n) { // tn = t0 + n * dt
        m_task->setTime(m_result[n - 1].t);
        Gamma = Var(x) - Var(z); // Gamma для t = tn - dt;
        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
            x[s] = x[s] + m_task->a(x[s]) * m_params->integrationStep() +
                   m_task->B(x[s]) * gaussianVector(m_task->dimV(), 0.0, sqrtdt);
            z[s] = z[s] + m_task->tau(z[s], Gamma) * m_params->integrationStep();
        }
        if (n % (m_params->predictionCount() * m_params->integrationCount()) == 0) {
            Gamma = Cov(x, x) - Cov(z, z); // здесь нужна Gamma для t = tn = tk
            for (size_t s = 0; s < m_params->sampleSize(); ++s) {
                y[s] = m_task->c(x[s]);
                h    = m_task->h(z[s], Gamma);
                G    = m_task->G(z[s], Gamma);
                F    = m_task->F(z[s], Gamma);
                z[s] = z[s] + Gamma * G.transpose() * PinvSVD(F) * (y[s] - h);
            }
        }
        writeResult(n);
    }
}


} // end ContinuousDiscrete

} // end Filters
