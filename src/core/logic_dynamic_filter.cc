#include "logic_dynamic_filter.h"

using Math::Statistic::Mean;
using Math::Statistic::Var;
using Math::Statistic::Cov;
using Math::LinAlg::Pinv;


namespace Core
{


LogicDynamicFilter::LogicDynamicFilter(PtrFilterParameters params, PtrTask task, FILTER_ID id)
    : Filter(params)
    , m_task(std::dynamic_pointer_cast<LogicDynamicTask>(task))
    , m_identifier(id)
{
    m_info->setType("лд");
}

double LogicDynamicFilter::execute_time_filter()
{
    double result = timerInstance.result_execute_time / (m_params->measurementCount() * m_params->sampleSize());
    return result;
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
    m_sampleI = m_task->generateArrayI(m_params->sampleSize(), 0);
    for (size_t s = 0; s < m_params->sampleSize(); s++) {
        m_sampleX[s] = m_task->x0();
        m_sampleY[s] = m_task->b(m_sampleI[s], m_sampleX[s]);
    }
}

void LogicDynamicFilter::computeBlock0() {

    Array<Vector> mx(m_task->countI);
    Array<Matrix> varX(m_task->countI);
    Array<Vector> my(m_task->countI);
    Array<Matrix> varY(m_task->countI);
    Array<Matrix> covarXY(m_task->countI);

    for (int i = 0; i < m_task->countI; i++) {
        mx[i] = Mean(m_sampleX, m_sampleI, i+1);
        varX[i] = Var(m_sampleX, mx[i],  m_sampleI, i+1);
        if (m_params->initialCondition() == INITIAL_CONDITIONS::MonteCarlo) {
            my[i] = Mean(m_sampleY, m_sampleI, i+1);
            varY[i] = Var(m_sampleY, my[i], m_sampleI, i+1);
            covarXY[i] = Cov(m_sampleX, m_sampleY, mx[i], my[i], m_sampleI, i+1);
        }
    }

    for (size_t s = 0; s < m_params->sampleSize(); s++) {
        for (int i = 0; i < m_task->countI; i++) {
            if (m_params->initialCondition() == INITIAL_CONDITIONS::GaussApproximation) {
                Omega[s][i] = m_task->Pr(i+1);
                Lambda[s][i] = m_task->meanX0();
                Psi[s][i] = m_task->varX0();
                Mu[s][i] = m_task->h(i+1, mx[i], varX[i]);
                Delta[s][i] = m_task->G(i+1, mx[i], varX[i]) - Lambda[s][i] * Mu[s][i].transpose();
                Phi[s][i] =  m_task->F(i+1, mx[i], varX[i]) - Mu[s][i]*Mu[s][i].transpose();
            } else { // Монте-Карло
                Omega[s][i] = m_task->Pr(i+1);
                Lambda[s][i] = mx[i];
                Psi[s][i] = varX[i];
                Mu[s][i] = my[i];
                Delta[s][i] = covarXY[i];
                Phi[s][i] =  varY[i];
            }
        }
    }
}

double LogicDynamicFilter::probabilityDensityN(const double &Omega, const Vector &u, const Vector &m, const Matrix &D) {
    double pi = Math::Const::PI;
    Matrix det = 2* pi * D;
    double deter = det.determinant();
    double d = sqrt(deter);
    Matrix pin = Pinv(D);
    double powerE = ((-1 * (u - m).transpose()) * pin * (u - m))(0, 0);
    double resExp = exp(powerE/2);
    double n = (Omega * resExp) / d;
    return n;
}

double LogicDynamicFilter::calculate_d(const Matrix &D) {
    double pi = Math::Const::PI;
    Matrix det = 2* pi * D;
    double deter = det.determinant();
    if (deter <= 0.0000000001) {
        m_bad = true;
    }
    if (deter < 0.0) {
        deter = abs(deter);
    }
    double d = sqrt(deter);
    return d;
}

double LogicDynamicFilter::calculate_e(const double &Omega, const Vector &u, const Vector &m, const Matrix &D) {
    Matrix pinD = Pinv(D);
    double powerE = (((u - m).transpose()) * pinD * (u - m))(0, 0);
    if (powerE < 0.0) {
        powerE = abs(powerE);
    }
    double resExp = exp((-1 * powerE)/2);
    double res = Omega * resExp;
    return res;
}

Array<double> LogicDynamicFilter::computeProbabilityDensityN(Array<double> omega, Vector sampleVector,
                                                             Array<Vector> m, Array<Matrix> D) {
    Array<double> resP(m_task->countI);

    m_bad = false;

    if (m_task->countI == 1 ) {
        resP[0] = 1;
    } else {
        Array<double> q(m_task->countI);
        double sumQ = 0;
        Array<double> e(m_task->countI);
        Array<double> d(m_task->countI);

        for (int i = 0; i < m_task->countI; i++) {
            q[i] = 1;
            e[i] = calculate_e(omega[i], sampleVector, m[i], D[i]);
            d[i] = calculate_d(D[i]);
            for (int j = 0; j < m_task->countI; j++) {
                if (j == i) {
                    q[i] = q[i]*e[i];
                } else {
                    q[i] = q[i]*d[i];
                }
            }
            sumQ = sumQ + q[i];
        }

        double SumP = 0;
        for (int i = 0; i < m_task->countI; i++) {
            if (sumQ <= 0.000000000000001) {
                resP[i] = omega[i];
            } else if (std::isnan(sumQ)) {
                resP[i] = omega[i];
            } else if (m_bad) {
                resP[i] = omega[i];
            } else {
                double res = q[i] / sumQ;
                resP[i] = res;
                SumP = SumP + resP[i];
            }

            if (std::isnan(resP[i])) {
//                qDebug() << "Nan!";
            }
        }
    }

//    if (m_task->countI == 1 ) {
//        resDouble = omega;
//    } else {
//        for (int i = 0; i < m_task->countI; i++) {
//            resP[i] = probabilityDensityN(omega[i], sampleVector, m[i], D[i]);
//        }
//        double resNumerator = 0.0;
//        for (int i = 0; i < m_task->countI; i++) {
//            resNumerator += resP[i];
//        }
//        for (int i = 0; i < m_task->countI; i++) {
//            if(resNumerator == 0.0) {
//                resDouble[i] = resP[i];
//            } else {
//                resDouble[i] = resP[i]/resNumerator;
//            }
//        }
//    }
//    return resDouble;
    return resP;
}

string LogicDynamicFilter::initialConditWithType()
{
    string condit = "";
    if (m_params->initialCondition() == INITIAL_CONDITIONS::GaussApproximation) {
            condit = ", н.у.-прибл";
    } else {
            condit = ", н.у.-точн";
    }
    return condit;
}

} // end Core

