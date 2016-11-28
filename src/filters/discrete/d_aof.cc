#include "d_aof.h"


namespace Filters
{

namespace Discrete
{


using Math::Rand::gaussianVector;
using Math::LinAlg::Pinv;


AOF::AOF(Core::PtrFilterParameters params, Core::PtrTask task)
    : DiscreteFilter(params, task)
{
    long n = task->dimX();
    m_info->setName(m_task->info()->type() + "AОФд (" + std::to_string(n * (n + 3) / 2) + ")");
}

void AOF::zeroIteration()
{
    DiscreteFilter::zeroIteration();

    Vector my0  = Math::Statistic::Mean(m_sampleY);
    Matrix Dy0  = Math::Statistic::Var(m_sampleY, my0);
    Matrix Dxy0 = Math::Statistic::Cov(m_sampleX, m_sampleY);
    Matrix H0   = Dxy0 * Pinv(Dy0);

    m_sampleP.resize(m_params->sampleSize());
    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        m_sampleP[s] = m_result[0].varX - H0 * Dxy0.transpose();
    }
}

void AOF::algorithm()
{
    Vector h, lambda;
    Matrix G, F, H, Psi;

    // Индекс n соответствует моменту времени tn = t0 + n * dt  (dt - шаг интегрирования):
    for (size_t n = 1; n < m_result.size(); ++n) {
        //интегрируем систему (получаем mx, Dx с шагам dt):
        m_task->setTime(m_result[n - 1].time);
        m_task->setStep(m_params->integrationStep());

        // Индекс s пробегает по всем элементам выборки:
        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
            m_sampleX[s] = m_task->a(m_sampleX[s]);
        }
        writeResult(n, true);

        // n = 1..K*L*N, если n нацело делится на L*N, значит сейчас время измерения tn = tk:
        if (n % (m_params->predictionCount() * m_params->integrationCount()) == 0) {
            double currentTime  = m_result[n].time;
            double previousTime = currentTime - m_params->measurementStep();
            m_task->setStep(m_params->measurementStep());

            // Индекс s пробегает по всем элементам выборки:
            for (size_t s = 0; s < m_params->sampleSize(); ++s) {
                //вычисляем lambda, Psi: время устонавливаем в предыдущий момент измерения:
                m_task->setTime(previousTime);
                lambda = m_task->tau(m_sampleZ[s], m_sampleP[s]);
                Psi    = m_task->Theta(m_sampleZ[s], m_sampleP[s]);

                //ставим время обратно и продолжаем:
                m_task->setTime(currentTime);

                h = m_task->h(lambda, Psi);
                G = m_task->G(lambda, Psi);
                F = m_task->F(lambda, Psi);
                H = Psi * G.transpose() * Pinv(F);

                m_sampleY[s] = m_task->b(m_sampleX[s]);
                m_sampleZ[s] = lambda + H * (m_sampleY[s] - h);
                m_sampleP[s] = Psi - H * G * Psi;
                m_sampleP[s] = 0.5 * (m_sampleP[s] + m_sampleP[s].transpose());
            }
            writeResult(n);
        }
    }
}


} // end Discrete

} // end Filters
