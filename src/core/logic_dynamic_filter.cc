#include "logic_dynamic_filter.h"

using Math::Statistic::Mean;
using Math::Statistic::Var;
using Math::Statistic::Cov;
using Math::LinAlg::Pinv;


namespace Core
{


LogicDynamicFilter::LogicDynamicFilter(PtrFilterParameters params, PtrTask task)
    : Filter(params)
    , m_task(std::dynamic_pointer_cast<LogicDynamicTask>(task))
{
    m_info->setType("лд");
}

void LogicDynamicFilter::init()
{
    m_sampleX.resize(m_params->sampleSize());
    m_sampleY.resize(m_params->sampleSize());
    m_sampleZ.resize(m_params->sampleSize());
    m_sampleE.resize(m_params->sampleSize());
    m_sampleI.resize(m_params->sampleSize());

    size_t size = size_t(m_params->measurementCount());
    m_result.resize(size);
    for (size_t i = 0; i < size; ++i) {
        m_result[i].time = m_params->measurementStep() * i;
    }
}

void LogicDynamicFilter::zeroIteration() {
    srand(1252); // Для генерации массива I нужно!

    // Инициализация
    Omega.resize(m_params->sampleSize());
    Lambda.resize(m_params->sampleSize());
    Mu.resize(m_params->sampleSize());
    Psi.resize(m_params->sampleSize());
    Delta.resize(m_params->sampleSize());
    Phi.resize(m_params->sampleSize());
    P.resize(m_params->sampleSize());
    K.resize(m_params->sampleSize());
    Sigma.resize(m_params->sampleSize());
    Upsilon.resize(m_params->sampleSize());

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

    m_sampleX.resize(m_params->sampleSize());
    m_sampleY.resize(m_params->sampleSize());
    m_sampleZ.resize(m_params->sampleSize());
    m_sampleE.resize(m_params->sampleSize());
    m_sampleI.resize(m_params->sampleSize());

    size_t size = size_t(m_params->measurementCount());
    m_result.resize(size);
    for (size_t i = 0; i < size; ++i) {
        m_result[i].time = m_params->measurementStep() * i;
    }

    // Блок 0
    computeZeroVectors();

    computeBlock0();
}

void LogicDynamicFilter::computeZeroVectors() {
    m_sampleI = m_task->generateArrayI(m_params->sampleSize());
    for (size_t s = 0; s < m_params->sampleSize(); s++) {
        m_sampleX[s] = m_task->x0();
        m_sampleY[s] = m_task->b(m_sampleI[s], m_sampleX[s]);
    }
}

void LogicDynamicFilter::computeBlock0() {

    Array<Vector> mx(m_task->countI);
    Array<Matrix> varX(m_task->countI);

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

double LogicDynamicFilter::probabilityDensityN(const Vector &u, const Vector &m, const Matrix &D) {
    double pi = Math::Const::PI;
    Matrix det = 2* pi * D;
    double deter = det.determinant();
    Matrix pin = Pinv(D);
    double exponent = exp(((-1 * (u - m).transpose()) * pin * (u - m))(0, 0));
    double n = exponent / sqrt(deter);
    return n;
}

Array<double> LogicDynamicFilter::computeProbabilityDensityN(Array<double> omega, Vector sampleVector,
                                                             Array<Vector> m, Array<Matrix> D) {
    Array<double> resDouble(m_task->countI);
    Array<double> resP(m_task->countI);
    if (m_task->countI == 1 ) {
        resDouble = omega;
    } else {
        for (int i = 0; i < m_task->countI; i++) {
            double N = probabilityDensityN(sampleVector, m[i], D[i]);
            resP[i] = omega[i]*N;
        }
        double resNumerator = 0.0;
        for (int i = 0; i < m_task->countI; i++) {
            resNumerator += resP[i];
        }
        for (int i = 0; i < m_task->countI; i++) {
            if(resNumerator == 0.0) {
                resDouble[i] = resP[i];
            } else {
                resDouble[i] = resP[i]/resNumerator;
            }
        }
    }
    return resDouble;
}

} // end Core

