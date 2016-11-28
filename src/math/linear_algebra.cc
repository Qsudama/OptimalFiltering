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
    Eigen::JacobiSVD<Matrix> svd;
    if (A.rows() < A.cols()) {
        svd = Matrix(A.transpose()).jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV);
    } else {
        svd = A.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV);
    }

    double max = svd.singularValues().array().abs().maxCoeff();
    double eps = Const::EPS * std::max(A.cols(), A.rows()) * max;

    Matrix singularValuesInverse =
        (svd.singularValues().array().abs() > eps).select(svd.singularValues().array().inverse(), 0);

    return svd.matrixV() * singularValuesInverse.asDiagonal() * svd.matrixU().adjoint();
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
