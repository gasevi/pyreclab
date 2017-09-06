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

PyTypeObject* ItemKnn_getTypeObject();
PyObject* ItemKnn_new( PyTypeObject* type, PyObject* args, PyObject* kwdict );
void ItemKnn_dealloc( PyItemKnn* self );
PyObject* ItemKnn_train( PyItemKnn* self, PyObject* args, PyObject* kwdict );
PyObject* ItemKnn_predict( PyItemKnn* self, PyObject* args );
PyObject* ItemKnn_recommend( PyItemKnn* self, PyObject* args, PyObject* kwdict );
PyObject* ItemKnn_test( PyItemKnn* self, PyObject* args, PyObject* kwdict );
PyObject* ItemKnn_testrec( PyItemKnn* self, PyObject* args, PyObject* kwdict );

#endif // __PY_ITEM_KNN_H__

