#include "src/math/linear_algebra.h"
#include "src/math/constants.h"


namespace Math
{

namespace LinAlg
{


Vector SolveSystem(const Matrix &A, const Vector &b)
{
    return A.colPivHouseholderQr().solve(b);
}

Matrix LU(const Matrix &A)
{
    return Eigen::FullPivLU<Matrix>(A).matrixLU();
}

Matrix LU(const Matrix &A, Matrix &L, Matrix &U, Matrix &permutationP, Matrix &permutationQ)
{
    Eigen::FullPivLU<Matrix> lu(A);
    L                                        = Matrix::Identity(A.rows(), A.cols());
    L.triangularView<Eigen::StrictlyLower>() = lu.matrixLU();
    U                                        = lu.matrixLU().triangularView<Eigen::Upper>();
    permutationP                             = lu.permutationP();
    permutationQ                             = lu.permutationQ();

    return lu.matrixLU();
}

Matrix Cholesky(const Matrix &A)
{
    return Eigen::LLT<Matrix>(A).matrixL();
}

Matrix Cholesky(const Matrix &A, Matrix &L, Matrix &U)
{
    Eigen::LLT<Matrix> llt(A);
    L = llt.matrixL();
    U = llt.matrixU();
    return llt.matrixL();
}

Matrix PinvSVD(const Matrix &A)
{
    // WARNING работает с матрицами (n x n) и (m x n) толькое если m > n
    //         если n > m - падает !
    Eigen::JacobiSVD<Matrix> svd(A, Eigen::ComputeThinU | Eigen::ComputeThinV);

    Vector singularValues    = svd.singularValues();
    Vector singularValuesInv = singularValues;

    double max = singularValues(0); //
    long   m   = (A.rows() > A.cols() ? A.rows() : A.cols());
    double eps = Const::EPS * m * max;

    for (long i = 0; i < singularValues.size(); ++i) {
        if (singularValues(i) > eps) {
            singularValuesInv(i) = 1.0 / singularValues(i);
        } else {
            singularValuesInv(i) = 0.0;
        }
    }

    return svd.matrixV() * singularValuesInv.asDiagonal() * svd.matrixU().transpose();
}

Matrix PinvGreville(const Matrix &A)
{
    // step 1:

    long   N = A.cols();
    Vector a = A.col(0);

    Matrix invA = Matrix::Zero(1, a.rows());

    if (!a.isZero()) {
        invA = (1.0 / a.col(0).dot(a.col(0))) * a.transpose();
    }

    // step 2:

    Vector    d;
    Vector    c;
    RowVector b;
    Matrix    B;

    a = A.col(1);
    d = invA * a;
    c = a - A.col(0) * d[0];
    if (!c.isZero()) {
        b = (1.0 / c.dot(c)) * c.transpose();
    } else {
        b = (1.0 / (1.0 + d.dot(d))) * d.transpose() * invA;
    }
    B    = invA - d * b;
    invA = B;
    invA.conservativeResize(invA.rows() + 1, invA.cols());
    invA.row(invA.rows() - 1) = b;

    // steps 3...N:

    for (long k = 2; k < N; ++k) {
        a        = A.col(k);
        Vector d = invA * a;
        c        = a - A.leftCols(k) * d;
        if (!c.isZero()) {
            b = (1.0 / c.dot(c)) * c.transpose();
        } else {
            b = (1.0 / (1.0 + d.dot(d))) * d.transpose() * invA;
        }
        B    = invA - d * b;
        invA = B;
        invA.conservativeResize(invA.rows() + 1, invA.cols());
        invA.row(invA.rows() - 1) = b;
    }

    return invA;
}


} // end LinAlg

} // end Math
