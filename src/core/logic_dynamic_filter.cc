#include "logic_dynamic_filter.h"

using Math::Statistic::Mean;
using Math::Statistic::Var;
using Math::Statistic::Cov;
using Math::LinAlg::Pinv;


namespace Core
{


LogicDynamicFilter::LogicDynamicFilter(PtrFilterParameters params, PtrTask task)
    : Filter(params)
    , m_task(std::dynamic_pointer_cast<LogicDynamicTask>(task))
{
    m_info->setType("лд");
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

void LogicDynamicFilter::zeroIteration() {}

double LogicDynamicFilter::probabilityDensityN(const Vector &u, const Vector &m, const Matrix &D) {
    double pi = Math::Const::PI;
    Matrix det = 2* pi * D;
    double n = exp((-1 * (u - m).transpose() * D * (u - m))(0, 0)) / sqrt(det.determinant());
    return n;
}

} // end Core

