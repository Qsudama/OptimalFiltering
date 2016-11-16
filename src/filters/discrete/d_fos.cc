#include "d_fos.h"
#include "src/math/statistic.h"

#include <QDebug>
namespace Filters
{

namespace Discrete
{

using Math::Rand::gaussianVector;
using Math::LinAlg::PinvSVD;
using Math::Statistic::Mean;
using Math::Statistic::Var;
using Math::Statistic::Cov;
using Math::MakeBlockVector;
using Math::MakeBlockMatrix;


FOS::FOS(Core::PtrFilterParameters params, Core::PtrTask task)
    : DiscreteFilter(params, task)
{
    long n = task->dimX();
    long m = task->dimY();
    m_info->setName(m_task->info()->type() + "ФОСд (" + std::to_string(n * (n + m) + n + n * (n + 1) / 2) + ")");
}

void FOS::algorithm()
{
    Vector        h, my, xi, lambda, u;
    Matrix        G, F, T, Psi, Gamma, GammaY, GammaZ, DxyDxz, Ddelta, Dxy, Dxz;
    Array<Vector> sampleDelta(m_params->sampleSize());

    for (size_t n = 1; n < m_result.size(); ++n) { // tn = t0 + n * dt
        //интегрируем систему (получаем mx, Dx с шагам dt):
        m_task->setTime(m_result[n - 1].time);
        m_task->setStep(m_params->integrationStep());

        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
            m_sampleX[s] = m_task->a(m_sampleX[s]);
        }
        writeResult(n, true);

        if (n % (m_params->predictionCount() * m_params->integrationCount()) == 0) {
            for (size_t s = 0; s < m_params->sampleSize(); ++s) {
                m_sampleY[s] = m_task->b(m_sampleX[s]);
                MakeBlockVector(m_sampleY[s], m_sampleZ[s], sampleDelta[s]);
            }
            my     = Mean(m_sampleY);
            Ddelta = Var(sampleDelta);
            Dxy    = Cov(m_sampleX, m_sampleY);
            Dxz    = Cov(m_sampleX, m_sampleZ);
            MakeBlockMatrix(Dxy, Dxz, DxyDxz);

            for (size_t s = 0; s < m_params->sampleSize(); ++s) {
                Gamma  = DxyDxz * PinvSVD(Ddelta);
                GammaY = Gamma.leftCols(m_task->dimY());
                GammaZ = Gamma.rightCols(m_task->dimX()); // dimZ = dimX
                xi    = m_result[n].meanX - GammaY * my - GammaZ * m_result[n].meanZ;
                T      = m_result[n].varX - GammaY * Dxy.transpose() - GammaZ * Dxz.transpose();
                u      = GammaY * m_sampleY[s] + GammaZ * m_sampleZ[s] + xi;

                //вычисляем lambda, Psi: время устонавливаем в предыдущий момент измерения:
                double currentTime  = m_task->time();
                double previousTime = currentTime - m_params->measurementStep();
                m_task->setStep(m_params->measurementStep());
                m_task->setTime(previousTime);
                lambda = m_task->tau(u, T);
                Psi    = m_task->Theta(u, T);

                //ставим время обратно и продолжаем:
                m_task->setTime(currentTime);

                h = m_task->h(lambda, Psi);
                G = m_task->G(lambda, Psi);
                F = m_task->F(lambda, Psi);

                m_sampleZ[s] = lambda + Psi * G.transpose() * PinvSVD(F) * (m_sampleY[s] - h);
            }
            writeResult(n);
        }
    }
}


} // end Discrete

} // end Filters
