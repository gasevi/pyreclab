#ifndef __SPARSE_ROW_H__
#define __SPARSE_ROW_H__

#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <iostream>


template<class smatrix_t>
class SparseRow
{
public:

   typedef boost::numeric::ublas::matrix_row< smatrix_t > matrix_row_t;

   typedef typename boost::numeric::ublas::matrix_row< smatrix_t >::iterator iterator;

   SparseRow();

   SparseRow( const SparseRow& srow );

   SparseRow( smatrix_t& smatrix, size_t row );

   ~SparseRow();

   void set( size_t i, double value );

   double get( size_t i );

   size_t size();

   double mean();

   iterator begin();

   iterator end();

   void print();

private:

   matrix_row_t* m_prowmat;

};

template<class smatrix_t>
SparseRow<smatrix_t>::SparseRow()
: m_prowmat( NULL )
{
}

template<class smatrix_t>
SparseRow<smatrix_t>::SparseRow( smatrix_t& smatrix, size_t row )
{
   m_prowmat = new matrix_row_t( smatrix, row );
}

template<class smatrix_t>
SparseRow<smatrix_t>::~SparseRow()
{
   if( NULL != m_prowmat )
   {
      //delete m_prowmat;
   }
}

template<class smatrix_t>
void SparseRow<smatrix_t>::set( size_t i, double value )
{
   if( NULL != m_prowmat )
   {
      m_prowmat->operator()( i ) = value;
   }
}

template<class smatrix_t>
double SparseRow<smatrix_t>::get( size_t i )
{
   double value = 0;
   if( NULL != m_prowmat )
   {
      value = m_prowmat->operator()( i );
   }
   return value;
}

template<class smatrix_t>
size_t SparseRow<smatrix_t>::size()
{
   size_t size = 0;
   if( NULL != m_prowmat )
   {
      size = m_prowmat->size();
   }
   return size;
}

template<class smatrix_t>
double SparseRow<smatrix_t>::mean()
{
   double sum = 0;
   double num = 0;
   SparseRow<smatrix_t>::iterator ind;
   SparseRow<smatrix_t>::iterator end = m_prowmat->end();
   for( ind = m_prowmat->begin() ; ind != end ; ++ind )
   {
      sum += *ind;
      ++num;
   }
   return sum/num;
}

template<class smatrix_t>
typename SparseRow<smatrix_t>::iterator SparseRow<smatrix_t>::begin()
{
   SparseRow<smatrix_t>::iterator it;
   if( NULL != m_prowmat )
   {
      it = m_prowmat->begin();
   }
   return it;
}

template<class smatrix_t>
typename SparseRow<smatrix_t>::iterator SparseRow<smatrix_t>::end()
{
   SparseRow<smatrix_t>::iterator it;
   if( NULL != m_prowmat )
   {
      it = m_prowmat->end();
   }
   return it;
}

template<class smatrix_t>
void SparseRow<smatrix_t>::print()
{
   std::cout << "vector: " << *m_prowmat << std::endl;
}

#endif // __SPARSE_ROW_H__

