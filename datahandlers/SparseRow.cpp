#include "SparseRow.h"

#include <boost/numeric/ublas/io.hpp>

#include <iostream>


SparseRow::SparseRow()
: m_prowmat( NULL )
{
}

SparseRow::SparseRow( sparse_matrix_t& smatrix, size_t row )
{
   m_prowmat = new matrix_row_t( smatrix, row );
}

SparseRow::~SparseRow()
{
   if( NULL != m_prowmat )
   {
      //delete m_prowmat;
   }
}

void SparseRow::set( size_t i, double value )
{
   if( NULL != m_prowmat )
   {
      m_prowmat->operator()( i ) = value;
   }
}

double SparseRow::get( size_t i )
{
   double value = 0;
   if( NULL != m_prowmat )
   {
      value = m_prowmat->operator()( i );
   }
   return value;
}

size_t SparseRow::size()
{
   size_t size = 0;
   if( NULL != m_prowmat )
   {
      size = m_prowmat->size();
   }
   return size;
}

double SparseRow::mean()
{
   double sum = 0;
   double num = 0;
   SparseRow::iterator ind;
   SparseRow::iterator end = m_prowmat->end();
   for( ind = m_prowmat->begin() ; ind != end ; ++ind )
   {
      sum += *ind;
      ++num;
   }
   return sum/num;
}

SparseRow::iterator SparseRow::begin()
{
   SparseRow::iterator it;
   if( NULL != m_prowmat )
   {
      it = m_prowmat->begin();
   }
   return it;
}

SparseRow::iterator SparseRow::end()
{
   SparseRow::iterator it;
   if( NULL != m_prowmat )
   {
      it = m_prowmat->end();
   }
   return it;
}

void SparseRow::print()
{
   std::cout << "vector: " << *m_prowmat << std::endl;
}

