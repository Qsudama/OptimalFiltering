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

void MakeBlockVector(const Vector &m1, const Vector &m2, Vector &b);
void MakeBlockMatrix(const Matrix &m1, const Matrix &m2, Matrix &b, bool vertical = false);


} // end Math


#endif // MATRIX_H
