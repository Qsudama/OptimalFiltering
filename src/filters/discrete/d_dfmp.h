#ifndef D_DFMP_H
#define D_DFMP_H

#include "src/core/discrete_filter.h"


namespace Filters {

    namespace Discrete {


        class DFMP : public Core::DiscreteFilter {

        public:
            DFMP(Core::PtrFilterParameters params, Core::PtrTask task);

        protected:
            void zeroIteration() override;
            void algorithm() override;

        private:
            Matrix L, Psi, G, F, T, Gamma;
            Vector o, h, chi;

            Array <Vector> Lambda, S, U, E;

            void computeFirstStep(long k, long i);
            void computeSecondStep(long k, long i);

            Vector compute_Lambda(long i);
            Vector compute_Z(long i);
            Vector compute_E(long i);
            void compute_U(long i);

            Matrix compute_Gamma();
            Matrix compute_Psi();
            Matrix compute_L();
            Matrix compute_T();

            Vector compute_chi();
            Vector compute_o();
        };


    } // end Discrete

} // end Filters

#endif // D_DFMP_H
