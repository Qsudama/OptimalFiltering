#include "cd_dfosbo.h"


namespace Filters
{

namespace ContinuousDiscrete
{


using Math::LinAlg::Pinv;
using Math::Statistic::Cov;
using Math::Statistic::Var;
using Math::Statistic::Mean;


DFOSBO::DFOSBO(Core::PtrFilterParameters params, Core::PtrTask task)
    : ContinuousDiscreteFilter(params, task)
{
    long p = task->dimY() * long(params->orderMult());
//    m_info->setName(m_task->info()->type() + "ФКПнд-дп (p=" + std::to_string(p) + ")");
    m_info->setName("ФКПнд-дп");
    m_info->setDimension("(p=" + std::to_string(p) + ")");
}

void DFOSBO::zeroIteration()
{
    ContinuousDiscreteFilter::zeroIteration(); // тут вычисляются общие для всех фильтров H0, e0 и
                                               // т.д. и заполняется результат 0-го шага

    m_sampleS.resize(m_params->sampleSize());

    long p = long(m_task->dimY()) * long(m_params->orderMult());

    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        m_sampleS[s] = Vector::Zero(p);
        for (long i = 0; i < long(m_task->dimY()); ++i) {
            m_sampleS[s][i] = m_sampleY[s][i];
        }
    }
}

void DFOSBO::algorithm()
{
    double sqrtdt = std::sqrt(m_params->integrationStep());
    Vector h, kappa, ms;
    Matrix G, F, T, Gamma, Ds, Dxs;

    // Индекс n соответствует моменту времени tn = t0 + n * dt  (dt - шаг интегрирования):
    for (size_t n = 1; n < m_result.size(); ++n) {
        m_task->setTime(m_result[n - 1].time);

        // Индекс s пробегает по всем элементам выборки:
        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
            m_sampleX[s] = m_sampleX[s] + m_task->a(m_sampleX[s]) * m_params->integrationStep() +
                           m_task->B(m_sampleX[s]) * sqrtdt * m_normalRand(m_task->dimV());
        }
        writeResult(n, true); //  mX, DX вычисляются, а mZ, DZ, mE, DE копируются из предыдущего
        m_task->setTime(m_result[n].time);

        // n = 1..K*L*N, если n нацело делится на N, значит сейчас время прогноза tn = tl:
        if (n % m_params->integrationCount() == 0) {
            Dxs   = Cov(m_sampleX, m_sampleS);
            ms    = Mean(m_sampleS);
            Ds    = Var(m_sampleS, ms);
            Gamma = Dxs * Pinv(Ds); // Gamma_k^i
            kappa = m_result[n].meanX - Gamma * ms;
            if (n % (m_params->predictionCount() * m_params->integrationCount()) == 0) {
                T = m_result[n].varX - Gamma * Dxs.transpose();
            }

            // Индекс s пробегает по всем элементам выборки:
            for (size_t s = 0; s < m_params->sampleSize(); ++s) {
                m_sampleZ[s] = Gamma * m_sampleS[s] + kappa;
            }
            writeResult(n);
        }

        // n = 1..K*L*N, если n нацело делится на L*N, значит сейчас время измерения tn = tk:
        if (n % (m_params->predictionCount() * m_params->integrationCount()) == 0) {
            // Индекс s пробегает по всем элементам выборки:
            for (size_t s = 0; s < m_params->sampleSize(); ++s) {
                m_sampleY[s] = m_task->c(m_sampleX[s], m_params->measurementStep()); // = Yk (Y в момент t = tk)

                h = m_task->h(m_sampleZ[s], T, m_params->measurementStep());
                G = m_task->G(m_sampleZ[s], T, m_params->measurementStep());
                F = m_task->F(m_sampleZ[s], T, m_params->measurementStep());

                m_sampleZ[s] = m_sampleZ[s] + T * G.transpose() * Pinv(F) * (m_sampleY[s] - h);

                long ny = long(m_task->dimY());
                long p  = ny * long(m_params->orderMult());
                for (long i = p - 1; i >= ny; --i) {
                    m_sampleS[s][i] = m_sampleS[s][i - ny];
                }
                for (long i = 0; i < long(ny); ++i) {
                    m_sampleS[s][i] = m_sampleY[s][i];
                }
            }
            writeResult(n);
        }
    }
}


} // end ContinuousDiscrete

} // end Filters
