#ifndef __PY_USER_KNN_H__
#define __PY_USER_KNN_H__

#include <Python.h> // This header was set at the first place to avoid warnings

#include "AlgUserBasedKnn.h"
#include "DataReader.h"
#include "MAE.h"
#include "RMSE.h"

class PyUserKnn
{
public:

   PyObject_HEAD
   DataReader* m_trainingReader;
   DataFrame* m_pTestData;
   AlgUserBasedKnn* m_recAlgorithm;
   MAE m_mae;
   RMSE m_rmse;
};

PyTypeObject* UserKnn_getTypeObject();
PyObject* UserKnn_new( PyTypeObject* type, PyObject* args, PyObject* kwdict );
void UserKnn_dealloc( PyUserKnn* self );
PyObject* UserKnn_train( PyUserKnn* self, PyObject* args, PyObject* kwdict );
PyObject* UserKnn_predict( PyUserKnn* self, PyObject* args );
PyObject* UserKnn_recommend( PyUserKnn* self, PyObject* args, PyObject* kwdict );
PyObject* UserKnn_test( PyUserKnn* self, PyObject* args, PyObject* kwdict );
PyObject* UserKnn_testrec( PyUserKnn* self, PyObject* args, PyObject* kwdict );

#endif // __PY_USER_KNN_H__

