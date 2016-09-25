#include "d_fos.h"
#include "src/math/statistic.h"

#include <QDebug>
namespace Filters
{

namespace Discrete
{

using Math::Rand::gaussianVector;
using Math::LinAlg::PinvSVD;
using Math::Statistic::Mean;
using Math::Statistic::Var;
using Math::Statistic::Cov;
using Math::MakeBlockVector;
using Math::MakeBlockMatrix;


FOS::FOS(Core::PtrFilterParameters params, Core::PtrTask task)
    : DiscreteFilter(params, task)
{
    long n = task->dimX();
    long m = task->dimY();
    m_info->setName(m_task->info()->type() + "ФОСд (" + std::to_string(n * (n + m) + n + n * (n + 1) / 2) + ")");
}

void FOS::algorithm()
{
    Matrix        Gamma, DxyDxz, Ddelta, Dxy, Dxz;
    Vector        my;
    Array<Vector> delta(m_params->sampleSize());

    for (size_t n = 1; n < m_result.size(); ++n) { // tn = t0 + n * dt
        //интегрируем систему (получаем mx, Dx с шагам dt):
        m_task->setTime(m_result[n - 1].t);
        m_task->setStep(m_params->integrationStep());
        for (size_t s = 0; s < m_params->sampleSize(); ++s) {
            x[s] = m_task->a(x[s]);
        }
        writeResult(n, true);

        if (n % (m_params->predictionCount() * m_params->integrationCount()) == 0) {
            for (size_t s = 0; s < m_params->sampleSize(); ++s) {
                y[s] = m_task->b(x[s]);
                MakeBlockVector(y[s], z[s], delta[s]);
            }
            my     = Mean(y);
            Ddelta = Var(delta);
            Dxy    = Cov(x, y);
            Dxz    = Cov(x, z);
            MakeBlockMatrix(Dxy, Dxz, DxyDxz);

            for (size_t s = 0; s < m_params->sampleSize(); ++s) {
                Gamma  = DxyDxz * PinvSVD(Ddelta);
                GammaY = Gamma.leftCols(y[s].size());
                GammaZ = Gamma.rightCols(z[s].size());
                ksi    = m_result[n].mx - GammaY * my - GammaZ * m_result[n].mz;
                T      = m_result[n].Dx - GammaY * Dxy.transpose() - GammaZ * Dxz.transpose();
                u      = GammaY * y[s] + GammaZ * z[s] + ksi;

                //вычисляем lambda, Psy: время устонавливаем в предыдущий момент измерения:
                double timeCurrent = m_task->time();
                double timePred    = timeCurrent - m_params->measurementStep();
                m_task->setStep(m_params->measurementStep());
                m_task->setTime(timePred);
                lambda = m_task->tau(u, T);
                Psy    = m_task->Theta(u, T);

                //ставим время обратно и продолжаем:
                m_task->setTime(timeCurrent);
                h    = m_task->h(lambda, Psy);
                G    = m_task->G(lambda, Psy);
                F    = m_task->F(lambda, Psy);
                z[s] = lambda + Psy * G.transpose() * PinvSVD(F) * (y[s] - h);
            }
            writeResult(n);
        }
    }
}


} // end Discrete

} // end Filters
