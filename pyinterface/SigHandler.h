#ifndef __PYRECLAB_INTERRUPTION_H__
#define __PYRECLAB_INTERRUPTION_H__

#include "FlowControl.h"

#include <signal.h>


class SigHandler
      : public FlowControl
{
public:

   SigHandler( int signum );

   ~SigHandler();

   bool interrupt();

private:

   SigHandler();

   static
   void handler( int signum );

   static
   bool m_sigReceived;

   struct sigaction* m_pTempAction;

};

#endif // __PYRECLAB_INTERRUPTION_H__

