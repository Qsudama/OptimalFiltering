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
    Vector h;
    Matrix G, F;
    Matrix Gamma  = Var(m_sampleX) - Var(m_sampleZ);
    double sqrtdt = std::sqrt(m_params->integrationStep());

    for (size_t n = 1; n < m_result.size(); ++n) { // tn = t0 + n * dt
        m_task->setTime(m_result[n - 1].time);
        Gamma = Var(m_sampleX) - Var(m_sampleZ); // Gamma для t = tn - dt;

        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
            m_sampleX[s] = m_sampleX[s] + m_task->a(m_sampleX[s]) * m_params->integrationStep() +
                           m_task->B(m_sampleX[s]) * gaussianVector(m_task->dimV(), 0.0, sqrtdt);
            m_sampleZ[s] = m_sampleZ[s] + m_task->tau(m_sampleZ[s], Gamma) * m_params->integrationStep();
        }
        if (n % (m_params->predictionCount() * m_params->integrationCount()) == 0) {
            Gamma = Cov(m_sampleX, m_sampleX) - Cov(m_sampleZ, m_sampleZ); // здесь нужна Gamma для t = tn = tk

            for (size_t s = 0; s < m_params->sampleSize(); ++s) {
                m_sampleY[s] = m_task->c(m_sampleX[s]);

                h = m_task->h(m_sampleZ[s], Gamma);
                G = m_task->G(m_sampleZ[s], Gamma);
                F = m_task->F(m_sampleZ[s], Gamma);

                m_sampleZ[s] = m_sampleZ[s] + Gamma * G.transpose() * PinvSVD(F) * (m_sampleY[s] - h);
            }
        }
        writeResult(n);
    }
}


} // end ContinuousDiscrete

} // end Filters
