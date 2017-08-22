#include "SymmMatrix.h"

#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <algorithm>
#include <limits>

using namespace std;

SymmMatrix::SymmMatrix( size_t dim )
: m_dim( dim ),
  m_elements( NULL )
{
   int len = m_dim*(m_dim+1)/2;
   m_elements = new double[len];
}

SymmMatrix::SymmMatrix( SymmMatrix& symmMatrix )
{
   m_dim = symmMatrix.m_dim;
   int len = m_dim*(m_dim+1)/2;
   m_elements = new double[len];
   copy( symmMatrix.m_elements, m_elements, m_elements + len );
}

SymmMatrix::~SymmMatrix()
{
   if( NULL != m_elements )
   {
      delete m_elements;
   }
}

void SymmMatrix::set( size_t row, size_t col, double element )
{
   if( m_dim*m_dim <= row*m_dim+col )
   {
      throw runtime_error( "Indicies outside the matrix's boundaries!" );
   }

   else if( row <= col )
   {
      int blk = row*(row-1)/2;
      //cout << "(" << row << ", " << col << ") -> " << row*m_dim-blk+col-row << endl;
      m_elements[row*m_dim-blk+col-row] = element;
   }
   else
   {
      int blk = col*(col-1)/2;
      //cout << "(" << row << ", " << col << ") -> " << col*m_dim-blk+row-col << endl;
      m_elements[col*m_dim-blk+row-col] = element;
   }
}

double SymmMatrix::get( size_t row, size_t col )
{
   double e = 0;
   if( m_dim*m_dim <= row*m_dim+col )
   {
      throw runtime_error( "Indicies outside the matrix's boundaries!" );
   }

   else if( row <= col )
   {
      int blk = row*(row-1)/2;
      e = m_elements[row*m_dim-blk+col-row];
   }
   else
   {
      int blk = col*(col-1)/2;
      e = m_elements[col*m_dim-blk+row-col];
   }
   return e;
}

int SymmMatrix::side()
{
   return m_dim;
}

void SymmMatrix::setToInf( size_t idx )
{
   for( size_t row = 0 ; row < m_dim ; ++row )
   {
      if( row < idx )
      {
         int blk = row*(row+1)/2;
         m_elements[row*m_dim-blk+idx-(row+1)] = numeric_limits<double>::max();
      }
      else if( row > idx )
      {
         int blk = idx*(idx+1)/2;
         m_elements[idx*m_dim-blk+row-(idx+1)] = numeric_limits<double>::max();
      }
   }         
}

void SymmMatrix::print()
{
   cout << endl;
   for( size_t i = 0 ; i < m_dim ; ++i )
   {
      for( size_t j = 0 ; j < m_dim ; ++j )
      {
         double n = get( i, j );
         if( n == numeric_limits<double>::max() )
         {
            cout << setw(3) << "-";
         }
         else
         {
            cout << setw(3) << n;
         }
      }
      cout << endl;
   }
   cout << endl;
}


