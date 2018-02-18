#ifndef __PYRECLAB_SIGNAL_HANDLER_H__
#define __PYRECLAB_SIGNAL_HANDLER_H__

#include <Python.h> // This header was set at the first place to avoid warnings
#include <signal.h>


class PrlSigHandler
{
public:

   enum EAlgType
   {
      UNKNOWN,
      FUNK_SVD,
      ITEM_AVG,
      ITEM_KNN,
      MOST_POPULAR,
      SLOPE_ONE,
      USER_AVG,
      USER_KNN,
      IF_ALS
   };

   static
   void handler( int signum );

   static
   void registerObj( PyObject* obj, EAlgType type );

   static
   void unregisterObj();

   static
   struct sigaction* handlesignal( int signum );

   static
   void restoresignal( int signum, struct sigaction* pAction );

private:

   static
   EAlgType m_algType;

   static
   PyObject* m_activeObj;

};

#endif // __PYRECLAB_SIGNAL_HANDLER_H__

