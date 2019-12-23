#ifndef __UNIFORM_REAL_DISTRIBUTION_H__
#define __UNIFORM_REAL_DISTRIBUTION_H__

#include <boost/random.hpp>
#include <boost/random/uniform_real.hpp>

class UniformRealDistribution
{
public:

   UniformRealDistribution( float min, float max );

   double operator()();

private:

   UniformRealDistribution();

   boost::mt19937 m_rng;

   boost::uniform_real<> m_urd;

   boost::variate_generator< boost::mt19937&, boost::uniform_real<> > m_var_uni;

};

#endif // __UNIFORM_REAL_DISTRIBUTION_H__

