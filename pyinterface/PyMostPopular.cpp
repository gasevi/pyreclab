#include "PyMostPopular.h"
#include "PyCommon.h"
#include "PrlSigHandler.h"
#include "DataReader.h"
#include "DataWriter.h"
#include "MAP.h"
#include "NDCG.h"

#include <Python.h>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

static
PyMethodDef MostPopular_methods[] =
{
   { "train",     (PyCFunction)MostPopular_train,     METH_VARARGS|METH_KEYWORDS, "train model" },
   { "testrec",   (PyCFunction)MostPopular_testrec,   METH_VARARGS|METH_KEYWORDS, "test recommendation model" },
   { "recommend", (PyCFunction)MostPopular_recommend, METH_VARARGS|METH_KEYWORDS, "recommend ranked items to a user" },
   { "MAP",       (PyCFunction)PyMAP<PyMostPopular>,  METH_VARARGS|METH_KEYWORDS, "calculate Normalized Discounted Cumulative Gain for a user" },
   { "nDCG",      (PyCFunction)PynDCG<PyMostPopular>, METH_VARARGS|METH_KEYWORDS, "calculate Mean Average Precision for a user" },
   { NULL, NULL, 0, NULL }
};

static PyTypeObject MostPopularType =
{
#if PY_MAJOR_VERSION >= 3
   PyVarObject_HEAD_INIT(NULL, 0)
#else
   PyObject_HEAD_INIT( NULL )
   0,                                        // ob_size
#endif
   "libpyreclab.MostPopular",                // tp_name
   sizeof(PyMostPopular),                    // tp_basicsize
   0,                                        // tp_itemsize
   (destructor)MostPopular_dealloc,          // tp_dealloc
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
   "MostPopular objects",                    // tp_doc
   0,                                        // tp_traverse
   0,                                        // tp_clear
   0,                                        // tp_richcompare
   0,                                        // tp_weaklistoffset
   0,                                        // tp_iter
   0,                                        // tp_iternext
   MostPopular_methods,                      // tp_methods
   0,                                        // tp_members
   0,                                        // tp_getset
   0,                                        // tp_base
   0,                                        // tp_dict
   0,                                        // tp_descr_get
   0,                                        // tp_descr_set
   0,                                        // tp_dictoffset
   0,                                        // tp_init
   0,                                        // tp_alloc
   MostPopular_new,                          // tp_new
};


PyTypeObject* MostPopular_getTypeObject()
{
   return &MostPopularType;
}

PyObject* MostPopular_new( PyTypeObject* type, PyObject* args, PyObject* kwdict )
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

   PyMostPopular* self = reinterpret_cast<PyMostPopular*>( type->tp_alloc( type, 0 ) );
   if( self != NULL )
   {
      self->m_trainingReader = new DataReader( dsfilename, dlmchar, header );
      if( NULL == self->m_trainingReader )
      {
         Py_DECREF( self );
         return NULL;
      }

      self->m_recAlgorithm = new AlgMostPopular( *self->m_trainingReader, usercol, itemcol, ratingcol );
      if( NULL == self->m_recAlgorithm )
      {
         Py_DECREF( self );
         return NULL;
      }
   }

   return reinterpret_cast<PyObject*>( self );
}

void MostPopular_dealloc( PyMostPopular* self )
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

PyObject* MostPopular_train( PyMostPopular* self, PyObject* args, PyObject* kwdict )
{
   int topn = 10;

   static char* kwlist[] = { const_cast<char*>( "topn" ),
                             NULL };

   if( !PyArg_ParseTupleAndKeywords( args, kwdict, "|i", kwlist, &topn ) )
   {
      return NULL;
   }

   PrlSigHandler::registerObj( reinterpret_cast<PyObject*>( self ), PrlSigHandler::MOST_POPULAR );
   struct sigaction* pOldAction = PrlSigHandler::handlesignal( SIGINT );
   int cause = 0;
   Py_BEGIN_ALLOW_THREADS
   cause = dynamic_cast<AlgMostPopular*>( self->m_recAlgorithm )->train();
   Py_END_ALLOW_THREADS
   PrlSigHandler::restoresignal( SIGINT, pOldAction );

   if( AlgMostPopular::STOPPED == cause )
   {
      PyGILState_STATE gstate = PyGILState_Ensure();
      PyErr_SetString( PyExc_KeyboardInterrupt, "SIGINT received" );
      PyGILState_Release( gstate );
      return NULL;
   }

   Py_INCREF( Py_None );
   return Py_None;
}

PyObject* MostPopular_testrec( PyMostPopular* self, PyObject* args, PyObject* kwdict )
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
      char dlm = '\t';
      string strfilename = output_file;
      if( strfilename.substr( strfilename.find_last_of( "." ) + 1 ) == "csv"  ||
          strfilename.substr( strfilename.find_last_of( "." ) + 1 ) == "json"    )
      {
         dlm = ',';
      }
      dataWriter.open( strfilename, dlm );
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

   DataFrame::iterator ind;
   DataFrame::iterator end = self->m_pTestData->end();
   for( ind = self->m_pTestData->begin() ; ind != end ; ++ind )
   {
      std::string userId = ind->first.first;
      std::string itemId = ind->first.second;

      vector<string> ranking;
      if( !self->m_recAlgorithm->recommend( userId, topn, ranking, includeRated ) )
      {
         continue;
      }

      PyObject* pyList = PyList_New( 0 );
      if( NULL == pyList )
      {
         return NULL;
      }

      vector<string>::iterator ind;
      vector<string>::iterator end = ranking.end();
      for( ind = ranking.begin() ; ind != end ; ++ind )
      {
#if PY_MAJOR_VERSION >= 3
         if( -1 == PyList_Append( pyList, PyUnicode_FromString( ind->c_str() ) ) )
#else
         if( -1 == PyList_Append( pyList, PyString_FromString( ind->c_str() ) ) )
#endif
         {
            return NULL;
         }
      }

      PyDict_SetItemString( pyDict, userId.c_str(), pyList );

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

PyObject* MostPopular_recommend( PyMostPopular* self, PyObject* args, PyObject* kwdict )
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


