#ifndef CONTINUOUS_DISCRETE_TASK_H
#define CONTINUOUS_DISCRETE_TASK_H

#include "task.h"


namespace Core
{


class ContinuousDiscreteTask : public Task
{

public:
    ContinuousDiscreteTask();

    Vector a(const Vector &x) const;
    Matrix B(const Vector &x) const;
    Vector tau(const Vector &m, const Matrix &D) const;
    Matrix Theta(const Vector &m, const Matrix &D) const;
    Matrix A(const Vector &m, const Matrix &D) const;
    Vector c(const Vector &x) const;
    Vector h(const Vector &m, const Matrix &D) const;
    Matrix G(const Vector &m, const Matrix &D) const;
    Matrix F(const Vector &m, const Matrix &D) const;


protected:
    virtual Vector funcA(const Vector &x) const = 0;
    virtual Matrix funcB(const Vector &x) const = 0;
    virtual Vector funcTau(const Vector &m, const Matrix &D) const   = 0;
    virtual Matrix funcTheta(const Vector &m, const Matrix &D) const = 0;
    virtual Matrix funcAA(const Vector &m, const Matrix &D) const    = 0;
    virtual Vector funcC(const Vector &pred_x) const = 0;
    virtual Vector funcH(const Vector &m, const Matrix &D) const = 0;
    virtual Matrix funcG(const Vector &m, const Matrix &D) const = 0;
    virtual Matrix funcF(const Vector &m, const Matrix &D) const = 0;
};


using PtrCDTask = std::shared_ptr<ContinuousDiscreteTask>;


} // end Core


#endif // CONTINUOUS_DISCRETE_TASK_H
