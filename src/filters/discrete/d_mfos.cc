#include "d_mfos.h"
#include "src/math/statistic.h"

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


MFOS::MFOS(Core::PtrFilterParameters params, Core::PtrTask task)
    : DiscreteFilter(params, task)
{
    long n = task->dimX();
    long m = task->dimY();
    long N = n * (n + m) + n + n * (n + 1) / 2 + 2 * n * (n + 1);
    m_info->setName(m_task->info()->type() + "МФОСд (" + std::to_string(N) + ")");
}

void MFOS::algorithm()
{

    Vector        h, n, e;
    Matrix        G, F, Psi, T, L, K;
    Array<Vector> sampleLambda(m_params->sampleSize());
    Array<Vector> sampleU(m_params->sampleSize());
    Array<Vector> sampleS(m_params->sampleSize());
    Array<Vector> sampleSigma(m_params->sampleSize());

    computeParams(0, sampleU, sampleS, T);
    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        sampleLambda[s] = m_task->tau(sampleU[s], T);
        Psi             = m_task->Theta(sampleU[s], T);
        h               = m_task->h(sampleLambda[s], Psi);
        G               = m_task->G(sampleLambda[s], Psi);
        F               = m_task->F(sampleLambda[s], Psi);
        sampleSigma[s]  = Psi * G.transpose() * PinvSVD(F) * (m_sampleY[s] - h);
    }
    computeAdditionParams(0, sampleS, sampleLambda, sampleSigma, L, K, n, e);

    // Индекс n соответствует моменту времени tn = t0 + n * dt  (dt - шаг интегрирования):
    for (size_t i = 1; i < m_result.size(); ++i) {
        m_task->setTime(m_result[i - 1].time);
        m_task->setStep(m_params->integrationStep());

        // Индекс s пробегает по всем элементам выборки:
        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
            m_sampleX[s] = m_task->a(m_sampleX[s]);
        }
        writeResult(i, true);

        // i = 1..K*L*N, если i нацело делится на L*N, значит сейчас время измерения ti = tk:
        if (i % (m_params->predictionCount() * m_params->integrationCount()) == 0) {
            // Индекс s пробегает по всем элементам выборки:
            for (size_t s = 0; s < m_params->sampleSize(); ++s) {
                //вычисляем lambda, Psi: время устонавливаем в предыдущий момент измерения:
                double currentTime  = m_result[i].time;
                double previousTime = currentTime - m_params->measurementStep();
                m_task->setStep(m_params->measurementStep());
                m_task->setTime(previousTime);
                sampleLambda[s] = L * sampleS[s] + n;
                Psi             = m_task->Theta(sampleU[s], T);

                //ставим время обратно и продолжаем:
                m_task->setTime(currentTime);

                h = m_task->h(sampleLambda[s], Psi);
                G = m_task->G(sampleLambda[s], Psi);
                F = m_task->F(sampleLambda[s], Psi);

                m_sampleY[s]   = m_task->b(m_sampleX[s]);
                sampleSigma[s] = Psi * G.transpose() * PinvSVD(F) * (m_sampleY[s] - h);
            }
            // Индекс s пробегает по всем элементам выборки:
            computeAdditionParams(i, sampleS, sampleLambda, sampleSigma, L, K, n, e);
            for (size_t s = 0; s < m_params->sampleSize(); ++s) {
                m_sampleZ[s] = sampleLambda[s] + K * sampleSigma[s] + e;
            }
            writeResult(i);
            computeParams(i, sampleU, sampleS, T);
        }
    }
}

void MFOS::computeParams(size_t n, Array<Vector> &sampleU, Array<Vector> &sampleS, Matrix &T)
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

    Gamma  = DxyDxz * PinvSVD(Ddelta);
    GammaY = Gamma.leftCols(m_task->dimY());
    GammaZ = Gamma.rightCols(m_task->dimX()); // dimZ = dimX

    chi = m_result[n].meanX - GammaY * my - GammaZ * m_result[n].meanZ;
    T   = m_result[n].varX - GammaY * Dxy.transpose() - GammaZ * Dxz.transpose();

    double currentTime  = m_result[n].time;
    double previousTime = currentTime - m_params->measurementStep();
    m_task->setStep(m_params->measurementStep());
    m_task->setTime(previousTime);
    // Индекс s пробегает по всем элементам выборки:
    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        sampleU[s] = GammaY * m_sampleY[s] + GammaZ * m_sampleZ[s] + chi;
        sampleS[s] = m_task->tau(sampleU[s], T);
    }
    m_task->setTime(currentTime);
}

void MFOS::computeAdditionParams(size_t nn, const Array<Vector> &sampleS, const Array<Vector> &sampleLambda,
                                 const Array<Vector> &sampleSigma, Matrix &L, Matrix &K, Vector &n, Vector &e)
{
    Vector me, msigma, ms;
    Matrix Dsigma_e, Dsigma, Ds, Dxs;

    Array<Vector> sampleE(m_params->sampleSize());

    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        sampleE[s] = m_sampleX[s] - sampleLambda[s];
    }
    me       = Mean(sampleE);
    msigma   = Mean(sampleSigma);
    ms       = Mean(sampleS);
    Dsigma   = Var(sampleSigma, msigma);
    Dsigma_e = Cov(sampleE, sampleSigma);
    Ds       = Var(sampleS, ms);
    Dxs      = Cov(m_sampleX, sampleS);

    K = Dsigma_e * PinvSVD(Dsigma);
    e = me - K * msigma;

    L = Dxs * PinvSVD(Ds);
    n = m_result[nn].meanX - L * ms;
}

} // end Discrete

} // end Filters
