#ifndef __UNIFORM_INT_DISTRIBUTION_H__
#define __UNIFORM_INT_DISTRIBUTION_H__

#include <boost/random.hpp>
#include <boost/random/uniform_int.hpp>

class UniformIntDistribution
{
public:

   UniformIntDistribution( int min, int max );

   double operator()();

private:

   UniformIntDistribution();

   boost::mt19937 m_rng;

   boost::uniform_int<> m_urd;

   boost::variate_generator< boost::mt19937&, boost::uniform_int<> > m_var_uni;

};

#endif // __UNIFORM_INT_DISTRIBUTION_H__

