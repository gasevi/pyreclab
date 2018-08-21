#ifndef __RATING_MATRIX_H__
#define __RATING_MATRIX_H__

#include "DataReader.h"
#include "SparseMatrix.h"
#include "SparseRow.h"

#include <vector>
#include <map>
#include <string>
#include <utility>
#include <climits>


template <class smatrix_t>
class RatingMatrix
{
public:

   RatingMatrix();

   RatingMatrix( DataReader& dreader,
                 int userpos = 0,
                 int itempos = 1,
                 int ratingpos = 2 );

   size_t items();

   size_t users();

   double get( size_t row, size_t col );

   std::string userId( size_t row );

   std::string itemId( size_t col );

   int row( std::string userId );

   int column( std::string itemId );

   SparseRow<smatrix_t> userVector( size_t row );

   SparseRow<smatrix_t> userVector( std::string userId );

   SparseColumn<smatrix_t> itemVector( size_t col );

   SparseColumn<smatrix_t> itemVector( std::string itemId );

   size_t numRatings();

   double sumRatings();

   int nonZeroRow( size_t row );

   int nonZeroCol( size_t col );

   int sumRow( size_t row, double& sum );

   int sumColumn( size_t col, double& sum );

   double meanRow( size_t row );

   double meanColumn( size_t col );

   int sumDiffColumns( size_t i, size_t j, double& sumdiff );

private:

   static
   const int kNumFieldsPerRow = 3;

   std::map<std::string, size_t> m_fwUserMapper;

   std::map<size_t, std::string> m_bwUserMapper;

   std::map<std::string, size_t> m_fwItemMapper;

   std::map<size_t, std::string> m_bwItemMapper;

   SparseMatrix<smatrix_t> m_smatrix;

   size_t m_numRatings;

   double m_sumRatings;

   double m_minRating;

   double m_maxRating;

};


template <class smatrix_t>
RatingMatrix<smatrix_t>::RatingMatrix()
{
}

template <class smatrix_t>
RatingMatrix<smatrix_t>::RatingMatrix( DataReader& dreader,
                                       int userpos,
                                       int itempos,
                                       int ratingpos )
: m_numRatings( 0 ),
  m_sumRatings( 0 ),
  m_minRating( INT_MAX ),
  m_maxRating( INT_MIN )
{
   std::map< std::pair<size_t,size_t>, double > inputData;
   while( !dreader.eof() )
   {
      std::vector<std::string> line;
      dreader.readline( line );
      if( line.empty() )
      {
         break;
      }

      if( line.size() < kNumFieldsPerRow )
      {
         std::cerr << "Warning: line " << dreader.currentline()
                   << " of file " << dreader.filename()
                   << " does not have enough fields. Discarded"
                   << std::endl;
         continue;
      }

      std::string userId = line[userpos];
      std::string itemId = line[itempos];
      std::stringstream ss( line[ratingpos] );
      double rating;
      ss >> rating;

      //std::cout << "user [" << userId << "] item [" << itemId << "] rating: [" << rating << "]" << std::endl;
      size_t row, col;
      std::map<std::string,size_t>::iterator ind;

      ind = m_fwUserMapper.find( userId );
      if( ind == m_fwUserMapper.end() )
      {
         row = m_fwUserMapper.size();
         m_fwUserMapper[userId] = row;
         m_bwUserMapper[row] = userId;
      }
      else
      {
         row = ind->second;
      }

      ind = m_fwItemMapper.find( itemId );
      if( ind == m_fwItemMapper.end() )
      {
         col = m_fwItemMapper.size();
         m_fwItemMapper[itemId] = col;
         m_bwItemMapper[col] = itemId;
      }
      else
      {
         col = ind->second;
      }

      if( inputData.find( std::pair<size_t,size_t>( row, col ) ) != inputData.end() )
      {
         std::cout << "warning: <user: " << userId
                   << " ,item: " << itemId
                   << "> pair duplicated on line " << dreader.currentline()
                   << std::endl;
         continue;
      }
      inputData[std::pair<size_t,size_t>( row, col )] = rating;

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

   m_smatrix.resize( m_fwUserMapper.size(), m_fwItemMapper.size() );
   std::map< std::pair<size_t,size_t>, double >::iterator ind;
   std::map< std::pair<size_t,size_t>, double >::iterator end = inputData.end();
   for( ind = inputData.begin() ; ind != end ; ++ind )
   {
      size_t row = ind->first.first;
      size_t col = ind->first.second;
      double rating = ind->second;

      //std::cout << "row: " << row << " col: " << col << " rating: " << rating << std::endl;
      m_smatrix.set( row, col, rating );
   }
   //std::cout << "min rating: " << m_minRating << " max rating: " << m_maxRating << std::endl;
   //std::cout << "dim: " << m_smatrix.rows() << "x" << m_smatrix.columns() << std::endl;
   //m_smatrix.print();
}

template <class smatrix_t>
size_t RatingMatrix<smatrix_t>::users()
{
   return m_smatrix.rows();
}

template <class smatrix_t>
size_t RatingMatrix<smatrix_t>::items()
{
   return m_smatrix.columns();
}

template <class smatrix_t>
double RatingMatrix<smatrix_t>::get( size_t row, size_t col )
{
   return m_smatrix.get( row, col );
}

template <class smatrix_t>
std::string RatingMatrix<smatrix_t>::userId( size_t row )
{
   std::string userId;
   if( m_bwUserMapper.find( row ) != m_bwUserMapper.end() )
   {
      userId = m_bwUserMapper[row];
   }
   return userId;
}

template <class smatrix_t>
std::string RatingMatrix<smatrix_t>::itemId( size_t col )
{
   std::string itemId;
   if( m_bwItemMapper.find( col ) != m_bwItemMapper.end() )
   {
      itemId = m_bwItemMapper[col];
   }
   return itemId;
}

template <class smatrix_t>
int RatingMatrix<smatrix_t>::row( std::string userId )
{
   int row = -1;
   if( m_fwUserMapper.find( userId ) != m_fwUserMapper.end() )
   {
      row = m_fwUserMapper[userId];
   }
   return row;
}

template <class smatrix_t>
int RatingMatrix<smatrix_t>::column( std::string itemId )
{
   int column = -1;
   if( m_fwItemMapper.find( itemId ) != m_fwItemMapper.end() )
   {
      column = m_fwItemMapper[itemId];
   }
   return column;
}

template <class smatrix_t>
SparseRow<smatrix_t> RatingMatrix<smatrix_t>::userVector( size_t row )
{
   SparseRow<smatrix_t> uvec;
   if( row < m_smatrix.rows() )
   {
      uvec = m_smatrix.row( row );
   }
   return uvec;
}

template <class smatrix_t>
SparseRow<smatrix_t> RatingMatrix<smatrix_t>::userVector( std::string userId )
{
   SparseRow<smatrix_t> uvec;
   if( m_fwUserMapper.find( userId ) != m_fwUserMapper.end() )
   {
      size_t row = m_fwUserMapper[userId];
      uvec = m_smatrix.row( row );
   }
   return uvec;
}

template <class smatrix_t>
SparseColumn<smatrix_t> RatingMatrix<smatrix_t>::itemVector( size_t col )
{
   SparseColumn<smatrix_t> ivec;
   if( col < m_smatrix.columns() )
   {
      ivec = m_smatrix.column( col );
   }
   return ivec;
}

template <class smatrix_t>
SparseColumn<smatrix_t> RatingMatrix<smatrix_t>::itemVector( std::string itemId )
{
   SparseColumn<smatrix_t> ivec;
   if( m_fwItemMapper.find( itemId ) != m_fwItemMapper.end() )
   {
      size_t col = m_fwItemMapper[itemId];
      ivec = m_smatrix.column( col );
   }
   return ivec;
}

template <class smatrix_t>
int RatingMatrix<smatrix_t>::nonZeroRow( size_t row )
{
   return m_smatrix.nonZeroRow( row );
}

template <class smatrix_t>
size_t RatingMatrix<smatrix_t>::numRatings()
{
   return m_numRatings;
}

template <class smatrix_t>
double RatingMatrix<smatrix_t>::sumRatings()
{
   return m_sumRatings;
}

template <class smatrix_t>
int RatingMatrix<smatrix_t>::nonZeroCol( size_t col )
{
   return m_smatrix.nonZeroCol( col );
}

template <class smatrix_t>
int RatingMatrix<smatrix_t>::sumRow( size_t row, double& sum )
{
   return m_smatrix.sumRow( row, sum );
}

template <class smatrix_t>
int RatingMatrix<smatrix_t>::sumColumn( size_t col, double& sum )
{
   return m_smatrix.sumColumn( col, sum );
}

template <class smatrix_t>
double RatingMatrix<smatrix_t>::meanRow( size_t row )
{
   return m_smatrix.meanRow( row );
}

template <class smatrix_t>
double RatingMatrix<smatrix_t>::meanColumn( size_t col )
{
   return m_smatrix.meanColumn( col );
}

template <class smatrix_t>
int RatingMatrix<smatrix_t>::sumDiffColumns( size_t i, size_t j, double& sumdiff )
{
   return m_smatrix.sumDiffColumns( i, j, sumdiff );
}

#endif // __RATING_MATRIX_H__


