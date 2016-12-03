#include "../src/math/linear_algebra.h"
#include <iostream>
#include <string>


using Math::Matrix;
using Math::LinAlg::PinvGreville;
using Math::LinAlg::PinvSVD;


Matrix createMatrixA1()
{
    Matrix A1(4, 3);

    A1(0, 0) = 1.0;
    A1(0, 1) = -1.0;
    A1(0, 2) = 0.0;

    A1(1, 0) = -1.0;
    A1(1, 1) = 2.0;
    A1(1, 2) = 1.0;

    A1(2, 0) = 2.0;
    A1(2, 1) = -3.0;
    A1(2, 2) = -1.0;

    A1(3, 0) = 0.0;
    A1(3, 1) = -1.0;
    A1(3, 2) = 1.0;

    return A1;
}

Matrix createMatrixA2()
{
    Matrix A2 = Matrix::Zero(3, 3);

    A2(0, 0) = 1.0;
    A2(1, 1) = 2.0;

    return A2;
}

Matrix createMatrixA3()
{
    Matrix A3(1, 3);

    A3(0, 0) = 1.0;
    A3(0, 1) = 2.0;
    A3(0, 2) = 3.0;

    return A3;
}

Matrix createMatrixA4()
{
    Matrix A4(2, 3);

    A4.fill(1.0);
    A4(0, 1) = 0.0;

    return A4;
}

void test(const Matrix &A, const std::string &testName)
{
    Matrix          invA;
    Eigen::IOFormat HeavyFmt(Eigen::FullPrecision, 0, ", ", ";\n", "[", "]", "[", "]");

    std::cout << "Begin " << testName << ":\n\n";
    std::cout << "Matrix A = \n" << A.format(HeavyFmt) << "\n\n";
    std::cout << "----------------------------------------------------------------------------------------------\n\n";

    std::cout << "SVD method:\n\n";
    invA = PinvSVD(A);
    std::cout << "I.1:  A+ =\n" << invA.format(HeavyFmt) << "\n\n";
    std::cout << "I.2:  (A+)+ [== A] =\n" << PinvSVD(invA).format(HeavyFmt) << "\n\n\n";
    std::cout << "II.1:  A * A+ * A [== A] = \n" << (A * invA * A).format(HeavyFmt) << "\n\n";
    std::cout << "II.2:  A+ * A * A+ [== A+] = \n" << (invA * A * invA).format(HeavyFmt) << "\n\n\n";
    std::cout << "III.1  A * A+ = \n" << (A * invA).format(HeavyFmt) << "\n\n";
    std::cout << "III.2  (A * A+)^T [== A * A+] = \n" << ((A * invA).transpose()).format(HeavyFmt) << "\n\n\n";
    std::cout << "IV.1  A+ * A = \n" << (invA * A).format(HeavyFmt) << "\n\n";
    std::cout << "IV.2  (A+ * A)^T [== A+ * A] = \n" << ((invA * A).transpose()).format(HeavyFmt) << "\n\n";

    std::cout << "----------------------------------------------------------------------------------------------\n\n";

    std::cout << "Greville method:\n\n";
    invA = PinvGreville(A);
    std::cout << "I.1:  A+ =\n" << invA.format(HeavyFmt) << "\n\n";
    std::cout << "I.2:  (A+)+ [== A] =\n" << PinvGreville(invA).format(HeavyFmt) << "\n\n\n";
    std::cout << "II.1:  A * A+ * A [== A] = \n" << (A * invA * A).format(HeavyFmt) << "\n\n";
    std::cout << "II.2:  A+ * A * A+ [== A+] = \n" << (invA * A * invA).format(HeavyFmt) << "\n\n\n";
    std::cout << "III.1  A * A+ = \n" << (A * invA).format(HeavyFmt) << "\n\n";
    std::cout << "III.2  (A * A+)^T [== A * A+] = \n" << ((A * invA).transpose()).format(HeavyFmt) << "\n\n\n";
    std::cout << "IV.1  A+ * A = \n" << (invA * A).format(HeavyFmt) << "\n\n";
    std::cout << "IV.2  (A+ * A)^T [== A+ * A] = \n" << ((invA * A).transpose()).format(HeavyFmt) << "\n\n";


    std::cout << "----------------------------------------------------------------------------------------------\n\n";
    std::cout << "Finish " << testName << ".\n\n";
}
