#ifndef DISCRETE_TASK_H
#define DISCRETE_TASK_H

#include "task.h"


namespace Core
{


class DiscreteTask : public Task
{

public:
    DiscreteTask();

    //объект:
    virtual Vector a(const Vector &x) const = 0;
    //измеритель:
    virtual Vector b(const Vector &x) const = 0;
    //функции прогноза:
    virtual Vector tau(const Vector &m, const Matrix &D) const   = 0;
    virtual Matrix Theta(const Vector &m, const Matrix &D) const = 0;
    //функции коррекции:
    virtual Vector h(const Vector &m, const Matrix &D) const = 0;
    virtual Matrix G(const Vector &m, const Matrix &D) const = 0;
    virtual Matrix F(const Vector &m, const Matrix &D) const = 0;


protected:
    virtual Matrix dadx(const Vector &x) const = 0;
    virtual Matrix dadv(const Vector &x) const = 0;
    virtual Matrix dbdx(const Vector &x) const = 0;
    virtual Matrix dbdw(const Vector &x) const = 0;
};


using PtrDTask = std::shared_ptr<DiscreteTask>;


} // end Core


#endif // DISCRETE_TASK_H
