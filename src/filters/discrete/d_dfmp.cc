#include "d_dfmp.h"
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



DFMP::DFMP(Core::PtrFilterParameters params, Core::PtrTask task) : DiscreteFilter(params, task)
{
    std::string argsCount = std::to_string(params->argumentsCount());
    std::string dimX = std::to_string(task->dimX());
//    m_info->setFullName(m_task->info()->type() + "ДФМПд-" + argsCount + " (p=" + dimX + ")");
    m_info->setName("ДФМПд");
    m_info->setDimension("- " + argsCount + " (p=" + dimX + ")");
}

void DFMP::zeroIteration()
{
    DiscreteFilter::zeroIteration();

    S.resize(m_params->sampleSize());
    U.resize(m_params->sampleSize());
    E.resize(m_params->sampleSize());
    Lambda.resize(m_params->sampleSize());

    for (size_t i = 0; i < m_params->sampleSize(); ++i) {
        Lambda[i] = Mean(m_sampleX);
        Uint argsCount = m_params->argumentsCount();
        if (argsCount == 2) {
            U[i] = m_sampleZ[i];
        } else if (argsCount == 3) {
            MakeBlockVector(m_sampleY[i], m_sampleZ[i], U[i]);
        }
    }

    Vector        my;
    Matrix        Gamma, DxyDxz, Ddelta, Dxy, Dxz;



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

void DFMP::algorithm()
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


void DFMP::computeFirstStep(long k, long i) {
    m_task->setTime(m_result[k - 1].time);
    m_sampleX[i] = m_task->a(m_sampleX[i]);
    S[i] = m_task->tau(E[i], T);
}

void DFMP::computeSecondStep(long k, long i) {
    m_task->setTime(m_result[k - 1].time);
    Lambda[i] = compute_Lambda(i);
    m_sampleY[i] = m_task->b(m_sampleX[i]);
    m_sampleZ[i] = compute_Z(i);
}


Vector DFMP::compute_Lambda(long i) {
    return L * S[i] + o;
}

Vector DFMP::compute_Z(long i) {
    h = m_task->h(Lambda[i], Psi);
    G = m_task->G(Lambda[i], Psi);
    F = m_task->F(Lambda[i], Psi);

    return Lambda[i] + Psi * G.transpose() * Pinv(F) * (m_sampleY[i] - h);
}

Vector DFMP::compute_E(long i) {
    return Gamma * U[i] + chi;
}

void DFMP::compute_U(long i) {
    Uint argsCount = m_params->argumentsCount();
    if (argsCount == 2) {
        U[i] = m_sampleZ[i];
    } else if (argsCount == 3) {
        MakeBlockVector(m_sampleY[i], m_sampleZ[i], U[i]);
    }
}


Matrix DFMP::compute_Gamma()
{

    return Cov(m_sampleX, U) * Pinv(Var(U));
}

Matrix DFMP::compute_Psi()
{
    return Var(m_sampleX) - L * Cov(m_sampleX, S).transpose();
}

Matrix DFMP::compute_L()
{
    return Cov(m_sampleX, S) * Pinv(Var(S));
}

Matrix DFMP::compute_T()
{

    return Var(m_sampleX) - Gamma * Cov(m_sampleX, U).transpose();
}


Vector DFMP::compute_chi()
{
    return Mean(m_sampleX) - Gamma * Mean(U);
}

Vector DFMP::compute_o()
{
    return Mean(m_sampleX) - L * Mean(S);
}




} // end Discrete

} // end Filters
