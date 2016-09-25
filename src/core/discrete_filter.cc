#include "discrete_filter.h"


using Math::Statistic::Mean;
using Math::Statistic::Var;
using Math::Statistic::Cov;
using Math::LinAlg::PinvSVD;


namespace Core
{


DiscreteFilter::DiscreteFilter(PtrFilterParameters params, PtrTask task)
    : Filter(params)
    , m_task(std::dynamic_pointer_cast<DiscreteTask>(task))
{
    m_info->setType("д");
}

void DiscreteFilter::zeroIteration()
{
    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        x[s] = m_task->x0();
        y[s] = m_task->b(x[s]);
    }

    Vector mx0  = Mean(x);
    Vector my0  = Mean(y);
    Matrix Dy0  = Var(y, my0);
    Matrix Dxy0 = Cov(x, y);
    Matrix H0   = Dxy0 * PinvSVD(Dy0);
    Vector e0   = mx0 - H0 * my0;

    for (size_t s = 0; s < m_params->sampleSize(); ++s) {
        z[s] = H0 * y[s] + e0;
        e[s] = x[s] - z[s];
    }

    m_result[0].mx = mx0;
    m_result[0].Dx = Var(x, m_result[0].mx);
    m_result[0].mz = Mean(z);
    m_result[0].me = Mean(e);
    m_result[0].Dz = Var(z, m_result[0].mz);
    m_result[0].De = Var(e, m_result[0].me);
    m_result[0].t  = 0.0;
}


} // end Core
