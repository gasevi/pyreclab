#include "PyBprMf.h"
#include "PyCommon.h"
#include "SigHandler.h"
#include "DataWriter.h"
#include "MAP.h"
#include "NDCG.h"

#include <iostream>
#include <string>

using namespace std;

static
PyMethodDef BprMf_methods[] =
{
   { "train",     (PyCFunction)BprMfTrain,           METH_VARARGS|METH_KEYWORDS, "train model" },
   { "reset",     (PyCFunction)PyReset<PyBprMf>,     METH_NOARGS,                "reset model parameters" },
   { "loss",      (PyCFunction)PyLoss<PyBprMf>,      METH_NOARGS,                "returns current loss value" },
   { "testrec",   (PyCFunction)PyTestrec<PyBprMf>,   METH_VARARGS|METH_KEYWORDS, "test recommendation model" },
   { "recommend", (PyCFunction)PyRecommend<PyBprMf>, METH_VARARGS|METH_KEYWORDS, "recommend ranked items to a user" },
   { "MAP",       (PyCFunction)PyMAP<PyBprMf>,       METH_VARARGS|METH_KEYWORDS, "calculate Mean Average Precision for a user" },
   { "nDCG",      (PyCFunction)PynDCG<PyBprMf>,      METH_VARARGS|METH_KEYWORDS, "calculate Normalized Discounted Cumulative Gain for a user" },
   { NULL, NULL, 0, NULL }
};

static PyTypeObject BprMfType =
{
#if PY_MAJOR_VERSION >= 3
   PyVarObject_HEAD_INIT(NULL, 0)
#else
   PyObject_HEAD_INIT( NULL )
   0,                                        // ob_size
#endif
   "libpyreclab.BprMf",                      // tp_name
   sizeof(PyBprMf),                          // tp_basicsize
   0,                                        // tp_itemsize
   (destructor)PyDealloc<PyBprMf>,           // tp_dealloc
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
   "BprMf objects",                          // tp_doc
   0,                                        // tp_traverse
   0,                                        // tp_clear
   0,                                        // tp_richcompare
   0,                                        // tp_weaklistoffset
   0,                                        // tp_iter
   0,                                        // tp_iternext
   BprMf_methods,                            // tp_methods
   0,                                        // tp_members
   0,                                        // tp_getset
   0,                                        // tp_base
   0,                                        // tp_dict
   0,                                        // tp_descr_get
   0,                                        // tp_descr_set
   0,                                        // tp_dictoffset
   0,                                        // tp_init
   0,                                        // tp_alloc
   BprMfNew,                                 // tp_new
};


PyTypeObject* BprMfGetType()
{
   return &BprMfType;
}

PyObject* BprMfNew( PyTypeObject* type, PyObject* args, PyObject* kwdict )
{
   int factors = 20;
   const char* dsfilename = NULL;
   char dlmchar = ',';
   int header = 0;
   int usercol = 0;
   int itemcol = 1;
   int ratingcol = 2;

   static char* kwlist[] = { const_cast<char*>( "factors" ),
                             const_cast<char*>( "dataset" ),
                             const_cast<char*>( "dlmchar" ),
                             const_cast<char*>( "header" ),
                             const_cast<char*>( "usercol" ),
                             const_cast<char*>( "itemcol" ),
                             const_cast<char*>( "ratingcol" ),
                             NULL };

   if( !PyArg_ParseTupleAndKeywords( args, kwdict, "is|ciiii", kwlist, &factors, &dsfilename,
                                     &dlmchar, &header, &usercol, &itemcol, &ratingcol ) )
   {
      return NULL;
   }

   if( NULL == dsfilename )
   {
      return NULL;
   }

   PyBprMf* self = reinterpret_cast<PyBprMf*>( type->tp_alloc( type, 0 ) );
   if( self != NULL )
   {
      self->m_trainingReader = new DataReader( dsfilename, dlmchar, header );
      if( NULL == self->m_trainingReader )
      {
         Py_DECREF( self );
         return NULL;
      }

      self->m_recAlgorithm = new AlgBprMf( factors, *self->m_trainingReader, usercol, itemcol, ratingcol );
      if( NULL == self->m_recAlgorithm )
      {
         Py_DECREF( self );
         return NULL;
      }
   }

   return reinterpret_cast<PyObject*>( self );
}

PyObject* BprMfTrain( PyBprMf* self, PyObject* args, PyObject* kwdict )
{
   size_t maxiter = 100;
   float lr = 0.1;
   float lambdaW = 0.01;
   float lambdaHp = 0.01;
   float lambdaHm = 0.01;

   static char* kwlist[] = { const_cast<char*>( "maxiter" ),
                             const_cast<char*>( "lr" ),
                             const_cast<char*>( "lambda_w" ),
                             const_cast<char*>( "lambda_hp" ),
                             const_cast<char*>( "lambda_hm" ),
                             NULL };

   if( !PyArg_ParseTupleAndKeywords( args, kwdict, "|iffff", kwlist, &maxiter, &lr, &lambdaW, &lambdaHp, &lambdaHm ) )
   {
      return NULL;
   }

   SigHandler sigHandler( SIGINT );
   int cause = 0;
   cause = dynamic_cast<AlgBprMf*>( self->m_recAlgorithm )->train( maxiter, lr, lambdaW, lambdaHp, lambdaHm, sigHandler );

   if( AlgBprMf::STOPPED == cause )
   {
      PyGILState_STATE gstate = PyGILState_Ensure();
      PyErr_SetString( PyExc_KeyboardInterrupt, "SIGINT received" );
      PyGILState_Release( gstate );
      return NULL;
   }

   Py_INCREF( Py_None );
   return Py_None;
}


