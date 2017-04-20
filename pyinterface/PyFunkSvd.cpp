#include "PyFunkSvd.h"
#include "AlgFactory.h"
#include "AlgFunkSvd.h"

#include <signal.h>
#include <iostream>
#include <string>

using namespace std;


PyObject* FunkSvd_new( PyTypeObject* type, PyObject* args, PyObject* kwdict )
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

      self->m_recAlgorithm = AlgFactory::getInstance( "SVD", *(self->m_ratingMatrix) );
      if( NULL == self->m_recAlgorithm )
      {
         Py_DECREF( self );
         return NULL;
      }
   }

   return reinterpret_cast<PyObject*>( self );
}

PyObject* FunkSvd_train( Recommender* self, PyObject* args, PyObject* kwdict )
{  
   size_t factors = 200;
   size_t maxiter = 100;
   float lr = 0.01;
   float lambda = 0.1;

   static char* kwlist[] = { const_cast<char*>( "factors" ),
                             const_cast<char*>( "maxiter" ),
                             const_cast<char*>( "lr" ),
                             const_cast<char*>( "lamb" ),
                             NULL };

   if( !PyArg_ParseTupleAndKeywords( args, kwdict, "|iiff", kwlist, &factors, &maxiter, &lr, &lambda ) )
   {
      return NULL;
   }

   self->m_currentRecSys = self->m_recAlgorithm;
   sighandler_t prevsighd = signal( SIGINT, Recommender::sighandler );

   int cause = 0;
   Py_BEGIN_ALLOW_THREADS
   cause = dynamic_cast<AlgFunkSvd*>( self->m_recAlgorithm )->train( factors, maxiter, lr, lambda );
   Py_END_ALLOW_THREADS

   signal( SIGINT, prevsighd );

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


