#include "matrix.h"

#include "src/helpers/alert_helper.h"

namespace Math
{


void MakeBlockVector(const Vector &vec1, const Vector &vec2, Vector &blockVec)
{
    blockVec.resize(vec1.size() + vec2.size());
    blockVec.head(vec1.size()) = vec1;
    blockVec.tail(vec2.size()) = vec2;
}

void MakeBlockMatrix(const Matrix &mat1, const Matrix &mat2, Matrix &blockMat, bool vertical)
{
    if (vertical) {
        if (mat1.cols() != mat2.cols()) {
            AlertHelper::showErrorAlertWithText("Math::MakeBlockMatrix\nКоличество столбцов mat1 не соответствует количеству столбцов mat2");
            return;
        }
        blockMat.resize(mat1.rows() + mat2.rows(), mat1.cols());
        blockMat.topRows(mat1.rows())    = mat1;
        blockMat.bottomRows(mat2.rows()) = mat2;
    } else {
        if (mat1.rows() != mat2.rows()) {
            AlertHelper::showErrorAlertWithText("Math::MakeBlockMatrix\nКоличество строк mat1 не соответствует количеству строк mat2");
            return;
        }
        blockMat.resize(mat1.rows(), mat1.cols() + mat2.cols());
        blockMat.leftCols(mat1.cols())  = mat1;
        blockMat.rightCols(mat2.cols()) = mat2;
    }
}

Vector ConvertMatrixToVector(const Matrix matrix)
{
    if (matrix.rows() != matrix.cols()) {
        return Vector::Zero(matrix.rows());
    }
    Vector result(matrix.rows());
    for (int i = 0; i < matrix.rows(); i++) {
        result[i] = matrix(i, i);
    }
    return result;
}

} // end Math
