#ifndef C_SCALAR_GAUSS
#define C_SCALAR_GAUSS

#include "c_scalar_linear.h"
#include "src/math/math.h"


namespace Tasks
{

//! Подмодуль, содержащий реализации задач для непрерывных фильтров оптимальной структуры.

namespace Continuous
{


/*!
 * \brief Тестовый скалярный пример (линеаризованный) для фильтров оптимальной структуры.
 */

class ScalarGauss : public ScalarLinear
{

public:
    //! \brief Конструктор.
    ScalarGauss();

    Matrix A(const Vector &m, const Matrix &D) const override;
    Matrix G(const Vector &m, const Matrix &D) const override;

protected:
    Matrix Q(const Vector &m, const Matrix &D) const override;

};


} // end Tasks::

} // end Tasks
#endif // C_SCALAR_GAUSS

