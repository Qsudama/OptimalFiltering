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
        m_result[i].I.resize(m_params->sampleSize());
        m_result[i].evaluationI.resize(m_params->sampleSize());
        m_result[i].deltaI.resize(m_params->sampleSize());
    }
}

void LogicDynamicFilter::zeroIteration()
{
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

    // Блок 0
    computeZeroVectors();
    computeBlock0();
}

void LogicDynamicFilter::computeZeroVectors()
{
    m_sampleI = m_task->generateArrayI(m_params->sampleSize(), 0);
    for (size_t s = 0; s < m_params->sampleSize(); s++) {
        m_sampleX[s] = m_task->x0();
        m_sampleY[s] = m_task->b(m_sampleI[s], m_sampleX[s]);
    }
}

void LogicDynamicFilter::computeBlock0()
{

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

void LogicDynamicFilter::computeBlock1(long s, size_t /*k*/)
{
    Array<double> det = calculateSqrtDeterminantForProbabilityDensityN(Phi[s]);
    Array<Matrix> pinDs = pinvDForProbabilityDensityN(Phi[s]);
    P[s] = computeProbabilityDensityN(Omega[s], m_sampleY[s], Mu[s], pinDs, det);
    for (int i = 0; i < m_task->countI; i++) {
        K[s][i] = Delta[s][i]*Pinv(Phi[s][i]);
        Sigma[s][i] = Lambda[s][i] + K[s][i]*(m_sampleY[s] - Mu[s][i]);
        Upsilon[s][i] = Psi[s][i] - K[s][i]*Delta[s][i].transpose();
    }
}


void LogicDynamicFilter::computeBlock2(long s, size_t k)
{
    Vector resZ = Vector::Zero(Sigma[s][0].size());
    Vector mult = Vector::Zero(Sigma[s][0].size());
    for (int i = 0; i < m_task->countI; i++) {
        mult = P[s][i]*Sigma[s][i];
        resZ += mult;
    }
    m_sampleZ[s] = resZ;
    m_result[k].I[s] = m_sampleI[s];
    double maxP = 0.0;
    for (int i = 0; i < m_task->countI; i++) {
        if (P[s][i] > maxP) {
            maxP = P[s][i];
            m_result[k].evaluationI[s] = i+1;
        }
    }
    m_result[k].deltaI[s] = abs(m_sampleI[s] - m_result[k].evaluationI[s]);
}

void LogicDynamicFilter::computeBlock4(long s, size_t /*k*/, const Array<double> &p, const Array<Vector> &sigma, const Array<Matrix> &upsilon)
{
    Array<double> resOmega(m_task->countI);
    Array<Vector> resLambda(m_task->countI);
    Array<Matrix> resPsi(m_task->countI);

    for (int l = 0; l < m_task->countI; l++) {
        for (int i = 0; i < m_task->countI; i++) {
            resOmega[i] = p[i]*m_task->nu(l+1, i+1, sigma[i], upsilon[i]);
        }
        for (int i = 0; i < m_task->countI; i++) {
            resLambda[i] = p[i]*m_task->tau(l+1, i+1, sigma[i], upsilon[i]);
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
        Lambda[s][l] = sumLambda/Omega[s][l];
        Psi[s][l] = sumPsi/Omega[s][l] - Lambda[s][l]*Lambda[s][l].transpose();
    }
}

void LogicDynamicFilter::computeBlock5(long s, size_t /*k*/)
{
    for (int i = 0; i < m_task->countI; i++) {
        Mu[s][i] = m_task->h(i+1, Lambda[s][i], Psi[s][i]);
        Delta[s][i] = m_task->G(i+1, Lambda[s][i], Psi[s][i]) - Lambda[s][i] * Mu[s][i].transpose();
        Phi[s][i] =  m_task->F(i+1, Lambda[s][i], Psi[s][i]) - Mu[s][i]*Mu[s][i].transpose();
    }
}

void LogicDynamicFilter::computeBlock6(size_t /*k*/)
{
    for (size_t s = 0; s < m_params->sampleSize(); s++) {
        m_sampleX[s] = m_task->a(m_sampleI[s], m_sampleX[s]);
        m_sampleY[s] = m_task->b(m_sampleI[s], m_sampleX[s]);
    }
}

Array<double> LogicDynamicFilter::computeProbabilityDensityN(const Array<double> &omega, const Vector &u, const Array<Vector> &m, const Array<Matrix> &pinD, const Array<double> &determinants)
{

    Array<double> resP(m_task->countI);

    if (m_task->countI == 1) {
        resP[0] = 1;
    } else {
        Array<double> temp(m_task->countI);

        for (int i = 0; i < m_task->countI; i++) {
            temp[i] = probabilityDensityN(omega[i], u, m[i], determinants[i], pinD[i]);
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

double LogicDynamicFilter::probabilityDensityN(const double &Omega, const Vector &u, const Vector &m, const double &det, const Matrix &pinD)
{
    Vector diff = u - m;
    double temp = (diff.transpose() * pinD * diff / 2)(0, 0);
    if (temp < 0.0) {
        temp = abs(temp);
    }
    double powerE = (-1 * temp);
//    if (powerE > 0.0) {
//        powerE = -1 * powerE;
//    }
    long double resExp = exp(powerE);
    double n = Omega * (resExp / det);
//    if (std::isnan(n)) {
//        qDebug() << "Nan";
//    }
    return n;
}

Array<double> LogicDynamicFilter::calculateSqrtDeterminantForProbabilityDensityN(const Array<Math::Matrix> &D)
{
    Array<double> res(D.size());
    for (size_t i = 0; i < D.size(); i++) {
        double pi = Math::Const::PI;
        Matrix det = 2 * pi * D[i];
        double deter = det.determinant();
        if (deter < 0.0) { // Это тестовый if. В теории его не должно быть.
            deter = abs(deter);
        }
        res[i] = sqrt(deter);
    }
    return res;
}

Array<Matrix> LogicDynamicFilter::pinvDForProbabilityDensityN(const Array<Matrix> &D)
{
    Array<Matrix> res(D.size());
    for (size_t i = 0; i < D.size(); i++) {
        res[i] = Pinv(D[i]);
    }
    return res;
}

string LogicDynamicFilter::probabilityForView()
{
    double e = m_task->params()->at("e");
    QString res = QString("%1").arg(e, 0, 'f', 2);
    return " e =" + res.toStdString() + "; ";
}

} // end Core

