#include "SigHandler.h"

#include <Python.h> // This header was set at the first place to avoid warnings
#include <iostream>

using namespace std;

bool SigHandler::m_sigReceived = false;

SigHandler::SigHandler()
{
}

SigHandler::SigHandler( int signum )
: m_pTempAction( NULL )
{
   struct sigaction newAction;
   newAction.sa_handler = SigHandler::handler;
   sigemptyset( &newAction.sa_mask );
   newAction.sa_flags = 0;
   sigaction( signum, &newAction, m_pTempAction );
}

SigHandler::~SigHandler()
{
   sigaction( SIGINT, NULL, m_pTempAction );
}

void SigHandler::handler( int signum )
{
   cout << "sighandler called" << endl;
   m_sigReceived = true;
   signum = 0; // avoid warning
}

bool SigHandler::interrupt()
{
   return m_sigReceived;
}

