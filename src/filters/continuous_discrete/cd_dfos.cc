#include "cd_dfos.h"


namespace Filters
{

namespace ContinuousDiscrete
{


using Math::Rand::gaussianVector;
using Math::LinAlg::Pinv;
using Math::Statistic::Cov;


DFOS::DFOS(Core::PtrFilterParameters params, Core::PtrTask task)
    : ContinuousDiscreteFilter(params, task)
{
    m_info->setName(m_task->info()->type() + "ФМПнд-дп (" + std::to_string(task->dimX()) + ")");
}

void DFOS::algorithm()
{
    Vector h, kappa;
    Matrix G, F, T, Gamma;
    Matrix Dxz    = Cov(m_sampleX, m_sampleZ);
    double sqrtdt = std::sqrt(m_params->integrationStep());

    // Индекс n соответствует моменту времени tn = t0 + n * dt  (dt - шаг интегрирования):
    for (size_t n = 1; n < m_result.size(); ++n) {
        m_task->setTime(m_result[n - 1].time);

        // Индекс s пробегает по всем элементам выборки:
        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
            m_sampleX[s] = m_sampleX[s] + m_task->a(m_sampleX[s]) * m_params->integrationStep() +
                           m_task->B(m_sampleX[s]) * gaussianVector(m_task->dimV(), 0.0, sqrtdt);
        }
        writeResult(n, true); //  mX, DX вычисляются, а mZ, DZ, mE, DE копируются из предыдущего
        m_task->setTime(m_result[n].time);

        // n = 1..K*L*N, если n нацело делится на N, значит сейчас время прогноза tn = tl:
        if (n % m_params->integrationCount() == 0) { // t = t_tau_k^i
            Dxz   = Cov(m_sampleX, m_sampleZ);
            Gamma = Dxz * Pinv(m_result[n].varZ);
            kappa = m_result[n].meanX - Gamma * m_result[n].meanZ;
            if (n % (m_params->predictionCount() * m_params->integrationCount()) == 0) {
                T = m_result[n].varX - Gamma * Dxz.transpose();
            }
            for (size_t s = 0; s < m_params->sampleSize(); ++s) {
                m_sampleZ[s] = Gamma * m_sampleZ[s] + kappa;
            }
            writeResult(n);
        }

        // n = 1..K*L*N, если n нацело делится на L*N, значит сейчас время измерения tn = tk:
        if (n % (m_params->predictionCount() * m_params->integrationCount()) == 0) {
            // Индекс s пробегает по всем элементам выборки:
            for (size_t s = 0; s < m_params->sampleSize(); ++s) {
                m_sampleY[s] = m_task->c(m_sampleX[s]); // = Yk (Y в момент t = tk)

                h = m_task->h(m_sampleZ[s], T);
                G = m_task->G(m_sampleZ[s], T);
                F = m_task->F(m_sampleZ[s], T);

                m_sampleZ[s] = m_sampleZ[s] + T * G.transpose() * Pinv(F) * (m_sampleY[s] - h);
            }
            writeResult(n);
        }
    }
}


} // end ContinuousDiscrete

} // end Filters
