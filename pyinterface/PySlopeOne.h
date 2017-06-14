#ifndef __PY_SLOPE_ONE_H__
#define __PY_SLOPE_ONE_H__

#include "AlgSlopeOne.h"
#include "DataReader.h"
#include "MAE.h"
#include "RMSE.h"

#include <Python.h>

typedef struct
{
   PyObject_HEAD
   DataReader* m_trainingReader;
   AlgSlopeOne* m_recAlgorithm;
   MAE m_mae;
   RMSE m_rmse;
} PySlopeOne;

PyObject* SlopeOne_new( PyTypeObject* type, PyObject* args, PyObject* kwdict );
void SlopeOne_dealloc( PySlopeOne* self );
PyObject* SlopeOne_train( PySlopeOne* self, PyObject* args );
PyObject* SlopeOne_predict( PySlopeOne* self, PyObject* args, PyObject* kwdict );
PyObject* SlopeOne_recommend( PySlopeOne* self, PyObject* args, PyObject* kwds );
PyObject* SlopeOne_test( PySlopeOne* self, PyObject* args, PyObject* kwdict );
PyObject* SlopeOne_testrec( PySlopeOne* self, PyObject* args, PyObject* kwdict );

static
PyMethodDef SlopeOne_methods[] =
{
   { "train",     (PyCFunction)SlopeOne_train,     METH_NOARGS,                "train model" },
   { "test",      (PyCFunction)SlopeOne_test,      METH_VARARGS|METH_KEYWORDS, "test model" },
   { "testrec",   (PyCFunction)SlopeOne_testrec,   METH_VARARGS|METH_KEYWORDS, "test recommendation model" },
   { "predict",   (PyCFunction)SlopeOne_predict,   METH_KEYWORDS,              "predict user's rating for an item" },
   { "recommend", (PyCFunction)SlopeOne_recommend, METH_KEYWORDS,              "recommend ranked items to a user" },
   { NULL }
};

static PyTypeObject SlopeOneType =
{
#if PY_MAJOR_VERSION >= 3
   PyVarObject_HEAD_INIT(NULL, 0)
#else
   PyObject_HEAD_INIT( NULL )
   0,                                        /* ob_size */
#endif
   "libpyreclab.SlopeOne",                   /* tp_name */
   sizeof(PySlopeOne),                       /* tp_basicsize */
   0,                                        /* tp_itemsize */
   (destructor)SlopeOne_dealloc,             /* tp_dealloc */
   0,                                        /* tp_print */
   0,                                        /* tp_getattr */
   0,                                        /* tp_setattr */
   0,                                        /* tp_compare */
   0,                                        /* tp_repr */
   0,                                        /* tp_as_number */
   0,                                        /* tp_as_sequence */
   0,                                        /* tp_as_mapping */
   0,                                        /* tp_hash */
   0,                                        /* tp_call */
   0,                                        /* tp_str */
   0,                                        /* tp_getattro */
   0,                                        /* tp_setattro */
   0,                                        /* tp_as_buffer */
   Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
   "SlopeOne objects",                        /* tp_doc */
   0,                                        /* tp_traverse */
   0,                                        /* tp_clear */
   0,                                        /* tp_richcompare */
   0,                                        /* tp_weaklistoffset */
   0,                                        /* tp_iter */
   0,                                        /* tp_iternext */
   SlopeOne_methods,                          /* tp_methods */
   0,                                        /* tp_members */
   0,                                        /* tp_getset */
   0,                                        /* tp_base */
   0,                                        /* tp_dict */
   0,                                        /* tp_descr_get */
   0,                                        /* tp_descr_set */
   0,                                        /* tp_dictoffset */
   0,                                        /* tp_init */
   0,                                        /* tp_alloc */
   SlopeOne_new,                              /* tp_new */
};


#endif // __PY_SLOPE_ONE_H__

