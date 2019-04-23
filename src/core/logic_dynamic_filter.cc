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

FilterTimeResult LogicDynamicFilter::execute_time_filter()
{
    double coef = m_params->measurementCount() * m_params->sampleSize();
    string filter_name = m_info->name() + m_info->dimension();
    return timerInstance.result_execute_time(filter_name, coef);
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

void LogicDynamicFilter::computeBlock1(long s, size_t k) {
    if (k == 1 && s == 125) {
        // все ноги растут отсюда. Очень плохая сигма в последствии дает inf при вычислении Mu
        // Плохая из за очень больших значений Lambda. Нужно смотреть ее подсчет на k = 0
//        qDebug() << "Внимание в блоке 1";
    }
//    P[s] = computeProbabilityDensityN(Omega[s], m_sampleY[s], Mu[s], Phi[s]);
    P[s] = test(Omega[s], m_sampleY[s], Mu[s], Phi[s]);

    //qDebug() << "s = " << s << " VectorRes = "  << rE[0] << " " << rE[1] << " " << rE[2];
    for (int i = 0; i < m_task->countI; i++) {
        K[s][i] = Delta[s][i]*Pinv(Phi[s][i]);
        Sigma[s][i] = Lambda[s][i] + K[s][i]*(m_sampleY[s] - Mu[s][i]);
        Upsilon[s][i] = Psi[s][i] - K[s][i]*Delta[s][i].transpose();
    }
}


void LogicDynamicFilter::computeBlock2(long s, size_t /*k*/) {
    Vector resZ = Vector::Zero(Sigma[s][0].size());
    Vector mult = Vector::Zero(Sigma[s][0].size());
    for (int i = 0; i < m_task->countI; i++) {
        mult = P[s][i]*Sigma[s][i];
        resZ += mult;
    }
    if (std::isnan(resZ[0])) {
//        qDebug() << "Nan! s = " << s << "k = " << k;
    }
    m_sampleZ[s] = resZ;
}

void LogicDynamicFilter::computeBlock4(long s, size_t k, Array<double> p, Array<Vector> sigma, Array<Matrix> upsilon) {
    if (k == 0 && s == 125) {
//        qDebug() << "Внимание в блоке 4";
    }
    Array<double> resOmega(m_task->countI);
    Array<Vector> resLambda(m_task->countI);
    Array<Matrix> resPsi(m_task->countI);

    for (int l = 0; l < m_task->countI; l++) {
        for (int i = 0; i < m_task->countI; i++) {
            resOmega[i] = p[i]*m_task->nu(l+1, i+1, sigma[i], upsilon[i]);
        }
        for (int i = 0; i < m_task->countI; i++) {
            resLambda[i] = p[i]*m_task->tau(l+1, i+1, sigma[i], upsilon[i]); // при к = 0 на 125 выборке при i=1 (2-ой режим) беда
        }
        for (int i = 0; i < m_task->countI; i++) {
            resPsi[i] = p[i]*m_task->Theta(l+1, i+1, sigma[i], upsilon[i]);
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

void LogicDynamicFilter::computeBlock5(long s, size_t k) {
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

void LogicDynamicFilter::computeBlock6(size_t /*k*/) {
    for (size_t s = 0; s < m_params->sampleSize(); s++) {
        m_sampleX[s] = m_task->a(m_sampleI[s], m_sampleX[s]);
        m_sampleY[s] = m_task->b(m_sampleI[s], m_sampleX[s]);
    }
}

double LogicDynamicFilter::calculate_d(const Matrix &D) {
    double pi = Math::Const::PI;
    Matrix det = 2 * pi * D;
    double deter = det.determinant();
//    if (deter <= 0.0000000001) {
//        m_bad = true;
//    }
    if (deter < 0.0) {
        deter = abs(deter);
    }
    double d = sqrt(deter);
    return d;
}

double LogicDynamicFilter::calculate_e(const double &Omega, const Vector &u, const Vector &m, const Matrix &D) {
    Matrix pinD = Pinv(D);
    Vector diff = u - m;
    double powerE = ((diff.transpose()) * pinD * diff)(0, 0);
    if (powerE < 0.0) {
        powerE = abs(powerE);
    }
    double resExp = exp((-1 * powerE)/2);
    double res = Omega * resExp;
    return res;
}

double LogicDynamicFilter::probabilityDensityN(double Omega, Vector u, Vector m, Matrix D) {
    double pi = Math::Const::PI;
    Matrix det = 2 * pi * D;
    double deter = det.determinant();
    if (deter < 0.0) {
        deter = abs(deter);
    }
    double d = sqrt(deter);
    Matrix pin = Pinv(D);
    Vector diff = u - m;
    double powerE = (-1 * diff.transpose() * pin * diff / 2)(0, 0);
    double resExp = exp(powerE);
    double n = Omega * (resExp / d);
    return n;
}


Array<double> LogicDynamicFilter::test(Array<double> omega, Vector u, Array<Vector> m, Array<Matrix> D) {

    Array<double> resP(m_task->countI);

    if (m_task->countI == 1 ) {
        resP[0] = 1;
    } else {
        Array<double> temp(m_task->countI);

        for (int i = 0; i < m_task->countI; i++) {
            temp[i] = probabilityDensityN(omega[i], u, m[i], D[i]);
        }
        double sum = 0.0;
        for (int i = 0; i < m_task->countI; i++) {
            sum = sum + temp[i];
        }
        for (int i = 0; i < m_task->countI; i++) {
            double tempP = temp[i];
            resP[i] = tempP/sum;
        }

    }
    return resP;
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

