#ifndef LANDINGREJECTIONLINEAR_H
#define LANDINGREJECTIONLINEAR_H
#endif // LANDINGREJECTIONLINEAR_H
#include "src/core/logic_dynamic_task.h"
#include "src/math/math.h"

class LandingRejectionLinear: public Core::LogicDynamicTask{

protected:
    LandingRejectionLinear();
    Matrix dadx(int i, const Vector &x) const override;
    Matrix dadv(int i, const Vector &x) const override;
    Matrix dbdx(int i, const Vector &x) const override;
    Matrix dbdw(int i, const Vector &x) const override;

    void loadParams() override;

    double e(const Vector &x) const;
    double d(const Vector &x) const;
    double Ex(const Vector &x) const;
    double Sk(double t) const;
    double gammaX(int i) const;
    double gammaY(int i) const;

    Matrix BwdbdwBwt(int i, const Vector &x) const;
    Vector bForZeroW(int i, const Vector &x) const;

protected:
    double m_turnTime;
    double m_e;
    double gamMinX; /*!< Для задачи со сбоями акселерометра */
    double gamMinY; /*!< Для задачи со сбоями акселерометра */
    int countIInTask; /*!< Количество режимов в задаче*/


    static constexpr double KB = 0.3; // b
    static constexpr double BB = 0.09; // Beta
    static constexpr double  CC = 0.043333333333333333; // c = 0.5*pho_0*sigma_x
    static constexpr double GG = 0.00372; // g
    static constexpr double RR = 3400.0; // R

   // LandingRejectionLinear();


    Vector a(int /*i*/, const Vector &x) const;

    Vector b(int i, const Vector &x) const;



    double A(int i, int l) const;

    double nu(int i, int l, const Vector &/*m*/, const Matrix &/*D*/) const;

    Vector tau(int i, int l, const Vector &z, const Matrix &/*P*/) const;

    Matrix Theta(int i, int l, const Vector &z, const Matrix &P) const;

    Vector h(int i, const Vector &m, const Matrix & /* D*/) const;

    Matrix G(int i, const Vector &m, const Matrix &D) const;

    Matrix F(int i, const Vector &m, const Matrix &D) const;



    double Pr(int i) const;

    Array<int> generateArrayI(int sizeS) const;

};
