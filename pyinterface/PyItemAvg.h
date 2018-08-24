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
   DataFrame* m_pTestData;
   AlgItemAvg* m_recAlgorithm;
   MAE m_mae;
   RMSE m_rmse;
};

PyTypeObject* ItemAvg_getTypeObject();
PyObject* ItemAvg_new( PyTypeObject* type, PyObject* args, PyObject* kwdict );
void ItemAvg_dealloc( PyItemAvg* self );
// It produces a segfault on Mac OS X EL Capitan / Python 3.6
//PyObject* ItemAvg_train( PyItemAvg* self );
PyObject* ItemAvg_train( PyItemAvg* self, PyObject* args, PyObject* kwdict );
PyObject* ItemAvg_predict( PyItemAvg* self, PyObject* args );
PyObject* ItemAvg_recommend( PyItemAvg* self, PyObject* args, PyObject* kwdict );
PyObject* ItemAvg_test( PyItemAvg* self, PyObject* args, PyObject* kwdict );
PyObject* ItemAvg_testrec( PyItemAvg* self, PyObject* args, PyObject* kwdict );

#endif // __PY_ITEM_AVG_H__

