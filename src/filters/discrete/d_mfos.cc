#include "d_mfos.h"
#include "src/math/statistic.h"

namespace Filters
{

namespace Discrete
{


using Math::LinAlg::Pinv;
using Math::Statistic::Mean;
using Math::Statistic::Var;
using Math::Statistic::Cov;
using Math::MakeBlockVector;


MFOS::MFOS(Core::PtrFilterParameters params, Core::PtrTask task)
    : DiscreteFilter(params, task)
{
//    m_info->setFullName(m_task->info()->type() + "МФМПд (p=" + std::to_string(task->dimX()) + ")");
    m_info->setName("МФМПд");
    m_info->setDimension("(p=" + std::to_string(task->dimX()) + ")");
}

void MFOS::algorithm()
{
    Vector        h, n, e;
    Matrix        G, F, Psi, T, L, K;
    Array<Vector> sampleLambda(m_params->sampleSize());
    Array<Vector> sampleU(m_params->sampleSize());
    Array<Vector> sampleS(m_params->sampleSize());
    Array<Vector> sampleSigma(m_params->sampleSize());

    m_task->setTime(m_result[0].time);
    m_task->setStep(m_params->measurementStep());

    computeParams(0, sampleU, sampleS, T);
    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        sampleLambda[s] = m_result[0].meanX;
        Psi             = m_result[0].varX;
        h               = m_task->h(sampleLambda[s], Psi);
        G               = m_task->G(sampleLambda[s], Psi);
        F               = m_task->F(sampleLambda[s], Psi);
        sampleSigma[s]  = Psi * G.transpose() * Pinv(F) * (m_sampleY[s] - h);
    }
    computeAdditionParams(sampleS, sampleLambda, sampleSigma, L, K, n, e);

    // Индекс k соответствует моменту времени tk = t0 + k * delta_t  (delta_t - интервал между измерениями):
    for (size_t k = 1; k < m_result.size(); ++k) {
        // Индекс s пробегает по всем элементам выборки:
        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
            m_task->setTime(m_result[k - 1].time);
            // X_k = a(X_{k-1}); время t = t_{k-1}
            m_sampleX[s] = m_task->a(m_sampleX[s]);
            // вычисляем lambda, Psi;  время t = t_{k-1}:
            sampleLambda[s] = L * sampleS[s] + n;
            Psi             = m_task->Theta(sampleU[s], T);

            //ставим время обратно и продолжаем:
            m_task->setTime(m_result[k].time);

            h = m_task->h(sampleLambda[s], Psi);
            G = m_task->G(sampleLambda[s], Psi);
            F = m_task->F(sampleLambda[s], Psi);

            m_sampleY[s]   = m_task->b(m_sampleX[s]);
            sampleSigma[s] = Psi * G.transpose() * Pinv(F) * (m_sampleY[s] - h);
        }
        computeAdditionParams(sampleS, sampleLambda, sampleSigma, L, K, n, e);
        // Индекс s пробегает по всем элементам выборки:
        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
            m_sampleZ[s] = sampleLambda[s] + K * sampleSigma[s] + e;
        }
        writeResult(k);
        computeParams(k, sampleU, sampleS, T);
    }
}

void MFOS::computeParams(size_t k, Array<Vector> &sampleU, Array<Vector> &sampleS, Matrix &T)
{
    Vector        my, chi;
    Matrix        Gamma, GammaY, GammaZ, DxyDxz, Ddelta, Dxy, Dxz;
    Array<Vector> sampleDelta(m_params->sampleSize());

    // Индекс s пробегает по всем элементам выборки:
    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        Uint argsCount = m_params->argumentsCount();
        if (argsCount == 2) {
            sampleDelta[s] = m_sampleZ[s];
        } else if (argsCount == 3) {
            MakeBlockVector(m_sampleY[s], m_sampleZ[s], sampleDelta[s]);
        }
    }

    Gamma  = Cov(m_sampleX, sampleDelta) * Pinv(Var(sampleDelta));
//    GammaY = Gamma.leftCols(m_task->dimY());
//    GammaZ = Gamma.rightCols(m_task->dimX()); // dimZ = dimX

    chi = m_result[0].meanX - Gamma * Mean(sampleDelta);
    T   = m_result[0].varX - Gamma * Cov(m_sampleX, sampleDelta).transpose();

    m_task->setTime(m_result[k].time);
    // Индекс s пробегает по всем элементам выборки:
    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        sampleU[s] = Gamma * sampleDelta[s] + chi;
        sampleS[s] = m_task->tau(sampleU[s], T);
    }
}

void MFOS::computeAdditionParams(const Array<Vector> &sampleS, const Array<Vector> &sampleLambda,
                                 const Array<Vector> &sampleSigma, Matrix &L, Matrix &K, Vector &n, Vector &e)
{
    Vector me, mx, msigma, ms;
    Matrix Dsigma_e, Dsigma, Ds, Dxs;

    Array<Vector> sampleE(m_params->sampleSize());

    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        sampleE[s] = m_sampleX[s] - sampleLambda[s];
    }
    mx       = Mean(m_sampleX);
    me       = Mean(sampleE);
    msigma   = Mean(sampleSigma);
    ms       = Mean(sampleS);
    Dsigma   = Var(sampleSigma, msigma);
    Dsigma_e = Cov(sampleE, sampleSigma);
    Ds       = Var(sampleS, ms);
    Dxs      = Cov(m_sampleX, sampleS);

    K = Dsigma_e * Pinv(Dsigma);
    e = me - K * msigma;

    L = Dxs * Pinv(Ds);
    n = mx - L * ms;
}


} // end Discrete

} // end Filters
