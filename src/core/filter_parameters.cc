#include "filter_parameters.h"


namespace Core
{


FilterParameters::FilterParameters(double maxTime, double measurementStep, double predictionStep,
                                   double integrationStep, ulong sampleSize, ulong orderMult)
    : m_maxTime(maxTime)
    , m_predictionStep(predictionStep)
    , m_integrationStep(integrationStep)
    , m_sampleSize(sampleSize)
    , m_orderMult(orderMult)
{
    setMeasurementStep(measurementStep);

    assert(sampleSize > 0 && "Core::FilterParameters::Constructor : corrupt value of sampleSize");
    assert(orderMult > 0 && "Core::FilterParameters::Constructor : corrupt value of orderMult");
}

const double &FilterParameters::maxTime() const
{
    return m_maxTime;
}

const double &FilterParameters::measurementStep() const
{
    return m_measurementStep;
}

const double &FilterParameters::predictionStep() const
{
    return m_predictionStep;
}

const double &FilterParameters::integrationStep() const
{
    return m_integrationStep;
}

const ulong &FilterParameters::measurementCount() const
{
    return m_measurementCount;
}

const ulong &FilterParameters::predictionCount() const
{
    return m_predictionCount;
}

const ulong &FilterParameters::integrationCount() const
{
    return m_integrationCount;
}

const ulong &FilterParameters::sampleSize() const
{
    return m_sampleSize;
}

const ulong &FilterParameters::orderMult() const
{
    return m_orderMult;
}

void FilterParameters::setMaxTime(double tmax)
{
    assert(tmax > 0.0 && "Core::FilterParameters::setMaxTime(tmax) : corrupt value of tmax");

    m_measurementCount = ulong(tmax / m_measurementStep);
    double T1          = m_measurementCount * m_measurementStep;
    if (tmax - T1 >= 0.5 * m_measurementStep) {
        m_maxTime = T1 + m_measurementStep;
        m_measurementCount++;
    } else {
        m_maxTime = T1;
    }
}

void FilterParameters::setMeasurementStep(double step)
{
    assert(step > 0.0 && "Core::FilterParameters::setMeasurementStep(step) : corrupt value of step");

    m_measurementStep  = step;
    m_measurementCount = ulong(m_maxTime / m_measurementStep);
    setPredictionStep(m_predictionStep);
}

void FilterParameters::setPredictionStep(double step)
{
    assert(step > 0.0 && "Core::FilterParameters::setPredictionStep(step) : corrupt value of step");

    m_predictionStep = step;
    correctStepAndCount(m_measurementStep, m_predictionStep, m_predictionCount);

    assert(m_predictionStep <= m_measurementStep &&
           "Core::FilterParameters::setPredictionStep(step) : logic error (prediction step > measurement step)");

    setMaxTime(m_maxTime);
    setIntegrationStep(m_integrationStep);
}

void FilterParameters::setIntegrationStep(double step)
{
    assert(step > 0.0 && "Core::FilterParameters::setIntegrationStep(step) : corrupt value of step");

    m_integrationStep = step;
    correctStepAndCount(m_predictionStep, m_integrationStep, m_integrationCount);

    assert(m_integrationStep <= m_predictionStep &&
           "Core::FilterParameters::setIntegrationStep(step) : logic error (integration step > prediction step)");
}

void FilterParameters::setSampleSize(ulong size)
{
    assert(size > 0 && "Core::FilterParameters::setSampleSize(size) : corrupt value of size");

    m_sampleSize = size;
}

void FilterParameters::setOrderMult(ulong order)
{
    assert(order > 0 && "Core::FilterParameters::setOrderMult(order) : corrupt value of order");

    m_orderMult = order;
}

void FilterParameters::correctStepAndCount(const double &intervalLength, double &step, ulong &count)
{
    count = ulong(intervalLength / step);
    if (intervalLength - count * step >= 0.5 * step) {
        ++count;
    }
    step = intervalLength / count;
}


} // end Core
