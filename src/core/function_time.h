#ifndef FUNCTION_TIME_H
#define FUNCTION_TIME_H


namespace Core
{


class FunctionTime
{
public:
    FunctionTime(double initialTime = 0.0, double initialStep = 0.001);
    virtual ~FunctionTime();

    double time() const;
    double step() const;

    void setTime(double newTime);
    void setStep(double newStep);
    void advanceTime(double step);
    void advanceTime();

protected:
    double m_time;
    double m_step;
};


} // end Core


#endif // FUNCTION_TIME_H
