#include "PyIFAlsConjugateGradient.h"
#include "PyCommon.h"
#include "PrlSigHandler.h"
#include "DataWriter.h"
#include "DataFrame.h"
#include "MAP.h"
#include "NDCG.h"

#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

static
PyMethodDef IFAlsConjugateGradient_methods[] =
{
   { "train",     (PyCFunction)IFAlsConjugateGradient_train,     METH_VARARGS|METH_KEYWORDS, "train model" },
   { "testrec",   (PyCFunction)IFAlsConjugateGradient_testrec,   METH_VARARGS|METH_KEYWORDS, "test recommendation model" },
   { "recommend", (PyCFunction)IFAlsConjugateGradient_recommend, METH_VARARGS|METH_KEYWORDS, "recommend ranked items to a user" },
   { "MAP",       (PyCFunction)PyMAP<PyIFAlsConjugateGradient>,  METH_VARARGS|METH_KEYWORDS, "calculate Normalized Discounted Cumulative Gain for a user" },
   { "nDCG",      (PyCFunction)PynDCG<PyIFAlsConjugateGradient>, METH_VARARGS|METH_KEYWORDS, "calculate Mean Average Precision for a user" },
   { NULL, NULL, 0, NULL }
};

static PyTypeObject IFAlsConjugateGradientType =
{
#if PY_MAJOR_VERSION >= 3
   PyVarObject_HEAD_INIT(NULL, 0)
#else
   PyObject_HEAD_INIT( NULL )
   0,                                        // ob_size
#endif
   "libpyreclab.IFAlsConjugateGradient",                      // tp_name
   sizeof(PyIFAlsConjugateGradient),                          // tp_basicsize
   0,                                        // tp_itemsize
   (destructor)IFAlsConjugateGradient_dealloc,                // tp_dealloc
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
   "IFAlsConjugateGradient objects",                          // tp_doc
   0,                                        // tp_traverse
   0,                                        // tp_clear
   0,                                        // tp_richcompare
   0,                                        // tp_weaklistoffset
   0,                                        // tp_iter
   0,                                        // tp_iternext
   IFAlsConjugateGradient_methods,                            // tp_methods
   0,                                        // tp_members
   0,                                        // tp_getset
   0,                                        // tp_base
   0,                                        // tp_dict
   0,                                        // tp_descr_get
   0,                                        // tp_descr_set
   0,                                        // tp_dictoffset
   0,                                        // tp_init
   0,                                        // tp_alloc
   IFAlsConjugateGradient_new,                                // tp_new
};


PyTypeObject* IFAlsConjugateGradient_getTypeObject()
{
   return &IFAlsConjugateGradientType;
}

PyObject* IFAlsConjugateGradient_new( PyTypeObject* type, PyObject* args, PyObject* kwdict )
{
   const char* dsfilename = NULL;
   char dlmchar = ',';
   int header = 0;
   int usercol = 0;
   int itemcol = 1;
   int obscol = 2;

   static char* kwlist[] = { const_cast<char*>( "dataset" ),
                             const_cast<char*>( "dlmchar" ),
                             const_cast<char*>( "header" ),
                             const_cast<char*>( "usercol" ),
                             const_cast<char*>( "itemcol" ),
                             const_cast<char*>( "observationcol" ),
                             NULL };

   if( !PyArg_ParseTupleAndKeywords( args, kwdict, "s|ciiii", kwlist, &dsfilename,
                                     &dlmchar, &header, &usercol, &itemcol, &obscol ) )
   {
      return NULL;
   }

   if( NULL == dsfilename )
   {
      return NULL;
   }

   PyIFAlsConjugateGradient* self = reinterpret_cast<PyIFAlsConjugateGradient*>( type->tp_alloc( type, 0 ) );
   //cout << "### ref count after: " << reinterpret_cast<PyObject*>( self )->ob_refcnt << endl;
   if( self != NULL )
   {
      self->m_trainingReader = new DataReader( dsfilename, dlmchar, header );
      if( NULL == self->m_trainingReader )
      {
         Py_DECREF( self );
         return NULL;
      }

      self->m_recAlgorithm = new AlgIFAlsConjugateGradient( *self->m_trainingReader, usercol, itemcol, obscol );
      if( NULL == self->m_recAlgorithm )
      {
         Py_DECREF( self );
         return NULL;
      }
   }

   return reinterpret_cast<PyObject*>( self );
}

void IFAlsConjugateGradient_dealloc( PyIFAlsConjugateGradient* self )
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

PyObject* IFAlsConjugateGradient_train( PyIFAlsConjugateGradient* self, PyObject* args, PyObject* kwdict )
{
   size_t factors = 50;
   size_t alsNumIter = 5;
   float lambda = 10;
   size_t cgNumIter = 2;

   static char* kwlist[] = { const_cast<char*>( "factors" ),
                             const_cast<char*>( "alsNumIter" ),
                             const_cast<char*>( "lambd" ),
                             const_cast<char*>( "cgNumIter" ),
                             NULL };

   if( !PyArg_ParseTupleAndKeywords( args, kwdict, "|iifi", kwlist, &factors, &alsNumIter, &lambda, &cgNumIter ) )
   {
      return NULL;
   }

   PrlSigHandler::registerObj( reinterpret_cast<PyObject*>( self ), PrlSigHandler::IF_ALS );
   struct sigaction* pOldAction = PrlSigHandler::handlesignal( SIGINT );
   int cause = -1;
   string eMsg;
   Py_BEGIN_ALLOW_THREADS
   try
   {
      cause = dynamic_cast<AlgIFAlsConjugateGradient*>( self->m_recAlgorithm )->train( factors, alsNumIter, lambda, cgNumIter );
   }
   catch( exception& e )
   {
      eMsg = e.what();
   }
   Py_END_ALLOW_THREADS
   PrlSigHandler::restoresignal( SIGINT, pOldAction );

   if( AlgIFAlsConjugateGradient::STOPPED == cause )
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

PyObject* IFAlsConjugateGradient_recommend( PyIFAlsConjugateGradient* self, PyObject* args, PyObject* kwdict )
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
   try
   {
      self->m_recAlgorithm->recommend( userId, topn, itemList, includeRated );
   }
   catch( exception& e )
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

PyObject* IFAlsConjugateGradient_testrec( PyIFAlsConjugateGradient* self, PyObject* args, PyObject* kwdict )
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


