#ifndef __PY_USER_AVG_H__
#define __PY_USER_AVG_H__

#include <Python.h> // This header was set at the first place to avoid warnings

#include "AlgUserAvg.h"
#include "DataReader.h"
#include "MAE.h"
#include "RMSE.h"

class PyUserAvg
{
public:

   PyObject_HEAD
   DataReader* m_trainingReader;
   AlgUserAvg* m_recAlgorithm;
   MAE m_mae;
   RMSE m_rmse;
};

PyTypeObject* UserAvg_getTypeObject();
PyObject* UserAvg_new( PyTypeObject* type, PyObject* args, PyObject* kwdict );
void UserAvg_dealloc( PyUserAvg* self );
// It produces a segfault on Mac OS X EL Capitan / Python 3.6
//PyObject* UserAvg_train( PyUserAvg* self, PyObject* args );
PyObject* UserAvg_train( PyUserAvg* self, PyObject* args, PyObject* kwdict );
PyObject* UserAvg_predict( PyUserAvg* self, PyObject* args );
PyObject* UserAvg_recommend( PyUserAvg* self, PyObject* args, PyObject* kwdict );
PyObject* UserAvg_test( PyUserAvg* self, PyObject* args, PyObject* kwdict );
PyObject* UserAvg_testrec( PyUserAvg* self, PyObject* args, PyObject* kwdict );


#endif // __PY_USER_AVG_H__

