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
//    Неправильно считает для скалярного примера
//    из-за того что таск 6-ти мерный а надо чтобы показывало 3-х мерный
    long n = task->dimX();
    string syffix_filter = "";
    if (m_task->countI > 1) {
        syffix_filter = "лд";
    }
    m_info->setName("AОФ" + syffix_filter);
    m_info->setType(syffix_filter);
    m_info->setCondition(initialConditWithType());
    m_info->setDimension("(p=" + to_string(n * (n + 3) / 2) + ")");
}

void AOF::algorithm()
{
    m_task->setStep(m_params->measurementStep());

    for (size_t k = 0; k < m_result.size(); ++k) { 
      // qDebug() << "\n\nK = " << k;

        m_task->setTime(m_result[k].time);
//        if (k == 1) {
//            qDebug() << "K = 1";
//        }
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
                computeBlock4(s, k);
                // Блок 5
                computeBlock5(s, k);
            }

            // Блок 6
            timerInstance.interrupt_timer();
            m_sampleI = m_task->generateArrayI(m_params->sampleSize(), k+1);
            computeBlock6();
            timerInstance.continue_timer();
        }
    }
//    LogInFileManager& logInstance = LogInFileManager::Instance();
//    logInstance.logInFileArrayVectors(Lambda, 0, "Lambda", "\t");
}

void AOF::computeBlock1(long s, size_t k) {
    if (k == 1 && s == 125) {
        // все ноги растут отсюда. Очень плохая сигма в последствии дает inf при вычислении Mu
        // Плохая из за очень больших значений Lambda. Нужно смотреть ее подсчет на k = 0
//        qDebug() << "Внимание в блоке 1";
    }
    P[s] = computeProbabilityDensityN(Omega[s], m_sampleY[s], Mu[s], Phi[s]);
    //qDebug() << "s = " << s << " VectorRes = "  << rE[0] << " " << rE[1] << " " << rE[2];
    for (int i = 0; i < m_task->countI; i++) {
        K[s][i] = Delta[s][i]*Pinv(Phi[s][i]);
        Sigma[s][i] = Lambda[s][i] + K[s][i]*(m_sampleY[s] - Mu[s][i]);
        Upsilon[s][i] = Psi[s][i] - K[s][i]*Delta[s][i].transpose();
    }
}

void AOF::computeBlock4(long s, size_t k) {
    if (k == 0 && s == 125) {
//        qDebug() << "Внимание в блоке 4";
    }
    Array<double> resOmega(m_task->countI);
    Array<Vector> resLambda(m_task->countI);
    Array<Matrix> resPsi(m_task->countI);

    for (int l = 0; l < m_task->countI; l++) {
        for (int i = 0; i < m_task->countI; i++) {
            resOmega[i] = P[s][i]*m_task->nu(l+1,i+1, Sigma[s][i], Upsilon[s][i]);
        }
        for (int i = 0; i < m_task->countI; i++) {
            resLambda[i] = P[s][i]*m_task->tau(l+1,i+1, Sigma[s][i], Upsilon[s][i]); // при к = 0 на 125 выборке при i=1 (2-ой режим) беда
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
        if (Omega[s][l] == 0.0) {
//            qDebug() << "Divided 0!!!";
        }
        Lambda[s][l] = sumLambda/Omega[s][l];
        Psi[s][l] = sumPsi/Omega[s][l] - Lambda[s][l]*Lambda[s][l].transpose();
    }
}

void AOF::computeBlock5(long s, size_t k) {
    for (int i = 0; i < m_task->countI; i++) {
        if (k == 0 && s == 125) {
//            qDebug() << "Внимание в блоке 5";
        }
        Mu[s][i] = m_task->h(i+1, Lambda[s][i], Psi[s][i]);
        Delta[s][i] = m_task->G(i+1, Lambda[s][i], Psi[s][i]) - Lambda[s][i] * Mu[s][i].transpose();

        Phi[s][i] =  m_task->F(i+1, Lambda[s][i], Psi[s][i]) - Mu[s][i]*Mu[s][i].transpose();
        Matrix phi = Phi[s][i];
        if (phi.cols() > 1) {
            if (std::isnan(phi(0, 0)) || std::isnan(phi(1, 1))) {
//                qDebug() << "Phi - Nan. k = " << k << "s = " << s;
            }
        }
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
