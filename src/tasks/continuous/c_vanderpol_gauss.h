#ifndef CONTINUOUS_VANDERPOL_GAUSS_H
#define CONTINUOUS_VANDERPOL_GAUSS_H

#include "c_vanderpol_linear.h"


namespace Tasks
{

namespace Continuous
{


/*!
 * \brief Осциллятор Ван-дер-Поля (непрерывный, гауссовский) для фильтров оптимальной структуры.
 */

class VanDerPolGauss : public VanDerPolLinear
{

public:
    //! \brief Конструктор.
    VanDerPolGauss();

    Matrix A(const Vector &m, const Matrix &D) const override;
    Matrix G(const Vector &m, const Matrix &D) const override;


protected:
    Matrix Q(const Vector &m, const Matrix &D) const override;
};


} // end Tasks::Continuous

} // end Tasks


#endif // CONTINUOUS_VANDERPOL_GAUSS_H
