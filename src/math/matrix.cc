#include "matrix.h"


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
        assert(mat1.cols() == mat2.cols() && "Math::MakeBlockMatrix(mat1, mat2, blockMat, vertical) : when vertical is "
                                             "true, mat1.cols() must be equal mat2.cols()");
        blockMat.resize(mat1.rows() + mat2.rows(), mat1.cols());
        blockMat.topRows(mat1.rows())    = mat1;
        blockMat.bottomRows(mat2.rows()) = mat2;
    } else {
        assert(mat1.rows() == mat2.rows() && "Math::MakeBlockMatrix(mat1, mat2, blockMat, vertical) : when vertical is "
                                             "false, mat1.rows() must be equal mat2.rows()");
        blockMat.resize(mat1.rows(), mat1.cols() + mat2.cols());
        blockMat.leftCols(mat1.cols())  = mat1;
        blockMat.rightCols(mat2.cols()) = mat2;
    }
}


} // end Math
