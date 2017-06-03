#include "d_duof.h"
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


DUOF::DUOF(Core::PtrFilterParameters params, Core::PtrTask task) : DiscreteFilter(params, task)
{
    m_info->setName(m_task->info()->type() + "ДУОФд (p=" + std::to_string(task->dimX()) + ")");
}

void DUOF::zeroIteration() {
    DiscreteFilter::zeroIteration();

    Lambda.resize(m_params->sampleSize());
    deltaY.resize(m_params->sampleSize());
    Omega.resize(m_params->sampleSize());
    A.resize(m_params->sampleSize());
}

void DUOF::algorithm() {

    m_task->setTime(m_result[0].time);
    m_task->setStep(m_params->measurementStep());

    for (size_t k = 1; k < m_result.size(); ++k) {

        for (size_t i = 0; i < m_params->sampleSize(); ++i) {
            computeFirstStep(k, i);
        }

        F = compute_F();
        g = compute_g();

        for (size_t i = 0; i < m_params->sampleSize(); ++i) {
            computeSecondStep(k, i);
        }

        H = compute_H();
        e = compute_e();

        for (size_t i = 0; i < m_params->sampleSize(); ++i) {
            computeThirdStep(k, i);
        }

        writeResult(k);
    }
}

#pragma mark - Main Steps Compution
void DUOF::computeFirstStep(long k, long i) {
    m_task->setTime(m_result[k - 1].time);
    m_sampleX[i] = m_task->a(m_sampleX[i]);
    A[i] = m_task->a_m(m_sampleZ[i]);
}

void DUOF::computeSecondStep(long k, long i) {
    m_task->setTime(m_result[k - 1].time);
    Lambda[i] = compute_Lambda(i);
    m_sampleE[i] = compute_E(i);
    m_sampleY[i] = m_task->b(m_sampleX[i], Lambda[i]);
    deltaY[i] = compute_DeltaY(i);
    Omega[i] = compute_Omega(i);
}

void DUOF::computeThirdStep(long k, long i) {
    m_task->setTime(m_result[k - 1].time);
    m_sampleZ[i] = compute_Z(i);
}

#pragma mark - Parameters Compution
Matrix DUOF::compute_F() {
    return Cov(m_sampleX, A) * Pinv(Var(A));
}

Vector DUOF::compute_g() {
    return Mean(m_sampleX) - F * Mean(A);
}

Matrix DUOF::compute_H() {
    return Cov(m_sampleE, Omega) * Pinv(Var(Omega));
}

Vector DUOF::compute_e() {
    return Mean(m_sampleE) - H * Mean(Omega);
}

Vector DUOF::compute_Lambda(long i) {
    return F * A[i] + g;
}

Vector DUOF::compute_DeltaY(long i) {
    return m_sampleY[i] - m_task->b_m(Lambda[i]);
}

Vector DUOF::compute_Omega(long i) {
    return deltaY[i];
}

Vector DUOF::compute_E(long i) {
    return m_sampleX[i] - Lambda[i];
}

Vector DUOF::compute_Z(long i) {
    return Lambda[i] + H * deltaY[i] + e;
}



} // end Discrete

} // end Filters
