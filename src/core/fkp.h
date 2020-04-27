#ifndef FKP_H
#define FKP_H

#include "src/core/logic_dynamic_filter.h"


class FKP:public Core::LogicDynamicFilter
{
public:


    void zeroIteration();

    void algorithm();

    void computeBlock1(long s) ;

    void computeBlock2(long s);

    void computeBlock3a(long k);
    void computeBlock3b() ;

    void computeBlock3c() ;

    void computeBlock4(long s);

    void computeBlock5(long s);

    void computeBlock6() ;
    Array<Array<double>> Xi;
    Array<Array<Vector>> u;
    Array<double> Q;
    Array<Vector> kappa;
    Array<Vector> meanZ;
    Array<Matrix> T;
    Array<Vector> meanS;
    Array<Matrix> Gamma;
    Array<Matrix> Dzz;

    Array<Vector> m_sampleS; /*!< Массив для хранения расширенных векторов состояния \f$S_k\f$. */


};

#endif // FKP_H
