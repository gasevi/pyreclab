#ifndef __PY_COMMON_H__
#define __PY_COMMON_H__

#include <Python.h> // This header was set at the first place to avoid warnings
#include <vector>
#include <map>
#include <stdexcept>

#include "DataReader.h"
#include "DataWriter.h"
#include "MAE.h"
#include "RMSE.h"
#include "MAP.h"
#include "NDCG.h"
#include "SigHandler.h"


template<class TPyInstance, class TAlgorithm>
PyObject* PyNew( PyTypeObject* type, PyObject* args, PyObject* kwdict )
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

   TPyInstance* self = reinterpret_cast<TPyInstance*>( type->tp_alloc( type, 0 ) );
   if( self != NULL )
   {
      self->m_trainingReader = new DataReader( dsfilename, dlmchar, header );
      if( NULL == self->m_trainingReader )
      {
         Py_DECREF( self );
         return NULL;
      }

      self->m_recAlgorithm = new TAlgorithm( *self->m_trainingReader, usercol, itemcol, ratingcol );
      if( NULL == self->m_recAlgorithm )
      {
         Py_DECREF( self );
         return NULL;
      }
   }

   return reinterpret_cast<PyObject*>( self );
}

template<class TPyInstance, class TAlgorithm>
PyObject* PyNewIF( PyTypeObject* type, PyObject* args, PyObject* kwdict )
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
                             const_cast<char*>( "observationcol" ),
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

   TPyInstance* self = reinterpret_cast<TPyInstance*>( type->tp_alloc( type, 0 ) );
   if( self != NULL )
   {
      self->m_trainingReader = new DataReader( dsfilename, dlmchar, header );
      if( NULL == self->m_trainingReader )
      {
         Py_DECREF( self );
         return NULL;
      }

      self->m_recAlgorithm = new TAlgorithm( *self->m_trainingReader, usercol, itemcol, ratingcol );
      if( NULL == self->m_recAlgorithm )
      {
         Py_DECREF( self );
         return NULL;
      }
   }

   return reinterpret_cast<PyObject*>( self );
}

template<class TPyInstance>
void PyDealloc( TPyInstance* self )
{
   if( NULL != self->m_recAlgorithm )
   {
      delete self->m_recAlgorithm;
   }
   if( NULL != self->m_trainingReader )
   {
      delete self->m_trainingReader;
   }
   if( NULL != self->m_pTestData )
   {
      delete self->m_pTestData;
   }
#if PY_MAJOR_VERSION >= 3
   Py_TYPE( self )->tp_free( reinterpret_cast<PyObject*>( self ) );
#else
   self->ob_type->tp_free( reinterpret_cast<PyObject*>( self ) );
#endif
}

template<class TPyInstance>
PyObject* PyReset( TPyInstance* self, PyObject* args )
{
   try
   {
      self->m_recAlgorithm->reset();
   }
   catch( std::invalid_argument& eMsg )
   {
      PyGILState_STATE gstate = PyGILState_Ensure();
      PyErr_SetString( PyExc_ValueError, eMsg.what() );
      PyGILState_Release( gstate );
      return NULL;
   }

   Py_RETURN_NONE;
}

template<class TPyInstance>
PyObject* PyPredict( TPyInstance* self, PyObject* args )
{
   const char* userId = NULL;
   const char* itemId = NULL;

   if( !PyArg_ParseTuple( args, "ss|", &userId, &itemId ) )
   {
      return NULL;
   }

   std::string strUserId = userId;
   std::string strItemId = itemId;
   float prating = -1;
   try
   {
      prating = self->m_recAlgorithm->predict( strUserId, strItemId );
   }
   catch( std::invalid_argument& eMsg )
   {
      PyGILState_STATE gstate = PyGILState_Ensure();
      PyErr_SetString( PyExc_ValueError, eMsg.what() );
      PyGILState_Release( gstate );
      return NULL;
   }

   return Py_BuildValue( "f", prating );
}

template<class TPyInstance>
PyObject* PyRecommend( TPyInstance* self, PyObject* args, PyObject* kwdict )
{
   const char* userId = NULL;
   int topn = 10;
   int includeRated = 0;

   static char* kwlist[] = { const_cast<char*>( "user" ),
                             const_cast<char*>( "topn" ),
                             const_cast<char*>( "includeRated" ),
                             NULL };

   if( !PyArg_ParseTupleAndKeywords( args, kwdict, "s|ii", kwlist, &userId, &topn, &includeRated ) )
   {
      return NULL;
   }

   std::vector<std::string> itemList;
   try
   {
      self->m_recAlgorithm->recommend( userId, topn, itemList, includeRated );
   }
   catch( std::exception& e )
   {
      PyGILState_STATE gstate = PyGILState_Ensure();
      PyErr_SetString( PyExc_ValueError, e.what() );
      PyGILState_Release( gstate );
      return NULL;
   }

   PyObject* pyList = PyList_New( 0 );
   if( NULL == pyList )
   {
      return NULL;
   }

   std::vector<std::string>::iterator ind;
   std::vector<std::string>::iterator end = itemList.end();
   for( ind = itemList.begin() ; ind != end ; ++ind )
   {
      if( -1 == PyList_Append( pyList, Py_BuildValue( "s", ind->c_str() ) ) )
      {
         return NULL;
      }
   }

   return pyList;
}

template<class TPyInstance>
PyObject* PyLoss( TPyInstance* self, PyObject* args )
{
   float loss = 0;
   try
   {
      loss = self->m_recAlgorithm->loss();
   }
   catch( std::invalid_argument& eMsg )
   {
      PyGILState_STATE gstate = PyGILState_Ensure();
      PyErr_SetString( PyExc_ValueError, eMsg.what() );
      PyGILState_Release( gstate );
      return NULL;
   }

   return Py_BuildValue( "f", loss );
}

template<class TPyInstance>
PyObject* PyTest( TPyInstance* self, PyObject* args, PyObject* kwdict )
{
   const char* input_file = NULL;
   const char* output_file = NULL;
   char dlmchar = ',';
   int header = 0;
   int usercol = 0;
   int itemcol = 1;
   int ratingcol = -1;

   static char* kwlist[] = { const_cast<char*>( "input_file" ),
                             const_cast<char*>( "output_file" ),
                             const_cast<char*>( "dlmchar" ),
                             const_cast<char*>( "header" ),
                             const_cast<char*>( "usercol" ),
                             const_cast<char*>( "itemcol" ),
                             const_cast<char*>( "ratingcol" ),
                             NULL };

   if( !PyArg_ParseTupleAndKeywords( args, kwdict, "s|sciiii", kwlist, &input_file,
                                     &output_file, &dlmchar, &header, &usercol, &itemcol, &ratingcol ) )
   {
      return NULL;
   }

   if( NULL == input_file )
   {
      return NULL;
   }

   DataWriter dataWriter;
   if( NULL != output_file )
   {
      char dlm = '\t';
      std::string strfilename = output_file;
      if( strfilename.substr( strfilename.find_last_of( "." ) + 1 ) == "csv" )
      {
         dlm = ',';
      }
      dataWriter.open( strfilename, dlm );
   }

   DataReader testReader( input_file, dlmchar, header );
   DataFrame testData( testReader, usercol, itemcol, ratingcol );

   PyObject* pyList = PyList_New( 0 );
   if( NULL == pyList )
   {
      return NULL;
   }

   MAE mae;
   RMSE rmse;

   int cause = 0;
   SigHandler sigHandler( SIGINT );

   DataFrame::iterator ind;
   DataFrame::iterator end = testData.end();
   for( ind = testData.begin() ; ind != end ; ++ind )
   {
      std::string userId = ind->first.first;
      std::string itemId = ind->first.second;
      double prediction = -1;
      try
      {
         prediction = self->m_recAlgorithm->predict( userId, itemId );
      }
      catch( std::invalid_argument& eMsg )
      {
         std::cerr << "Warning: " << eMsg.what() << std::endl;
         continue;
      }

      PyObject* pyTuple = PyTuple_New( 3 );
      if( NULL == pyTuple )
      {
         return NULL;
      }

#if PY_MAJOR_VERSION >= 3
      PyTuple_SET_ITEM( pyTuple, 0, PyUnicode_FromString( userId.c_str() ) );
      PyTuple_SET_ITEM( pyTuple, 1, PyUnicode_FromString( itemId.c_str() ) );
#else
      PyTuple_SET_ITEM( pyTuple, 0, PyString_FromString( userId.c_str() ) );
      PyTuple_SET_ITEM( pyTuple, 1, PyString_FromString( itemId.c_str() ) );
#endif
      PyTuple_SET_ITEM( pyTuple, 2, PyFloat_FromDouble( prediction ) );
      if( -1 == PyList_Append( pyList, pyTuple ) )
      {
         return NULL;
      }

      if( ratingcol >= 0 )
      {
         double rating = ind->second;
         mae.append( rating, prediction );
         rmse.append( rating, prediction );
      }

      if( dataWriter.isOpen() )
      {
         std::vector<std::string> vline;
         vline.push_back( userId );
         vline.push_back( itemId );
         std::ostringstream ss;
         ss << prediction;
         vline.push_back( ss.str() );
         dataWriter.write( vline );
      }

      if( sigHandler.interrupt() )
      {
         cause = 10;
         break;
      }
   }

   if( cause == 10 )
   {
      PyGILState_STATE gstate = PyGILState_Ensure();
      PyErr_SetString( PyExc_KeyboardInterrupt, "SIGINT received" );
      PyGILState_Release( gstate );
      return NULL;
   }

   PyObject* pyTupleResult = PyTuple_New( 3 );
   PyTuple_SET_ITEM( pyTupleResult, 0, pyList );
   PyTuple_SET_ITEM( pyTupleResult, 1, PyFloat_FromDouble( mae.eval() ) );
   PyTuple_SET_ITEM( pyTupleResult, 2, PyFloat_FromDouble( rmse.eval() ) );

   return pyTupleResult;
}

template<class TPyInstance>
PyObject* PyTestrec( TPyInstance* self, PyObject* args, PyObject* kwdict )
{
   const char* input_file = NULL;
   const char* output_file = NULL;
   char dlmchar = ',';
   int header = 0;
   int usercol = 0;
   int itemcol = 1;
   int ratingcol = -1;
   int topn = 10;
   float relevanceThreshold = 0;
   int includeRated = 0;

   static char* kwlist[] = { const_cast<char*>( "input_file" ),
                             const_cast<char*>( "output_file" ),
                             const_cast<char*>( "dlmchar" ),
                             const_cast<char*>( "header" ),
                             const_cast<char*>( "usercol" ),
                             const_cast<char*>( "itemcol" ),
                             const_cast<char*>( "ratingcol" ),
                             const_cast<char*>( "topn" ),
                             const_cast<char*>( "relevance_threshold" ),
                             const_cast<char*>( "includeRated" ),
                             NULL };

   if( !PyArg_ParseTupleAndKeywords( args, kwdict, "s|sciiiiifi", kwlist, &input_file,
                                     &output_file, &dlmchar, &header, &usercol, &itemcol, &ratingcol, &topn, &relevanceThreshold, &includeRated ) )
   {
      return NULL;
   }

   if( NULL == input_file )
   {
      return NULL;
   }

   DataWriter dataWriter;
   if( NULL != output_file )
   {
      std::string strfilename = output_file;
      dataWriter.open( strfilename );
   }

   DataReader testReader( input_file, dlmchar, header );
   if( NULL != self->m_pTestData )
   {
      delete self->m_pTestData;
      self->m_pTestData = NULL;
   }
   self->m_pTestData = new DataFrame( testReader, usercol, itemcol, ratingcol );

   PyObject* pyDict = PyDict_New();
   if( NULL == pyDict )
   {
      return NULL;
   }

   MAP meanAP;
   NDCG nDcg;

   int cause = 0;
   SigHandler sigHandler( SIGINT );

   std::map<std::string, int> userFilter;
   DataFrame::iterator ind;
   DataFrame::iterator end = self->m_pTestData->end();
   for( ind = self->m_pTestData->begin() ; ind != end ; ++ind )
   {
      // Recommned item list once per user
      std::string userId = ind->first.first;
      if( userFilter.find( userId ) != userFilter.end() )
      {
         continue;
      }
      userFilter[userId] = 0;

      std::vector<std::string> ranking;
      try
      {
         if( !self->m_recAlgorithm->recommend( userId, topn, ranking, includeRated ) )
         {
            continue;
         }
      }
      catch( std::invalid_argument& eMsg )
      {
         std::cerr << "Warning: " << eMsg.what() << std::endl;
         continue;
      }

      PyObject* pyList = PyList_New( 0 );
      if( NULL == pyList )
      {
         return NULL;
      }

      std::vector<std::string>::iterator rankind;
      std::vector<std::string>::iterator rankend = ranking.end();
      for( rankind = ranking.begin() ; rankind != rankend ; ++rankind )
      {
#if PY_MAJOR_VERSION >= 3
         if( -1 == PyList_Append( pyList, PyUnicode_FromString( rankind->c_str() ) ) )
#else
         if( -1 == PyList_Append( pyList, PyString_FromString( rankind->c_str() ) ) )
#endif
         {
            return NULL;
         }
      }

      if( PyDict_SetItemString( pyDict, userId.c_str(), pyList ) < 0 )
      {
         Py_DECREF( pyList );
         return NULL;
      }

      if( itemcol >= 0 && ratingcol >= 0 )
      {
         std::vector<std::string> preferences = self->m_pTestData->filter( userId, relevanceThreshold );
         meanAP.append( ranking, preferences );
         nDcg.append( ranking, preferences );
      }

      if( dataWriter.isOpen() )
      {
         dataWriter.write( userId, ranking );
      }

      if( sigHandler.interrupt() )
      {
         cause = 10;
         break;
      }
   }

   if( cause == 10 )
   {
      PyGILState_STATE gstate = PyGILState_Ensure();
      PyErr_SetString( PyExc_KeyboardInterrupt, "SIGINT received" );
      PyGILState_Release( gstate );
      return NULL;
   }

   PyObject* pyTupleResult = PyTuple_New( 3 );
   PyTuple_SET_ITEM( pyTupleResult, 0, pyDict );
   PyTuple_SET_ITEM( pyTupleResult, 1, PyFloat_FromDouble( meanAP.eval() ) );
   PyTuple_SET_ITEM( pyTupleResult, 2, PyFloat_FromDouble( nDcg.eval() ) );

   return pyTupleResult;
}

template<class TPyInstance>
PyObject* PyMAP( TPyInstance* self, PyObject* args, PyObject* kwdict )
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

   std::vector<std::string> ranking;
   if( numLines <= 0 )
   {
      try
      {
         if( !self->m_recAlgorithm->recommend( userId, topN, ranking, includeRated ) )
         {
            PyGILState_STATE gstate = PyGILState_Ensure();
            PyErr_SetString( PyExc_RuntimeError, "It was not possible to recommend items" );
            PyGILState_Release( gstate );
            return NULL;
         }
      }
      catch( std::invalid_argument& eMsg )
      {
         PyGILState_STATE gstate = PyGILState_Ensure();
         PyErr_SetString( PyExc_RuntimeError, eMsg.what() );
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
         ranking.push_back( std::string( PyBytes_AS_STRING( strObj ) ) );
#else
         ranking.push_back( std::string( PyString_AsString( strObj ) ) );
#endif
      }
      topN = ranking.size();
   }

   std::vector<std::string> preferences = self->m_pTestData->filter( userId, relevanceThreshold );
   MAP meanAP;
   meanAP.append( ranking, preferences );

   return Py_BuildValue( "f", meanAP.eval() );
}

template<class TPyInstance>
PyObject* PynDCG( TPyInstance* self, PyObject* args, PyObject* kwdict )
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

   std::vector<std::string> ranking;
   if( numLines <= 0 )
   {
      try
      {
         if( !self->m_recAlgorithm->recommend( userId, topN, ranking, includeRated ) )
         {
            PyGILState_STATE gstate = PyGILState_Ensure();
            PyErr_SetString( PyExc_RuntimeError, "It was not possible to recommend items" );
            PyGILState_Release( gstate );
            return NULL;
         }
      }
      catch( std::invalid_argument& eMsg )
      {
         PyGILState_STATE gstate = PyGILState_Ensure();
         PyErr_SetString( PyExc_RuntimeError, eMsg.what() );
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
         ranking.push_back( std::string( PyBytes_AS_STRING( strObj ) ) );
#else
         ranking.push_back( std::string( PyString_AsString( strObj ) ) );
#endif
      }
      topN = ranking.size();
   }

   std::vector<std::string> preferences = self->m_pTestData->filter( userId, relevanceThreshold );
   NDCG nDcg;
   nDcg.append( ranking, preferences );

   return Py_BuildValue( "f", nDcg.eval() );
}

#endif // __PY_COMMON_H__

