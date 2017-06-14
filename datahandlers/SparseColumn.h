#ifndef __SPARSE_COLUMN_H__
#define __SPARSE_COLUMN_H__

#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <iostream>


template<class smatrix_t>
class SparseColumn
{
public:

   typedef boost::numeric::ublas::matrix_column< smatrix_t > matrix_column_t;

   typedef typename boost::numeric::ublas::matrix_column< smatrix_t >::iterator iterator;

   SparseColumn();

   SparseColumn( const SparseColumn& scol );

   SparseColumn( smatrix_t& smatrix, size_t column );

   ~SparseColumn();

   double get( size_t i );

   size_t size();

   double mean();

   iterator begin();

   iterator end();

   void print();

private:

   matrix_column_t* m_pcolmat;

};


template<class smatrix_t>
SparseColumn<smatrix_t>::SparseColumn()
: m_pcolmat( NULL )
{
}

template<class smatrix_t>
SparseColumn<smatrix_t>::SparseColumn( smatrix_t& smatrix, size_t column )
{
   m_pcolmat = new matrix_column_t( smatrix, column );
}

template<class smatrix_t>
SparseColumn<smatrix_t>::~SparseColumn()
{
   if( NULL != m_pcolmat )
   {
      //delete m_pcolmat;
   }
}

template<class smatrix_t>
double SparseColumn<smatrix_t>::get( size_t i )
{
   return m_pcolmat->operator()( i );
}

template<class smatrix_t>
size_t SparseColumn<smatrix_t>::size()
{
   size_t size = 0;
   if( NULL != m_pcolmat )
   {
      size = m_pcolmat->size();
   }
   return size;
}

template<class smatrix_t>
double SparseColumn<smatrix_t>::mean()
{
   double sum = 0;
   double num = 0;
   SparseColumn<smatrix_t>::iterator ind;
   SparseColumn<smatrix_t>::iterator end = m_pcolmat->end();
   for( ind = m_pcolmat->begin() ; ind != end ; ++ind )
   {
      sum += *ind;
      ++num;
   }
   return sum/num;
}

template<class smatrix_t>
typename SparseColumn<smatrix_t>::iterator SparseColumn<smatrix_t>::begin()
{
   SparseColumn<smatrix_t>::iterator it;
   if( NULL != m_pcolmat )
   {
      it = m_pcolmat->begin();
   }
   return it;
}

template<class smatrix_t>
typename SparseColumn<smatrix_t>::iterator SparseColumn<smatrix_t>::end()
{
   SparseColumn<smatrix_t>::iterator it;
   if( NULL != m_pcolmat )
   {
      it = m_pcolmat->end();
   }
   return it;
}

template<class smatrix_t>
void SparseColumn<smatrix_t>::print()
{
   std::cout << "vector: " << *m_pcolmat << std::endl;
}

#endif // __SPARSE_COLUMN_H__

