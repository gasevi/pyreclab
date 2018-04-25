#ifndef __PY_SLOPE_ONE_H__
#define __PY_SLOPE_ONE_H__

#include <Python.h> // This header was set at the first place to avoid warnings

#include "AlgSlopeOne.h"
#include "DataReader.h"
#include "MAE.h"
#include "RMSE.h"

class PySlopeOne
{
public:

   PyObject_HEAD
   DataReader* m_trainingReader;
   DataFrame* m_pTestData;
   AlgSlopeOne* m_recAlgorithm;
   MAE m_mae;
   RMSE m_rmse;
};

PyTypeObject* SlopeOne_getTypeObject();
PyObject* SlopeOne_new( PyTypeObject* type, PyObject* args, PyObject* kwdict );
void SlopeOne_dealloc( PySlopeOne* self );
// It produces a segfault on Mac OS X EL Capitan / Python 3.6
//PyObject* SlopeOne_train( PySlopeOne* self, PyObject* args );
PyObject* SlopeOne_train( PySlopeOne* self, PyObject* args, PyObject* kwdict );
PyObject* SlopeOne_predict( PySlopeOne* self, PyObject* args );
PyObject* SlopeOne_recommend( PySlopeOne* self, PyObject* args, PyObject* kwdict );
PyObject* SlopeOne_test( PySlopeOne* self, PyObject* args, PyObject* kwdict );
PyObject* SlopeOne_testrec( PySlopeOne* self, PyObject* args, PyObject* kwdict );
PyObject* SlopeOne_MAP( PySlopeOne* self, PyObject* args, PyObject* kwdict );
PyObject* SlopeOne_nDCG( PySlopeOne* self, PyObject* args, PyObject* kwdict );


#endif // __PY_SLOPE_ONE_H__

