#ifndef LOGIC_DYNAMIC_FILTER_H
#define LOGIC_DYNAMIC_FILTER_H

#include "filter.h"
#include "logic_dynamic_task.h"
#include "src/math/linear_algebra.h"
#include "src/math/constants.h"


namespace Core
{


/*! \brief Базовый класс для всех логико-динамических фильтров оптимальной структуры.

    \see LogicDynamicTask.
*/

class LogicDynamicFilter : public Filter
{

public:
    //! \brief Конструктор.
    LogicDynamicFilter(PtrFilterParameters params, PtrTask task);

protected:
    void init() override;

    /*! \brief Нулевая итерация алгоритма (инициализирует начальные состояния).

        Практически для всех логико-динамических фильтров оптимальной структуры имеет вид:
        \f[Z_0 = H_0 \cdot  Y_0 + e_0,\f]
        \f[H_0 = D_{00}^{xy} \cdot  (D_0^y)^{-1},\f]
        \f[e_0 = m_0^x - H_0 \cdot  m_0^y.\f]

        В гауссовском АОФ (ФНА) добавляется уравнение для апостериорной ковариации объекта:
        \f[P_0 = D_0^x - H_0 \cdot  (D_{00}^{xy})^T.\f]
    */
    void zeroIteration() override;

    double probabilityDensityN(const Vector &u, const Vector &m, const Matrix &D);

protected:
    PtrLDTask  m_task; /*!< Указатель на экземпляр задачи, с которой происходит работа. */

//    Array<double>> Omega;
//    Array<Array<Vector>> Lambda;
//    Array<Array<Vector>> Mu;
//    Array<Array<Matrix>> Psi;
//    Array<Array<Matrix>> Delta;
//    Array<Array<Matrix>> Phi;
//    Array<Array<double>> P;
//    Array<Array<Matrix>> K;
//    Array<Array<Vector>> Sigma;
//    Array<Array<Matrix>> Upsilon;

    Array<double> Xi;
    Array<double> Q;
    Array<Vector> kappa;
    Array<Matrix> T;
    Array<Vector> u;
    Array<Vector> meanX;
    Array<Vector> meanZ;
    Array<Matrix> Gamma;
    Array<Matrix> Dzz;
    Array<Matrix> Dxx;
    Array<Matrix> Dxz;
};


//! \brief Тип умного указателя на LogicDynamicFilter.
using PtrLDFilter = std::shared_ptr<LogicDynamicFilter>;


} // end Core


#endif // LOGIC_DYNAMIC_FILTER_H
