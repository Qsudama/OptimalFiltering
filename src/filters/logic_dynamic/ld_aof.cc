#include "ld_aof.h"

#include "iostream"
#include <limits>
#include <cmath>

using namespace Core;

namespace Filters
{

namespace LogicDynamic
{


using Math::LinAlg::Pinv;
using Math::Statistic::Mean;
using Math::Statistic::Var;
using Math::Statistic::Cov;

AOF::AOF(Core::PtrFilterParameters params, Core::PtrTask task, FILTER_ID id)
    : LogicDynamicFilter(params, task, id)
{
    long n = task->dimX();
    string syffix_filter = "";
    if (m_task->countI > 1) {
        syffix_filter = "лд";
    }

    m_info->setName("AОФ" + syffix_filter + probabilityForView());
    m_info->setType(syffix_filter);
    m_info->setDimension("(p=" + to_string(n * (n + 3) / 2) + ")");
}

void AOF::algorithm()
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

        if (k <= m_result.size()) {

            m_task->setTime(m_result[k+1].time); // Время

            for (size_t s = 0; s < m_params->sampleSize(); ++s) {
                // Блок 4
                computeBlock4(s, k, P[s], Sigma[s], Upsilon[s]);
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
//    LogInFileManager& logInstance = LogInFileManager::Instance();
//    logInstance.logInFileArrayVectors(Lambda, 0, "Lambda", "\t");
}

} // end LogicDynamic

} // end Filters
