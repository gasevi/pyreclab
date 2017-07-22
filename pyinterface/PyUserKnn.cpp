#include "PyUserKnn.h"
#include "pyinterface.h"
#include "DataWriter.h"

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

   PyUserKnn* self = reinterpret_cast<PyUserKnn*>( type->tp_alloc( type, 0 ) );
   if( self != NULL )
   {
      self->m_trainingReader = new DataReader( dsfilename, dlmchar, header );
      if( NULL == self->m_trainingReader )
      {
         Py_DECREF( self );
         return NULL;
      }

      self->m_recAlgorithm = new AlgUserBasedKnn( *self->m_trainingReader, usercol, itemcol, ratingcol );
      if( NULL == self->m_recAlgorithm )
      {
         Py_DECREF( self );
         return NULL;
      }
   }

   return reinterpret_cast<PyObject*>( self );
}

void UserKnn_dealloc( PyUserKnn* self )
{
   if( NULL != self->m_recAlgorithm )
   {
      delete self->m_recAlgorithm;
   }
   if( NULL != self->m_trainingReader )
   {
      delete self->m_trainingReader;
   }
#if PY_MAJOR_VERSION >= 3
   Py_TYPE( self )->tp_free( reinterpret_cast<PyObject*>( self ) );
#else
   self->ob_type->tp_free( reinterpret_cast<PyObject*>( self ) );
#endif
}

PyObject* UserKnn_train( PyUserKnn* self, PyObject* args, PyObject* kwdict )
{  
   int k = 10;
   const char* simType = NULL;

   static char* kwlist[] = { const_cast<char*>( "k" ),
                             const_cast<char*>( "similarity" ),
                             NULL };

   if( !PyArg_ParseTupleAndKeywords( args, kwdict, "|is", kwlist, &k, &simType ) )
   {
      return NULL;
   }

   string strSimType = NULL != simType ? simType : "pearson";
   transform( strSimType.begin(), strSimType.end(), strSimType.begin(), ::tolower );
   if( !strSimType.empty() && strSimType != "pearson" && strSimType != "cosine" )
   {
      PyGILState_STATE gstate = PyGILState_Ensure();
      string eMsg = "Unknown similarity metric '";
      eMsg += strSimType + "'";
      PyErr_SetString( PyExc_ValueError, eMsg.c_str() );
      PyGILState_Release( gstate );
      return NULL;
   }

   PrlSigHandler::registerObj( reinterpret_cast<PyObject*>( self ), PrlSigHandler::USER_KNN );
   struct sigaction* pOldAction = PrlSigHandler::handlesignal( SIGINT );
   int cause = -1;
   string eMsg;
   Py_BEGIN_ALLOW_THREADS
   try
   {
      cause = dynamic_cast<AlgUserBasedKnn*>( self->m_recAlgorithm )->train( k, strSimType );
   }
   catch( const char* e )
   {
      eMsg = e;
   }
   Py_END_ALLOW_THREADS
   PrlSigHandler::restoresignal( SIGINT, pOldAction );

   if( AlgUserBasedKnn::STOPPED == cause )
   {
      PyGILState_STATE gstate = PyGILState_Ensure();
      PyErr_SetString( PyExc_KeyboardInterrupt, "SIGINT received" );
      PyGILState_Release( gstate );
      return NULL;
   }
   else if( 0 > cause )
   {
      PyGILState_STATE gstate = PyGILState_Ensure();
      PyErr_SetString( PyExc_RuntimeError, eMsg.c_str() );
      PyGILState_Release( gstate );
      return NULL;
   }

   Py_INCREF( Py_None );
   return Py_None;
}

PyObject* UserKnn_predict( PyUserKnn* self, PyObject* args )
{
   const char* userId = NULL;
   const char* itemId = NULL;

   if( !PyArg_ParseTuple( args, "ss|", &userId, &itemId ) )
   {
      return NULL;
   }

   string strUserId = userId;
   string strItemId = itemId;
   float prating = self->m_recAlgorithm->predict( strUserId, strItemId );

   return Py_BuildValue( "f", prating );
}

PyObject* UserKnn_recommend( PyUserKnn* self, PyObject* args, PyObject* kwdict )
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

   vector<string> itemList;
   self->m_recAlgorithm->recommend( userId, topn, itemList, includeRated );

   PyObject* pyList = PyList_New( 0 );
   if( NULL == pyList )
   {
      return NULL;
   }

   vector<string>::iterator ind;
   vector<string>::iterator end = itemList.end();
   for( ind = itemList.begin() ; ind != end ; ++ind )
   {
      if( -1 == PyList_Append( pyList, Py_BuildValue( "s", ind->c_str() ) ) )
      {
         return NULL;
      }
   }

   return pyList;
}

PyObject* UserKnn_test( PyUserKnn* self, PyObject* args, PyObject* kwdict )
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
      string strfilename = output_file;
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

   DataFrame::iterator ind;
   DataFrame::iterator end = testData.end();
   for( ind = testData.begin() ; ind != end ; ++ind )
   {
      std::string userId = ind->first.first;
      std::string itemId = ind->first.second;
      double prediction = self->m_recAlgorithm->predict( userId, itemId );

      PyObject* pyTuple = PyTuple_New( 3 );
      if( NULL == pyTuple )
      {
         return NULL;
      }

#if PY_MAJOR_VERSION >= 3
      PyTuple_SET_ITEM( pyTuple, 0, PyBytes_FromString( userId.c_str() ) );
      PyTuple_SET_ITEM( pyTuple, 1, PyBytes_FromString( itemId.c_str() ) );
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
         self->m_mae.append( rating, prediction );
         self->m_rmse.append( rating, prediction );
      }

      if( dataWriter.isOpen() )
      {
         vector<string> vline;
         vline.push_back( userId );
         vline.push_back( itemId );
         std::ostringstream ss;
         ss << prediction;
         vline.push_back( ss.str() );
         dataWriter.write( vline );
      }
   }

   PyObject* pyTupleResult = PyTuple_New( 3 );
   PyTuple_SET_ITEM( pyTupleResult, 0, pyList );
   PyTuple_SET_ITEM( pyTupleResult, 1, PyFloat_FromDouble( self->m_mae.eval() ) );
   PyTuple_SET_ITEM( pyTupleResult, 2, PyFloat_FromDouble( self->m_rmse.eval() ) );

   return pyTupleResult;
}

PyObject* UserKnn_testrec( PyUserKnn* self, PyObject* args, PyObject* kwdict )
{
   const char* input_file = NULL;
   const char* output_file = NULL;
   char dlmchar = ',';
   int header = 0;
   int usercol = 0;
   int itemcol = 1;
   int ratingcol = -1;
   int topn = 10;

   static char* kwlist[] = { const_cast<char*>( "input_file" ),
                             const_cast<char*>( "output_file" ),
                             const_cast<char*>( "dlmchar" ),
                             const_cast<char*>( "header" ),
                             const_cast<char*>( "usercol" ),
                             const_cast<char*>( "itemcol" ),
                             const_cast<char*>( "ratingcol" ),
                             const_cast<char*>( "topn" ),
                             NULL };

   if( !PyArg_ParseTupleAndKeywords( args, kwdict, "s|sciiiii", kwlist, &input_file,
                                     &output_file, &dlmchar, &header, &usercol, &itemcol, &ratingcol, &topn ) )
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
      string strfilename = output_file;
      dataWriter.open( strfilename );
   }

   DataReader testReader( input_file, dlmchar, header );
   DataFrame testData( testReader, usercol, itemcol, ratingcol );

   PyObject* pyDict = PyDict_New();
   if( NULL == pyDict )
   {
      return NULL;
   }

   map<string, int> userFilter;
   DataFrame::iterator ind;
   DataFrame::iterator end = testData.end();
   for( ind = testData.begin() ; ind != end ; ++ind )
   {
      // Recommned item list once per user
      std::string userId = ind->first.first;
      if( userFilter.find( userId ) != userFilter.end() )
      {
         continue;
      }
      userFilter[userId] = 0;

      vector<string> ranking;
      if( !self->m_recAlgorithm->recommend( userId, topn, ranking ) )
      {
         continue;
      }

      PyObject* pyList = PyList_New( 0 );
      vector<string>::iterator rankind;
      vector<string>::iterator rankend = ranking.end();
      for( rankind = ranking.begin() ; rankind != rankend ; ++rankind )
      {
#if PY_MAJOR_VERSION >= 3
         if( -1 == PyList_Append( pyList, PyBytes_FromString( rankind->c_str() ) ) )
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
         // Calculate p@r, nDCG
         ;
      }

      if( dataWriter.isOpen() )
      {
         dataWriter.write( userId, ranking );
      }
   }

   PyObject* pyTupleResult = PyTuple_New( 1 );
   PyTuple_SET_ITEM( pyTupleResult, 0, pyDict );
   //PyTuple_SET_ITEM( pyTupleResult, 1, PyFloat_FromDouble( self->m_mae.eval() ) );
   //PyTuple_SET_ITEM( pyTupleResult, 2, PyFloat_FromDouble( self->m_rmse.eval() ) );

   return pyTupleResult;
}



