#include "d_fos.h"
#include "src/math/statistic.h"

#include <QDebug>
namespace Filters
{

namespace Discrete
{

using Math::Rand::gaussianVector;
using Math::LinAlg::Pinv;
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
    Vector h, lambda;
    Matrix G, F, Psi, T;

    Array<Vector> sampleU(m_params->sampleSize());

    computeParams(0, sampleU, T);

    // Индекс n соответствует моменту времени tn = t0 + n * dt  (dt - шаг интегрирования):
    for (size_t n = 1; n < m_result.size(); ++n) {
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
                lambda = m_task->tau(sampleU[s], T);
                Psi    = m_task->Theta(sampleU[s], T);

                //ставим время обратно и продолжаем:
                m_task->setTime(currentTime);

                h = m_task->h(lambda, Psi);
                G = m_task->G(lambda, Psi);
                F = m_task->F(lambda, Psi);

                m_sampleY[s] = m_task->b(m_sampleX[s]);
                m_sampleZ[s] = lambda + Psi * G.transpose() * Pinv(F) * (m_sampleY[s] - h);
            }
            writeResult(n);
            computeParams(n, sampleU, T);
        }
    }
}

void FOS::computeParams(size_t n, Array<Vector> &u, Matrix &T)
{
    Vector        my, chi;
    Matrix        Gamma, GammaY, GammaZ, DxyDxz, Ddelta, Dxy, Dxz;
    Array<Vector> sampleDelta(m_params->sampleSize());

    // Индекс s пробегает по всем элементам выборки:
    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        MakeBlockVector(m_sampleY[s], m_sampleZ[s], sampleDelta[s]);
    }

    Ddelta = Var(sampleDelta);
    my     = Mean(m_sampleY);
    Dxy    = Cov(m_sampleX, m_sampleY);
    Dxz    = Cov(m_sampleX, m_sampleZ);
    MakeBlockMatrix(Dxy, Dxz, DxyDxz);

    Gamma  = DxyDxz * Pinv(Ddelta);
    GammaY = Gamma.leftCols(m_task->dimY());
    GammaZ = Gamma.rightCols(m_task->dimX()); // dimZ = dimX

    chi = m_result[n].meanX - GammaY * my - GammaZ * m_result[n].meanZ;
    T   = m_result[n].varX - GammaY * Dxy.transpose() - GammaZ * Dxz.transpose();

    // Индекс s пробегает по всем элементам выборки:
    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        u[s] = GammaY * m_sampleY[s] + GammaZ * m_sampleZ[s] + chi;
    }
}


} // end Discrete

} // end Filters
