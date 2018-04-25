#ifndef __PRL_EM_NDCG_H__
#define __PRL_EM_NDCG_H__

#include <string>
#include <vector>
#include <utility>

class NDCG
{
public:

   void append( std::vector<std::string>& retrieved, std::vector<std::string>& relevant );

   void clear();

   double eval();

private:

   size_t m_numQueries;

   double m_cumNDCG;

};

#endif // __PRL_EM_NDCG_H__


