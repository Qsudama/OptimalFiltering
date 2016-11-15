#ifndef FILTER_PARAMETERS_H
#define FILTER_PARAMETERS_H

#include "src/config.h"
#include <cassert>
#include <memory>


namespace Core
{


class FilterParameters
{

public:
    FilterParameters(double maxTime, double measurementStep, double predictionStep, double integrationStep,
                     ulong sampleSize, ulong orderMult = 1);

    const double &maxTime() const;
    const double &measurementStep() const;
    const double &predictionStep() const;
    const double &integrationStep() const;

    const ulong &measurementCount() const;
    const ulong &predictionCount() const;
    const ulong &integrationCount() const;
    const ulong &sampleSize() const;
    const ulong &orderMult() const;

    void setMaxTime(double tmax);
    void setMeasurementStep(double step);
    void setPredictionStep(double step);
    void setIntegrationStep(double step);
    void setSampleSize(ulong size);
    void setOrderMult(ulong order);


private:
    double m_maxTime;
    double m_measurementStep;
    double m_predictionStep;
    double m_integrationStep;

    ulong m_measurementCount;
    ulong m_predictionCount;
    ulong m_integrationCount;
    ulong m_sampleSize;
    ulong m_orderMult;
};


using PtrFilterParameters = std::shared_ptr<FilterParameters>;


} // end Core


#endif // FILTER_PARAMETERS_H
