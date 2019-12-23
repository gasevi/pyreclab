#include "UniformIntDistribution.h"

#include <ctime>
#include <sys/time.h>

using namespace std;


UniformIntDistribution::UniformIntDistribution( int min, int max )
: m_urd( min, max ),
  m_var_uni( m_rng, m_urd )
{
   m_rng.seed( static_cast<std::uint32_t>( clock() ) );
}

double UniformIntDistribution::operator()()
{
   return m_var_uni();
}

