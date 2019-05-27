#include "ld_fos.h"
#include "src/math/statistic.h"


namespace Filters
{

namespace LogicDynamic
{

using Math::LinAlg::Pinv;
using Math::Statistic::Mean;
using Math::Statistic::Var;
using Math::Statistic::Cov;


FOS::FOS(Core::PtrFilterParameters params, Core::PtrTask task, FILTER_ID id)
    : LogicDynamicFilter(params, task, id)
{
    long n = task->dimX();
    string syffix_filter = "";
    if (m_task->countI > 1) {
        syffix_filter = "лд";
    }
    m_info->setName("ФОС" + syffix_filter + probabilityForView());
    m_info->setType(syffix_filter);
    m_info->setDimension("(p=" + std::to_string(n) + ")");
}

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
            computeBlock1(s, k);
            // Блок 2
            computeBlock2(s, k);
        }

        // Блок 3
        writeResult(k, m_task->countI); // таймер паузится внутри
        // Блок 3а смотри в Блок 2
        // Блок 3б
        timerInstance.interrupt_timer();
        computeBlock3b();
        // Блок 3в
        computeBlock3c();
        timerInstance.continue_timer();

        if (k <= m_result.size()) {

            m_task->setTime(m_result[k+1].time); // Время

            for (size_t s = 0; s < m_params->sampleSize(); ++s) {
                // Блок 4
                computeBlock4(s, k, Xi[s], u[s], T);
                // Блок 5
                computeBlock5(s, k);
            }

            // Блок 6
            timerInstance.interrupt_timer();
            m_sampleI = m_task->generateArrayI(m_params->sampleSize(), k+1);
            computeBlock6(k);
            timerInstance.continue_timer();
        }
    }
}

void FOS::computeBlock3b()
{
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

void FOS::computeBlock3c()
{
    for (size_t s = 0; s < m_params->sampleSize(); s++) {
        if (s == 306) {
            qDebug() << "K";
        }
        Xi[s] = computeProbabilityDensityN(Q, m_sampleZ[s], meanZ, Dzz);
        for (int i = 0; i < m_task->countI; i++) {
            u[s][i] = Gamma[i]*m_sampleZ[s] + kappa[i];
        }
    }
}

} // end LogicDynamic

} // end Filters
