#ifndef D_FKP_H
#define D_FKP_H

#include "src/core/discrete_filter.h"


namespace Filters {

    namespace Discrete {


        class FKP : public Core::DiscreteFilter {

        public:
            FKP(Core::PtrFilterParameters params, Core::PtrTask task);

        protected:
            void zeroIteration() override;
            void algorithm() override;

            Array<Vector> m_sampleS; /*!< Массив для хранения расширенных векторов состояния \f$S_k\f$. */
        private:

            void computeParams(size_t k, Array<Math::Vector> &u, Math::Matrix &T);
        };


    } // end Discrete

} // end Filters

#endif // D_FKP_H
