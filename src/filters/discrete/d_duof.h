#ifndef D_DUOF_H
#define D_DUOF_H

#include "src/core/discrete_filter.h"


namespace Filters {

    namespace Discrete {


        class DUOF : public Core::DiscreteFilter {

        public:
            DUOF(Core::PtrFilterParameters params, Core::PtrTask task);

        protected:
            void zeroIteration() override;
            void algorithm() override;

        private:
            Matrix F, H;
            Vector g, e;
            Array <Vector> Lambda, deltaY, Omega, A;

            void computeFirstStep(long k, long i);
            void computeSecondStep(long k, long i);
            void computeThirdStep(long k, long i);

            Vector compute_Lambda(long i);
            Vector compute_DeltaY(long i);
            Vector compute_Omega(long i);
            Vector compute_E(long i);
            Vector compute_Z(long i);

            Matrix compute_F();
            Matrix compute_H();

            Vector compute_g();
            Vector compute_e();
        };


    } // end Discrete

} // end Filters

#endif // D_DUOF_H
