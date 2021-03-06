﻿#include "filter_parameters.h"

#include "src/helpers/alert_helper.h"

namespace Core
{


FilterParameters::FilterParameters(double maxTime, double measurementStep, double predictionStep,
                                   double integrationStep, Uint sampleSize, Uint specificRealization, Uint orderMult, Uint argumentsCount)
    : m_maxTime(maxTime)
    , m_predictionStep(predictionStep)
    , m_integrationStep(integrationStep)
    , m_sampleSize(sampleSize)
    , m_specificRealization(specificRealization)
    , m_orderMult(orderMult)
    , m_argumentsCount(argumentsCount)
{
    setMeasurementStep(measurementStep);

    m_initialCondition = INITIAL_CONDITIONS::GaussApproximation;

    if (sampleSize <= 0) {
        AlertHelper::showErrorAlertWithText("Core::FilterParameters::Constructor\nРазмер выборки <= 0");
        return;
    }
    if (orderMult <= 0) {
        AlertHelper::showErrorAlertWithText("Core::FilterParameters::Constructor\nКратность порядка фильтра <= 0");
        return;
    }
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

const Uint &FilterParameters::measurementCount() const
{
    return m_measurementCount;
}

const Uint &FilterParameters::predictionCount() const
{
    return m_predictionCount;
}

const Uint &FilterParameters::integrationCount() const
{
    return m_integrationCount;
}

const Uint &FilterParameters::sampleSize() const
{
    return m_sampleSize;
}

const Uint &FilterParameters::specificRealization() const
{
    return m_specificRealization;
}

const Uint &FilterParameters::orderMult() const
{
    return m_orderMult;
}

const Uint &FilterParameters::argumentsCount() const
{
    return m_argumentsCount;
}

const Core::INITIAL_CONDITIONS &FilterParameters::initialCondition() const
{
    return m_initialCondition;
}

void FilterParameters::setMaxTime(double tmax)
{
    if (tmax <= 0.0) {
        AlertHelper::showErrorAlertWithText("Core::FilterParameters::setMaxTime\nВремя окончания фильтрации <= 0");
        return;
    }
    m_measurementCount = Uint(tmax / m_measurementStep);
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
    if (step <= 0.0) {
        AlertHelper::showErrorAlertWithText("Core::FilterParameters::setMeasurementStep\nИнтервал между измерениями <= 0");
        return;
    }
    m_measurementStep  = step;
    m_measurementCount = Uint(m_maxTime / m_measurementStep);
    double predictionStep = std::min(m_predictionStep, m_measurementStep);
    setPredictionStep(predictionStep);
}

void FilterParameters::setPredictionStep(double step)
{
    if (step <= 0.0) {
        AlertHelper::showErrorAlertWithText("Core::FilterParameters::setPredictionStep\nИнтервал между прогнозами <= 0");
        return;
    }
    m_predictionStep = step;
    correctStepAndCount(m_measurementStep, m_predictionStep, m_predictionCount);

    if (m_predictionStep > m_measurementStep) {
        AlertHelper::showErrorAlertWithText("Core::FilterParameters::setPredictionStep\nИнтервал между прогзнозами > интервал между измерениями");
        return;
    }
    setMaxTime(m_maxTime);
    setIntegrationStep(m_integrationStep);
}

void FilterParameters::setIntegrationStep(double step)
{
    if (step <= 0.0) {
        AlertHelper::showErrorAlertWithText("Core::FilterParameters::setIntegrationStep\nШаг интегрирования <= 0");
        return;
    }
    m_integrationStep = step;
    correctStepAndCount(m_predictionStep, m_integrationStep, m_integrationCount);

    if (m_integrationStep > m_predictionStep) {
        AlertHelper::showErrorAlertWithText("Core::FilterParameters::setIntegrationStep\nШаг интегрирования > интервал между прогзнозами");
        return;
    }
}

void FilterParameters::setSampleSize(Uint size)
{
    if (size <= 0.0) {
        AlertHelper::showErrorAlertWithText("Core::FilterParameters::setSampleSize\nРазмер выборки <= 0");
        return;
    }
    m_sampleSize = size;
}

bool FilterParameters::setSpecificRealization(Uint realization)
{
    if (realization > m_sampleSize) {
        AlertHelper::showErrorAlertWithText("Core::FilterParameters::setSpecificRealization\nНомер выводимой реализации не должен быть больше количества реализаций");
        return false;
    }
    m_specificRealization = realization;
    return true;
}


void FilterParameters::setOrderMult(Uint order)
{
    if (order <= 0.0) {
        AlertHelper::showErrorAlertWithText("Core::FilterParameters::setOrderMult\nКратность порядка фильтра <= 0");
        return;
    }
    m_orderMult = order;
}

void FilterParameters::setArgumentsCount(Uint order)
{
    if (order <= 0.0) {
        AlertHelper::showErrorAlertWithText("Core::FilterParameters::setArgumentsCount\nКоличество аргументов в фильтре <= 0");
        return;
    }
    m_argumentsCount = order;
}

void FilterParameters::setInitialCondition(Core::INITIAL_CONDITIONS condition)
{
    m_initialCondition = condition;
}

void FilterParameters::correctStepAndCount(const double &intervalLength, double &step, Uint &count)
{
    count = Uint(intervalLength / step);
    if (intervalLength - count * step >= 0.5 * step) {
        ++count;
    }
    step = intervalLength / count;
}


} // end Core
