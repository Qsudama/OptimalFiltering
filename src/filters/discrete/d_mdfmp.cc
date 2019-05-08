#include "d_mdfmp.h"
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


MDFMP::MDFMP(Core::PtrFilterParameters params, Core::PtrTask task) : DiscreteFilter(params, task)
{
    std::string argsCount = std::to_string(params->argumentsCount());
    std::string dimX = std::to_string(task->dimX());
//    m_info->setFullName(m_task->info()->type() + "МДФМПд-" + argsCount + " (p=" + dimX + ")");
    m_info->setName("МДФМПд");
    m_info->setDimension("- " + argsCount + " (p=" + dimX + ")");
}

void MDFMP::zeroIteration()
{
    DiscreteFilter::zeroIteration();

    S.resize(m_params->sampleSize());
    U.resize(m_params->sampleSize());
    E.resize(m_params->sampleSize());
    Sigma.resize(m_params->sampleSize());
    Delta.resize(m_params->sampleSize());
    Lambda.resize(m_params->sampleSize());

    for (size_t i = 0; i < m_params->sampleSize(); ++i) {
        Lambda[i] = Mean(m_sampleX);
        switch (m_params->argumentsCount()) {
        case 2:
            U[i] = m_sampleZ[i];
            break;
        case 3:
            MakeBlockVector(m_sampleY[i], m_sampleZ[i], U[i]);
            break;
        default:
            break;
        }
    }

    Vector        my;
    Matrix        Gamma, GammaY, GammaZ, DxyDxz, Ddelta, Dxy, Dxz;

    Gamma  = Cov(m_sampleX, U) * Pinv(Var(U));
//    GammaY = Gamma.leftCols(m_task->dimY());
//    GammaZ = Gamma.rightCols(m_task->dimX()); // dimZ = dimX

    chi = m_result[0].meanX - Gamma * Mean(U);
    T   = m_result[0].varX - Gamma * Cov(m_sampleX, U).transpose();

    // Индекс s пробегает по всем элементам выборки:
    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        E[s] = Gamma * U[s] + chi;
    }
}

void MDFMP::algorithm()
{

    m_task->setTime(m_result[0].time);
    m_task->setStep(m_params->measurementStep());

    for (size_t k = 1; k < m_result.size(); ++k) {

        for (size_t i = 0; i < m_params->sampleSize(); ++i) {
            computeFirstStep(k, i);
        }

        L = compute_L();
        Psi = compute_Psi();
        o = compute_o();

        for (size_t i = 0; i < m_params->sampleSize(); ++i) {
            computeSecondStep(k, i);
        }

        H = compute_H();
        e = compute_e();

        for (size_t i = 0; i < m_params->sampleSize(); ++i) {
            m_sampleZ[i] = compute_Z(i);
        }

        for (size_t i = 0; i < m_params->sampleSize(); ++i) {
            compute_U(i);
        }

        Gamma = compute_Gamma();
        chi = compute_chi();
        T = compute_T();

        for (size_t i = 0; i < m_params->sampleSize(); ++i) {
            E[i] = compute_E(i);
        }

        writeResult(k);
    }
}


void MDFMP::computeFirstStep(long k, long i) {
    m_task->setTime(m_result[k - 1].time);
    m_sampleX[i] = m_task->a(m_sampleX[i]);
    S[i] = m_task->tau(E[i], T);
}

void MDFMP::computeSecondStep(long k, long i) {
    m_task->setTime(m_result[k - 1].time);
    Lambda[i] = compute_Lambda(i);
    m_sampleY[i] = m_task->b(m_sampleX[i]);
    Sigma[i] = compute_Sigma(i);
    Delta[i] = m_sampleX[i] - Lambda[i];
}


Vector MDFMP::compute_Lambda(long i) {
    return L * S[i] + o;
}

Vector MDFMP::compute_Sigma(long i) {
    h = m_task->h(Lambda[i], Psi);
    G = m_task->G(Lambda[i], Psi);
    F = m_task->F(Lambda[i], Psi);

    return Psi * G.transpose() * Pinv(F) * (m_sampleY[i] - h);
}

Vector MDFMP::compute_Z(long i) {
    return Lambda[i] + H * Sigma[i] + e;
}

Vector MDFMP::compute_E(long i) {
    return Gamma * U[i] + chi;
}

void MDFMP::compute_U(long i) {
    switch (m_params->argumentsCount()) {
    case 2:
        U[i] = m_sampleZ[i];
        break;
    case 3:
        MakeBlockVector(m_sampleY[i], m_sampleZ[i], U[i]);
        break;
    default:
        break;
    }
}


Matrix MDFMP::compute_Gamma()
{
    return Cov(m_sampleX, U) * Pinv(Var(U));
}

Matrix MDFMP::compute_Psi()
{
    return Var(m_sampleX) - L * Cov(m_sampleX, S).transpose();
}

Matrix MDFMP::compute_L()
{
    return Cov(m_sampleX, S) * Pinv(Var(S));
}

Matrix MDFMP::compute_T()
{

    return Var(m_sampleX) - Gamma * Cov(m_sampleX, U).transpose();
}

Matrix MDFMP::compute_H()
{
    return Cov(Delta, Sigma) * Pinv(Var(Delta));
}


Vector MDFMP::compute_chi()
{
    return Mean(m_sampleX) - Gamma * Mean(U);
}

Vector MDFMP::compute_o()
{
    return Mean(m_sampleX) - L * Mean(S);
}

Vector MDFMP::compute_e()
{
    return Mean(Delta) - H * Mean(Sigma);
}



} // end Discrete

} // end Filters
