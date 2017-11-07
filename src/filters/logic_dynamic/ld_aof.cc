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

AOF::AOF(Core::PtrFilterParameters params, Core::PtrTask task)
    : LogicDynamicFilter(params, task)
{
    long n = task->dimX();
    m_info->setName(m_task->info()->type() + "AОФлд (p=" + std::to_string(n * (n + 3) / 2) + ")");
}

void AOF::algorithm()
{
    m_task->setStep(m_params->measurementStep());

    for (size_t k = 0; k < m_result.size(); ++k) { 
       //qDebug() << "Пора";

        m_task->setTime(m_result[k].time);

        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
            // Блок 1
            computeBlock1(s);;
            // Блок 2
            computeBlock2(s);
        }

        // Блок 3
        writeResult(k, m_task->countI);
        if (k <= m_result.size()) {

            m_task->setTime(m_result[k+1].time); // Время

            for (size_t s = 0; s < m_params->sampleSize(); ++s) {
                // Блок 4
                computeBlock4(s);
                // Блок 5
                computeBlock5(s);
            }

            // Блок 6
            m_sampleI = m_task->generateArrayI(m_params->sampleSize());
            computeBlock6();
        }
    }
}

void AOF::computeBlock1(long s) {
    P[s] = computeProbabilityDensityN(Omega[s], m_sampleY[s], Mu[s], Phi[s]);
    for (int i = 0; i < m_task->countI; i++) {
        K[s][i] = Delta[s][i]*Pinv(Phi[s][i]);
        Sigma[s][i] = Lambda[s][i] + K[s][i]*(m_sampleY[s] - Mu[s][i]);
        Upsilon[s][i] = Psi[s][i] - K[s][i]*Delta[s][i].transpose();
    }
}

void AOF::computeBlock2(long s) {
    Vector resZ = Vector::Zero(Sigma[s][0].size());
    Vector mult = Vector::Zero(Sigma[s][0].size());
    for (int i = 0; i < m_task->countI; i++) {
        mult = P[s][i]*Sigma[s][i];
        resZ += mult;
    }
    m_sampleZ[s] = resZ;
}

void AOF::computeBlock4(long s) {
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
}

void AOF::computeBlock5(long s) {
    for (int i = 0; i < m_task->countI; i++) {
        Mu[s][i] = m_task->h(i+1, Lambda[s][i], Psi[s][i]);
        Delta[s][i] = m_task->G(i+1, Lambda[s][i], Psi[s][i]) - Lambda[s][i] * Mu[s][i].transpose();
        Phi[s][i] =  m_task->F(i+1, Lambda[s][i], Psi[s][i]) - Mu[s][i]*Mu[s][i].transpose();
    }
}

void AOF::computeBlock6() {
    for (size_t s = 0; s < m_params->sampleSize(); s++) {
        m_sampleX[s] = m_task->a(m_sampleI[s], m_sampleX[s]);
        m_sampleY[s] = m_task->b(m_sampleI[s], m_sampleX[s]);
    }
}

} // end LogicDynamic

} // end Filters
