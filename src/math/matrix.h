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


void MakeBlockVector(const Vector &vec1, const Vector &vec2, Vector &blockVec);
void MakeBlockMatrix(const Matrix &mat1, const Matrix &mat2, Matrix &blockMat, bool vertical = false);


} // end Math


#endif // MATRIX_H
