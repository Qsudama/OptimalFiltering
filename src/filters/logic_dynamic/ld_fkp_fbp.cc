#include "ld_fkp_fbp.h"
#include "src/math/statistic.h"


namespace Filters
{

namespace LogicDynamic
{

using Math::LinAlg::Pinv;
using Math::Statistic::Mean;
using Math::Statistic::Var;
using Math::Statistic::Cov;


FKP_FBP::FKP_FBP(Core::PtrFilterParameters params, Core::PtrTask task, FILTER_ID id)
    : LogicDynamicFilter(params, task, id)
{
    string filter_identifier = "";
    string syffix_filter = "";
    if (m_task->countI > 1) {
        syffix_filter = "лд";
    }
    long ny;
    if (m_identifier == FILTER_ID::LDFBP) {
        ny = long(m_task->dimX());
        filter_identifier = "ФБП";
    } else {
        ny = long(m_task->dimY());
        filter_identifier = "ФКП";
    }

    long p  = ny * long(m_params->orderMult());

    m_info->setName(filter_identifier + syffix_filter);
    m_info->setType(syffix_filter);
    m_info->setCondition(initialConditWithType());
    m_info->setDimension("(p=" + to_string(p) + ")");
}

void FKP_FBP::zeroIteration()
{
    LogicDynamicFilter::zeroIteration();

    Xi.resize(m_params->sampleSize());
    u.resize(m_params->sampleSize());

    Q.resize(m_task->countI);
    kappa.resize(m_task->countI);
    T.resize(m_task->countI);
    meanS.resize(m_task->countI);
    Gamma.resize(m_task->countI);
    Dzz.resize(m_task->countI);

    m_sampleS.resize(m_params->sampleSize()); // размерность векторов меняется динамически в процессе

    for (size_t s = 0; s < m_params->sampleSize(); s++) {
        Xi[s].resize(m_task->countI);
        u[s].resize(m_task->countI);
        if (m_identifier == FILTER_ID::LDFBP) {
            m_sampleS[s] = Vector::Zero(m_task->dimX());
        } else {
            m_sampleS[s] = Vector::Zero(m_task->dimY());
        }
    }
}

void FKP_FBP::algorithm()
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
        // Блок 3а
        computeBlock3a(k);
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

void FKP_FBP::computeBlock3a(long k) {
    long m;
    if (m_identifier == FILTER_ID::LDFBP) {
        m = long(m_task->dimX());
    } else {
        m = long(m_task->dimY());
    }
    long l = long(m_params->orderMult());
    long p = l*m;
    if (k <= l-1) {
        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
            int sizeS = m_sampleS[s].size();
            if (k == 0) {
                sizeS = 0;
            }
            Vector def = Vector(m_sampleS[s]);
            m_sampleS[s] = Vector::Zero(sizeS+m);
            for(long i = 0; i < sizeS+m; i++) {
                if (i < m) {
                    if (m_identifier == FILTER_ID::LDFBP) {
                        m_sampleS[s][i] = m_sampleZ[s][i];
                    } else if (m_identifier == FILTER_ID::LDFKP) {
                        m_sampleS[s][i] = m_sampleY[s][i];
                    }
                } else {
                    m_sampleS[s][i] = def[i-m];
                }
            }
        }
    } else {
        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
            for (long i = p - 1; i >= m; i--) {
                m_sampleS[s][i] = m_sampleS[s][i - m];
            }
            for (long i = 0; i < long(m); i++) {
                if (m_identifier == FILTER_ID::LDFBP) {
                    m_sampleS[s][i] = m_sampleZ[s][i];
                } else if (m_identifier == FILTER_ID::LDFKP) {
                    m_sampleS[s][i] = m_sampleY[s][i];
                }
            }
        }
    }
}

void FKP_FBP::computeBlock3b() {
    Array<Vector> meanX(m_task->countI);
    Array<Matrix> Dxx(m_task->countI);
    Array<Matrix> Dxz(m_task->countI);

    for (int i = 0; i < m_task->countI; i++) {
        Q[i] = m_task->Pr(i+1);
        meanX[i] = Mean(m_sampleX, m_sampleI, i+1);
        meanS[i] = Mean(m_sampleS, m_sampleI, i+1);
        Dxx[i] = Cov(m_sampleX, m_sampleX, m_sampleI, i+1);
        Dxz[i] = Cov(m_sampleX, m_sampleS, m_sampleI, i+1);
        Dzz[i] = Cov(m_sampleS, m_sampleS, m_sampleI, i+1);

        Gamma[i] = Dxz[i] * Pinv(Dzz[i]);
        kappa[i] = meanX[i] - Gamma[i]*meanS[i];
        T[i] = Dxx[i] - Gamma[i]*Dxz[i].transpose();
    }
}

void FKP_FBP::computeBlock3c() {
    for (size_t s = 0; s < m_params->sampleSize(); s++) {
//        Xi[s] = computeProbabilityDensityN(Q, m_sampleS[s], meanS, Dzz);
        Xi[s] = test(Q, m_sampleS[s], meanS, Dzz);
        for (int i = 0; i < m_task->countI; i++) {
            u[s][i] = Gamma[i]*m_sampleS[s] + kappa[i];
        }
    }
}

} // end LogicDynamic

} // end Filters

