#include "SparseColumn.h"

#include <iostream>
#include <boost/numeric/ublas/io.hpp>


SparseColumn::SparseColumn()
: m_pcolmat( NULL )
{
}

SparseColumn::SparseColumn( sparse_matrix_t& smatrix, size_t column )
{
   m_pcolmat = new matrix_column_t( smatrix, column );
}

SparseColumn::~SparseColumn()
{
   if( NULL != m_pcolmat )
   {
      //delete m_pcolmat;
   }
}

double SparseColumn::get( size_t i )
{
   return m_pcolmat->operator()( i );
}

size_t SparseColumn::size()
{
   size_t size = 0;
   if( NULL != m_pcolmat )
   {
      size = m_pcolmat->size();
   }
   return size;
}

double SparseColumn::mean()
{
   double sum = 0;
   double num = 0;
   SparseColumn::iterator ind;
   SparseColumn::iterator end = m_pcolmat->end();
   for( ind = m_pcolmat->begin() ; ind != end ; ++ind )
   {
      sum += *ind;
      ++num;
   }
   return sum/num;
}

SparseColumn::iterator SparseColumn::begin()
{
   SparseColumn::iterator it;
   if( NULL != m_pcolmat )
   {
      it = m_pcolmat->begin();
   }
   return it;
}

SparseColumn::iterator SparseColumn::end()
{
   SparseColumn::iterator it;
   if( NULL != m_pcolmat )
   {
      it = m_pcolmat->end();
   }
   return it;
}

void SparseColumn::print()
{
   std::cout << "vector: " << *m_pcolmat << std::endl;
}


