#ifndef TASK_H
#define TASK_H

#include "function_time.h"
#include "info.h"
#include "src/array.h"
#include "src/config.h"
#include "src/math/matrix.h"
#include "src/math/random.h"
#include "src/math/statistic.h"
#include <map>
#include <memory>


using Math::Matrix;
using Math::Vector;
using Math::RowVector;


namespace Core
{


using TaskParameters    = std::map<std::string, double>;
using PtrTaskParameters = std::shared_ptr<TaskParameters>;


class Task : public FunctionTime
{

public:
    Task();
    virtual ~Task();

    Task(const Task &) = delete;
    Task &operator=(const Task &) = delete;

    PtrTaskParameters params();
    PtrTaskParameters consts() const;
    PtrInfo           info() const;

    virtual Math::Vector x0() const;

    long dimX() const;
    long dimY() const;
    long dimV() const;
    long dimW() const;

    const Math::Vector &meanX() const;
    const Math::Vector &meanV() const;
    const Math::Vector &meanW() const;
    const Math::Matrix &varX() const;
    const Math::Matrix &varV() const;
    const Math::Matrix &varW() const;

    void setMeanX(const Math::Vector &m);
    void setMeanV(const Math::Vector &m);
    void setMeanW(const Math::Vector &m);
    void setVarX(const Math::Matrix &D);
    void setVarV(const Math::Matrix &D);
    void setVarW(const Math::Matrix &D);

    void changeParameter(const std::string &key, double newValue);


protected:
    virtual void loadParams();


protected:
    PtrTaskParameters m_params;
    PtrTaskParameters m_consts;
    PtrInfo           m_info;

    long m_dimX;
    long m_dimY;
    long m_dimV;
    long m_dimW;

    Math::Vector m_meanX;
    Math::Vector m_meanV;
    Math::Vector m_meanW;

    Math::Matrix m_varX;
    Math::Matrix m_varV;
    Math::Matrix m_varW;
};


using PtrTask = std::shared_ptr<Task>;


} // end Core

#endif // TASK_H
