#ifndef __SIMILARITY_METRICS_H__
#define __SIMILARITY_METRICS_H__

#include <string>

template<class VectorType>
class SimStrategy
{
public:

   double similarity( VectorType& v1, VectorType& v2 )
   {
      return correlate( v1, v2 );
   }

private:

   virtual
   double correlate( VectorType& v1, VectorType& v2 ) = 0;

};


template<class VectorType>
class SimCosine
      : public SimStrategy<VectorType>
{
private:

   double correlate( VectorType& v1, VectorType& v2 )
   {
      if( v1.size() != v2.size() )
      {
         throw "Vectors must be the same lengths";
      }

      double dotp = 0;
      double mod1 = 0;
      double mod2 = 0;

      typename VectorType::iterator ind;
      typename VectorType::iterator end = v1.end();
      for( ind = v1.begin() ; ind != end ; ++ind )
      {
         dotp += (*ind) * v2.get( ind.index() );
         mod1 += (*ind) * (*ind);
      }

      end = v2.end();
      for( ind = v2.begin() ; ind != end ; ++ind )
      {
         mod2 += (*ind) * (*ind);
      }

      return dotp / ( sqrt( mod1 ) * sqrt( mod2 ) );
   }
};


template<class VectorType>
class SimPearson
      : public SimStrategy<VectorType>
{
private:

   double correlate( VectorType& v1, VectorType& v2 )
   {
      if( v1.size() != v2.size() )
      {
         throw "Vectors must be the same lengths";
      }

      double mean1 = v1.mean();
      double mean2 = v2.mean();

      double devp = 0;
      double dev1 = 0;
      double dev2 = 0;

      typename VectorType::iterator ind;
      typename VectorType::iterator end = v1.end();
      for( ind = v1.begin() ; ind != end ; ++ind )
      {
         double rv1 = *ind;
         double rv2 = v2.get( ind.index() );

         if( rv2 > 0 )
         {
            devp += ( rv1 - mean1 ) * ( rv2 - mean2 );
            dev1 += ( rv1 - mean1 ) * ( rv1 - mean1 );
            dev2 += ( rv2 - mean2 ) * ( rv2 - mean2 );
         }
      }

      double sim = 0;
      if( ( dev1 != 0 ) && ( dev2 != 0 ) )
      {
         sim = devp / ( sqrt( dev1 ) * sqrt( dev2 ) );
      }

      return sim;
   }
};


template<class VectorType>
class Similarity
{
public:

   enum SimType
   {
      COSINE,
      PEARSON
   };

   Similarity()
   : m_similarity( NULL )
   {
   }

   Similarity( SimType simType )
   : m_similarity( NULL )
   {
      set( simType );
   }

   Similarity( std::string& simType )
   : m_similarity( NULL )
   {
      if( simType.empty() )
      {
         set( Similarity<VectorType>::PEARSON );
      }
      else
      {
         set( simType );
      }
   }

   ~Similarity()
   {
      if( NULL != m_similarity )
      {
         delete m_similarity;
      }
   }

   void set( std::string& strSimType )
   {
      transform( strSimType.begin(), strSimType.end(), strSimType.begin(), ::tolower );
      if( strSimType == "pearson" )
      {
         set( Similarity<VectorType>::PEARSON );
      }
      else if( strSimType == "cosine" )
      {
         set( Similarity<VectorType>::COSINE );
      }
      else
      {
         throw "Unknown similarity metric";
      }
   }

   void set( SimType simType )
   {
      if( NULL != m_similarity )
      {
         delete m_similarity;
      }

      switch( simType )
      {
      case COSINE:
         m_similarity = new SimCosine<VectorType>();
         break;
      case PEARSON:
         m_similarity = new SimPearson<VectorType>();
         break;
      }
   }

   double calculate( VectorType& v1, VectorType& v2 )
   {
      return m_similarity->similarity( v1, v2 );
   }

private:

   SimStrategy<VectorType>* m_similarity;

};


#endif // __SIMILARITY_METRICS_H__

