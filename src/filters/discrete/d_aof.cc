#include "d_aof.h"


namespace Filters
{

namespace Discrete
{


using Math::LinAlg::Pinv;


AOF::AOF(Core::PtrFilterParameters params, Core::PtrTask task)
    : DiscreteFilter(params, task)
{
    long n = task->dimX();
    m_info->setName(m_task->info()->type() + "AОФд (p=" + std::to_string(n * (n + 3) / 2) + ")");
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

    m_task->setStep(m_params->measurementStep());

    // Индекс k соответствует моменту времени tk = t0 + k * delta_t  (delta_t - интервал между измерениями):
    for (size_t k = 1; k < m_result.size(); ++k) {
        // Индекс s пробегает по всем элементам выборки:
        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
            m_task->setTime(m_result[k - 1].time);
            // X_k = a(X_{k-1}); время t = t_{k-1}
            m_sampleX[s] = m_task->a(m_sampleX[s]);
            // вычисляем lambda, Psi;  время t = t_{k-1}:
            lambda = m_task->tau(m_sampleZ[s], m_sampleP[s]);
            Psi    = m_task->Theta(m_sampleZ[s], m_sampleP[s]);

            //ставим текущее (t = tk) время и продолжаем:
            m_task->setTime(m_result[k].time);

            h = m_task->h(lambda, Psi);
            G = m_task->G(lambda, Psi); 
            F = m_task->F(lambda, Psi);
            H = Psi * G.transpose() * Pinv(F);

            m_sampleY[s] = m_task->b(m_sampleX[s]);
            m_sampleZ[s] = lambda + H * (m_sampleY[s] - h);
            m_sampleP[s] = Psi - H * G * Psi;
            m_sampleP[s] = 0.5 * (m_sampleP[s] + m_sampleP[s].transpose());
        }
        writeResult(k);
    }
}


} // end Discrete

} // end Filters
