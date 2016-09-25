#include "matrix.h"


namespace Math
{


void MakeBlockVector(const Vector &m1, const Vector &m2, Vector &b)
{
    b.resize(m1.size() + m2.size());
    b.head(m1.size()) = m1;
    b.tail(m2.size()) = m2;
}

void MakeBlockMatrix(const Matrix &m1, const Matrix &m2, Matrix &b, bool vertical)
{
    if (vertical) {
        assert(m1.cols() == m2.cols());
        b.resize(m1.rows() + m2.rows(), m1.cols());
        b.topRows(m1.rows())    = m1;
        b.bottomRows(m2.rows()) = m2;
    } else {
        assert(m1.rows() == m2.rows());
        b.resize(m1.rows(), m1.cols() + m2.cols());
        b.leftCols(m1.cols())  = m1;
        b.rightCols(m2.cols()) = m2;
    }
}


} // end Math
