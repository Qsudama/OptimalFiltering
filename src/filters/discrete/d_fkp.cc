#include "d_fkp.h"
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
using Math::MakeBlockMatrix;


FKP::FKP(Core::PtrFilterParameters params, Core::PtrTask task) : DiscreteFilter(params, task)
{

    m_info->setName(m_task->info()->type() + "ФКПд (p=" + std::to_string(task->dimX()) + ")");
}

void FKP::zeroIteration() {
    DiscreteFilter::zeroIteration();

    m_sampleS.resize(m_params->sampleSize());
}

void FKP::algorithm() {

    Vector h, lambda;
    Matrix G, F, Psi, T;

    long ny = long(m_task->dimY());
    long p = ny * long(m_params->orderMult());
    Array<Vector> sampleU(m_params->sampleSize());

    m_task->setStep(m_params->measurementStep());

    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        m_sampleS[s] = Vector::Zero(p);
        for (size_t i = 0; i < ny; i++) {
            m_sampleS[s][i] = m_sampleY[s][i];
        }
    }

    computeParams(0, sampleU, T);

    // Индекс k соответствует моменту времени tk = t0 + k * delta_t  (delta_t - интервал между измерениями):
    for (size_t k = 1; k < m_result.size(); ++k) {
        // Индекс s пробегает по всем элементам выборки:
        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
            m_task->setTime(m_result[k - 1].time);
            // X_k = a(X_{k-1}); время t = t_{k-1}
            m_sampleX[s] = m_task->a(m_sampleX[s]);
            // вычисляем lambda, Psi;  время t = t_{k-1}:
            lambda = m_task->tau(sampleU[s], T);
            Psi    = m_task->Theta(sampleU[s], T);

            //ставим время обратно и продолжаем:
            m_task->setTime(m_result[k].time);

            h = m_task->h(lambda, Psi);
            G = m_task->G(lambda, Psi);
            F = m_task->F(lambda, Psi);

            m_sampleY[s] = m_task->b(m_sampleX[s]);
            m_sampleZ[s] = lambda + Psi * G.transpose() * Pinv(F) * (m_sampleY[s] - h);

            for (long i = p - 1; i >= ny; --i) {
                m_sampleS[s][i] = m_sampleS[s][i - ny];
            }
            for (long i = 0; i < long(ny); ++i) {
                m_sampleS[s][i] = m_sampleY[s][i];
            }
        }
        writeResult(k);
        computeParams(k, sampleU, T);
    }
}

void FKP::computeParams(size_t k, Array<Vector> &u, Matrix &T)
{
    Vector        mx, mu, chi;
    Matrix        Gamma, Du, Dx, DxDu;

    Du   = Var(m_sampleS);
    Dx   = Var(m_sampleX);
    mx   = Mean(m_sampleX);
    mu   = Mean(m_sampleS);
    DxDu = Cov(m_sampleX, m_sampleS);

    Gamma  = DxDu * Pinv(Du);

    chi = m_result[k].meanX - Gamma * mu;
    T   = m_result[k].varX - Gamma * DxDu.transpose();

    // Индекс s пробегает по всем элементам выборки:
    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        u[s] = Gamma * m_sampleS[s] + chi;
    }
}



} // end Discrete

} // end Filters
