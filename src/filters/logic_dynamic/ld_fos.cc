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
using Math::MakeBlockVector;
using Math::MakeBlockMatrix;


FOS::FOS(Core::PtrFilterParameters params, Core::PtrTask task)
    : LogicDynamicFilter(params, task)
{
    m_info->setName(m_task->info()->type() + "ФМПлд (p=" + std::to_string(task->dimX()) + ")");
}

void FOS::algorithm()
{

//    Xi.resize(m_task->countI);
//    Q.resize(m_task->countI);
//    kappa.resize(m_task->countI);
//    T.resize(m_task->countI);
//    u.resize(m_task->countI);
//    meanX.resize(m_task->countI);
//    meanZ.resize(m_task->countI);
//    Gamma.resize(m_task->countI);
//    Dzz.resize(m_task->countI);
//    Dxx.resize(m_task->countI);
//    Dxz.resize(m_task->countI);

//    m_task->setStep(m_params->measurementStep());

//    // Индекс k соответствует моменту времени tk = t0 + k * delta_t  (delta_t - интервал между измерениями):
//    for (size_t k = 1; k < m_result.size(); ++k) {

//        // X_k = a(X_{k-1}); время t = t_{k-1}
//        m_task->setTime(m_result[k - 1].time);

//        m_sampleI = m_task->generateArrayI(m_params->sampleSize());

//        // Индекс s пробегает по всем элементам выборки:
//        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
//            m_sampleX[s] = m_task->a(m_sampleI[s], m_sampleX[s]);
//            m_sampleY[s] = m_task->b(m_sampleI[s], m_sampleX[s]);
//        }

//        for (int i = 0; i < m_task->countI; i++) {
//            Mu[i] = m_task->h(i+1, Lambda[i], Psi[i]);
//            Delta[i] = m_task->G(i+1, Lambda[i], Psi[i]) - Lambda[i] * Mu[i].transpose();
//            Phi[i] =  m_task->F(i+1, Lambda[i], Psi[i]) - Mu[i]*Mu[i].transpose();

//            K[i] = Delta[i]*Pinv(Phi[i]);
//            Upsilon[i] = Psi[i] - K[i]*Delta[i].transpose();
//        }
//        for (size_t s = 0; s < m_params->sampleSize(); ++s) {

////            Matrix resUpsilon = Matrix::Zero(Upsilon[0].rows(), Upsilon[0].cols());

//            Array<double> resP(m_task->countI);
//            for (int i = 0; i < m_task->countI; i++) {
//                double N = probabilityDensityN(m_sampleY[s], Mu[i], Phi[i]);
//                resP[i] = Omega[i]*N;
//            }
//            double resNumerator = 0.0;
//            for (int i = 0; i < m_task->countI; i++) {
//                resNumerator += resP[i];
//            }
//            for (int i = 0; i < m_task->countI; i++) {
//                P[i] = resP[i]/resNumerator;
//            }

//            for (int i = 0; i < m_task->countI; i++) {
//                Sigma[i] = Lambda[i] + K[i]*(m_sampleY[s] - Mu[i]);
//            }

//            Vector resZ = Vector::Zero(m_sampleZ[s].size());
//            for (int i = 0; i < m_task->countI; i++) {
//                resZ += P[i]*Sigma[i];
//            }
//            m_sampleZ[s] = resZ;
////            resUpsilon += Upsilon[i];
////            m_sampleP[s] = resUpsilon;
//        }

//        for (int i = 0; i < m_task->countI; i++) {

//            meanX[i] = Mean(m_sampleX, m_sampleI, i+1);
//            meanZ[i] = Mean(m_sampleZ, m_sampleI, i+1);
//            Dxx[i] = Cov(m_sampleX, m_sampleX, m_sampleI, i+1);
//            Dzz[i] = Cov(m_sampleZ, m_sampleZ, m_sampleI, i+1);
//            Dxz[i] = Cov(m_sampleX, m_sampleZ, m_sampleI, i+1);

//            Q[i] = m_task->Pr(i+1);

//            Gamma[i] = Dxz[i]*Pinv(Dzz[i]);
//            kappa[i] = meanX[i] - Gamma[i] * meanZ[i];
//            Matrix rezPinv = Pinv(Dxz[i]);
//            T[i] = Dxx[i] - Gamma[i] * rezPinv.transpose();
//        }
//        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
//            Array<double> resXi(m_task->countI);
//            for (int i = 0; i < m_task->countI; i++) {
//                double N = probabilityDensityN(m_sampleZ[s], meanZ[i], Dzz[i]);
//                resXi[i] = Q[i]*N;
//            }
//            double resNumerator = 0.0;
//            for (int i = 0; i < m_task->countI; i++) {
//                resNumerator += resXi[i];
//            }
//            for (int i = 0; i < m_task->countI; i++) {
//                Xi[i] = resXi[i]/resNumerator;
//            }
//        }
//        //ставим текущее (t = tk) время и продолжаем:
//        m_task->setTime(m_result[k].time);

//        Array<double> resOmega(m_task->countI);
//        Array<Vector> resLambda(m_task->countI);
//        Array<Matrix> resPsi(m_task->countI);

//        for (int i = 0; i < m_task->countI; i++) {
//            for (int l = 0; l < m_task->countI; l++) {
//                resOmega[l] = Xi[i]*m_task->nu(i+1,l+1, u[i], T[i]);
//            }
//            for (int l = 0; l < m_task->countI; l++) {
//                resLambda[l] = Xi[i]*m_task->tau(i+1,l+1, u[i], T[i]);
//            }
//            for (int l = 0; l < m_task->countI; l++) {
//                resPsi[l] = Xi[i]*m_task->Theta(i+1,l+1, u[i], T[i]);
//            }
//            double sumOmega = 0.0;
//            Vector sumLambda = Vector::Zero(Lambda[i].size());
//            Matrix sumPsi = Matrix::Zero(Psi[i].rows(), Psi[i].cols());
//            for (int l = 0; l < m_task->countI; l++) {
//                sumOmega += resOmega[l];
//                sumLambda += resLambda[l];
//                sumPsi += resPsi[l];
//            }
//            Omega[i] = sumOmega;
//            Lambda[i] = sumLambda/Omega[i];
//            Psi[i] = sumPsi/Omega[i] - Lambda[i]*Lambda[i].transpose();
//        }

//        writeResult(k, m_task->countI, false);
//    }
}

void FOS::computeParams(size_t k, Array<Vector> &u, Matrix &T)
{
    Vector        my, chi;
    Matrix        Gamma, GammaY, GammaZ, DxyDxz, Ddelta, Dxy, Dxz;
    Array<Vector> sampleDelta(m_params->sampleSize());

    // Индекс s пробегает по всем элементам выборки:
    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        MakeBlockVector(m_sampleY[s], m_sampleZ[s], sampleDelta[s]);
    }

    Ddelta = Var(sampleDelta);
    my     = Mean(m_sampleY);
    Dxy    = Cov(m_sampleX, m_sampleY);
    Dxz    = Cov(m_sampleX, m_sampleZ);
    MakeBlockMatrix(Dxy, Dxz, DxyDxz);

    Gamma  = DxyDxz * Pinv(Ddelta);
    GammaY = Gamma.leftCols(m_task->dimY());
    GammaZ = Gamma.rightCols(m_task->dimX()); // dimZ = dimX

    chi = m_result[k].meanX - GammaY * my - GammaZ * m_result[k].meanZ;
    T   = m_result[k].varX - GammaY * Dxy.transpose() - GammaZ * Dxz.transpose();

    // Индекс s пробегает по всем элементам выборки:
    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        u[s] = GammaY * m_sampleY[s] + GammaZ * m_sampleZ[s] + chi;
    }
}


} // end LogicDynamic

} // end Filters
