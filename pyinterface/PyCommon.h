#ifndef __PY_COMMON_H__
#define __PY_COMMON_H__

#include <Python.h> // This header was set at the first place to avoid warnings
#include <vector>

#include "MAP.h"
#include "NDCG.h"

using namespace std;


template<class T>
PyObject* PyMAP( T* self, PyObject* args, PyObject* kwdict )
{
   const char* userId = NULL;
   PyObject* retrieved = NULL;
   int topN = 10;
   float relevanceThreshold = 0;
   int includeRated = 0;

   static char* kwlist[] = { const_cast<char*>( "user_id" ),
                             const_cast<char*>( "retrieved" ),
                             const_cast<char*>( "topn" ), 
                             const_cast<char*>( "relevance_threshold" ),
                             const_cast<char*>( "include_rated" ),
                             NULL
                           };

   if( !PyArg_ParseTupleAndKeywords( args, kwdict, "s|O!ifi", kwlist, &userId, &PyList_Type, &retrieved, &topN, &relevanceThreshold, &includeRated ) )
   {
      return NULL;
   }

   if( NULL == self->m_pTestData )
   {
      PyGILState_STATE gstate = PyGILState_Ensure();
      PyErr_SetString( PyExc_RuntimeError, "Test data not found" );
      PyGILState_Release( gstate );
      return NULL;
   }

   int numLines = 0;
   if( retrieved != NULL )
   {
      numLines = PyList_Size( retrieved );
   }

   vector<string> ranking;
   if( numLines <= 0 )
   {
      if( !self->m_recAlgorithm->recommend( userId, topN, ranking, includeRated ) )
      {
         PyGILState_STATE gstate = PyGILState_Ensure();
         PyErr_SetString( PyExc_RuntimeError, "It was not possible to recommend items" );
         PyGILState_Release( gstate );
         return NULL;
      }
   }
   else
   {
      for( int i = 0 ; i < numLines ; ++i )
      {
         PyObject* strObj = PyList_GetItem( retrieved, i );
#if PY_MAJOR_VERSION >= 3
         ranking.push_back( string( PyBytes_AS_STRING( strObj ) ) );
#else
         ranking.push_back( string( PyString_AsString( strObj ) ) );
#endif
      }
      topN = ranking.size();
   }

   vector<string> preferences = self->m_pTestData->filter( userId, relevanceThreshold );
   MAP meanAP;
   meanAP.append( ranking, preferences );

   return Py_BuildValue( "f", meanAP.eval() );
}

template<class T>
PyObject* PynDCG( T* self, PyObject* args, PyObject* kwdict )
{
   const char* userId = NULL;
   PyObject* retrieved = NULL;
   int topN = 10;
   float relevanceThreshold = 0;
   int includeRated = 0;

   static char* kwlist[] = { const_cast<char*>( "user_id" ),
                             const_cast<char*>( "retrieved" ),
                             const_cast<char*>( "topn" ), 
                             const_cast<char*>( "relevance_threshold" ),
                             const_cast<char*>( "include_rated" ),
                             NULL
                           };

   if( !PyArg_ParseTupleAndKeywords( args, kwdict, "s|O!ifi", kwlist, &userId, &PyList_Type, &retrieved, &topN, &relevanceThreshold, &includeRated ) )
   {
      return NULL;
   }  

   if( NULL == self->m_pTestData )
   {
      PyGILState_STATE gstate = PyGILState_Ensure();
      PyErr_SetString( PyExc_RuntimeError, "Test data not found" );
      PyGILState_Release( gstate );
      return NULL;
   }  

   int numLines = 0;
   if( retrieved != NULL )
   {
      numLines = PyList_Size( retrieved );
   }

   vector<string> ranking;
   if( numLines <= 0 )
   {
      if( !self->m_recAlgorithm->recommend( userId, topN, ranking, includeRated ) )
      {
         PyGILState_STATE gstate = PyGILState_Ensure();
         PyErr_SetString( PyExc_RuntimeError, "It was not possible to recommend items" );
         PyGILState_Release( gstate );
         return NULL;
      }  
   }
   else
   {
      for( int i = 0 ; i < numLines ; ++i )
      {
         PyObject* strObj = PyList_GetItem( retrieved, i );
#if PY_MAJOR_VERSION >= 3
         ranking.push_back( string( PyBytes_AS_STRING( strObj ) ) );
#else
         ranking.push_back( string( PyString_AsString( strObj ) ) );
#endif
      }
      topN = ranking.size();
   }

   vector<string> preferences = self->m_pTestData->filter( userId, relevanceThreshold );
   NDCG nDcg;
   nDcg.append( ranking, preferences );

   return Py_BuildValue( "f", nDcg.eval() );
}

#endif // __PY_COMMON_H__

