#ifndef __PY_ITEM_AVG_H__
#define __PY_ITEM_AVG_H__

#include <Python.h> // This header was set at the first place to avoid warnings

#include "AlgItemAvg.h"
#include "DataReader.h"
#include "MAE.h"
#include "RMSE.h"

class PyItemAvg
{
public:

   PyObject_HEAD
   DataReader* m_trainingReader;
   AlgItemAvg* m_recAlgorithm;
   MAE m_mae;
   RMSE m_rmse;
};

PyObject* ItemAvg_new( PyTypeObject* type, PyObject* args, PyObject* kwdict );
void ItemAvg_dealloc( PyItemAvg* self );
// It produces a segfault on Mac OS X EL Capitan / Python 3.6
//PyObject* ItemAvg_train( PyItemAvg* self );
PyObject* ItemAvg_train( PyItemAvg* self, PyObject* args, PyObject* kwdict );
PyObject* ItemAvg_predict( PyItemAvg* self, PyObject* args );
PyObject* ItemAvg_recommend( PyItemAvg* self, PyObject* args, PyObject* kwdict );
PyObject* ItemAvg_test( PyItemAvg* self, PyObject* args, PyObject* kwdict );
PyObject* ItemAvg_testrec( PyItemAvg* self, PyObject* args, PyObject* kwdict );

static
PyMethodDef ItemAvg_methods[] =
{
   // It produces a segfault on Mac OS X EL Capitan / Python 3.6
   //{ "train",     (PyCFunction)ItemAvg_train,     METH_NOARGS,                "train model" },
   { "train",     (PyCFunction)ItemAvg_train,     METH_VARARGS|METH_KEYWORDS, "train model" },
   { "test",      (PyCFunction)ItemAvg_test,      METH_VARARGS|METH_KEYWORDS, "test model" },
   { "testrec",   (PyCFunction)ItemAvg_testrec,   METH_VARARGS|METH_KEYWORDS, "test recommendation model" },
   { "predict",   (PyCFunction)ItemAvg_predict,   METH_VARARGS,               "predict user's rating for an item" },
   { "recommend", (PyCFunction)ItemAvg_recommend, METH_VARARGS|METH_KEYWORDS, "recommend ranked items to a user" },
   { NULL, NULL, 0, NULL }
};

static PyTypeObject ItemAvgType =
{
#if PY_MAJOR_VERSION >= 3
   PyVarObject_HEAD_INIT(NULL, 0)
#else
   PyObject_HEAD_INIT( NULL )
   0,                                        /* ob_size */
#endif
   "libpyreclab.ItemAvg",                    /* tp_name */
   sizeof(PyItemAvg),                        /* tp_basicsize */
   0,                                        /* tp_itemsize */
   (destructor)ItemAvg_dealloc,              /* tp_dealloc */
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
   "ItemAvg objects",                        /* tp_doc */
   0,                                        /* tp_traverse */
   0,                                        /* tp_clear */
   0,                                        /* tp_richcompare */
   0,                                        /* tp_weaklistoffset */
   0,                                        /* tp_iter */
   0,                                        /* tp_iternext */
   ItemAvg_methods,                          /* tp_methods */
   0,                                        /* tp_members */
   0,                                        /* tp_getset */
   0,                                        /* tp_base */
   0,                                        /* tp_dict */
   0,                                        /* tp_descr_get */
   0,                                        /* tp_descr_set */
   0,                                        /* tp_dictoffset */
   0,                                        /* tp_init */
   0,                                        /* tp_alloc */
   ItemAvg_new,                              /* tp_new */
};


#endif // __PY_ITEM_AVG_H__

