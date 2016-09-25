#include "cd_dfosbo.h"


namespace Filters
{

namespace ContinuousDiscrete
{

using Math::Rand::gaussianVector;
using Math::LinAlg::PinvSVD;
using Math::Statistic::Cov;
using Math::Statistic::Var;
using Math::Statistic::Mean;


DFOSBO::DFOSBO(Core::PtrFilterParameters params, Core::PtrTask task)
    : ContinuousDiscreteFilter(params, task)
{
    long p = task->dimY() * long(params->orderMult());
    m_info->setName(m_task->info()->type() + "ФКПнд-дп (" + std::to_string(p) + ")");
}

void DFOSBO::init()
{
    Filter::init();
    Zb.resize(m_params->sampleSize());
}

void DFOSBO::zeroIteration()
{
    ContinuousDiscreteFilter::zeroIteration(); // тут вычисляются общие для всех фильтров H0, e0 и
                                               // т.д. и заполняется результат 0-го шага

    Zb.resize(m_params->sampleSize());

    ny = uint(m_task->dimY());
    p  = ny * m_params->orderMult();

    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        Zb[s] = Vector::Zero(int(p));
        for (long i = 0; i < long(ny); ++i) {
            Zb[s][i] = y[s][i];
        }
    }
}

void DFOSBO::algorithm()
{
    double sqrtdt = std::sqrt(m_params->integrationStep());

    for (size_t n = 1; n < m_result.size();
         ++n) { // n = 1 .. m_params->measurementCount() * m_params->predictionCount() * m_params->integrationCount()
        m_task->setTime(m_result[n - 1].t);
        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
            x[s] = x[s] + m_task->a(x[s]) * m_params->integrationStep() +
                   m_task->B(x[s]) * gaussianVector(m_task->dimV(), 0.0, sqrtdt);
        }
        writeResult(n, true); //  mX, DX вычисляются, а mZ, DZ, mE, DE копируются из предыдущего

        if (n % m_params->integrationCount() == 0) {
            DxZb  = Cov(x, Zb);
            mZb   = Mean(Zb);
            DZb   = Var(Zb, mZb);
            Gamma = DxZb * PinvSVD(DZb);
            kappa = m_result[n].mx - Gamma * mZb;
            if (n % (m_params->predictionCount() * m_params->integrationCount()) == 0) {
                T = m_result[n].Dx - Gamma * DxZb.transpose();
            }
            for (size_t s = 0; s < m_params->sampleSize(); ++s) {
                z[s] = Gamma * Zb[s] + kappa;
            }
            writeResult(n);
        }

        if (n % (m_params->predictionCount() * m_params->integrationCount()) == 0) {
            for (size_t s = 0; s < m_params->sampleSize(); ++s) {
                y[s] = m_task->c(x[s]); // = Yk (Y в момент t = tk)
                h    = m_task->h(z[s], T);
                G    = m_task->G(z[s], T);
                F    = m_task->F(z[s], T);
                z[s] = z[s] + T * G.transpose() * PinvSVD(F) * (y[s] - h);

                for (long i = long(p - 1); i >= long(ny); --i) {
                    Zb[s][i] = Zb[s][i - long(ny)];
                }
                for (long i = 0; i < long(ny); ++i) {
                    Zb[s][i] = y[s][i];
                }
            }
            writeResult(n);
        }
    }
}


} // end ContinuousDiscrete

} // end Filters
