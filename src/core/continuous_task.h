#ifndef CONTINUOUS_TASK_H
#define CONTINUOUS_TASK_H

#include "src/math/linear_algebra.h"
#include "task.h"


namespace Core
{


class ContinuousTask : public Task
{

public:
    ContinuousTask();
    ~ContinuousTask();

    Vector a(const Vector &x) const;
    Matrix B(const Vector &x) const;
    Vector c(const Vector &x) const;
    Matrix D(const Vector &x) const;
    Matrix A(const Vector &m, const Matrix &D) const;
    Matrix G(const Vector &m, const Matrix &D) const;
    Matrix K(const Vector &x, const Matrix &D) const;
    Matrix Psy(const Vector &x, const Matrix &D) const;

protected:
    virtual Vector funcA(const Vector &x) const = 0;
    virtual Matrix funcB(const Vector &x) const = 0;
    virtual Vector funcC(const Vector &x) const = 0;
    virtual Matrix funcD(const Vector &x) const = 0;
    virtual Matrix funcAA(const Vector &m, const Matrix &D) const = 0;
    virtual Matrix funcG(const Vector &m, const Matrix &D) const  = 0;
    virtual Matrix funcQ(const Vector &m, const Matrix &D) const  = 0;
    virtual Matrix funcS(const Vector &m, const Matrix &D) const  = 0;
    virtual Matrix funcR(const Vector &m, const Matrix &D) const  = 0;
};


using PtrCTask = std::shared_ptr<ContinuousTask>;


} // end Core


#endif // CONTINUOUS_TASK_H
