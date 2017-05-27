#include "ld_aof.h"

#include "iostream"


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
    LogicDynamicFilter::zeroIteration();

//    Vector my0  = Math::Statistic::Mean(m_sampleY);
//    Matrix Dy0  = Math::Statistic::Var(m_sampleY, my0);
//    Matrix Dxy0 = Math::Statistic::Cov(m_sampleX, m_sampleY);
//    Matrix H0   = Dxy0 * Pinv(Dy0);

    m_sampleP.resize(m_params->sampleSize());
//    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
//        m_sampleP[s] = m_result[0].varX - H0 * Dxy0.transpose();
//    }
}

void AOF::algorithm()
{
    // Zero step
    m_sampleI = m_task->generateArrayI(m_params->sampleSize());

    Array<double> Omega(m_task->countI);
    Array<Vector> Lambda(m_task->countI);
    Array<Vector> Mu(m_task->countI);
    Array<Matrix> Psi(m_task->countI);
    Array<Matrix> Delta(m_task->countI);
    Array<Matrix> Phi(m_task->countI);
    Array<double> P(m_task->countI);
    Array<Matrix> K(m_task->countI);
    Array<Vector> Sigma(m_task->countI);
    Array<Matrix> Upsilon(m_task->countI);

    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        m_sampleX[s] = m_task->x0();
        m_sampleY[s] = m_task->b(m_sampleI[s], m_sampleX[s]);
    }

    Array<Vector> mx(m_task->countI);
    Array<Matrix> Dxx(m_task->countI);

    for (int i = 0; i < m_task->countI; i++) {
        mx[i] = Mean(m_sampleX, m_sampleI, i+1);
        Dxx[i] = Cov(m_sampleX, m_sampleX, m_sampleI, i+1);
    }

    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        for (int i = 0; i < m_task->countI; i++) {
            P[i] = m_task->Pr(i+1);
            Omega[i] = P[i];
            Lambda[i] = mx[i];
            Mu[i] = m_task->h(i+1, mx[i], Dxx[i]);
            Psi[i] = Dxx[i];

            Delta[i] = m_task->G(i+1, mx[i], Dxx[i]) - Lambda[i] * Mu[i].transpose();
            Phi[i] =  m_task->F(i+1, mx[i], Dxx[i]) - Mu[i]*Mu[i].transpose();
            K[i] = Delta[i]*Pinv(Phi[i]);
            Sigma[i] = Lambda[i] + K[i]*(m_sampleY[s] - Mu[i]);
        }
        Vector res = Vector::Zero(Sigma[0].size());
        for (int i = 0; i < m_task->countI; i++) {
            res += P[i]*Sigma[i];
        }
        m_sampleZ[s] = res;
        m_sampleP[s] = Matrix::Zero(6,6);
        m_sampleE[s] = m_sampleX[s] - m_sampleZ[s];
    }

    Vector resMx = Vector::Zero(mx[0].size());
    for (int i = 0; i < m_task->countI; i++) {
        resMx += mx[i];
    }

    m_result[0].meanX = resMx/m_task->countI;
    m_result[0].meanZ = Mean(m_sampleZ);
    m_result[0].meanE = Mean(m_sampleE);
    m_result[0].varX  = Var(m_sampleX, m_result[0].meanX);
    m_result[0].varZ  = Var(m_sampleZ, m_result[0].meanZ);
    m_result[0].varE  = Var(m_sampleE, m_result[0].meanE);
    m_result[0].time  = 0.0;


    m_task->setStep(m_params->measurementStep());

    // Индекс k соответствует моменту времени tk = t0 + k * delta_t  (delta_t - интервал между измерениями):
    for (size_t k = 1; k < m_result.size(); ++k) {
        m_sampleI = m_task->generateArrayI(m_params->sampleSize());
        // Индекс s пробегает по всем элементам выборки:
        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
            m_task->setTime(m_result[k - 1].time);

            // X_k = a(X_{k-1}); время t = t_{k-1}
            m_sampleX[s] = m_task->a(m_sampleI[s], m_sampleX[s]);
            m_sampleY[s] = m_task->b(m_sampleI[s], m_sampleX[s]);

            Vector resZ = Vector::Zero(m_sampleZ[0].size());
            for (int i = 0; i < m_task->countI; i++) {

                Mu[i] = m_task->h(i+1, Lambda[i], Psi[i]);
                Delta[i] = m_task->G(i+1, Lambda[i], Psi[i]) - Lambda[i] * Mu[i].transpose();
                Phi[i] =  m_task->F(i+1, Lambda[i], Psi[i]) - Mu[i]*Mu[i].transpose();
                double N = probabilityDensityN(m_sampleY[s], Mu[i], Phi[i]);
                double resP = Omega[i]*N;
                double resNumerator = 0.0;
                for (int j = 0; j < m_task->countI; j++) {
                    resNumerator += Omega[j]*N;
                }
                P[i] = resP/resNumerator;

                K[i] = Delta[i]*Pinv(Phi[i]);
                Sigma[i] = Lambda[i] + K[i]*(m_sampleY[s] - Mu[i]);
                Upsilon[i] = Psi[i] - K[i]*Delta[i].transpose();
                resZ += P[i]*Sigma[i];
            }

            m_sampleZ[s] = resZ;

            //ставим текущее (t = tk) время и продолжаем:
            m_task->setTime(m_result[k].time);

            Array<double> resOmega(m_task->countI);
            Array<Vector> resLambda(m_task->countI);
            Array<Matrix> resPsi(m_task->countI);

            Matrix resUpsilon = Matrix::Zero(Upsilon[0].rows(), Upsilon[0].cols());

            for (int i = 0; i < m_task->countI; i++) {
                for (int l = 0; l < m_task->countI; l++) {
                    resOmega[l] = P[i]*m_task->nu(i+1,l+1, Sigma[i], Upsilon[i]);
                }
                for (int l = 0; l < m_task->countI; l++) {
                    resLambda[l] = P[i]*m_task->tau(i+1,l+1, Sigma[i], Upsilon[i]);
                }
                for (int l = 0; l < m_task->countI; l++) {
                    resPsi[l] = P[i]*m_task->Theta(i+1,l+1, Sigma[i], Upsilon[i]);
                }
                double sumOmega = 0.0;
                Vector sumLambda = Vector::Zero(Lambda[i].size());
                Matrix sumPsi = Matrix::Zero(Psi[i].rows(), Psi[i].cols());
                for (int l = 0; l < m_task->countI; l++) {
                    sumOmega += resOmega[l];
                    sumLambda += resLambda[l];
                    sumPsi += resPsi[l];
                }
                Omega[i] = sumOmega;
                Lambda[i] = sumLambda/Omega[i];
                Psi[i] = sumPsi/Omega[i] - Lambda[i]*Lambda[i].transpose();
                resUpsilon += Upsilon[i];
            }

            m_sampleP[s] = resUpsilon;
        }
        writeResult(k);
    }
}


} // end LogicDynamic

} // end Filters
