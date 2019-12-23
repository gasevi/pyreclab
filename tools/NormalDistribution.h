#ifndef __NORMAL_DISTRIBUTION_H__
#define __NORMAL_DISTRIBUTION_H__

#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>

class NormalDistribution
{
public:

   NormalDistribution( float mean, float std );

   double operator()();

private:

   NormalDistribution();

   boost::mt19937 m_rng;

   boost::normal_distribution<> m_nd;

   boost::variate_generator< boost::mt19937&, boost::normal_distribution<> > m_var_nor;

};

#endif // __NORMAL_DISTRIBUTION_H__

