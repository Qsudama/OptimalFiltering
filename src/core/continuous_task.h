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

    //объект:
    virtual Vector a(const Vector &x) const = 0;
    virtual Matrix B(const Vector &x) const = 0;
    //измеритель:
    virtual Vector c(const Vector &x) const = 0;
    virtual Matrix D(const Vector &x) const = 0;

    virtual Matrix A(const Vector &m, const Matrix &D) const = 0;
    virtual Matrix G(const Vector &m, const Matrix &D) const = 0;
    virtual Matrix K(const Vector &x, const Matrix &D) const;
    virtual Matrix Psi(const Vector &x, const Matrix &D) const;


protected:
    virtual Matrix Q(const Vector &m, const Matrix &D) const = 0;
    virtual Matrix S(const Vector &m, const Matrix &D) const = 0;
    virtual Matrix R(const Vector &m, const Matrix &D) const = 0;
};


using PtrCTask = std::shared_ptr<ContinuousTask>;


} // end Core


#endif // CONTINUOUS_TASK_H
