#ifndef CONTINUOUS_DISCRETE_TASK_H
#define CONTINUOUS_DISCRETE_TASK_H

#include "task.h"


namespace Core
{


class ContinuousDiscreteTask : public Task
{

public:
    ContinuousDiscreteTask();

    //объект:
    virtual Vector a(const Vector &x) const = 0;
    virtual Matrix B(const Vector &x) const = 0;
    //измеритель:
    virtual Vector c(const Vector &x) const = 0;
    //функции прогноза:
    virtual Vector tau(const Vector &m, const Matrix &D) const   = 0;
    virtual Matrix Theta(const Vector &m, const Matrix &D) const = 0;
    virtual Matrix A(const Vector &m, const Matrix &D) const     = 0;
    //функции коррекции:
    virtual Vector h(const Vector &m, const Matrix &D) const = 0;
    virtual Matrix G(const Vector &m, const Matrix &D) const = 0;
    virtual Matrix F(const Vector &m, const Matrix &D) const = 0;
};


using PtrCDTask = std::shared_ptr<ContinuousDiscreteTask>;


} // end Core


#endif // CONTINUOUS_DISCRETE_TASK_H
