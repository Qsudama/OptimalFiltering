#include "random.h"
#include "constants.h"

#include "src/helpers/alert_helper.h"

namespace Math
{


Uint RandomProperties::defaultSeed()
{
    static Uint seed = std::numeric_limits<Uint>::max() / 2;
    return seed;
}

Uint RandomProperties::randomSeed()
{
    Uint seed = Uint(std::chrono::system_clock::now().time_since_epoch().count());
    return seed;
}

//-----//

MultivariateNormalDistribution::MultivariateNormalDistribution()
{
    m_univariateNormalDistribution.param(std::normal_distribution<double>::param_type(0.0, 1.0));
}

void MultivariateNormalDistribution::setSeed(Uint seed)
{
    m_generator.seed(seed);
}

Vector MultivariateNormalDistribution::normal01(long dim) const
{
//    assert(dim > 0 && "Math::MultivariateNormalDistribution::normal01(dim) : corrupt value of dim");
    if (dim <= 0) {
        AlertHelper::showErrorAlertWithText("Math::MultivariateNormalDistribution::normal01\nРазмерность <= 0");
        return Vector::Zero(dim);
    }
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

//    assert(dim > 0 && "Math::MultivariateNormalDistribution::operator()(mean, var) : corrupt dimension of mean");
//    assert(var.rows() == dim && "Math::MultivariateNormalDistribution::operator()(mean, var) : corrupt dimension of var (row's count)");
//    assert(var.cols() == dim && "Math::MultivariateNormalDistribution::operator()(mean, var) : corrupt dimension of var (col's count)");
    if (dim <= 0) {
        AlertHelper::showErrorAlertWithText("Math::MultivariateNormalDistribution::operator(mean, var)\nРазмерность <= 0");
        return Vector::Zero(dim);
    }
    if (var.rows() != dim) {
        AlertHelper::showErrorAlertWithText("Math::MultivariateNormalDistribution::operator(mean, var)\nКоличество строк матрицы не соответствует размерности");
        return Vector::Zero(dim);
    }
    if (var.cols() != dim) {
        AlertHelper::showErrorAlertWithText("Math::MultivariateNormalDistribution::operator(mean, var)\nКоличество столбцов матрицы не соответствует размерности");
        return Vector::Zero(dim);
    }
    return mean + LinAlg::Cholesky(var) * normal01(dim);
}


} // end Math
