#include "cd_aof.h"


namespace Filters
{

namespace ContinuousDiscrete
{


using Math::LinAlg::Pinv;


AOF::AOF(Core::PtrFilterParameters params, Core::PtrTask task)
    : ContinuousDiscreteFilter(params, task)
{
    long n = task->dimX();
//    m_info->setName(m_task->info()->type() + "AОФнд (p=" + std::to_string(n * (n + 3) / 2) + ")");
    m_info->setName("AОФнд");
    m_info->setDimension("(p=" + std::to_string(n * (n + 3) / 2) + ")");
}

void AOF::zeroIteration()
{
    ContinuousDiscreteFilter::zeroIteration();

    m_sampleP.resize(m_params->sampleSize());
    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        m_sampleP[s] = m_result[0].varX;
    }
}

void AOF::algorithm()
{
    double sqrtdt = std::sqrt(m_params->integrationStep());
    Vector h;
    Matrix G, F, A, K, Theta;

    // Индекс n соответствует моменту времени tn = t0 + n * dt  (dt - шаг интегрирования):
    for (size_t n = 1; n < m_result.size(); ++n) {
        m_task->setTime(m_result[n - 1].time);

        // Индекс s пробегает по всем элементам выборки:
        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
            m_sampleX[s] = m_sampleX[s] + m_task->a(m_sampleX[s]) * m_params->integrationStep() +
                           m_task->B(m_sampleX[s]) * sqrtdt * m_normalRand(m_task->dimV());

            A            = m_task->A(m_sampleZ[s], m_sampleP[s]);
            Theta        = m_task->Theta(m_sampleZ[s], m_sampleP[s]);
            m_sampleZ[s] = m_sampleZ[s] + m_task->tau(m_sampleZ[s], m_sampleP[s]) * m_params->integrationStep();

            m_sampleP[s] =
                m_sampleP[s] + (A * m_sampleP[s] + m_sampleP[s] * A.transpose() + Theta) * m_params->integrationStep();
            m_sampleP[s] = 0.5 * (m_sampleP[s] + m_sampleP[s].transpose());
        }
        m_task->setTime(m_result[n].time);

        // n = 1..K*L*N, если n нацело делится на L*N, значит сейчас время измерения tn = tk:
        if (n % (m_params->predictionCount() * m_params->integrationCount()) == 0) {
            // Индекс s пробегает по всем элементам выборки:
            for (size_t s = 0; s < m_params->sampleSize(); ++s) {
                m_sampleY[s] = m_task->c(m_sampleX[s], m_params->measurementStep());

                h = m_task->h(m_sampleZ[s], m_sampleP[s], m_params->measurementStep());
                G = m_task->G(m_sampleZ[s], m_sampleP[s], m_params->measurementStep());
                F = m_task->F(m_sampleZ[s], m_sampleP[s], m_params->measurementStep());
                K = m_sampleP[s] * G.transpose() * Pinv(F);

                m_sampleZ[s] = m_sampleZ[s] + K * (m_sampleY[s] - h);
                m_sampleP[s] = m_sampleP[s] - K * G * m_sampleP[s];
                m_sampleP[s] = 0.5 * (m_sampleP[s] + m_sampleP[s].transpose());
            }
        }
        writeResult(n);
    }
}


} // end ContinuousDiscrete

} // end Filters
