#include "fos.h"
using Math::LinAlg::Pinv;
using Math::Statistic::Mean;
using Math::Statistic::Var;
using Math::Statistic::Cov;
void FOS::zeroIteration()
{
    LogicDynamicFilter::zeroIteration();

    Xi.resize(m_params->sampleSize());
    u.resize(m_params->sampleSize());

    Q.resize(m_task->countI);
    kappa.resize(m_task->countI);
    T.resize(m_task->countI);
    meanZ.resize(m_task->countI);
    Gamma.resize(m_task->countI);
    Dzz.resize(m_task->countI);

    for (size_t s = 0; s < m_params->sampleSize(); s++) {
        Xi[s].resize(m_task->countI);
        u[s].resize(m_task->countI);
    }
}


void FOS::algorithm()
{
    m_task->setStep(m_params->measurementStep());

    for (size_t k = 0; k < m_result.size(); ++k) {

        m_task->setTime(m_result[k].time);

        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
            // Блок 1
            computeBlock1(s);;
            // Блок 2
            computeBlock2(s);
        }

        // Блок 3
        writeResult(k, m_task->countI);
        // Блок 3а смотри в Блок 2
        // Блок 3б
        computeBlock3b();
        // Блок 3в
        computeBlock3c();
        if (k == 44) {
            computeBlock3c();
        }

        if (k <= m_result.size()) {

            m_task->setTime(m_result[k+1].time); // Время

            for (size_t s = 0; s < m_params->sampleSize(); ++s) {
                // Блок 4
                computeBlock4(s);
                // Блок 5
                computeBlock5(s);
            }

            // Блок 6
            m_sampleI = m_task->generateArrayI(m_params->sampleSize(),k);
            computeBlock6();
        }
    }
}

void FOS::computeBlock1(long s) {
    Array<double> det = calculateDeterminantForProbabilityDensityN(Phi[s]);
    P[s] = computeProbabilityDensityN(Omega[s], m_sampleY[s], Mu[s], Phi[s],det);
    for (int i = 0; i < m_task->countI; i++) {
        K[s][i] = Delta[s][i]*Pinv(Phi[s][i]);
        Sigma[s][i] = Lambda[s][i] + K[s][i]*(m_sampleY[s] - Mu[s][i]);
        Upsilon[s][i] = Psi[s][i] - K[s][i]*Delta[s][i].transpose();
    }
}
void FOS::computeBlock2(long s) {
    Vector resZ = Vector::Zero(Sigma[s][0].size());
    Vector mult = Vector::Zero(Sigma[s][0].size());
    for (int i = 0; i < m_task->countI; i++) {
        mult = P[s][i]*Sigma[s][i];
        resZ += mult;
    }
    m_sampleZ[s] = resZ;
}


void FOS::computeBlock3b() {
    Array<Vector> meanX(m_task->countI);
    Array<Matrix> Dxx(m_task->countI);
    Array<Matrix> Dxz(m_task->countI);

    for (int i = 0; i < m_task->countI; i++) {
        Q[i] = m_task->Pr(i+1);
        meanX[i] = Mean(m_sampleX, m_sampleI, i+1);
        meanZ[i] = Mean(m_sampleZ, m_sampleI, i+1);
        Dxx[i] = Cov(m_sampleX, m_sampleX, m_sampleI, i+1);
        Dxz[i] = Cov(m_sampleX, m_sampleZ, m_sampleI, i+1);
        Dzz[i] = Cov(m_sampleZ, m_sampleZ, m_sampleI, i+1);

        Gamma[i] = Dxz[i] * Pinv(Dzz[i]);
        kappa[i] = meanX[i] - Gamma[i]*meanZ[i];
        T[i] = Dxx[i] - Gamma[i]*Dxz[i].transpose();
    }
}

void FOS::computeBlock3c() {
    for (size_t s = 0; s < m_params->sampleSize(); s++) {
        Array<double> det = calculateDeterminantForProbabilityDensityN(Dzz);
        Xi[s] = computeProbabilityDensityN(Q, m_sampleZ[s], meanZ, Dzz,det);
        for (int i = 0; i < m_task->countI; i++) {
            u[s][i] = Gamma[i]*m_sampleZ[s] + kappa[i];
        }
    }
}

void FOS::computeBlock4(long s) {
    Array<double> resOmega(m_task->countI);
    Array<Vector> resLambda(m_task->countI);
    Array<Matrix> resPsi(m_task->countI);

    for (int l = 0; l < m_task->countI; l++) {
        for (int i = 0; i < m_task->countI; i++) {
            resOmega[i] = Xi[s][i]*m_task->nu(l+1,i+1, u[s][i], T[i]);
        }
        for (int i = 0; i < m_task->countI; i++) {
            resLambda[i] = Xi[s][i]*m_task->tau(l+1,i+1, u[s][i], T[i]);
        }
        for (int i = 0; i < m_task->countI; i++) {
            resPsi[i] = Xi[s][i]*m_task->Theta(l+1,i+1, u[s][i], T[i]);
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

void FOS::computeBlock5(long s) {
    for (int i = 0; i < m_task->countI; i++) {
        Mu[s][i] = m_task->h(i+1, Lambda[s][i], Psi[s][i]);
        Delta[s][i] = m_task->G(i+1, Lambda[s][i], Psi[s][i]) - Lambda[s][i] * Mu[s][i].transpose();
        Phi[s][i] =  m_task->F(i+1, Lambda[s][i], Psi[s][i]) - Mu[s][i]*Mu[s][i].transpose();
    }
}

void FOS::computeBlock6() {
    for (size_t s = 0; s < m_params->sampleSize(); s++) {
        m_sampleX[s] = m_task->a(m_sampleI[s], m_sampleX[s]);
        m_sampleY[s] = m_task->b(m_sampleI[s], m_sampleX[s]);
    }
}

