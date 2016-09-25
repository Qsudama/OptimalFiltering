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

    P.resize(m_params->sampleSize());
    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        P[s] = m_result[0].Dx;
    }
}

void AOF::algorithm()
{
    for (size_t n = 1; n < m_result.size(); ++n) { // tn = t0 + n * dt
        //интегрируем систему (получаем mx, Dx с шагам dt):
        m_task->setTime(m_result[n - 1].t);
        m_task->setStep(m_params->integrationStep());
        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
            x[s] = m_task->a(x[s]);
        }

        if (n % (m_params->predictionCount() * m_params->integrationCount()) == 0) {
            for (size_t s = 0; s < m_params->sampleSize(); ++s) {
                //вычисляем lambda, Psy: время устонавливаем в предыдущий момент измерения:
                double timeCurrent = m_task->time();
                double timePred    = timeCurrent - m_params->measurementStep();
                m_task->setStep(m_params->measurementStep());
                m_task->setTime(timePred);
                lambda = m_task->tau(z[s], P[s]);
                Psy    = m_task->Theta(z[s], P[s]);
                //ставим время обратно и продолжаем:
                m_task->setTime(timeCurrent);
                y[s] = m_task->b(x[s]);
                h    = m_task->h(lambda, Psy);
                G    = m_task->G(lambda, Psy);
                F    = m_task->F(lambda, Psy);
                H    = Psy * G.transpose() * PinvSVD(F);
                z[s] = lambda + H * (y[s] - h);
                P[s] = Psy - H * G * Psy;
                P[s] = 0.5 * (P[s] + P[s].transpose());
            }
            writeResult(n);
        } else {
            writeResult(n, true);
        }
    }
}


} // end Discrete

} // end Filters
