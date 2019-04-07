#include "cd_fos.h"


namespace Filters
{

namespace ContinuousDiscrete
{


using Math::LinAlg::Pinv;
using Math::Statistic::Cov;
using Math::Statistic::Var;
using Math::Statistic::Mean;


FOS::FOS(Core::PtrFilterParameters params, Core::PtrTask task)
    : ContinuousDiscreteFilter(params, task)
{
//    m_info->setName(m_task->info()->type() + "ФМПнд-нп (p=" + std::to_string(task->dimX()) + ")");
    m_info->setName("ФМПнд-дп");
    m_info->setDimension("(p=" + std::to_string(task->dimX()) + ")");
}

void FOS::algorithm()
{
    Vector h;
    Matrix G, F;
    Matrix Gamma  = Var(m_sampleX) - Var(m_sampleZ);
    double sqrtdt = std::sqrt(m_params->integrationStep());

    // Индекс n соответствует моменту времени tn = t0 + n * dt  (dt - шаг интегрирования):
    for (size_t n = 1; n < m_result.size(); ++n) {
        m_task->setTime(m_result[n - 1].time);
        Gamma = Var(m_sampleX) - Var(m_sampleZ); // Gamma для t = tn - dt;

        // Индекс s пробегает по всем элементам выборки:
        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
            m_sampleX[s] = m_sampleX[s] + m_task->a(m_sampleX[s]) * m_params->integrationStep() +
                           m_task->B(m_sampleX[s]) * sqrtdt * m_normalRand(m_task->dimV());
            m_sampleZ[s] = m_sampleZ[s] + m_task->tau(m_sampleZ[s], Gamma) * m_params->integrationStep();
        }
        m_task->setTime(m_result[n].time);

        // n = 1..K*L*N, если n нацело делится на L*N, значит сейчас время измерения tn = tk:
        if (n % (m_params->predictionCount() * m_params->integrationCount()) == 0) {
            Gamma = Cov(m_sampleX, m_sampleX) - Cov(m_sampleZ, m_sampleZ); // здесь нужна Gamma для t = tn = tk

            // Индекс s пробегает по всем элементам выборки:
            for (size_t s = 0; s < m_params->sampleSize(); ++s) {
                m_sampleY[s] = m_task->c(m_sampleX[s], m_params->measurementStep());

                h = m_task->h(m_sampleZ[s], Gamma, m_params->measurementStep());
                G = m_task->G(m_sampleZ[s], Gamma, m_params->measurementStep());
                F = m_task->F(m_sampleZ[s], Gamma, m_params->measurementStep());

                m_sampleZ[s] = m_sampleZ[s] + Gamma * G.transpose() * Pinv(F) * (m_sampleY[s] - h);
            }
        }
        writeResult(n);
    }
}


} // end ContinuousDiscrete

} // end Filters
