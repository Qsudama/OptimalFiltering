#ifndef LOGIC_DYNAMIC_FILTER_H
#define LOGIC_DYNAMIC_FILTER_H

#include "filter.h"
#include "logic_dynamic_task.h"
#include "src/math/linear_algebra.h"
#include "src/math/constants.h"

using namespace std;

namespace Core
{


/*! \brief Базовый класс для всех логико-динамических фильтров оптимальной структуры.

    \see LogicDynamicTask.
*/

class LogicDynamicFilter : public Filter
{

public:
    //! \brief Конструктор.
    LogicDynamicFilter(PtrFilterParameters params, PtrTask task, FILTER_ID id);

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

    FilterTimeResult execute_time_filter() override;

    void computeBlock1(long s, size_t k);
    void computeBlock2(long s, size_t k);

    void computeBlock4(long s, size_t k, const Array<double> &p, const Array<Vector> &sigma, const Array<Matrix> &upsilon);
    void computeBlock5(long s, size_t k);
    void computeBlock6(size_t k);

    Array<double> computeProbabilityDensityN(const Array<double> &omega, const Vector &u, const Array<Vector> &m, const Array<Matrix> &pinD, const Array<double> &determinants);
    Array<double> calculateSqrtDeterminantForProbabilityDensityN(const Array<Matrix> &D);
    Array<Matrix> pinvDForProbabilityDensityN(const Array<Matrix> &D);

    string probabilityForView();
protected:
    PtrLDTask  m_task; /*!< Указатель на экземпляр задачи, с которой происходит работа. */

    Array<Array<double>> Omega;
    Array<Array<Vector>> Lambda;
    Array<Array<Vector>> Mu;
    Array<Array<Matrix>> Psi;
    Array<Array<Matrix>> Delta;
    Array<Array<Matrix>> Phi;
    Array<Array<double>> P;
    Array<Array<Matrix>> K;
    Array<Array<Vector>> Sigma;
    Array<Array<Matrix>> Upsilon; 

    FILTER_ID m_identifier;

private:

    void computeZeroVectors();
    void computeBlock0();

    double probabilityDensityN(const double &Omega, const Vector &u, const Vector &m, const double &det, const Matrix &pinD);
};


//! \brief Тип умного указателя на LogicDynamicFilter.
using PtrLDFilter = std::shared_ptr<LogicDynamicFilter>;


} // end Core


#endif // LOGIC_DYNAMIC_FILTER_H
