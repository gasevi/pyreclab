#include "RatingMatrix.h"

#include <vector>
#include <map>
#include <utility>
#include <climits>

using namespace std;


RatingMatrix::RatingMatrix()
{
}

RatingMatrix::RatingMatrix( DataReader& dreader,
                            int userpos,
                            int itempos,
                            int ratingpos,
                            SparseMatrix::EOrder order )
: m_numRatings( 0 ),
  m_sumRatings( 0 ),
  m_minRating( INT_MAX ),
  m_maxRating( INT_MIN ),
  m_smatrix( order )
{
   map< pair<size_t,size_t>, double > inputData;
   while( !dreader.eof() )
   {
      vector<string> line = dreader.readline();
      if( line.empty() )
      {
         break;
      }
      string userId = line[userpos];
      string itemId = line[itempos];
      std::stringstream ss( line[ratingpos] );
      double rating;
      ss >> rating;

      //cout << "user [" << userId << "] item [" << itemId << "] rating: [" << rating << "]" << endl;
      size_t row, col;
      map<string,size_t>::iterator ind;

      ind = m_userMapper.find( userId );
      if( ind == m_userMapper.end() )
      {
         row = m_userMapper.size();
         m_userMapper[userId] = row;
      }
      else
      {
         row = ind->second;
      }

      ind = m_itemMapper.find( itemId );
      if( ind == m_itemMapper.end() )
      {
         col = m_itemMapper.size();
         m_itemMapper[itemId] = col;
      }
      else
      {
         col = ind->second;
      }

      if( inputData.find( pair<size_t,size_t>( row, col ) ) != inputData.end() )
      {
         cout << "warning: <user:" << userId << " ,item:" << itemId << "> pair duplicated" << endl;
         continue;
      }
      inputData[pair<size_t,size_t>( row, col )] = rating;

      // Statistical data
      ++m_numRatings;
      m_sumRatings += rating;
      if( rating < m_minRating )
      {
         m_minRating = rating;
      }
      if( rating > m_maxRating )
      {
         m_maxRating = rating;
      }
   }

   m_smatrix.resize( m_userMapper.size(), m_itemMapper.size() );
   map< pair<size_t,size_t>, double >::iterator ind;
   map< pair<size_t,size_t>, double >::iterator end = inputData.end();
   for( ind = inputData.begin() ; ind != end ; ++ind )
   {
      size_t row = ind->first.first;
      size_t col = ind->first.second;
      double rating = ind->second;

      //cout << "row: " << row << " col: " << col << " rating: " << rating << endl;
      m_smatrix.set( row, col, rating );
   }
   //cout << "min rating: " << m_minRating << " max rating: " << m_maxRating << endl;
   //cout << "dim: " << m_smatrix.rows() << "x" << m_smatrix.columns() << endl;
   //m_smatrix.print();
}

size_t RatingMatrix::users()
{
   return m_smatrix.rows();
}

size_t RatingMatrix::items()
{
   return m_smatrix.columns();
}

double RatingMatrix::get( size_t row, size_t col )
{
   return m_smatrix.get( row, col );
}

string RatingMatrix::userId( size_t row )
{
   string userId;
   map<string,size_t>::iterator ind;
   map<string,size_t>::iterator end = m_userMapper.end();
   for( ind = m_userMapper.begin() ; ind != end ; ++ind )
   {
      if( ind->second == row )
      {
         userId = ind->first;
         break;
      }
   }
   return userId;
}

string RatingMatrix::itemId( size_t col )
{
   string itemId;
   map<string,size_t>::iterator ind;
   map<string,size_t>::iterator end = m_itemMapper.end();
   for( ind = m_itemMapper.begin() ; ind != end ; ++ind )
   {
      if( ind->second == col )
      {
         itemId = ind->first;
         break;
      }
   }
   return itemId;
}

int RatingMatrix::row( std::string userId )
{
   int row = -1;
   if( m_userMapper.find( userId ) != m_userMapper.end() )
   {
      row = m_userMapper[userId];
   }
   return row;
}

int RatingMatrix::column( std::string itemId )
{
   int column = -1;
   if( m_itemMapper.find( itemId ) != m_itemMapper.end() )
   {
      column = m_itemMapper[itemId];
   }
   return column;
}

SparseRow RatingMatrix::userVector( size_t row )
{
   SparseRow uvec;
   if( row < m_smatrix.rows() )
   {
      uvec = m_smatrix.row( row );
   }
   return uvec;
}

SparseRow RatingMatrix::userVector( string userId )
{
   SparseRow uvec;
   if( m_userMapper.find( userId ) != m_userMapper.end() )
   {
      size_t row = m_userMapper[userId];
      uvec = m_smatrix.row( row );
   }
   return uvec;
}

SparseColumn RatingMatrix::itemVector( size_t col )
{
   SparseColumn ivec;
   if( col < m_smatrix.columns() )
   {
      ivec = m_smatrix.column( col );
   }
   return ivec;
}

SparseColumn RatingMatrix::itemVector( string itemId )
{
   SparseColumn ivec;
   if( m_itemMapper.find( itemId ) != m_itemMapper.end() )
   {
      size_t col = m_itemMapper[itemId];
      ivec = m_smatrix.column( col );
   }
   return ivec;
}

int RatingMatrix::nonZeroRow( size_t row )
{
   m_smatrix.nonZeroRow( row );
}

size_t RatingMatrix::numRatings()
{
   return m_numRatings;
}

double RatingMatrix::sumRatings()
{
   return m_sumRatings;
}

int RatingMatrix::nonZeroCol( size_t col )
{
   m_smatrix.nonZeroCol( col );
}

int RatingMatrix::sumRow( size_t row, double& sum )
{
   return m_smatrix.sumRow( row, sum );
}

int RatingMatrix::sumColumn( size_t col, double& sum )
{
   return m_smatrix.sumColumn( col, sum );
}

double RatingMatrix::meanRow( size_t row )
{
   return m_smatrix.meanRow( row );
}

double RatingMatrix::meanColumn( size_t col )
{
   return m_smatrix.meanColumn( col );
}

int RatingMatrix::sumDiffColumns( size_t i, size_t j, double& sumdiff )
{
   return m_smatrix.sumDiffColumns( i, j, sumdiff );
}


