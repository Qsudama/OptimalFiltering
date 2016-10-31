#include "task.h"


namespace Core
{


Task::Task()
    : FunctionTime(0.0)
    , m_params(new TaskParameters)
    , m_consts(new TaskParameters)
    , m_info(new Info())
    , m_dimX(2)
    , m_dimY(2)
    , m_dimV(2)
    , m_dimW(2)
    , mx(Vector::Zero(2))
    , mv(Vector::Zero(2))
    , mw(Vector::Zero(2))
    , Dx(Matrix::Zero(2, 2))
    , Dv(Matrix::Zero(2, 2))
    , Dw(Matrix::Zero(2, 2))
{
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

const Vector &Task::meanX() const
{
    return mx;
}

const Vector &Task::meanV() const
{
    return mv;
}

const Vector &Task::meanW() const
{
    return mw;
}

const Matrix &Task::varX() const
{
    return Dx;
}

const Matrix &Task::varV() const
{
    return Dv;
}

const Matrix &Task::varW() const
{
    return Dw;
}

Vector Task::x0() const
{
    return Math::Rand::gaussianVector(mx, Dx);
}

void Task::setMeanX(const Vector &m)
{
    assert(m.size() == m_dimX && "Core::Task::setMeanX(m) : corrupt dimension of m");
    mx = m;
}

void Task::setMeanV(const Vector &m)
{
    // WARNING: assert(m.size() == m_dimV)
    mv = m;
}

void Task::setMeanW(const Vector &m)
{
    // WARNING: assert(m.size() == m_dimW)
    mw = m;
}

void Task::setVarX(const Matrix &D)
{
    assert(D.rows() == m_dimX && "Core::Task::setVarX(D) : corrupt dimension of D (row's count)");
    assert(D.cols() == m_dimX && "Core::Task::setVarX(D) : corrupt dimension of D (col's count)");
    Dx = D;
}

void Task::setVarV(const Matrix &D)
{
    // WARNING: asserts
    Dv = D;
}

void Task::setVarW(const Matrix &D)
{
    // WARNING: asserts
    Dw = D;
}

void Task::changeParameter(const std::string &key, double newValue)
{
    (*m_params).at(key) = newValue;
    loadParams();
}

void Task::loadParams()
{
}


} // end Core
