#include "d_aof.h"


namespace Filters
{

namespace Discrete
{


using Math::Rand::gaussianVector;
using Math::LinAlg::PinvSVD;


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
    Matrix H0   = Dxy0 * PinvSVD(Dy0);

    m_sampleP.resize(m_params->sampleSize());
    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        m_sampleP[s] = m_result[0].varX - H0 * Dxy0.transpose();
    }
}

void AOF::algorithm()
{
    Vector h, lambda;
    Matrix G, F, H, Psi;

    for (size_t n = 1; n < m_result.size(); ++n) { // tn = t0 + n * dt
        //интегрируем систему (получаем mx, Dx с шагам dt):
        m_task->setTime(m_result[n - 1].time);
        m_task->setStep(m_params->integrationStep());

        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
            m_sampleX[s] = m_task->a(m_sampleX[s]);
        }

        if (n % (m_params->predictionCount() * m_params->integrationCount()) == 0) {
            for (size_t s = 0; s < m_params->sampleSize(); ++s) {
                //вычисляем lambda, Psi: время устонавливаем в предыдущий момент измерения:
                double currentTime  = m_task->time();
                double previousTime = currentTime - m_params->measurementStep();
                m_task->setStep(m_params->measurementStep());
                m_task->setTime(previousTime);
                lambda = m_task->tau(m_sampleZ[s], m_sampleP[s]);
                Psi    = m_task->Theta(m_sampleZ[s], m_sampleP[s]);

                //ставим время обратно и продолжаем:
                m_task->setTime(currentTime);
                m_sampleY[s] = m_task->b(m_sampleX[s]);

                h = m_task->h(lambda, Psi);
                G = m_task->G(lambda, Psi);
                F = m_task->F(lambda, Psi);
                H = Psi * G.transpose() * PinvSVD(F);

                m_sampleZ[s] = lambda + H * (m_sampleY[s] - h);
                m_sampleP[s] = Psi - H * G * Psi;
                m_sampleP[s] = 0.5 * (m_sampleP[s] + m_sampleP[s].transpose());
            }
            writeResult(n);
        } else {
            writeResult(n, true);
        }
    }
}


} // end Discrete

} // end Filters
