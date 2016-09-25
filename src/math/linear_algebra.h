#ifndef LINALG_H
#define LINALG_H

#include "src/math/matrix.h"


namespace Math
{


namespace LinAlg
{


Vector SolveSystem(const Matrix &A, const Vector &b);

Matrix LU(const Matrix &A);
Matrix LU(const Matrix &A, Matrix &L, Matrix &U, Matrix &permutationP, Matrix &permutationQ);

Matrix Cholesky(const Matrix &A);
Matrix Cholesky(const Matrix &A, Matrix &L, Matrix &U);

Matrix PinvSVD(const Matrix &A);
Matrix PinvGreville(const Matrix &A);


} // end LinAlg

} // end Math


#endif // LINALG_H
