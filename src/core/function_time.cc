#include "function_time.h"


namespace Core
{


FunctionTime::FunctionTime(double initialTime, double initialStep)
    : logInstance(LogInConsoleManager::Instance())
    , m_time(initialTime)
    , m_step(initialStep)
{
}

FunctionTime::~FunctionTime()
{
}

double FunctionTime::time() const
{
    return m_time;
}

double FunctionTime::step() const
{
    return m_step;
}

void FunctionTime::setTime(double newTime)
{
    m_time = newTime;
}

void FunctionTime::setStep(double newStep)
{
    m_step = newStep;
}

void FunctionTime::advanceTime(double step)
{
    setTime(m_time + step);
}

void FunctionTime::advanceTime()
{
    advanceTime(m_step);
}


} // end Core
