#include "d_mfos.h"
#include "src/math/statistic.h"

#include <iostream>

using namespace std;

namespace Filters
{

namespace Discrete
{


using Math::Rand::gaussianVector;
using Math::LinAlg::PinvSVD;
using Math::LinAlg::PinvGreville;
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
    Vector h;
    Matrix G, F, Psi, T;
    Array<Vector> m_sampleSigma(m_params->sampleSize());
    Array<Vector> sampleU(m_params->sampleSize());

    Vector vectorN, vector_e;
    Matrix Dxs, Ds, DSigma, DSigmaE, K, L;

    Array<Vector> E(m_params->sampleSize());
    Array<Vector> lambda(m_params->sampleSize());
    Array<Vector> S(m_params->sampleSize());
    computeParams(0, sampleU, T);

    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        S[s] = Vector::Zero(3);
        m_sampleSigma[s] = Vector::Zero(3);
        lambda[s] = Vector::Zero(3);
        E[s] = Vector::Zero(3);
    }

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
            // Индекс s пробегает по всем элементам выборки:
            for (size_t s = 0; s < m_params->sampleSize(); ++s) {

                S[s] = m_task->tau(sampleU[s], T);
                Ds = Var(S);

                //вычисляем lambda, Psi: время устонавливаем в предыдущий момент измерения:
                double currentTime  = m_task->time();
                double previousTime = currentTime - m_params->measurementStep();
                m_task->setStep(m_params->measurementStep());
                m_task->setTime(previousTime);

                Psi    = m_task->Theta(sampleU[s], T);
                Dxs = Cov(m_sampleX, S);
                L = Dxs * PinvSVD(Ds);
                vectorN = m_result[n].meanX - L * Mean(S);
                lambda[s] = L * S[s] + vectorN;

                //ставим время обратно и продолжаем:
                m_task->setTime(currentTime);

                h = m_task->h(lambda[s], Psi);
                G = m_task->G(lambda[s], Psi);
                F = m_task->F(lambda[s], Psi);
                m_sampleY[s] = m_task->b(m_sampleX[s]);
                m_sampleSigma[s] = Psi * G.transpose() * PinvGreville(F) * (m_sampleY[s] - h);

                DSigma = Var(m_sampleSigma);
                for (size_t s = 0; s < m_params->sampleSize(); ++s) {
                    E[s] = m_sampleX[s] - lambda[s];
                }
                DSigmaE = Cov(E, m_sampleSigma);
                K = DSigmaE*PinvGreville(DSigma);
                vector_e = Mean(E) - K*Mean(m_sampleSigma);

                m_sampleZ[s] = lambda[s] + K*m_sampleSigma[s]+vector_e;
            }
            writeResult(n);
            computeParams(n, sampleU, T);
        }
    }
}

void MFOS::computeParams(size_t n, Array<Vector> &u, Matrix &T)
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

    Gamma  = DxyDxz * PinvGreville(Ddelta);
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
