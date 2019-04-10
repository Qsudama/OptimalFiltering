#ifndef MATRIX_H
#define MATRIX_H

#include "src/config.h"
#include "third_party/eigen/Dense"


namespace Math
{


using Matrix    = Eigen::MatrixXd;
using Vector    = Eigen::VectorXd;
using RowVector = Eigen::RowVectorXd;

using Vector2       = Eigen::Vector2d;
using VectorInt2    = Eigen::Vector2i;
using RowVector2    = Eigen::RowVector2d;
using RowVectorInt2 = Eigen::RowVector2i;

using Vector3       = Eigen::Vector3d;
using VectorInt3    = Eigen::Vector3i;
using RowVector3    = Eigen::RowVector3d;
using RowVectorInt3 = Eigen::RowVector3i;


/*!
 * \brief Создает вектор из двух векторов.
 * \param[in] vec1 - верхний вектор.
 * \param[in] vec2 - нижний вектор.
 * \param[out] blockVec - результат \f$ b = (v_1\ v_2)^T\f$.
 */

void MakeBlockVector(const Vector &vec1, const Vector &vec2, Vector &blockVec);

/*!
 * \brief Cоздает блочную матрицу из двух матриц
 * \param[in] mat1 - первая матрица.
 * \param[in] mat2 - вторая матрица.
 * \param[out] blockMat - результат \f$ b = (m_1\ m_2)\f$, при vertical == true: \f$ b = (m_1\ m_2)^T\f$.
 * \param[in] vertical - если true, то m1 и m2 выстроятся вертикально, иначе - горизонтально (по-умолчанию).
 */

void MakeBlockMatrix(const Matrix &mat1, const Matrix &mat2, Matrix &blockMat, bool vertical = false);

/*!
 * \brief Cоздает вектор из диагональных элементов квадратичной матрицы.
 * Если матрица не квадратичная - вернет вектор забитый нулями.
 * \param matrix матрица из которой составляется вектор.
 */

Vector ConvertMatrixToVector(const Matrix matrix);

} // end Math


#endif // MATRIX_H
