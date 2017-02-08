#include "PyUserAvg.h"
#include "AlgFactory.h"
#include "DataWriter.h"

#include <iostream>
#include <string>
#include <sstream>

using namespace std;


PyObject* UserAvg_new( PyTypeObject* type, PyObject* args, PyObject* kwdict )
{
   const char* dsfilename = NULL;
   char dlmchar = ',';
   int header = 0;
   int usercol = 0;
   int itemcol = 1;
   int ratingcol = 2;

   static char* kwlist[] = { const_cast<char*>( "dataset" ),
                             const_cast<char*>( "dlmchar" ),
                             const_cast<char*>( "header" ),
                             const_cast<char*>( "usercol" ),
                             const_cast<char*>( "itemcol" ),
                             const_cast<char*>( "ratingcol" ),
                             NULL };

   if( !PyArg_ParseTupleAndKeywords( args, kwdict, "s|ciiii", kwlist, &dsfilename,
                                     &dlmchar, &header, &usercol, &itemcol, &ratingcol ) )
   {
      return NULL;
   }

   if( NULL == dsfilename )
   {
      return NULL;
   }

   Recommender* self = reinterpret_cast<Recommender*>( type->tp_alloc( type, 0 ) );
   if( self != NULL )
   {
      self->m_trainingReader = new DataReader( dsfilename, dlmchar, header );
      if( NULL == self->m_trainingReader )
      {
         Py_DECREF( self );
         return NULL;
      }

      self->m_ratingMatrix = new RatingMatrix( *(self->m_trainingReader), usercol, itemcol, ratingcol );
      if( NULL == self->m_ratingMatrix )
      {
         Py_DECREF( self );
         return NULL;
      }

      self->m_recAlgorithm = AlgFactory::getInstance( "UserAvg", *(self->m_ratingMatrix) );
      if( NULL == self->m_recAlgorithm )
      {
         Py_DECREF( self );
         return NULL;
      }
   }

   return reinterpret_cast<PyObject*>( self );
}


