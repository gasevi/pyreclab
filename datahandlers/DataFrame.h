#ifndef __DATA_FRAME_H__
#define __DATA_FRAME_H__

#include "DataReader.h"
#include "SparseMatrix.h"

#include <string>
#include <map>
#include <utility>


class DataFrame
{
public:

   typedef std::map< std::pair<std::string,std::string>, double >::iterator iterator;

   DataFrame();

   DataFrame( DataReader& dreader, int userpos = 1, int itempos = 2, int ratingpos = -1 );

   size_t columns();

   size_t rows();

   iterator begin();

   iterator end();

   DataFrame& operator=( DataFrame& dataFrame );

private:

   std::map<std::pair<std::string,std::string>,double> m_userItemPairs;

};

#endif // __DATA_FRAME_H__

