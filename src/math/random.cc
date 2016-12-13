#include "random.h"
#include "constants.h"


namespace Math
{


MultivariateNormalDistribution::MultivariateNormalDistribution()
{
    m_univariateNormalDistribution.param(std::normal_distribution<double>::param_type(0.0, 1.0));
}

void MultivariateNormalDistribution::setSeed(ulong seed)
{
    m_generator.seed(seed);
}

Vector MultivariateNormalDistribution::normal01(long dim) const
{
    assert(dim > 0);

    Vector x(dim);
    for (long i = 0; i < dim; ++i) {
        x[i] = m_univariateNormalDistribution(m_generator);
    }

    return x;
}

Vector MultivariateNormalDistribution::operator()(long dim) const
{
    return normal01(dim);
}

Vector MultivariateNormalDistribution::operator()(const Vector &mean, const Matrix &var) const
{
    long dim = mean.size();

    assert(dim > 0);
    assert(var.rows() == dim);
    assert(var.cols() == dim);

    return mean + LinAlg::Cholesky(var) * normal01(dim);
}


} // end Math
