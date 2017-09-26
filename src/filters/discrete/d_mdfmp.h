#ifndef D_MDFMP_H
#define D_MDFMP_H

#include "src/core/discrete_filter.h"


namespace Filters {

    namespace Discrete {


        class MDFMP : public Core::DiscreteFilter {

        public:
            MDFMP(Core::PtrFilterParameters params, Core::PtrTask task);

        protected:
            void zeroIteration() override;
            void algorithm() override;

        private:
            Matrix L, Psi, G, F, T, Gamma, H;
            Vector o, h, chi, e;

            Array <Vector> Lambda, Sigma, Delta, S, U, E;

            void computeFirstStep(long k, long i);
            void computeSecondStep(long k, long i);

            Vector compute_Lambda(long i);
            Vector compute_Sigma(long i);
            Vector compute_Z(long i);
            Vector compute_E(long i);
            void compute_U(long i);

            Matrix compute_Gamma();
            Matrix compute_Psi();
            Matrix compute_L();
            Matrix compute_T();
            Matrix compute_H();

            Vector compute_chi();
            Vector compute_o();
            Vector compute_e();
        };


    } // end Discrete

} // end Filters

#endif // D_MDFMP_H
