#ifndef __PY_BPR_MF_H__
#define __PY_BPR_MF_H__

#include <Python.h> // This header was set at the first place to avoid warnings

#include "AlgBprMf.h"
#include "DataReader.h"

class PyBprMf
{
public:

   PyObject_HEAD
   DataReader* m_trainingReader;
   DataFrame* m_pTestData;
   AlgBprMf* m_recAlgorithm;
};

PyTypeObject* BprMfGetType();
PyObject* BprMfNew( PyTypeObject* type, PyObject* args, PyObject* kwdict );
PyObject* BprMfTrain( PyBprMf* self, PyObject* args, PyObject* kwdict );

#endif // __PY_BPR_MF_H__

