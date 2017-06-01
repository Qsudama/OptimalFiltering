#include "ld_aof.h"

#include "iostream"
#include <limits>
#include <cmath>


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
    m_sampleP.resize(m_params->sampleSize());
//    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
//        m_sampleP[s] = m_result[0].varX - H0 * Dxy0.transpose();
//    }
}

void AOF::algorithm()
{
    Array<Array<double>> Omega(m_params->sampleSize());
    Array<Array<Vector>> Lambda(m_params->sampleSize());
    Array<Array<Vector>> Mu(m_params->sampleSize());
    Array<Array<Matrix>> Psi(m_params->sampleSize());
    Array<Array<Matrix>> Delta(m_params->sampleSize());
    Array<Array<Matrix>> Phi(m_params->sampleSize());
    Array<Array<double>> P(m_params->sampleSize());
    Array<Array<Matrix>> K(m_params->sampleSize());
    Array<Array<Vector>> Sigma(m_params->sampleSize());
    Array<Array<Matrix>> Upsilon(m_params->sampleSize());

    for (size_t s = 0; s < m_params->sampleSize(); s++) {
        Omega[s].resize(m_task->countI);
        Lambda[s].resize(m_task->countI);
        Mu[s].resize(m_task->countI);
        Psi[s].resize(m_task->countI);
        Delta[s].resize(m_task->countI);
        Phi[s].resize(m_task->countI);
        P[s].resize(m_task->countI);
        K[s].resize(m_task->countI);
        Sigma[s].resize(m_task->countI);
        Upsilon[s].resize(m_task->countI);
    }

    Array<Vector> mx(m_task->countI);
    Array<Matrix> varX(m_task->countI);

    m_task->setStep(m_params->measurementStep());

    // Индекс k соответствует моменту времени tk = t0 + k * delta_t  (delta_t - интервал между измерениями):
    for (size_t k = 0; k < m_result.size(); ++k) {

        m_task->setTime(m_result[k].time);

        m_sampleI = m_task->generateArrayI(m_params->sampleSize());

        if (k == 0) {
            for (size_t s = 0; s < m_params->sampleSize(); s++) {
                m_sampleX[s] = m_task->x0();
                m_sampleY[s] = m_task->b(m_sampleI[s], m_sampleX[s]);
            }
            for (int i = 0; i < m_task->countI; i++) {
                mx[i] = Mean(m_sampleX, m_sampleI, i+1);
                varX[i] = Cov(m_sampleX, m_sampleX, m_sampleI, i+1);
            }

            for (size_t s = 0; s < m_params->sampleSize(); s++) {
                for (int i = 0; i < m_task->countI; i++) {
                    Omega[s][i] = m_task->Pr(i+1);
                    Lambda[s][i] = mx[i];
                    Mu[s][i] = m_task->h(i+1, mx[i], varX[i]);
                    Psi[s][i] = varX[i];

                    Delta[s][i] = m_task->G(i+1, mx[i], varX[i]) - Lambda[s][i] * Mu[s][i].transpose();
                    Phi[s][i] =  m_task->F(i+1, mx[i], varX[i]) - Mu[s][i]*Mu[s][i].transpose();
                }
            }
        }

        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
            // Блок 1
            computeProbabilityDensityN(P[s], Omega[s], m_sampleY[s], Mu[s], Phi[s]);

            for (int i = 0; i < m_task->countI; i++) {
                K[s][i] = Delta[s][i]*Pinv(Phi[s][i]);
                Sigma[s][i] = Lambda[s][i] + K[s][i]*(m_sampleY[s] - Mu[s][i]);
                Upsilon[s][i] = Psi[s][i] - K[s][i]*Delta[s][i].transpose();
            }
            // Блок 2
            Vector resZ = Vector::Zero(Sigma[s][0].size());
            for (int i = 0; i < m_task->countI; i++) {
                resZ += P[s][i]*Sigma[s][i];
            }
            m_sampleZ[s] = resZ;
        }

        // Блок 3
        m_task->setTime(m_result[k].time); // Время

        writeResult(k, m_task->countI);

        m_task->setTime(m_result[k+1].time); // Время

        // Блок 4
        for (size_t s = 0; s < m_params->sampleSize(); ++s) {

//            m_task->setTime(m_result[k+1].time); // Время

            Array<double> resOmega(m_task->countI);
            Array<Vector> resLambda(m_task->countI);
            Array<Matrix> resPsi(m_task->countI);

            for (int l = 0; l < m_task->countI; l++) {
                for (int i = 0; i < m_task->countI; i++) {
                    resOmega[i] = P[s][i]*m_task->nu(l+1,i+1, Sigma[s][i], Upsilon[s][i]);
                }
                for (int i = 0; i < m_task->countI; i++) {
                    resLambda[i] = P[s][i]*m_task->tau(l+1,i+1, Sigma[s][i], Upsilon[s][i]);
                }
                for (int i = 0; i < m_task->countI; i++) {
                    resPsi[i] = P[s][i]*m_task->Theta(l+1,i+1, Sigma[s][i], Upsilon[s][i]);
                }
                double sumOmega = 0.0;
                Vector sumLambda = Vector::Zero(Lambda[s][l].size());
                Matrix sumPsi = Matrix::Zero(Psi[s][l].rows(), Psi[s][l].cols());
                for (int i = 0; i < m_task->countI; i++) {
                    sumOmega += resOmega[i];
                    sumLambda += resLambda[i];
                    sumPsi += resPsi[i];
                }
                Omega[s][l] = sumOmega;
                Lambda[s][l] = sumLambda/Omega[s][l];
                Psi[s][l] = sumPsi/Omega[s][l] - Lambda[s][l]*Lambda[s][l].transpose();
            }

            // Блок 5
            for (int i = 0; i < m_task->countI; i++) {
                Mu[s][i] = m_task->h(i+1, Lambda[s][i], Psi[s][i]);
                Delta[s][i] = m_task->G(i+1, Lambda[s][i], Psi[s][i]) - Lambda[s][i] * Mu[s][i].transpose();
                Phi[s][i] =  m_task->F(i+1, Lambda[s][i], Psi[s][i]) - Mu[s][i]*Mu[s][i].transpose();
            }
        }

        // Блок 6
        m_sampleI = m_task->generateArrayI(m_params->sampleSize());
        for (size_t s = 0; s < m_params->sampleSize(); s++) {
            m_sampleX[s] = m_task->a(m_sampleI[s], m_sampleX[s]);
            m_sampleY[s] = m_task->b(m_sampleI[s], m_sampleX[s]);
        }

        m_task->setTime(m_result[k].time); // Время

//        for (size_t s = 0; s < m_params->sampleSize(); ++s) {

//            Matrix resUpsilon = Matrix::Zero(Upsilon[0].rows(), Upsilon[0].cols());

//            resUpsilon += Upsilon[i];
//            m_sampleP[s] = resUpsilon;
//        }

//        }

    }
}

void AOF::computeProbabilityDensityN(Array<double> &resDouble, Array<double> omega,
                                     Vector sampleVector, Array<Vector> mu, Array<Matrix> D) {
    Array<double> resP(m_task->countI);
    for (int i = 0; i < m_task->countI; i++) {
        if (m_task->countI == 1 ) {
            resDouble[i] = omega[i];
        } else {
            double N = probabilityDensityN(sampleVector, mu[i], D[i]);
            resP[i] = omega[i]*N;
            double resNumerator = 0.0;
            for (int i = 0; i < m_task->countI; i++) {
                resNumerator += resP[i];
            }
            for (int i = 0; i < m_task->countI; i++) {
                resDouble[i] = resP[i]/resNumerator;
            }
        }
    }
}

} // end LogicDynamic

} // end Filters
