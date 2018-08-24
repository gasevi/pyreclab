#ifndef __DATA_FRAME_H__
#define __DATA_FRAME_H__

#include "DataReader.h"

#include <string>
#include <map>
#include <utility>


class DataFrame
{
public:

   typedef std::map< std::pair<std::string,std::string>, double >::iterator iterator;

   DataFrame();

   DataFrame( DataReader& dreader, int userpos = 0, int itempos = 1, int ratingpos = -1 );

   size_t columns();

   size_t rows();

   std::vector<std::string> filter( std::string userId, double ratingThr = 0 );

   iterator begin();

   iterator end();

   DataFrame& operator=( DataFrame& dataFrame );

private:

   std::map< std::pair<std::string,std::string>, double > m_userItemPairs;

   std::map< std::string, std::vector< std::pair<std::string, double> > > m_userConsumption;

};

#endif // __DATA_FRAME_H__

