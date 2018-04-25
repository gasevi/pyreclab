#ifndef __PRL_EM_MAP_H__
#define __PRL_EM_MAP_H__

#include <string>
#include <vector>
#include <utility>

class MAP
{
public:

   void append( std::vector<std::string>& retrieved, std::vector<std::string>& relevant );

   void clear();

   double eval();

private:

   std::vector<double> m_avgPrecision;

};

#endif // __PRL_EM_MAP_H__


