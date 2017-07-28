#ifndef __PY_ITEM_KNN_H__
#define __PY_ITEM_KNN_H__

#include <Python.h> // This header was set at the first place to avoid warnings

#include "AlgItemBasedKnn.h"
#include "DataReader.h"
#include "MAE.h"
#include "RMSE.h"

class PyItemKnn
{
public:

   PyObject_HEAD
   DataReader* m_trainingReader;
   AlgItemBasedKnn* m_recAlgorithm;
   MAE m_mae;
   RMSE m_rmse;
};

PyObject* ItemKnn_new( PyTypeObject* type, PyObject* args, PyObject* kwdict );
void ItemKnn_dealloc( PyItemKnn* self );
PyObject* ItemKnn_train( PyItemKnn* self, PyObject* args, PyObject* kwdict );
PyObject* ItemKnn_predict( PyItemKnn* self, PyObject* args );
PyObject* ItemKnn_recommend( PyItemKnn* self, PyObject* args, PyObject* kwdict );
PyObject* ItemKnn_test( PyItemKnn* self, PyObject* args, PyObject* kwdict );
PyObject* ItemKnn_testrec( PyItemKnn* self, PyObject* args, PyObject* kwdict );

static
PyMethodDef ItemKnn_methods[] =
{
   { "train",     (PyCFunction)ItemKnn_train,     METH_VARARGS|METH_KEYWORDS, "train model" },
   { "test",      (PyCFunction)ItemKnn_test,      METH_VARARGS|METH_KEYWORDS, "test model" },
   { "testrec",   (PyCFunction)ItemKnn_testrec,   METH_VARARGS|METH_KEYWORDS, "test recommendation model" },
   { "predict",   (PyCFunction)ItemKnn_predict,   METH_VARARGS,               "predict user's rating for an item" },
   { "recommend", (PyCFunction)ItemKnn_recommend, METH_VARARGS|METH_KEYWORDS, "recommend ranked items to a user" },
   { NULL }
};

static PyTypeObject ItemKnnType =
{
#if PY_MAJOR_VERSION >= 3
   PyVarObject_HEAD_INIT(NULL, 0)
#else
   PyObject_HEAD_INIT( NULL )
   0,                                        /* ob_size */
#endif
   "libpyreclab.ItemKnn",                    /* tp_name */
   sizeof(PyItemKnn),                        /* tp_basicsize */
   0,                                        /* tp_itemsize */
   (destructor)ItemKnn_dealloc,              /* tp_dealloc */
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
   "ItemKnn objects",                        /* tp_doc */
   0,                                        /* tp_traverse */
   0,                                        /* tp_clear */
   0,                                        /* tp_richcompare */
   0,                                        /* tp_weaklistoffset */
   0,                                        /* tp_iter */
   0,                                        /* tp_iternext */
   ItemKnn_methods,                          /* tp_methods */
   0,                                        /* tp_members */
   0,                                        /* tp_getset */
   0,                                        /* tp_base */
   0,                                        /* tp_dict */
   0,                                        /* tp_descr_get */
   0,                                        /* tp_descr_set */
   0,                                        /* tp_dictoffset */
   0,                                        /* tp_init */
   0,                                        /* tp_alloc */
   ItemKnn_new,                              /* tp_new */
};


#endif // __PY_ITEM_KNN_H__

