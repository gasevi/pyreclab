#include "PyUserKnn.h"
#include "AlgFactory.h"
#include "AlgUserBasedKnn.h"

#include <iostream>
#include <string>

using namespace std;


PyObject* UserKnn_new( PyTypeObject* type, PyObject* args, PyObject* kwdict )
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

      self->m_recAlgorithm = AlgFactory::getInstance( "UserKnn", *(self->m_ratingMatrix) );
      if( NULL == self->m_recAlgorithm )
      {
         Py_DECREF( self );
         return NULL;
      }
   }

   return reinterpret_cast<PyObject*>( self );
}

PyObject* UserKnn_train( Recommender* self, PyObject* args, PyObject* kwdict )
{  
   int k = 10;

   static char* kwlist[] = { const_cast<char*>( "k" ),
                             NULL };

   if( !PyArg_ParseTupleAndKeywords( args, kwdict, "|i", kwlist, &k ) )
   {
      return NULL;
   }

   self->m_currentRecSys = self->m_recAlgorithm;
   struct sigaction* pOldAction = self->handlesignal( SIGINT );

   int cause = 0;
   Py_BEGIN_ALLOW_THREADS
   cause = dynamic_cast<AlgUserBasedKnn*>( self->m_recAlgorithm )->train( k );
   Py_END_ALLOW_THREADS

   self->restoresignal( SIGINT, pOldAction );

   if( RecSysAlgorithm::STOPPED == cause )
   {
      PyGILState_STATE gstate = PyGILState_Ensure();
      PyErr_SetString( PyExc_KeyboardInterrupt, "SIGINT received" );
      PyGILState_Release( gstate );
      return NULL;
   }

   Py_INCREF( Py_None );
   return Py_None;
}


