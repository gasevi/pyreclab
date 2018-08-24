#include "DataFrame.h"

#include <iostream>
#include <vector>
#include <sstream>

using namespace std;


DataFrame::DataFrame()
{
}

DataFrame::DataFrame( DataReader& dreader, int userpos, int itempos, int ratingpos )
{
   while( !dreader.eof() )
   {
      vector<string> line;
      dreader.readline( line );
      if( line.empty() )
      {
         break;
      }
      int nfields = static_cast<int>( line.size() );

      string userId = line[userpos];

      string itemId = "";
      if( itempos >= 0 && nfields > itempos )
      {
         itemId = line[itempos];
      }

      double rating = -1;
      if( ratingpos >= 0 && nfields > ratingpos )
      {
         std::stringstream ss( line[ratingpos] );
         ss >> rating;
      }

      //cout << "user [" << userId << "] item [" << itemId << "] rating: [" << rating << "]" << endl;
      if( m_userItemPairs.find( pair<string,string>( userId, itemId ) ) != m_userItemPairs.end() )
      {
         cout << "Warning: <user: " << userId
              << " ,item: " << itemId
              << "> pair duplicated on line " << dreader.currentline()
              << endl;
         continue;
      }
      m_userItemPairs[pair<string,string>( userId, itemId )] = rating;
      m_userConsumption[userId].push_back( pair<string, double>( itemId, rating ) );
   }
}

size_t DataFrame::rows()
{
   return m_userItemPairs.size();
}

size_t DataFrame::columns()
{
   return 3;
}

vector<string> DataFrame::filter( std::string userId, double ratingThr )
{
   vector<string> data;
   if( m_userConsumption.find( userId ) != m_userConsumption.end() )
   {
      vector< pair<string, double> >::iterator ind;
      vector< pair<string, double> >::iterator end = m_userConsumption[userId].end();
      for( ind = m_userConsumption[userId].begin() ; ind != end ; ++ind )
      {
         if( ind->second >= ratingThr )
         {
            data.push_back( ind->first );
         }
      }
   }
   return data;
}

DataFrame::iterator DataFrame::begin()
{
   return m_userItemPairs.begin();
}

DataFrame::iterator DataFrame::end()
{
   return m_userItemPairs.end();
}

DataFrame& DataFrame::operator=( DataFrame& dataFrame )
{
   if( this == &dataFrame )
   {
      return *this;
   }
   m_userItemPairs = dataFrame.m_userItemPairs;
   return *this;
}


