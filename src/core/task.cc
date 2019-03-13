#include "task.h"


namespace Core
{


Task::Task()
    : FunctionTime(0.0)
    , logInstance(LogInConsoleManager::Instance())
    , m_params(std::make_shared<TaskParameters>())
    , m_consts(std::make_shared<TaskParameters>())
    , m_info(std::make_shared<Info>())
    , m_dimX(2)
    , m_dimY(2)
    , m_dimV(2)
    , m_dimW(2)
    , m_meanX0(Vector::Zero(2))
    , m_meanV(Vector::Zero(2))
    , m_meanW(Vector::Zero(2))
    , m_varX0(Matrix::Zero(2, 2))
    , m_varV(Matrix::Zero(2, 2))
    , m_varW(Matrix::Zero(2, 2))
{
    m_normalRand.setSeed(Math::RandomProperties::defaultSeed());
}

Task::~Task()
{
}

PtrTaskParameters Task::params()
{
    return m_params;
}

PtrTaskParameters Task::consts() const
{
    return m_consts;
}

PtrInfo Task::info() const
{
    return m_info;
}

long Task::dimX() const
{
    return m_dimX;
}

long Task::dimY() const
{
    return m_dimY;
}

long Task::dimV() const
{
    return m_dimV;
}

long Task::dimW() const
{
    return m_dimW;
}

const Vector &Task::meanX0() const
{
    return m_meanX0;
}

const Vector &Task::meanV() const
{
    return m_meanV;
}

const Vector &Task::meanW() const
{
    return m_meanW;
}

const Matrix &Task::varX0() const
{
    return m_varX0;
}

const Matrix &Task::varV() const
{
    return m_varV;
}

const Matrix &Task::varW() const
{
    return m_varW;
}

Vector Task::x0() const
{
    return m_normalRand(m_meanX0, m_varX0);
}

void Task::setMeanX0(const Vector &mean)
{
    assert(mean.size() == m_dimX && "Core::Task::setMeanX(mean) : corrupt dimension of mean");
    m_meanX0 = mean;
}

void Task::setMeanV(const Vector &mean)
{
    assert(mean.size() == m_dimV && "Core::Task::setMeanV(mean) : corrupt dimension of mean");
    m_meanV = mean;
}

void Task::setMeanW(const Vector &mean)
{
    assert(mean.size() == m_dimW && "Core::Task::setMeanW(mean) : corrupt dimension of mean");
    m_meanW = mean;
}

void Task::setVarX0(const Matrix &var)
{
    assert(var.rows() == m_dimX && "Core::Task::setVarX(var) : corrupt dimension of var (row's count)");
    assert(var.cols() == m_dimX && "Core::Task::setVarX(var) : corrupt dimension of var (col's count)");
    m_varX0 = var;
}

void Task::setVarV(const Matrix &var)
{
    assert(var.rows() == m_dimV && "Core::Task::setVarV(var) : corrupt dimension of var (row's count)");
    assert(var.cols() == m_dimV && "Core::Task::setVarV(var) : corrupt dimension of var (col's count)");
    m_varV = var;
}

void Task::setVarW(const Matrix &var)
{
    assert(var.rows() == m_dimW && "Core::Task::setVarW(var) : corrupt dimension of var (row's count)");
    assert(var.cols() == m_dimW && "Core::Task::setVarW(var) : corrupt dimension of var (col's count)");
    m_varW = var;
}

void Task::changeParameter(const std::string &key, double value)
{
    (*m_params).at(key) = value;
    loadParams();
}

void Task::loadParams()
{
}

} // end Core
