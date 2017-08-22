#include "pyinterface.h"
#include "DataWriter.h"
#include "PyMostPopular.h"
#include "PyUserAvg.h"
#include "PyItemAvg.h"
#include "PyUserKnn.h"
#include "PyItemKnn.h"
#include "PySlopeOne.h"
#include "PyFunkSvd.h"

#include <iostream>
#include <signal.h>
#include <string>
#include <sstream>

using namespace std;

PyObject* PrlSigHandler::m_activeObj = NULL;
PrlSigHandler::EAlgType PrlSigHandler::m_algType = PrlSigHandler::UNKNOWN;

void PrlSigHandler::handler( int signum )
{
   if( NULL != m_activeObj )
   {
      switch( m_algType )
      {
      case FUNK_SVD:
         reinterpret_cast<PyFunkSvd*>( m_activeObj )->m_recAlgorithm->stop();
         break;
      case ITEM_AVG:
         reinterpret_cast<PyItemAvg*>( m_activeObj )->m_recAlgorithm->stop();
         break;
      case ITEM_KNN:
         reinterpret_cast<PyItemKnn*>( m_activeObj )->m_recAlgorithm->stop();
         break;
      case MOST_POPULAR:
         reinterpret_cast<PyMostPopular*>( m_activeObj )->m_recAlgorithm->stop();
         break;
      case SLOPE_ONE:
         reinterpret_cast<PySlopeOne*>( m_activeObj )->m_recAlgorithm->stop();
         break;
      case USER_AVG:
         reinterpret_cast<PyUserAvg*>( m_activeObj )->m_recAlgorithm->stop();
         break;
      case USER_KNN:
         reinterpret_cast<PyUserKnn*>( m_activeObj )->m_recAlgorithm->stop();
         break;
      default:
         break;
      }
   }
}

void PrlSigHandler::registerObj( PyObject* obj, EAlgType type )
{
   m_algType = type;
   m_activeObj = obj;
}

void PrlSigHandler::unregisterObj()
{
   m_activeObj = NULL;
}

struct sigaction* PrlSigHandler::handlesignal( int signum )
{
   struct sigaction newAction;
   struct sigaction* pOldAction = NULL;
   newAction.sa_handler = PrlSigHandler::handler;
   sigemptyset( &newAction.sa_mask );
   newAction.sa_flags = 0;
   sigaction( signum, &newAction, pOldAction );
   return pOldAction;
}

void PrlSigHandler::restoresignal( int signum, struct sigaction* pAction )
{
   sigaction( SIGINT, NULL, pAction );
}

#if PY_MAJOR_VERSION >= 3
struct module_state
{
   PyObject* error;
};
#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))
#endif

/*
void Recommender_dealloc( Recommender* self )
{
   Py_XDECREF( self->m_trainingReader );
   Py_XDECREF( self->m_ratingMatrix );
   Py_XDECREF( self->m_recAlgorithm );
#if PY_MAJOR_VERSION >= 3
   Py_TYPE( self )->tp_free( reinterpret_cast<PyObject*>( self ) );
#else
   self->ob_type->tp_free( reinterpret_cast<PyObject*>( self ) );
#endif
}

PyObject* Recommender_train( Recommender* self, PyObject* args )
{
   self->m_currentRecSys = self->m_recAlgorithm;
   struct sigaction* pOldAction = self->handlesignal( SIGINT );

   int cause = 0;
   Py_BEGIN_ALLOW_THREADS

   cause = self->m_recAlgorithm->train();
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

PyObject* Recommender_predict( Recommender* self, PyObject* args, PyObject* kwdict )
{
   const char* userId = NULL;
   const char* itemId = NULL;
   static char* kwlist[] = { const_cast<char*>( "user" ),
                             const_cast<char*>( "item" ),
                             NULL };

   if( !PyArg_ParseTupleAndKeywords( args, kwdict, "ss|", kwlist, &userId, &itemId ) )
   {
      return NULL;
   }

   float prating = self->m_recAlgorithm->predict( userId, itemId );

   return Py_BuildValue( "f", prating );
}

PyObject* Recommender_recommend( Recommender* self, PyObject* args, PyObject* kwds )
{
   const char* userId = NULL;
   int topn = 10;

   static char* kwlist[] = { const_cast<char*>( "user" ),
                             const_cast<char*>( "topn" ),
                             NULL };

   if( !PyArg_ParseTupleAndKeywords( args, kwds, "s|i", kwlist, &userId, &topn ) )
   {
      return NULL;
   }

   vector<string> itemList;
   self->m_recAlgorithm->recommend( userId, topn, itemList );

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

PyObject* Recommender_test( Recommender* self, PyObject* args, PyObject* kwdict )
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

PyObject* Recommender_testrec( Recommender* self, PyObject* args, PyObject* kwdict )
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
*/

static PyMethodDef libpyreclabMethods[] =
{
   { NULL, NULL }
};

#if PY_MAJOR_VERSION >= 3
static int libpyreclab_traverse(PyObject *m, visitproc visit, void *arg)
{
   Py_VISIT( GETSTATE( m )->error );
   return 0;
}

static int libpyreclab_clear( PyObject *m )
{
   Py_CLEAR( GETSTATE( m )->error );
   return 0;
}

static struct PyModuleDef moduledef =
{
   PyModuleDef_HEAD_INIT,
   "libpyreclab",
   NULL,
   sizeof( struct module_state ),
   libpyreclabMethods,
   NULL,
   libpyreclab_traverse,
   libpyreclab_clear,
   NULL
};

PyMODINIT_FUNC PyInit_libpyreclab()
#else
extern "C" void initlibpyreclab()
#endif
{
   if( PyType_Ready( &MostPopularType ) < 0 )
   {
#if PY_MAJOR_VERSION >= 3
      return NULL;
#else
      return;
#endif
   }

   if( PyType_Ready( &ItemAvgType ) < 0 )
   {
#if PY_MAJOR_VERSION >= 3
      return NULL;
#else
      return;
#endif
   }

   if( PyType_Ready( &UserAvgType ) < 0 )
   {
#if PY_MAJOR_VERSION >= 3
      return NULL;
#else
      return;
#endif
   }

   if( PyType_Ready( &UserKnnType ) < 0 )
   {
#if PY_MAJOR_VERSION >= 3
      return NULL;
#else
      return;
#endif
   }

   if( PyType_Ready( &ItemKnnType ) < 0 )
   {
#if PY_MAJOR_VERSION >= 3
      return NULL;
#else
      return;
#endif
   }

   if( PyType_Ready( &SlopeOneType ) < 0 )
   {
#if PY_MAJOR_VERSION >= 3
      return NULL;
#else
      return;
#endif
   }

   if( PyType_Ready( &FunkSvdType ) < 0 )
   {
#if PY_MAJOR_VERSION >= 3
      return NULL;
#else
      return;
#endif
   }

#if PY_MAJOR_VERSION >= 3
   PyObject* module = PyModule_Create( &moduledef );
#else
   PyObject* module = Py_InitModule( "libpyreclab", libpyreclabMethods );
#endif
   if( NULL == module )
   {
#if PY_MAJOR_VERSION >= 3
      return NULL;
#else
      return;
#endif
   }

   Py_INCREF( &MostPopularType );
   PyModule_AddObject( module, "MostPopular", (PyObject*)&MostPopularType );
   Py_INCREF( &UserAvgType );
   PyModule_AddObject( module, "UserAvg", (PyObject*)&UserAvgType );
   Py_INCREF( &ItemAvgType );
   PyModule_AddObject( module, "ItemAvg", (PyObject*)&ItemAvgType );
   Py_INCREF( &UserKnnType );
   PyModule_AddObject( module, "UserKnn", (PyObject*)&UserKnnType );
   Py_INCREF( &ItemKnnType );
   PyModule_AddObject( module, "ItemKnn", (PyObject*)&ItemKnnType );
   Py_INCREF( &SlopeOneType );
   PyModule_AddObject( module, "SlopeOne", (PyObject*)&SlopeOneType );
   Py_INCREF( &FunkSvdType );
   PyModule_AddObject( module, "SVD", (PyObject*)&FunkSvdType );

#if PY_MAJOR_VERSION >= 3
    return module;
#endif
}


