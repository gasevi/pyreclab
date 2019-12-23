#include "NormalDistribution.h"

#include <ctime>

using namespace std;


NormalDistribution::NormalDistribution( float mean, float std )
: m_nd( mean, std ),
  m_var_nor( m_rng, m_nd )
{
   m_rng.seed( static_cast<std::uint32_t>( clock() ) );
}

double NormalDistribution::operator()()
{
   return m_var_nor();
}


