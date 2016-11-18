#include "d_mfos.h"
#include "src/math/statistic.h"

using namespace std;

namespace Filters
{

namespace Discrete
{


using Math::Rand::gaussianVector;
using Math::LinAlg::PinvSVD;
using Math::Statistic::Mean;
using Math::Statistic::Var;
using Math::Statistic::Cov;
using Math::MakeBlockVector;
using Math::MakeBlockMatrix;


MFOS::MFOS(Core::PtrFilterParameters params, Core::PtrTask task)
    : DiscreteFilter(params, task)
{
    long n = task->dimX();
    long m = task->dimY();
    long N = n * (n + m) + n + n * (n + 1) / 2 + 2 * n * (n + 1);
    m_info->setName(m_task->info()->type() + "МФОСд (" + std::to_string(N) + ")");
}

void MFOS::algorithm()
{
    // TODO
}

} // end Discrete

} // end Filters
