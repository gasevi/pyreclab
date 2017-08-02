#include "DenseMatrix.h"

#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <algorithm>
#include <limits>

using namespace std;

DenseMatrix::DenseMatrix( size_t rows, size_t columns )
: m_rows( rows ),
  m_columns( columns ),
  m_elements( NULL )
{
   m_elements = new double[rows*columns];
}

DenseMatrix::DenseMatrix( DenseMatrix& matrix )
{
   m_rows = matrix.m_rows;
   m_columns = matrix.m_columns;
   m_elements = new double[m_rows*m_columns];
   copy( matrix.m_elements, m_elements, m_elements + m_rows*m_columns );
}

DenseMatrix::~DenseMatrix()
{
   if( NULL != m_elements )
   {
      delete m_elements;
   }
}

void DenseMatrix::set( size_t row, size_t col, double element )
{
   if( m_rows <= row || m_columns <= col )
   {
      throw runtime_error( "Indicies outside the matrix's boundaries!" );
   }

   m_elements[row*m_columns+col] = element;
}

double DenseMatrix::get( size_t row, size_t col )
{
   if( m_rows <= row || m_columns <= col )
   {
      throw runtime_error( "Indicies outside the matrix's boundaries!" );
   }

   return m_elements[row*m_columns+col];
}

void DenseMatrix::add( size_t row, size_t col, double element )
{
   m_elements[row*m_columns+col] += element;
}

size_t DenseMatrix::rows()
{
   return m_rows;
}

size_t DenseMatrix::columns()
{
   return m_columns;
}

void DenseMatrix::print()
{
   cout << endl;
   for( size_t i = 0 ; i < m_rows ; ++i )
   {
      for( size_t j = 0 ; j < m_columns ; ++j )
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


