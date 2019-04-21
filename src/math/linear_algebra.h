#ifndef LINALG_H
#define LINALG_H

#include "src/math/matrix.h"


namespace Math
{

/*!
\brief Модуль, содержащий описание и реализацию методов линейной алгебры.
*/

namespace LinAlg
{


//! \brief Набор идентификаторов методов псевдообращения матриц.
enum PseudoinverseMethods {
    GREVILLE = 0, //!< Псевдообращение по методу Гревиля.
    SVD = 1 //!< Псевдообращение с использованием сингулярных значений.
};


//! \brief Класс, упрощающий выбор метода псевдообращения матриц.
class CurrentPinvMethod
{
public:
    //! \brief Возвращает ссылку на объект класса (singleton).
    static CurrentPinvMethod &instance();

    //! \brief Возвращает идентификатор выбранного метода.
    PseudoinverseMethods get() const;

    //! \brief Устанавливает другой метод.
    void set(PseudoinverseMethods method);


private:
    //! \brief Конструктор.
    CurrentPinvMethod();

    CurrentPinvMethod(const CurrentPinvMethod &) = delete;
    CurrentPinvMethod &operator=(const CurrentPinvMethod &) = delete;

    PseudoinverseMethods m_value;
};


/*!
 * \brief Производит \f$LU\f$-разложение матрицы \f$A\f$.
 * \param[in] A - матрица, которую нужно разложить.
 * \param[out] L - нижняя треугольная матрица \f$L\f$.
 * \param[out] U - верхняя треугольная матрица \f$U\f$.
 * \param[out] permutationP - левая матрица перестановок \f$P\f$.
 * \param[out] permutationQ - правая матрица перестановок \f$Q\f$.
 * \return матрицу \f$LU\f$.
 *
 * \f$LU\f$-разложение имеет вид: \f$A = P^{-1} \cdot L\cdot U \cdot Q^{-1}\f$.
 */

Matrix LU(const Matrix &A, Matrix &L, Matrix &U, Matrix &permutationP, Matrix &permutationQ);


//! \see LU(const Matrix &A, Matrix &L, Matrix &U, Matrix &permutationP, Matrix &permutationQ).

Matrix LU(const Matrix &A);


/*!
 * \brief Производит разложение Холецкого матрицы \f$A = L \cdot L^{T} = U^{T} \cdot U\f$.
 * \param[in] A - матрица, которую нужно разложить.
 * \param[out] L - нижняя треугольная матрица \f$L\f$.
 * \param[out] U - верхняя треугольная матрица \f$U\f$.
 * \return матрицу \f$L\f$.
 *
 * Разложение Холецкого имеет вид: \f$A = L \cdot L^{T} = U^{T} \cdot U\f$.
 */

Matrix Cholesky(const Matrix &A, Matrix &L, Matrix &U);


//! \see Cholesky(const Matrix &A, Matrix &L, Matrix &U).

Matrix Cholesky(const Matrix &A);


/*!
 * \brief Вычисляет псевдообратную матрицу SVD-методом.
 * \param[in] A - матрица, которую следует обратить.
 * \return псевдообратную матрицу \f$A^{-1}\f$.
 */

Matrix PinvSVD(const Matrix &A);


/*!
 * \brief Вычисляет псевдообратную матрицу методом Гревилля.
 * \param[in] A - матрица, которую следует обратить.
 * \return псевдообратную матрицу \f$A^{-1}\f$.
 */

Matrix PinvGreville(const Matrix &A);


/*!
 * \brief Решает СЛАУ \f$ A \cdot x = b\f$.
 * \param[in] A - матрица коэффициентов левой части СЛАУ.
 * \param[in] b - вектор правой части СЛАУ.
 * \return вектор \f$x\f$.
 */

Matrix Pinv(const Matrix &A);


/*!
 * \brief Вычисляет псевдообратную матрицу методом по-умолчанию.
 * \param[in] A - матрица, которую следует обратить.
 * \return псевдообратную матрицу \f$A^{-1}\f$.
 * \see PseudoinverseMethods
 * \see
 */

Vector SolveSystem(const Matrix &A, const Vector &b);


} // end LinAlg

} // end Math


#endif // LINALG_H
