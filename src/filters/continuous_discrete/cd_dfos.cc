#include "cd_dfos.h"


namespace Filters
{

namespace ContinuousDiscrete
{

using Math::Rand::gaussianVector;
using Math::LinAlg::PinvSVD;
using Math::Statistic::Cov;


DFOS::DFOS(Core::PtrFilterParameters params, Core::PtrTask task)
    : ContinuousDiscreteFilter(params, task)
{
    m_info->setName(m_task->info()->type() + "ФМПнд-дп (" + std::to_string(task->dimX()) + ")");
}

void DFOS::algorithm()
{
    Matrix Dxz    = Cov(x, z);
    double sqrtdt = std::sqrt(m_params->integrationStep());

    for (size_t n = 1; n < m_result.size(); ++n) {
        m_task->setTime(m_result[n - 1].t);
        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
            x[s] = x[s] + m_task->a(x[s]) * m_params->integrationStep() +
                   m_task->B(x[s]) * gaussianVector(m_task->dimV(), 0.0, sqrtdt);
        }
        writeResult(n, true); //  mX, DX вычисляются, а mZ, DZ, mE, DE копируются из предыдущего

        if (n % m_params->integrationCount() == 0) { // t = t_tau_k^i
            Dxz   = Cov(x, z);
            Gamma = Dxz * PinvSVD(m_result[n].Dz);
            kappa = m_result[n].mx - Gamma * m_result[n].mz;
            if (n % (m_params->predictionCount() * m_params->integrationCount()) == 0) {
                T = m_result[n].Dx - Gamma * Dxz.transpose();
            }
            for (size_t s = 0; s < m_params->sampleSize(); ++s) {
                z[s] = Gamma * z[s] + kappa;
            }
            writeResult(n);
        }
        if (n % (m_params->predictionCount() * m_params->integrationCount()) == 0) // t = tk
        {
            for (size_t s = 0; s < m_params->sampleSize(); ++s) {
                y[s] = m_task->c(x[s]); // = Yk (Y в момент t = tk)
                h    = m_task->h(z[s], T);
                G    = m_task->G(z[s], T);
                F    = m_task->F(z[s], T);
                z[s] = z[s] + T * G.transpose() * PinvSVD(F) * (y[s] - h);
            }
            writeResult(n);
        }
    }
}


} // end ContinuousDiscrete

} // end Filters
