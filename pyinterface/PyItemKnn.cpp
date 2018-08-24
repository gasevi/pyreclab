#include "PyItemKnn.h"
#include "PyCommon.h"
#include "PrlSigHandler.h"
#include "DataWriter.h"
#include "MAP.h"
#include "NDCG.h"

#include <iostream>
#include <string>

using namespace std;

static
PyMethodDef ItemKnn_methods[] =
{
   { "train",     (PyCFunction)ItemKnn_train,     METH_VARARGS|METH_KEYWORDS, "train model" },
   { "test",      (PyCFunction)ItemKnn_test,      METH_VARARGS|METH_KEYWORDS, "test model" },
   { "testrec",   (PyCFunction)ItemKnn_testrec,   METH_VARARGS|METH_KEYWORDS, "test recommendation model" },
   { "predict",   (PyCFunction)ItemKnn_predict,   METH_VARARGS,               "predict user's rating for an item" },
   { "recommend", (PyCFunction)ItemKnn_recommend, METH_VARARGS|METH_KEYWORDS, "recommend ranked items to a user" },
   { "MAP",       (PyCFunction)PyMAP<PyItemKnn>,  METH_VARARGS|METH_KEYWORDS, "calculate Normalized Discounted Cumulative Gain for a user" },
   { "nDCG",      (PyCFunction)PynDCG<PyItemKnn>, METH_VARARGS|METH_KEYWORDS, "calculate Mean Average Precision for a user" },
   { NULL, NULL, 0, NULL }
};

static PyTypeObject ItemKnnType =
{
#if PY_MAJOR_VERSION >= 3
   PyVarObject_HEAD_INIT(NULL, 0)
#else
   PyObject_HEAD_INIT( NULL )
   0,                                        // ob_size
#endif
   "libpyreclab.ItemKnn",                    // tp_name
   sizeof(PyItemKnn),                        // tp_basicsize
   0,                                        // tp_itemsize
   (destructor)ItemKnn_dealloc,              // tp_dealloc
   0,                                        // tp_print
   0,                                        // tp_getattr
   0,                                        // tp_setattr
   0,                                        // tp_compare
   0,                                        // tp_repr
   0,                                        // tp_as_number
   0,                                        // tp_as_sequence
   0,                                        // tp_as_mapping
   0,                                        // tp_hash
   0,                                        // tp_call
   0,                                        // tp_str
   0,                                        // tp_getattro
   0,                                        // tp_setattro
   0,                                        // tp_as_buffer
   Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, // tp_flags
   "ItemKnn objects",                        // tp_doc
   0,                                        // tp_traverse
   0,                                        // tp_clear
   0,                                        // tp_richcompare
   0,                                        // tp_weaklistoffset
   0,                                        // tp_iter
   0,                                        // tp_iternext
   ItemKnn_methods,                          // tp_methods
   0,                                        // tp_members
   0,                                        // tp_getset
   0,                                        // tp_base
   0,                                        // tp_dict
   0,                                        // tp_descr_get
   0,                                        // tp_descr_set
   0,                                        // tp_dictoffset
   0,                                        // tp_init
   0,                                        // tp_alloc
   ItemKnn_new,                              // tp_new
};


PyTypeObject* ItemKnn_getTypeObject()
{
   return &ItemKnnType;
}

PyObject* ItemKnn_new( PyTypeObject* type, PyObject* args, PyObject* kwdict )
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

   PyItemKnn* self = reinterpret_cast<PyItemKnn*>( type->tp_alloc( type, 0 ) );
   if( self != NULL )
   {
      self->m_trainingReader = new DataReader( dsfilename, dlmchar, header );
      if( NULL == self->m_trainingReader )
      {
         Py_DECREF( self );
         return NULL;
      }

      self->m_recAlgorithm = new AlgItemBasedKnn( *self->m_trainingReader, usercol, itemcol, ratingcol );
      if( NULL == self->m_recAlgorithm )
      {
         Py_DECREF( self );
         return NULL;
      }
   }

   return reinterpret_cast<PyObject*>( self );
}

void ItemKnn_dealloc( PyItemKnn* self )
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

PyObject* ItemKnn_train( PyItemKnn* self, PyObject* args, PyObject* kwdict )
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

   PrlSigHandler::registerObj( reinterpret_cast<PyObject*>( self ), PrlSigHandler::ITEM_KNN );
   struct sigaction* pOldAction = PrlSigHandler::handlesignal( SIGINT );
   int cause = -1;
   string eMsg;
   Py_BEGIN_ALLOW_THREADS
   try
   {
      cause = dynamic_cast<AlgItemBasedKnn*>( self->m_recAlgorithm )->train( k, strSimType );
   }
   catch( const char* e )
   {
      eMsg = e;
   }
   Py_END_ALLOW_THREADS
   PrlSigHandler::restoresignal( SIGINT, pOldAction );

   if( AlgItemBasedKnn::STOPPED == cause )
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

PyObject* ItemKnn_predict( PyItemKnn* self, PyObject* args )
{
   const char* userId = NULL;
   const char* itemId = NULL;

   if( !PyArg_ParseTuple( args, "ss|", &userId, &itemId ) )
   {
      return NULL;
   }

   string strUserId = userId;
   string strItemId = itemId;
   float prating = -1;
   try
   {
      prating = self->m_recAlgorithm->predict( strUserId, strItemId );
   }
   catch( invalid_argument& eMsg )
   {
      PyGILState_STATE gstate = PyGILState_Ensure();
      PyErr_SetString( PyExc_ValueError, eMsg.what() );
      PyGILState_Release( gstate );
      return NULL;
   }

   return Py_BuildValue( "f", prating );
}

PyObject* ItemKnn_recommend( PyItemKnn* self, PyObject* args, PyObject* kwdict )
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

PyObject* ItemKnn_test( PyItemKnn* self, PyObject* args, PyObject* kwdict )
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
      string userId = ind->first.first;
      string itemId = ind->first.second;
      double prediction = -1;
      try
      {
         prediction = self->m_recAlgorithm->predict( userId, itemId );
      }
      catch( invalid_argument& eMsg )
      {
         cerr << "Warning: " << eMsg.what() << endl;
         continue;
      }

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

PyObject* ItemKnn_testrec( PyItemKnn* self, PyObject* args, PyObject* kwdict )
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
      string strfilename = output_file;
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

   map<string, int> userFilter;
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

      vector<string> ranking;
      if( !self->m_recAlgorithm->recommend( userId, topn, ranking, includeRated ) )
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
         vector<string> preferences;
         DataFrame::iterator subind;
         DataFrame::iterator subend = self->m_pTestData->end();
         for( subind = self->m_pTestData->begin() ; subind != subend ; ++subind )
         {
            if( subind->first.first == userId && subind->second > relevanceThreshold )
            {
               preferences.push_back( subind->first.second );
            }
         }
         meanAP.append( ranking, preferences );
         nDcg.append( ranking, preferences );
      }

      if( dataWriter.isOpen() )
      {
         dataWriter.write( userId, ranking );
      }
   }

   PyObject* pyTupleResult = PyTuple_New( 3 );
   PyTuple_SET_ITEM( pyTupleResult, 0, pyDict );
   PyTuple_SET_ITEM( pyTupleResult, 1, PyFloat_FromDouble( meanAP.eval() ) );
   PyTuple_SET_ITEM( pyTupleResult, 2, PyFloat_FromDouble( nDcg.eval() ) );

   return pyTupleResult;
}


