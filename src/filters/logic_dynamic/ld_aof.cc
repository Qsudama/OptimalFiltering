#include "ld_aof.h"

namespace Filters
{

namespace LogicDynamic
{


using Math::LinAlg::Pinv;
using Math::Statistic::Mean;
using Math::Statistic::Var;
using Math::Statistic::Cov;
using Math::MakeBlockVector;
using Math::MakeBlockMatrix;

AOF::AOF(Core::PtrFilterParameters params, Core::PtrTask task)
    : LogicDynamicFilter(params, task)
{
    long n = task->dimX();
    m_info->setName(m_task->info()->type() + "AОФлд (p=" + std::to_string(n * (n + 3) / 2) + ")");
}

void AOF::zeroIteration()
{
//    LogicDynamicFilter::zeroIteration();

//    Vector my0  = Math::Statistic::Mean(m_sampleY);
//    Matrix Dy0  = Math::Statistic::Var(m_sampleY, my0);
//    Matrix Dxy0 = Math::Statistic::Cov(m_sampleX, m_sampleY);
//    Matrix H0   = Dxy0 * Pinv(Dy0);

//    m_sampleP.resize(m_params->sampleSize());
//    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
//        m_sampleP[s] = m_result[0].varX - H0 * Dxy0.transpose();
//    }
}

void AOF::algorithm()
{
    int i = 1;
    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        m_sampleI[s] = m_task->nextI(i);
        m_sampleX[s] = m_task->x0();
        m_sampleY[s] = m_task->b(m_sampleI[s], m_sampleX[s]);

    }

    Vector mx  = Mean(m_sampleX, m_sampleI, i);
    Matrix Dx = Cov(m_sampleX, m_sampleY, m_sampleI, i);

    double P = m_task->Pr()[i-1];

    Omega = P;
    Lambda = mx;
    Mu = m_task->h(i, mx, Dx);
    Psi = Dx;
    Delta = m_task->G(i, mx, Dx) - Lambda * Mu.transpose();
    Phi = m_task->F(i, mx, Dx) - Mu*Mu.transpose();

    Matrix K = Delta*Pinv(Phi);
    Vector sigma = Lambda + K*(m_sampleY[0] - Mu);

    Matrix Upsilon;


    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        m_sampleZ[s] = P*sigma;
        m_sampleE[s] = m_sampleX[s] - m_sampleZ[s];
    }

    m_result[0].meanX = mx;
    m_result[0].meanZ = Mean(m_sampleZ);
    m_result[0].meanE = Mean(m_sampleE);
    m_result[0].varX  = Var(m_sampleX, m_result[0].meanX);
    m_result[0].varZ  = Var(m_sampleZ, m_result[0].meanZ);
    m_result[0].varE  = Var(m_sampleE, m_result[0].meanE);
    m_result[0].time  = 0.0;


//    Vector h, lambda;
//    Matrix G, F, H, Psi;

//    m_task->setStep(m_params->measurementStep());

//    // Индекс k соответствует моменту времени tk = t0 + k * delta_t  (delta_t - интервал между измерениями):
//    for (size_t k = 1; k < m_result.size(); ++k) {
//        Array<int> sampleI = m_sampleI;
//        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
//            m_sampleI[s] = m_task->nextI(sampleI[s]);
//        }
//        // Индекс s пробегает по всем элементам выборки:
//        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
//            m_task->setTime(m_result[k - 1].time);
//            // X_k = a(X_{k-1}); время t = t_{k-1}
//            m_sampleX[s] = m_task->a(m_sampleI[s], m_sampleX[s]);
//            // вычисляем lambda, Psi;  время t = t_{k-1}:
//            lambda = m_task->tau(m_sampleI[s], sampleI[s], m_sampleZ[s], m_sampleP[s]);
//            Psi    = m_task->Theta(m_sampleI[s], sampleI[s], m_sampleZ[s], m_sampleP[s]);

//            //ставим текущее (t = tk) время и продолжаем:
//            m_task->setTime(m_result[k].time);

//            h = m_task->h(m_sampleI[s], lambda, Psi);
//            G = m_task->G(m_sampleI[s], lambda, Psi);
//            F = m_task->F(m_sampleI[s], lambda, Psi);
//            H = Psi * G.transpose() * Pinv(F);

//            m_sampleY[s] = m_task->b(m_sampleI[s], m_sampleX[s]);
//            m_sampleZ[s] = lambda + H * (m_sampleY[s] - h);
//            m_sampleP[s] = Psi - H * G * Psi;
//            m_sampleP[s] = 0.5 * (m_sampleP[s] + m_sampleP[s].transpose());
//        }
//        writeResult(k);
//    }
}


} // end LogicDynamic

} // end Filters
