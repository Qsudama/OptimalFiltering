#ifndef DISCRETE_MFOS_H
#define DISCRETE_MFOS_H

#include "src/core/discrete_filter.h"


namespace Filters
{

namespace Discrete
{


class MFOS : public Core::DiscreteFilter
{

public:
    MFOS(Core::PtrFilterParameters params, Core::PtrTask task);


protected:
    /*!
     \brief Выполняет алгоритм.

     Вектор \f$Z_t\f$ оценки вычисляется следующим образом:
     \f[S_{k} = \tau_{k-1}(U_{k-1}, T_{k-1}),\f]
     \f[\Lambda_k = L_{k} \cdot S_{k} + n_{k},\f]
     \f[\Psi_k = \Theta_{k-1}(U_{k-1}, T_{k-1}),\f]
     \f[\Sigma_k = \Psi_k \cdot G_k^T(\Lambda_k, \Psi_k) \cdot
                              F_k^{-1}(\Lambda_k, \Psi_k) \cdot [Y_k - h_k(\Lambda_k, \Psi_k)],\f]
     \f[Z_k = \Lambda_k + K_k \cdot \Sigma_k + e_k.\f]
     \f[U_k = \Gamma_k^y \cdot Y_k + \Gamma_k^z \cdot Z_k + \chi_k.\f]

     Параметры \f$\Gamma_k^y,\ \Gamma_k^z,\ T_k,\ \chi_k\f$ вычисляются в computeParams().

     Дополнительные параметры \f$K_k,\ e_k,\ L_{k},\ n_{k}\f$ вычисляются в computeAdditionParams().

     \see Core::DiscreteFilter
     \see Core::DiscreteTask
    */
    void algorithm() override;

private:
    /*!
      \brief Вычисляет параметры и заполняет массивы \f$S_{k}\f$,  \f$U_k\f$.
      \param[in] n - индекс, соответствующий времени \f$t = n \cdot \Delta t\f$.
      \param[out] sampleU - масcив под выборку \f$U_k\f$.
      \param[out] s - масcив под выборку \f$S_{k}\f$.
      \param[out] T - матрица \f$T_k\f$.

     \f[D_k^\delta = cov([Y_k^T\ \ Z_k^T]^T),\f]
     \f[[\Gamma_k^y\ \ \Gamma_k^z] = [D_{kk}^{xy}\ \ D_{kk}^{xz}] \cdot (D_k^\delta)^{-1},\f]
     \f[\chi_k = m_k^x - \Gamma_k^y \cdot m_k^y - \Gamma_k^z \cdot m_k^z,\f]
     \f[T_k = D_k^x - \Gamma_k^y \cdot (D_{kk}^{xy})^T - \Gamma_k^z \cdot (D_{kk}^{xz})^T,\f]
     */
    void computeParams(size_t n, Array<Math::Vector> &sampleU, Array<Math::Vector> &sampleS, Math::Matrix &T);

    /*!
      \brief Вычисляет дополнительные параметры.
      \param[in] nn - индекс, соответствующий времени \f$t = nn \cdot \Delta t\f$.
      \param[in] sampleLambda - выборка \f$\Lambda_k\f$.
      \param[in] sampleSigma - выборка \f$\Sigma_k\f$.
      \param[out] L - матрица \f$L_{k}\f$.
      \param[out] K - матрица \f$K_k\f$.
      \param[out] n - матрица \f$n_{k}\f$.
      \param[out] e - матрица \f$e_k\f$.

     \f[K_k = D_{kk}^{e\sigma} \cdot (D_k^\sigma)^{-1},\f]
     \f[e_k = m_k^e - K_k \cdot m_k^\sigma,\f]
     \f[L_{k} = D_{k, k-1}^{x,s} \cdot (D_{k-1}^s)^{-1},\f]
     \f[n_{k} = m_k^x - L_{k}\cdot m_{k-1}^s.\f]
    */
    void computeAdditionParams(size_t nn, const Array<Math::Vector> &sampleS, const Array<Math::Vector> &sampleLambda,
                               const Array<Math::Vector> &sampleSigma, Matrix &L, Matrix &K, Vector &n, Vector &e);
};


} // end Discrete

} // end Filters


#endif // DISCRETE_MFOS_H
