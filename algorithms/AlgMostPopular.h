#ifndef __ALG_MOST_POPULAR_H__
#define __ALG_MOST_POPULAR_H__

#include "RecSysAlgorithm.h"

#include <string>
#include <vector>


class AlgMostPopular
      : public RecSysAlgorithm
{
public:

   AlgMostPopular( RatingMatrix& ratingMatrix );

   int train();

   void test( DataFrame& dataFrame );

   bool recommend( const std::string& userId, size_t n, std::vector<std::string>& ranking );
 
private:

   size_t m_topn;

   std::vector< std::pair<double, size_t> > m_itemList;

};

#endif // __ALG_MOST_POPULAR_H__

